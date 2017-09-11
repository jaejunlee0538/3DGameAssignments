#include "stdafx.h"
#include "cSkinnedModel.h"
#include "cFrameHierarchyAllocator.h"


cSkinnedModel::BoneWrapper::~BoneWrapper() {
	cFrameHierarchyAllocator frameHierarchAlloc;
	D3DXFrameDestroy(rootBone, &frameHierarchAlloc);
}

cSkinnedModel::cSkinnedModel()
	: m_rootBone(NULL), m_pAnimControl(NULL)
{
	m_position = D3DXVECTOR3(0, 0, 0);
	D3DXQuaternionIdentity(&m_rotation);
	EnableControl(false);
	
}

cSkinnedModel::~cSkinnedModel()
{
	SAFE_RELEASE(m_rootBone);
	SAFE_RELEASE(m_pAnimControl);
}

cSkinnedModel * cSkinnedModel::Clone()
{
	ID3DXAnimationController * animController = nullptr;
	m_pAnimControl->CloneAnimationController(m_pAnimControl->GetMaxNumAnimationOutputs(), m_pAnimControl->GetMaxNumAnimationSets(), m_pAnimControl->GetMaxNumTracks(), m_pAnimControl->GetMaxNumEvents(),&animController);
	
	cSkinnedModel * cloned = new cSkinnedModel();
	cloned->m_rootBone = m_rootBone;
	cloned->m_animationNames = m_animationNames;
	cloned->m_pAnimControl = animController;
	
	return cloned;
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
	SAFE_RELEASE(m_rootBone);
	m_rootBone = new BoneWrapper((Bone*)rootBone);
	SetupBoneMatixPtrs(m_rootBone->rootBone);
	SetupAllAnimationNames();
	SetState(STATE_IDLE);
	return true;
}

void cSkinnedModel::Update()
{
	
	if (m_enableControl) {
		if (g_pKeyManager->isOnceKeyDown(VK_SPACE)) {
			switch (m_state) {
			case STATE_IDLE:
				SetState(STATE_ATTACK);
				break;
			}
		}

		if (m_state != STATE_ATTACK) {
			D3DXVECTOR3					moveVector;
			D3DXQUATERNION				rotationVector;
			if (g_pKeyManager->isStayKeyDown('W')) {
				moveVector = D3DXVECTOR3(0, 0, -0.1);
			}
			else if (g_pKeyManager->isStayKeyDown('S')) {
				moveVector = D3DXVECTOR3(0, 0, 0.1);
			}
			else {
				moveVector = D3DXVECTOR3(0, 0, 0);
			}

			if (g_pKeyManager->isStayKeyDown('A')) {
				D3DXQuaternionRotationYawPitchRoll(&rotationVector, -0.05, 0, 0);
			}
			else if (g_pKeyManager->isStayKeyDown('D')) {
				D3DXQuaternionRotationYawPitchRoll(&rotationVector, 0.05, 0, 0);
			}
			else {
				D3DXQuaternionIdentity(&rotationVector);
			}

			if (D3DXVec3LengthSq(&moveVector) > 0.0 || D3DXQuaternionIsIdentity(&rotationVector) == false) {
				if (m_state != STATE_RUNNING) {
					SetState(STATE_RUNNING);
				}
			}
			else {
				if (m_state != STATE_IDLE) {
					SetState(STATE_IDLE);
				}
			}

			switch (m_state) {
			case STATE_RUNNING:
			{
				TranslateLocal(moveVector.x, moveVector.y, moveVector.z);
				RotateLocal(rotationVector);
			}
			break;
			}
		}

	}

	D3DXTRACK_DESC desc, desc2;
	m_pAnimControl->GetTrackDesc(PREV_ANIM_TRACK, &desc);
	m_pAnimControl->GetTrackDesc(CURRENT_ANIM_TRACK, &desc2);
	LogDebugMessage("%.3lf %d %.3lf %.3lf | %d %.3lf %.3lf \n", m_pAnimControl->GetTime(), desc2.Enable, desc2.Position, desc2.Weight, desc.Enable, desc.Position, desc.Weight);
	

	if (desc2.Position > m_periodCurrentAnim) {
		m_pAnimControl->SetTrackPosition(CURRENT_ANIM_TRACK, desc2.Position - m_periodCurrentAnim);
	}
	if (desc2.Position > m_callbackTime && m_callbackShouldBeCalled) {
		CBAnimation();
		m_callbackShouldBeCalled = false;
	}

	if (desc2.Position > m_nextLoopCallbackTime) {
		CBAnimationOneLoop();
		m_nextLoopCallbackTime += m_periodCurrentAnim;
	}

	m_prevPosition = m_position;
}

