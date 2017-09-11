#include "stdafx.h"
#include "cSkinnedModel.h"
#include "cFrameHierarchyAllocator.h"

cSkinnedModel::cSkinnedModel()
	: m_rootBone(NULL), 
	m_pAnimControl(NULL),
	m_pEffect(NULL)
{
}

cSkinnedModel::~cSkinnedModel()
{
	if (m_rootBone) {
		cFrameHierarchyAllocator frameHierarchAlloc;
		D3DXFrameDestroy(m_rootBone, &frameHierarchAlloc);
	}
	SAFE_RELEASE(m_pAnimControl);
	SAFE_RELEASE(m_pEffect);
}

bool cSkinnedModel::Init(const char * modelFileX)
{
	cFrameHierarchyAllocator frameHierarchAlloc;
	LPD3DXFRAME rootBone = NULL;
	D3DXLoadMeshHierarchyFromXA(
		modelFileX,
		D3DXMESH_MANAGED,
		g_pDeviceManager->GetDevice(),
		&frameHierarchAlloc,
		NULL,
		&rootBone,
		&m_pAnimControl);
	if (rootBone == NULL) {
		return false;
	}
	m_rootBone = (Bone*)rootBone;
	SetupBoneMatixPtrs(m_rootBone);
	SetupAllAnimationNames();
	if (!LoadEffect("SkinningShader.fx")) {
		return false;
	}
	return true;
}

void cSkinnedModel::Update()
{
	m_pAnimControl->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL);
	m_rootBone->calculateCombinedTransformationMatrix(NULL);
}

void cSkinnedModel::Render(Bone* bone)
{
	D3DXMATRIXA16 matView, matProj, matVP;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	matVP = matView * matProj;
	if (bone==NULL) {
		Render(m_rootBone);
	}
	else {
		if (bone->pMeshContainer) {
			BoneMesh* pBoneMesh = (BoneMesh*)bone->pMeshContainer;
			if (pBoneMesh->pSkinInfo) {
				//pBoneMesh->pSkinInfo->GetMaxVertexInfluences()
				//스키닝 정보가 있는 경우에는 스키닝 수행
				DWORD numBones = pBoneMesh->pSkinInfo->GetNumBones();
				for (int i = 0; i < numBones; ++i) {
					if (pBoneMesh->boneMatrixPtrs[i]) {
						//offset 행렬과 메시에 영향을 주는 본의 행렬을 조합하여
						//메시를 스키닝할 행렬 배열을 생성한다.
						D3DXMatrixMultiply(&pBoneMesh->currentBoneMatrices[i], 
							&pBoneMesh->boneOffsetMatrices[i], 
							pBoneMesh->boneMatrixPtrs[i]);
					}
					else {
						pBoneMesh->currentBoneMatrices[i] = pBoneMesh->boneOffsetMatrices[i];
					}
				}
				if (m_pEffect) {
					DWORD numBoneInfluces;
					pBoneMesh->pSkinInfo->GetMaxVertexInfluences(&numBoneInfluces);
					m_pEffect->SetInt("numBoneInfluences", numBoneInfluces);
					if (FAILED(m_pEffect->SetVector(
						"lightPos", &D3DXVECTOR4(100, 100, 100, 1.0f)
					))) {
						OutputDebugString("Set lightPos failed\n");
					}
					if (FAILED(m_pEffect->SetMatrix(
						"matViewProj",
						&matVP))) {
						OutputDebugString("Set matViewProj failed\n");
					}
					if (FAILED(m_pEffect->SetMatrixArray(
						"matrixPalette",
						pBoneMesh->currentBoneMatrices.data(),
						pBoneMesh->pSkinInfo->GetNumBones()))) {
						OutputDebugString("Set MatrixPalette failed\n");
					}
					D3DXHANDLE hTech = m_pEffect->GetTechniqueByName("SkinningTech");

					for (size_t i = 0; i < pBoneMesh->NumMaterials; ++i) {
						int attrID = pBoneMesh->attrTable[i].AttribId;
						if (FAILED(m_pEffect->SetTexture(
							"texDiffuse",
							pBoneMesh->textures[attrID]))) {
							OutputDebugString("Set texDiffuse failed\n");
						}

						m_pEffect->SetTechnique(hTech);
						m_pEffect->Begin(NULL, NULL);
						m_pEffect->BeginPass(0);

						pBoneMesh->MeshData.pMesh->DrawSubset(attrID);
						m_pEffect->EndPass();
						m_pEffect->End();
					}
				}
				//Software-Skinning용 vertex update
				//BYTE *src = NULL, *dst = NULL;
				//pBoneMesh->MeshData.pMesh->LockVertexBuffer(0, (LPVOID*)&dst);
				//pBoneMesh->originalMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&src);
				//pBoneMesh->pSkinInfo->UpdateSkinnedMesh(pBoneMesh->currentBoneMatrices.data(), NULL, src, dst);
				//pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();
				//pBoneMesh->originalMesh->UnlockVertexBuffer();
				////Software-Skinning 렌더
				//for (size_t i = 0; i < pBoneMesh->NumMaterials; ++i) {
				//	int attrID = pBoneMesh->attrTable[i].AttribId;
				//	g_pD3DDevice->SetTexture(0, pBoneMesh->textures[attrID]);
				//	g_pD3DDevice->SetMaterial(&pBoneMesh->materials[attrID]);
				//	pBoneMesh->MeshData.pMesh->DrawSubset(attrID);
				//}
			}
			else {
				//스키닝 정보가 없으면 일반 메시 렌더링 수행
				for (size_t i = 0; i < pBoneMesh->NumMaterials; ++i) {
					if (pBoneMesh->MeshData.pMesh) {
						g_pD3DDevice->SetTexture(0, pBoneMesh->textures[i]);
						g_pD3DDevice->SetMaterial(&pBoneMesh->materials[i]);
						g_pD3DDevice->SetTransform(D3DTS_WORLD, &bone->combinedTransformationMatrix);

						pBoneMesh->MeshData.pMesh->DrawSubset(i);
					}
				}
			}
		}

		Bone* sibling = (Bone*)bone->pFrameSibling;
		if (sibling) {
			Render(sibling);
		}

		Bone* child = (Bone*)bone->pFrameFirstChild;
		if (child) {
			Render(child);
		}
	}
}

