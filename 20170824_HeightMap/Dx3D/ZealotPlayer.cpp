#include "stdafx.h"
#include "ZealotPlayer.h"
#include "cSkinnedMesh.h"
#include "cKeyManager.h"

ZealotPlayer::ZealotPlayer()
	:m_pSkinnedMesh(NULL)
{
}

ZealotPlayer::~ZealotPlayer()
{
	SAFE_DELETE(m_pSkinnedMesh);
}

void ZealotPlayer::setup(cSkinnedMesh * mesh)
{
	m_pSkinnedMesh = mesh;
	setState(STATE_IDLE);
}

void ZealotPlayer::render() const
{
	GameObject::render();
	D3DXMATRIXA16 matWorld = getWorldMatrix();
	m_pSkinnedMesh->UpdateAndRender(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	//m_pSphere->DrawSubset(0);
}

void ZealotPlayer::update(bool isDirty)
{
	if (g_pKeyManager->isStayKeyDown('W')) {
		GameObject::setLocalSpeed(0,0, -5.0f);
	}
	else if (g_pKeyManager->isStayKeyDown('S')) {
		GameObject::setLocalSpeed(0, 0, 5.0f);
	}
	else {
		GameObject::setLocalSpeed(0, 0, 0);
	}
	if (g_pKeyManager->isStayKeyDown('A')) {
		GameObject::setAngularSpeed(0,-1.0f, 0);
	}
	else if (g_pKeyManager->isStayKeyDown('D')) {
		GameObject::setAngularSpeed(0, 1.0f, 0);
	}
	else {
		GameObject::setAngularSpeed(0, 0, 0);
	}

	GameObject::update(isDirty);

	switch (m_state) {
	case STATE_IDLE:
		if (isMoving()) {
			setState(STATE_WALK);
		}
		break;
	case STATE_ATTACK:

		break;
	case STATE_WALK:
		if (!isMoving()) {
			setState(STATE_IDLE);
		}
		break;
	}
}

void ZealotPlayer::reupdateTransform()
{
	GameObject::update(false);
}

void ZealotPlayer::setState(ZealotState state)
{
	if (m_state == state)
		return;
	m_state = state;
	switch (m_state) {
	case STATE_IDLE:
		m_pSkinnedMesh->SetAnimationIndex(4);
		break;
	case STATE_ATTACK:
		m_pSkinnedMesh->SetAnimationIndex(1);
		break;
	case STATE_WALK:
		m_pSkinnedMesh->SetAnimationIndex(3);
		break;
	}
}