void cSkinnedModel::Render(Bone* bone)
{
	if (bone==NULL) {
	m_pAnimControl->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL);
		D3DXMATRIX matWorld;
		D3DXMatrixRotationQuaternion(&matWorld, &m_rotation);
		matWorld._41 = m_position.x;
		matWorld._42 = m_position.y;
		matWorld._43 = m_position.z;
		m_rootBone->rootBone->calculateCombinedTransformationMatrix(&matWorld);

		Render(m_rootBone->rootBone);
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
					g_pD3DDevice->SetTexture(0, pBoneMesh->textures[i]);
					g_pD3DDevice->SetMaterial(&pBoneMesh->materials[i]);
					g_pD3DDevice->SetTransform(D3DTS_WORLD, &bone->combinedTransformationMatrix);
					pBoneMesh->originalMesh->DrawSubset(i);
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

void cSkinnedModel::SetState(CharacterStates state)
{
	LogDebugMessage("Set STate %d\n", state);
	switch (state) {
	case STATE_IDLE:
		m_state = STATE_IDLE;
		SetAnimation("Stand", 0.1);
		break;
	case STATE_ATTACK:
		m_state = STATE_ATTACK;
		SetAnimation("Attack", 0.1);
		break;
	case STATE_RUNNING:
		m_state = STATE_RUNNING;
		SetAnimation("Walk", 0.1);
		break;
	}
}

bool cSkinnedModel::SetAnimation(int idx, float transitionTime)
{
	LogDebugMessage("Animation %d\n", idx);
	ID3DXKeyframedAnimationSet* newAnim=nullptr;
	m_pAnimControl->GetAnimationSet(idx, (ID3DXAnimationSet**)&newAnim);
	if (!newAnim) {
		return false;
	}
	
	ID3DXKeyframedAnimationSet* prevAnim = nullptr;
	//현재 재생 중인 애니메이션 셋을 받아온다.
	m_pAnimControl->GetTrackAnimationSet(CURRENT_ANIM_TRACK, (ID3DXAnimationSet**)&prevAnim);
	if (newAnim == prevAnim) {
		newAnim->Release();
		prevAnim->Release();
		return true;
	}

	m_pAnimControl->UnkeyAllTrackEvents(PREV_ANIM_TRACK);
	m_pAnimControl->UnkeyAllTrackEvents(CURRENT_ANIM_TRACK);

	//현재 재생 중인 애니메이션 셋을 PREV트랙에 넣고 새로운 애니메이션을 새로운 트랙에 삽입한다.
	m_pAnimControl->SetTrackAnimationSet(CURRENT_ANIM_TRACK, newAnim);
	m_pAnimControl->SetTrackAnimationSet(PREV_ANIM_TRACK, prevAnim);
	
	m_pAnimControl->SetTrackEnable(CURRENT_ANIM_TRACK, TRUE);
	m_pAnimControl->SetTrackEnable(PREV_ANIM_TRACK, TRUE);

	double dt = prevAnim->GetPeriod() - prevAnim->GetPeriodicPosition(0.0);
	
	float currentTime = m_pAnimControl->GetTime();
	m_pAnimControl->SetTrackPosition(CURRENT_ANIM_TRACK, 0.0);
	m_pAnimControl->SetTrackPosition(PREV_ANIM_TRACK, prevAnim->GetPeriodicPosition(0.0));
	m_pAnimControl->KeyTrackPosition(PREV_ANIM_TRACK, prevAnim->GetPeriod(), currentTime + dt);

	m_pAnimControl->KeyTrackEnable(PREV_ANIM_TRACK, FALSE, currentTime + transitionTime);
	m_pAnimControl->SetTrackWeight(CURRENT_ANIM_TRACK, 0.0f);
	m_pAnimControl->SetTrackWeight(PREV_ANIM_TRACK, 1.0f);
	
	m_pAnimControl->KeyTrackWeight(CURRENT_ANIM_TRACK, 1.0f, currentTime, transitionTime, D3DXTRANSITION_LINEAR);
	m_pAnimControl->KeyTrackWeight(PREV_ANIM_TRACK, 0.0f, currentTime, transitionTime, D3DXTRANSITION_LINEAR);

	m_callbackTime = newAnim->GetPeriod() / 2.0;
	m_callbackShouldBeCalled = true;
	m_nextLoopCallbackTime = newAnim->GetPeriod();
	m_periodCurrentAnim = newAnim->GetPeriod();

	newAnim->Release();
	prevAnim->Release();
	m_currentAnimationIdx = idx;
	return true;
}

bool cSkinnedModel::SetAnimation(const std::string & animName, float transitionTime)
{
	for (int i = 0; i < m_animationNames.size(); ++i) {
		if (animName == m_animationNames[i]) {
			return SetAnimation(i, transitionTime);
		}
	}
	return false;
}

bool cSkinnedModel::GetAnimationName(int idx, std::string& name) const
{
	if (idx >= 0 && idx < m_animationNames.size()) {
		name = m_animationNames[idx];
		return true;
	}
	return false;
}

void cSkinnedModel::TranslateLocal(float dx, float dy, float dz)
{
	D3DXVECTOR3 delta(dx, dy, dz);
	D3DXMATRIX rot;
	D3DXMatrixRotationQuaternion(&rot, &m_rotation);
	D3DXVec3TransformNormal(&delta, &delta, &rot);
	Translate(delta.x, delta.y, delta.z);
}

void cSkinnedModel::Translate(float dx, float dy, float dz)
{
	m_position += D3DXVECTOR3(dx, dy, dz);
}

void cSkinnedModel::RotateLocal(D3DXQUATERNION quat)
{
	m_rotation = m_rotation * quat;
}

STDMETHODIMP cSkinnedModel::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	LogDebugMessage("CAllback\n");
	return S_OK;
}