size_t cSkinnedModel::GetNumAnimations() const
{
	if (m_pAnimControl) {
		return m_pAnimControl->GetNumAnimationSets();
	}
	return 0;
}

bool cSkinnedModel::SetAnimation(int idx)
{
	ID3DXAnimationSet* anim=nullptr;
	m_pAnimControl->GetAnimationSet(idx, &anim);
	if (!anim) {
		return false;
	}

	m_pAnimControl->SetTrackAnimationSet(0, anim);
	anim->Release();
	return true;
}

bool cSkinnedModel::LoadEffect(const char * effectFile)
{
	SAFE_RELEASE(m_pEffect);

	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	if (FAILED(D3DXCreateEffectFromFile(g_pD3DDevice, effectFile,
		NULL, NULL, dwShaderFlags, NULL, &m_pEffect, &pError))) {
		if (pError) {
			int size = pError->GetBufferSize();
			void *ack = pError->GetBufferPointer();

			if (ack)
			{
				char* str = new char[size];
				sprintf(str, (const char*)ack, size);
				OutputDebugString(str);
				delete[] str;
			}
		}
		return false;
	}
	return true;
}

bool cSkinnedModel::GetAnimationName(int idx, std::string& name) const
{
	if (idx >= 0 && idx < m_animationNames.size()) {
		name = m_animationNames[idx];
		return true;
	}
	return false;
}

void cSkinnedModel::SetupBoneMatixPtrs(Bone * bone)
{
	if (bone == NULL)
		return;
	if (bone->pMeshContainer && bone->pMeshContainer->pSkinInfo) {
		BoneMesh* boneMesh = (BoneMesh*)bone->pMeshContainer;

		DWORD numBones = boneMesh->pSkinInfo->GetNumBones();
		boneMesh->boneMatrixPtrs.resize(numBones);
		for (int i = 0; i < numBones; ++i) {
			Bone* boneFound = (Bone*)D3DXFrameFind(m_rootBone, boneMesh->pSkinInfo->GetBoneName(i));
			if (boneFound) {
				boneMesh->boneMatrixPtrs[i] = &boneFound->combinedTransformationMatrix;
			}
			else {
				boneMesh->boneMatrixPtrs[i] = NULL;
			}
		}
	}
	if (bone->pFrameFirstChild) {
		SetupBoneMatixPtrs((Bone*)bone->pFrameFirstChild);
	}
	if (bone->pFrameSibling) {
		SetupBoneMatixPtrs((Bone*)bone->pFrameSibling);
	}
}

void cSkinnedModel::SetupAllAnimationNames()
{
	int numAnims = GetNumAnimations();
	m_animationNames.resize(numAnims);
	for (int i = 0; i < numAnims; ++i) {
		LPD3DXANIMATIONSET animSet;
		m_pAnimControl->GetAnimationSet(i, &animSet);
		m_animationNames[i] = animSet->GetName();
		animSet->Release();
	}
}
