#include "stdafx.h"
#include "cPlayer.h"
#include "cFrame.h"
#include "cAseLoader.h"
#include "cKeyManager.h"
#define GRAVITY	50.0f
cPlayer::cPlayer()
{
	m_bRunning = false;
	D3DXMatrixIdentity(&m_playerOffset);
	D3DXMatrixIdentity(&m_matWorld);
}

cPlayer::~cPlayer()
{
	if (m_pRunning) {
		m_pRunning->Destroy();
	}
	if (m_pIdle) {
		m_pIdle->Destroy();
	}
}

void cPlayer::Init()
{
	{
		cAseLoader aseLoader;
		m_pRunning = aseLoader.Load("woman/woman_01_all.ASE");
	}
	{
		cAseLoader aseLoader;
		m_pIdle = aseLoader.Load("woman/woman_01_all_stand.ASE");
	}

	D3DXMatrixRotationY(&m_playerOffset, D3DX_PI);
}

void cPlayer::Update(DWORD time)
{
	cGameObject::Update(time);
	if (D3DXVec3LengthSq(&m_vMoveDirection) > 0.001) {
		m_bRunning = true;
	}
	else {
		m_bRunning = false;
	}
	if (m_pRunning) {
		m_pRunning->Update(time, &(m_playerOffset*m_matWorld));
	}
	if (m_pIdle) {
		m_pIdle->Update(time, &(m_playerOffset*m_matWorld));
	}
}

void cPlayer::Render()
{
	if (m_bRunning) {
		m_pRunning->RenderMesh();
	}
	else {
		m_pIdle->RenderMesh();
	}
}
