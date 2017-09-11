#include "StdAfx.h"
#include "cGameObject.h"
#include "ColliderBase.h"

cGameObject::cGameObject(void)
	: m_vPos(0, 0, 0)
	, m_vPrevPosition(0, 0, 0)
	, m_isDirection(true)
	, m_pCollider(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cGameObject::~cGameObject(void)
{
	SAFE_DELETE(m_pCollider);
}

void cGameObject::Update(DWORD time)
{
	m_vMoveDirection = m_vPos - m_vPrevPosition;
	if (m_isDirection)
	{		
		if (D3DXVec3LengthSq(&m_vMoveDirection) <= D3DX_16F_EPSILON)
		{
			m_matWorld._41 = m_vPos.x;
			m_matWorld._42 = m_vPos.y;
			m_matWorld._43 = m_vPos.z;
			m_bMoving = false;
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
		m_bMoving = true;
	}
	else
	{
		D3DXMatrixTranslation(&m_matWorld, m_vPos.x, m_vPos.y, m_vPos.z);
	}
	m_vPrevPosition = m_vPos;
}

void cGameObject::SetCollider(ColliderBase * collider)
{
	SAFE_DELETE(m_pCollider);
	m_pCollider = collider;
	if (m_pCollider) {
		m_pCollider->SetParent(this);
	}
}

bool cGameObject::CheckRayCollision(D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir, float * distance) const
{
	if (m_pCollider == NULL) {
		return false;
	}
	return m_pCollider->CheckRayCollision(rayPos, rayDir, distance);
}
