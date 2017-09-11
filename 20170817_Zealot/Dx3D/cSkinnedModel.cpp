#include "stdafx.h"
#include "cSkinnedModel.h"
#include "cFrameHierarchyAllocator.h"

cSkinnedModel::cSkinnedModel()
	: m_rootBone(NULL), m_pAnimControl(NULL)
{
}

cSkinnedModel::~cSkinnedModel()
{
	if (m_rootBone) {
		cFrameHierarchyAllocator frameHierarchAlloc;
		D3DXFrameDestroy(m_rootBone, &frameHierarchAlloc);
	}
	SAFE_RELEASE(m_pAnimControl);
}

bool cSkinnedModel::LoadFromX(const char * fileName)
{
	cFrameHierarchyAllocator frameHierarchAlloc;
	LPD3DXFRAME rootBone = NULL;
	D3DXLoadMeshHierarchyFromXA(
		fileName,
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
	return true;
}

void cSkinnedModel::Update()
{
	m_pAnimControl->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL);
	m_rootBone->calculateCombinedTransformationMatrix(NULL);
}

void cSkinnedModel::Render(Bone* bone)
{
	if (bone==NULL) {
		Render(m_rootBone);
	}
	else {
		if (bone->pMeshContainer) {
			BoneMesh* pBoneMesh = (BoneMesh*)bone->pMeshContainer;
			if (pBoneMesh->pSkinInfo) {
				DWORD numBones = pBoneMesh->pSkinInfo->GetNumBones();
				for (int i = 0; i < numBones; ++i) {
					if (pBoneMesh->boneMatrixPtrs[i]) {
						D3DXMatrixMultiply(&pBoneMesh->currentBoneMatrices[i], &pBoneMesh->boneOffsetMatrices[i], pBoneMesh->boneMatrixPtrs[i]);
						//pBoneMesh->currentBoneMatrices[i] = *pBoneMesh->boneMatrixPtrs[i];
						//pBoneMesh->currentBoneMatrices[i] = pBoneMesh->boneOffsetMatrices[i];
					}
					else {
						pBoneMesh->currentBoneMatrices[i] = pBoneMesh->boneOffsetMatrices[i];
					}
				}

				BYTE *src = NULL, *dst = NULL;
				pBoneMesh->MeshData.pMesh->LockVertexBuffer(0, (LPVOID*)&dst);
				pBoneMesh->originalMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&src);
				pBoneMesh->pSkinInfo->UpdateSkinnedMesh(pBoneMesh->currentBoneMatrices.data(), NULL, src, dst);
				pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();
				pBoneMesh->originalMesh->UnlockVertexBuffer();

				for (size_t i = 0; i < pBoneMesh->NumMaterials; ++i) {
					int attrID = pBoneMesh->attrTable[i].AttribId;
					g_pD3DDevice->SetTexture(0, pBoneMesh->textures[attrID]);
					g_pD3DDevice->SetMaterial(&pBoneMesh->materials[attrID]);
					pBoneMesh->MeshData.pMesh->DrawSubset(attrID);
				}
			}
			else {
				for (size_t i = 0; i < pBoneMesh->NumMaterials; ++i) {
					if (pBoneMesh->originalMesh) {
						g_pD3DDevice->SetTexture(0, pBoneMesh->textures[i]);
						g_pD3DDevice->SetMaterial(&pBoneMesh->materials[i]);
						g_pD3DDevice->SetTransform(D3DTS_WORLD, &bone->combinedTransformationMatrix);

						pBoneMesh->originalMesh->DrawSubset(i);
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
	//LogDebugMessage("Animation %s\n", anim->GetName());
	//LogDebugMessage("\t Num Anim : %d\n", anim->GetNumAnimations());
	//LogDebugMessage("\tPeriod : %lf\n", anim->GetPeriod());
	//int numAnim = anim->GetNumAnimations();
	//for (int i = 0; i < numAnim; ++i) {
	//	const char* name;
	//	anim->GetAnimationNameByIndex(i, &name);
	//	LogDebugMessage("\t\t%d : %s\n",i, name);
	//}
	//LogDebugMessage("\tPeriodPos : %lf\n", anim->GetPeriodicPosition());
	m_pAnimControl->SetTrackAnimationSet(0, anim);
	//m_pAnimControl->SetTrackDesc()
	LPD3DXTRACK_DESC trackDesc;
	//m_pAnimControl->KeyPriorityBlend()
	//m_pAnimControl->KeyTrackWeight()
	anim->Release();
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
			//LogDebugMessage("Bone : %s\n", boneMesh->pSkinInfo->GetBoneName(i));
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
