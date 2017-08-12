#include "StdAfx.h"
#include "cCrtCtrl.h"


cCrtCtrl::cCrtCtrl(void)
	: m_vDir(0, 0, 1)
	, m_vPos(0, 0, 0)
	, m_bMoving(false)
	, m_lookDir(DIR_RIGHT)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cCrtCtrl::~cCrtCtrl(void)
{
}

void cCrtCtrl::Update()
{
	if(g_pKeyManager->isOnceKeyDown('W'))
	{
		m_bMoving = true;
	}
	if(g_pKeyManager->isOnceKeyDown('S'))
	{
		TurnLeft();
		TurnLeft();
	}
	if (g_pKeyManager->isOnceKeyDown('A')) 
	{
		TurnLeft();
	}
	if (g_pKeyManager->isOnceKeyDown('D'))
	{
		TurnRight();
	}

	m_vDir = D3DXVECTOR3(0, 0, 1);
	D3DXMATRIX matR;
	D3DXMatrixRotationY(&matR, GetAngle());
	D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matR);

	D3DXMATRIX matT;
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = matR * matT;
}

D3DXVECTOR3* cCrtCtrl::GetPosition()
{
	return &m_vPos;
}

float cCrtCtrl::GetAngle() const
{
	switch (m_lookDir) {
	case DIR_UP:
		return D3DX_PI;
	case DIR_RIGHT:
		return -D3DX_PI / 2;
	case DIR_DOWN:
		return 0;
	case DIR_LEFT:
		return D3DX_PI / 2;
	}
	assert(false);
}

void cCrtCtrl::TurnLeft()
{
	int dir = m_lookDir;
	dir--;
	if (dir < 0) {
		dir = NUM_DIRS - 1;
	}
	m_lookDir = static_cast<LookDirections>(dir);
}

void cCrtCtrl::TurnRight()
{
	int dir = m_lookDir;
	dir++;
	if (dir == NUM_DIRS) {
		dir = 0;
	}
	m_lookDir = static_cast<LookDirections>(dir);
}
