#include "stdafx.h"
#include "cFrameHierarchyAllocator.h"

void Bone::calculateCombinedTransformationMatrix(Bone * parent)
{
	if (parent == NULL) {
		//루트 Bone
		combinedTransformationMatrix = TransformationMatrix;
	}
	else {
		combinedTransformationMatrix =  TransformationMatrix * parent->combinedTransformationMatrix;
	}
	Bone* sibling = static_cast<Bone*>(this->pFrameSibling);
	if (sibling) {
		sibling->calculateCombinedTransformationMatrix(parent);
	}

	Bone* child = static_cast<Bone*>(this->pFrameFirstChild);
	if (child) {
		child->calculateCombinedTransformationMatrix(this);
	}
}


cFrameHierarchyAllocator::cFrameHierarchyAllocator()
{
}


cFrameHierarchyAllocator::~cFrameHierarchyAllocator()
{
}

STDMETHODIMP cFrameHierarchyAllocator::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	Bone * bone = new Bone();
	ZeroMemory(bone, sizeof(Bone));
	if (Name != NULL) {
		size_t lenName = strlen(Name) + 1;
		bone->Name = new char[lenName];
		strcpy_s(bone->Name, lenName, Name);
	}

	D3DXMatrixIdentity(&(bone->combinedTransformationMatrix));
	D3DXMatrixIdentity(&(bone->TransformationMatrix));

	*ppNewFrame = bone;
	return D3D_OK;
}

STDMETHODIMP cFrameHierarchyAllocator::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA * pMeshData, CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	BoneMesh *pMeshContainer = new BoneMesh();
	ZeroMemory(pMeshContainer, sizeof(BoneMesh));
	size_t lenName = strlen(Name) + 1;
	pMeshContainer->Name = new char[lenName];
	strcpy_s(pMeshContainer->Name, lenName, Name);
	
	//메시 데이터 복사
	pMeshContainer->MeshData = *pMeshData;
	pMeshContainer->MeshData.pMesh->AddRef();

	if (pEffectInstances) {
		//이펙트 복사
		pMeshContainer->pEffects = new D3DXEFFECTINSTANCE();
		pMeshContainer->pEffects->NumDefaults = pEffectInstances->NumDefaults;
		pMeshContainer->pEffects->pDefaults = new D3DXEFFECTDEFAULT[pEffectInstances->NumDefaults];
		for (size_t i = 0; i < pMeshContainer->pEffects->NumDefaults; ++i) {
			pMeshContainer->pEffects->pDefaults[i] = pEffectInstances->pDefaults[i];
			size_t lenParamName = strlen(pEffectInstances->pDefaults[i].pParamName) + 1;
			pMeshContainer->pEffects->pDefaults[i].pParamName = new char[lenParamName];
			strcpy_s(pMeshContainer->pEffects->pDefaults[i].pParamName, lenParamName, pEffectInstances->pDefaults[i].pParamName);
			pMeshContainer->pEffects->pDefaults[i].pValue = new BYTE[pEffectInstances->pDefaults[i].NumBytes];
			memcpy(pEffectInstances->pDefaults[i].pValue, pEffectInstances->pDefaults[i].pValue, pEffectInstances->pDefaults[i].NumBytes);
		}
		
		if (pEffectInstances->pEffectFilename) {
			size_t lenEffect = strlen(pEffectInstances->pEffectFilename) + 1;
			pMeshContainer->pEffects->pEffectFilename = new char[lenEffect];
			strcpy_s(pMeshContainer->pEffects->pEffectFilename, lenEffect, pEffectInstances->pEffectFilename);
		}
	}

	//매터리얼 개수 및 벡터 초기화
	pMeshContainer->NumMaterials = NumMaterials;
	pMeshContainer->materials.resize(NumMaterials);
	pMeshContainer->textures.resize(NumMaterials);
	//매터리얼 및 텍스쳐 정보를 채운다.
	for (size_t i = 0; i < NumMaterials; ++i) {
		memcpy(&pMeshContainer->materials[i], &(pMaterials[i].MatD3D), sizeof(D3DMATERIAL9));
		pMeshContainer->materials[i].Ambient = pMeshContainer->materials[i].Diffuse;//X파일을 Ambient성분이 없다.
		pMeshContainer->textures[i] = g_pTextureManager->GetTexture(pMaterials[i].pTextureFilename);
	}

	if (pAdjacency) {
		//Face의 인접정보 저장
		pMeshContainer->pAdjacency = new DWORD[pMeshData->pMesh->GetNumFaces() * 3];
		memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD)*pMeshData->pMesh->GetNumFaces() * 3);
	}

	if (pSkinInfo) {
		//SkinInfo 복사 후 AddRef.
		pMeshContainer->pSkinInfo = pSkinInfo;
		pMeshContainer->pSkinInfo->AddRef();

		//소프트웨어 스키닝을 위해 메시를 복제
		pMeshData->pMesh->CloneMeshFVF(
			D3DXMESH_MANAGED,
			pMeshData->pMesh->GetFVF(),
			g_pD3DDevice,
			&pMeshContainer->originalMesh);
		//메시 AttributeTable 복사
		pMeshData->pMesh->GetAttributeTable(NULL, &pMeshContainer->numAttrGroups);
		pMeshContainer->attrTable = new D3DXATTRIBUTERANGE[pMeshContainer->numAttrGroups];
		pMeshData->pMesh->GetAttributeTable(pMeshContainer->attrTable, NULL);

		int numBones = pMeshContainer->pSkinInfo->GetNumBones();
		//BoneOffset 행렬들을 저장하기 위해 벡터할당
		pMeshContainer->boneOffsetMatrices.resize(numBones);
		for (size_t i = 0; i < numBones; ++i) {
			pMeshContainer->boneOffsetMatrices[i] = *pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
		}
		//Bone개수에 맞게 CurrentBoneMatrices 벡터 할당
		pMeshContainer->currentBoneMatrices.resize(numBones);
	}

	//메시 컨테이너 출력
	*ppNewMeshContainer = pMeshContainer;
	return D3D_OK;
}

