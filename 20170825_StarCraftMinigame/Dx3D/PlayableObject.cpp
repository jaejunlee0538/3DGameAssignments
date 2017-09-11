#include "stdafx.h"
#include "PlayableObject.h"
#include "cSkinnedMesh.h"
#include "cKeyManager.h"
#include "MessageTypes.h"

PlayableObject::PlayableObject(int maxHP, cSkinnedMesh * mesh)
	:m_pSkinnedMesh(mesh), m_maxHP(maxHP), m_currentHP(maxHP)
{
	if (m_pSkinnedMesh) {
		m_pSkinnedMesh->SetAnimationCallback(std::bind(&PlayableObject::cbAnimation, this, std::placeholders::_1));
	}
}

PlayableObject::~PlayableObject()
{
	SAFE_DELETE(m_pSkinnedMesh);
}

void PlayableObject::render() const
{
	GameObject::render();
	D3DXMATRIXA16 matWorld = getWorldMatrix();
	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pSkinnedMesh->UpdateAndRender(&matWorld);
}

void PlayableObject::update(bool isDirty)
{
	GameObject::update(isDirty);
	if (m_currentAnim != m_animWalk && isMoving()) {
		setState(STATE_WALK);
	}
	else if (m_currentAnim == m_animWalk && !isMoving()) {
		setState(STATE_IDLE);
	}

	switch (m_state) {
	case STATE_WALK:
	{
		D3DXVECTOR3 velocity = GameObject::getSpeed();
		D3DXVec3Normalize(&velocity, &velocity);
		setRotation(0, atan2(velocity.z, velocity.x), 0);
	}
		break;
	}
}

void PlayableObject::attack()
{
	if (m_state == STATE_IDLE) {
		setState(STATE_ATTACK);
	}
}

void PlayableObject::reupdateTransform()
{
	GameObject::update(false);
}

void PlayableObject::setState(States state)
{
	switch (state)
	{
	case PlayableObject::STATE_IDLE:
		m_pSkinnedMesh->SetAnimationByName(m_animIdle, {});
		m_currentAnim = m_animIdle;
		m_state = state;
		break;
	case PlayableObject::STATE_WALK:
		m_pSkinnedMesh->SetAnimationByName(m_animWalk, {});
		m_currentAnim = m_animWalk;
		m_state = state;
		break;
	case PlayableObject::STATE_ATTACK:
		m_pSkinnedMesh->SetAnimationByName(m_animAttack, { AnimCallbackKey(0.5f, ANIM_CB_ATTACK), AnimCallbackKey(0.99f, ANIM_CB_ATTACK_FINISHED) });
		m_currentAnim = m_animAttack;
		m_state = state;
		break;
	default:
		break;
	}
}

void PlayableObject::cbAnimation(int data)
{
	switch (data) {
	case ANIM_CB_ATTACK:
	{
		if (m_pWorld) {
			attackImpl();
		}
	}
		break;
	case ANIM_CB_ATTACK_FINISHED:
	{
		//Idle 애니메이션 재생
		setState(STATE_IDLE);
	}
	break;
	}
}
