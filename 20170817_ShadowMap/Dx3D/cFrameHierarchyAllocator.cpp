#include "stdafx.h"
#include "cFrameHierarchyAllocator.h"

void Bone::calculateCombinedTransformationMatrix(Bone * parent)
{
	if (parent == NULL) {
		//��Ʈ Bone
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
	
	//�޽� ������ ����
	pMeshContainer->MeshData = *pMeshData;
	pMeshContainer->MeshData.pMesh->AddRef();

	if (pEffectInstances) {
		//����Ʈ ����
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

	//���͸��� ���� �� ���� �ʱ�ȭ
	pMeshContainer->NumMaterials = NumMaterials;
	pMeshContainer->materials.resize(NumMaterials);
	pMeshContainer->textures.resize(NumMaterials);
	//���͸��� �� �ؽ��� ������ ä���.
	for (size_t i = 0; i < NumMaterials; ++i) {
		memcpy(&pMeshContainer->materials[i], &(pMaterials[i].MatD3D), sizeof(D3DMATERIAL9));
		pMeshContainer->materials[i].Ambient = pMeshContainer->materials[i].Diffuse;//X������ Ambient������ ����.
		pMeshContainer->textures[i] = g_pTextureManager->GetTexture(pMaterials[i].pTextureFilename);
	}

	if (pAdjacency) {
		//Face�� �������� ����
		pMeshContainer->pAdjacency = new DWORD[pMeshData->pMesh->GetNumFaces() * 3];
		memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD)*pMeshData->pMesh->GetNumFaces() * 3);
	}

	if (pSkinInfo) {
		//SkinInfo ���� �� AddRef.
		pMeshContainer->pSkinInfo = pSkinInfo;
		pMeshContainer->pSkinInfo->AddRef();

		//����Ʈ���� ��Ű���� ���� �޽ø� ����
		pMeshData->pMesh->CloneMeshFVF(
			D3DXMESH_MANAGED,
			pMeshData->pMesh->GetFVF(),
			g_pD3DDevice,
			&pMeshContainer->originalMesh);
		//�޽� AttributeTable ����
		pMeshData->pMesh->GetAttributeTable(NULL, &pMeshContainer->numAttrGroups);
		pMeshContainer->attrTable = new D3DXATTRIBUTERANGE[pMeshContainer->numAttrGroups];
		pMeshData->pMesh->GetAttributeTable(pMeshContainer->attrTable, NULL);

		int numBones = pMeshContainer->pSkinInfo->GetNumBones();
		//BoneOffset ��ĵ��� �����ϱ� ���� �����Ҵ�
		pMeshContainer->boneOffsetMatrices.resize(numBones);
		for (size_t i = 0; i < numBones; ++i) {
			pMeshContainer->boneOffsetMatrices[i] = *pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
		}
		//Bone������ �°� CurrentBoneMatrices ���� �Ҵ�
		pMeshContainer->currentBoneMatrices.resize(numBones);
	}

	//�޽� �����̳� ���
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

		//�̸� ����
		if (pMeshContainer->Name) {
			SAFE_DELETE_ARR(pMeshContainer->Name);
		}

		//����Ʈ ����
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

		//���͸��� ���� ����
		for (size_t i = 0; i < pMeshContainer->NumMaterials; ++i) {
			SAFE_RELEASE(pMeshContainer->textures[i]);
		}
		pMeshContainer->textures.clear();
		pMeshContainer->materials.clear();

		//Face ���� ���� ����
		if (pMeshContainer->pAdjacency) {
			SAFE_DELETE_ARR(pMeshContainer->pAdjacency);
		}
		
		//SkinInfo���� ���� ����
		if (pMeshContainer->pSkinInfo) {
			SAFE_RELEASE(pMeshContainer->pSkinInfo);
			SAFE_DELETE_ARR(pMeshContainer->attrTable);
			SAFE_RELEASE(pMeshContainer->originalMesh);
			pMeshContainer->boneOffsetMatrices.clear();
			pMeshContainer->currentBoneMatrices.clear();
		}

		//�޽� �����̳� ����
		SAFE_DELETE(pMeshContainer);
	}
	return S_OK;
}