STDMETHODIMP cFrameHierarchyAllocator::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	Bone* bone = static_cast<Bone*>(pFrameToFree);
	if (bone) {
		if (bone->Name) {
			delete[] bone->Name;
			bone->Name = NULL;
		}
		delete bone;
	}
	return S_OK;
}

STDMETHODIMP cFrameHierarchyAllocator::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	BoneMesh* pMeshContainer = static_cast<BoneMesh*>(pMeshContainerToFree);
	if (pMeshContainer) {

		//이름 제거
		if (pMeshContainer->Name) {
			SAFE_DELETE_ARR(pMeshContainer->Name);
		}

		//이펙트 제거
		if (pMeshContainer->pEffects) {
			if (pMeshContainer->pEffects->pEffectFilename) {
				SAFE_DELETE_ARR(pMeshContainer->pEffects->pEffectFilename);
			}
			for (size_t i = 0; i < pMeshContainer->pEffects->NumDefaults; ++i) {
				if (pMeshContainer->pEffects->pDefaults[i].pParamName) {
					SAFE_DELETE_ARR(pMeshContainer->pEffects->pDefaults[i].pParamName);
				}
				if (pMeshContainer->pEffects->pDefaults[i].pValue) {
					SAFE_DELETE_ARR(pMeshContainer->pEffects->pDefaults[i].pValue);
				}
			}
			SAFE_DELETE_ARR(pMeshContainer->pEffects->pDefaults);
		}

		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

		//매터리얼 정보 제거
		for (size_t i = 0; i < pMeshContainer->NumMaterials; ++i) {
			SAFE_RELEASE(pMeshContainer->textures[i]);
		}
		pMeshContainer->textures.clear();
		pMeshContainer->materials.clear();

		//Face 인접 정보 제거
		if (pMeshContainer->pAdjacency) {
			SAFE_DELETE_ARR(pMeshContainer->pAdjacency);
		}
		
		//SkinInfo관련 정보 제거
		if (pMeshContainer->pSkinInfo) {
			SAFE_RELEASE(pMeshContainer->pSkinInfo);
			SAFE_DELETE_ARR(pMeshContainer->attrTable);
			SAFE_RELEASE(pMeshContainer->originalMesh);
			pMeshContainer->boneOffsetMatrices.clear();
			pMeshContainer->currentBoneMatrices.clear();
		}

		//메쉬 컨테이너 제거
		SAFE_DELETE(pMeshContainer);
	}
	return S_OK;
}

