#include "StdAfx.h"
#include "cGameObject.h"


cGameObject::cGameObject(void)
	: m_vPos(0, 0, 0)
	, m_pAction(NULL)
	, m_vPrevPosition(0, 0, 0)
	, m_isDirection(false)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cGameObject::~cGameObject(void)
{
	SAFE_RELEASE(m_pAction);
}

void cGameObject::Update()
{
	m_vPrevPosition = m_vPos;

	if (m_pAction)
	{
		m_pAction->Update();
	}

	if (m_isDirection)
	{
		D3DXVECTOR3 vDirection = m_vPos - m_vPrevPosition;
		
		if (D3DXVec3LengthSq(&vDirection) <= D3DX_16F_EPSILON)
			return;

		D3DXVec3Normalize(&vDirection, &vDirection);

		D3DXMATRIX matR;
		D3DXMatrixLookAtLH(&matR, 
			&D3DXVECTOR3(0, 0, 0),
			&vDirection,
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
