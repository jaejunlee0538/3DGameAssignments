#include "StdAfx.h"
#include "cGameObject.h"


cGameObject::cGameObject(void)
	: m_vPos(0, 0, 0)
	, m_pAction(NULL)
	, m_vPrevPosition(0, 0, 0)
	, m_isDirection(true)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cGameObject::~cGameObject(void)
{
	SAFE_RELEASE(m_pAction);
}

void cGameObject::Update(DWORD time)
{
	m_vPrevPosition = m_vPos;

	if (m_pAction){
		m_pAction->Update();
	}
	m_vMoveDirection = m_vPos - m_vPrevPosition;
	if (m_isDirection)
	{		
		if (D3DXVec3LengthSq(&m_vMoveDirection) <= D3DX_16F_EPSILON)
		{
			m_matWorld._41 = m_vPos.x;
			m_matWorld._42 = m_vPos.y;
			m_matWorld._43 = m_vPos.z;
			return;
		}

		D3DXVec3Normalize(&m_vMoveDirection, &m_vMoveDirection);

		D3DXMATRIX matR;
		D3DXMatrixLookAtLH(&matR, 
			&D3DXVECTOR3(0, 0, 0),
			&m_vMoveDirection,
			&D3DXVECTOR3(0, 1, 0));
		D3DXMatrixTranspose(&matR, &matR);
		
		D3DXMATRIX matT;
		D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);

		m_matWorld = matR * matT;
	}
	else
	{
		D3DXMatrixTranslation(&m_matWorld, m_vPos.x, m_vPos.y, m_vPos.z);
	}
}
