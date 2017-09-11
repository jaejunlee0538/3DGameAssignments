#include "StdAfx.h"
#include "cCrtCtrl.h"
#include "iMap.h"

cCrtCtrl::cCrtCtrl(void)
	: m_vDir(0, 0, 1)
	, m_vPos(0, 0, 0)
	, m_fSpeed(0.1f)
	, m_fAngle(0.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cCrtCtrl::~cCrtCtrl(void)
{
}

void cCrtCtrl::Update(iMap* pMap/* = NULL*/)
{
	D3DXVECTOR3 vPos = m_vPos;

	if(GetKeyState('W') & 0x8000)
	{
		vPos = m_vPos + m_vDir * m_fSpeed;
	}
	if(GetKeyState('S') & 0x8000)
	{
		vPos = m_vPos - m_vDir * m_fSpeed;
	}

	if(GetKeyState('A') & 0x8000)
	{
		m_fAngle -= 0.1f;
	}

	if(GetKeyState('D') & 0x8000)
	{
		m_fAngle += 0.1f;
	}
	
	if (pMap)
	{
		if(pMap->GetHeight(vPos.x, vPos.y, vPos.z))
		{
			m_vPos = vPos;
		}
	}
	else
	{
		m_vPos = vPos;
	}

	m_vDir = D3DXVECTOR3(0, 0, 1);
	D3DXMATRIX matR;
	D3DXMatrixRotationY(&matR, m_fAngle);
	D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matR);

	D3DXMATRIX matT;
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = matR * matT;
}

D3DXVECTOR3* cCrtCtrl::GetPosition()
{
	return &m_vPos;
}