void cSkinnedModel::CBAnimation()
{
	
}

void cSkinnedModel::CBAnimationOneLoop()
{
	switch (m_state) {
	case STATE_ATTACK:
		SetState(STATE_IDLE);
		break;
	}
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
			Bone* boneFound = (Bone*)D3DXFrameFind(m_rootBone->rootBone, boneMesh->pSkinInfo->GetBoneName(i));
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
		ID3DXKeyframedAnimationSet* animSet = nullptr;
		m_pAnimControl->GetAnimationSet(i, (ID3DXAnimationSet**)&animSet);

		m_animationNames[i] = animSet->GetName();

		//D3DXKEY_CALLBACK callback[2];
		//callback[0].Time = 0;
		//callback[0].pCallbackData = this;
		//callback[1].Time = float(animSet->GetPeriod() * 0.5*animSet->GetSourceTicksPerSecond());
		//callback[1].pCallbackData = this;

		//ID3DXKeyframedAnimationSet* newAnimSet = NULL;
		//HRESULT ret = D3DXCreateKeyframedAnimationSet(animSet->GetName(),
		//	animSet->GetSourceTicksPerSecond(), animSet->GetPlaybackType(),
		//	animSet->GetNumAnimations(), 2, callback, &newAnimSet);
		//assert(FAILED(ret) == false);
		//for (int i = 0; i < animSet->GetNumAnimations(); ++i) {
		//	std::vector<D3DXKEY_QUATERNION> quat(animSet->GetNumRotationKeys(i));
		//	std::vector<D3DXKEY_VECTOR3> translation(animSet->GetNumTranslationKeys(i));
		//	std::vector<D3DXKEY_VECTOR3> scale(animSet->GetNumScaleKeys(i));

		//	animSet->GetRotationKeys(i, quat.data());
		//	animSet->GetTranslationKeys(i, translation.data());
		//	animSet->GetScaleKeys(i, scale.data());
		//	DWORD aniIdx;
		//	newAnimSet->RegisterAnimationSRTKeys(animSet->GetName(),
		//		scale.size(), quat.size(), translation.size(),
		//		scale.data(), quat.data(), translation.data(), &aniIdx);
		//}
		//
		//ret = m_pAnimControl->UnregisterAnimationSet(animSet);
		//assert(FAILED(ret) == false);
		//ret = m_pAnimControl->RegisterAnimationSet(newAnimSet);
		//assert(FAILED(ret) == false);
		//newAnimSet->Release();
		//D3DXCreateKeyframedAnimationSet()

		//D3DXCreateKeyframedAnimationSet()
		//LogDebugMessage("%d\n", testIface == animSet);
		/*ID3DXBuffer* compressedInfo = 0;
		animSet->Compress(D3DXCOMPRESS_DEFAULT,
			0.5f, 0, &compressedInfo);

		D3DXKEY_CALLBACK callback[2];
		callback[0].Time = 0;
		callback[0].pCallbackData = this;
		callback[1].Time = float(animSet->GetPeriod() * 0.5*animSet->GetSourceTicksPerSecond());
		callback[1].pCallbackData = this;

		ID3DXCompressedAnimationSet* compressedAnimSet = 0;
		D3DXCreateCompressedAnimationSet(animSet->GetName(),
			animSet->GetSourceTicksPerSecond(),
			animSet->GetPlaybackType(), compressedInfo,
			2, callback, &compressedAnimSet);

		m_pAnimControl->UnregisterAnimationSet(animSet);
		m_pAnimControl->RegisterAnimationSet(compressedAnimSet);

		SAFE_RELEASE(compressedInfo);
		SAFE_RELEASE(compressedAnimSet);*/

		SAFE_RELEASE(animSet);
	}
}


