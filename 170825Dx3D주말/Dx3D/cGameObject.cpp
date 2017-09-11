#include "StdAfx.h"
#include "cGameObject.h"


cGameObject::cGameObject(void)
	: m_vPos(0.0f, 0, 0.0f)
	, m_pAction(NULL)
	, m_vPrevPosition(0, 0, 0)
	, m_isDirection(false)
	, m_moving(false)
	, m_vDir(0, 0, 1)
	, m_fSpeed(0.1f)
	, m_fAngle(0.0f)
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

	m_moving = false;
	if (m_pAction) {
		m_pAction->Update();
		if (!m_pAction->IsFinished()) {
			m_moving = true;
		}
	}

	if (m_isDirection)
	{
		m_vDirection = m_vPos - m_vPrevPosition;
		m_vDirection *= -1;
		if (D3DXVec3LengthSq(&m_vDirection) <= D3DX_16F_EPSILON)
			return;

		D3DXVec3Normalize(&m_vDirection, &m_vDirection);

		D3DXMATRIX matR;
		D3DXMatrixLookAtLH(&matR,
			&D3DXVECTOR3(0, 0, 0),
			&m_vDirection,
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
	m_moving = true;

	//D3DXVECTOR3 vPos = m_vPos;

	//if (g_pKeyManager->isStayKeyDown('W'))
	//{
	//	vPos = m_vPos - m_vDir * m_fSpeed;
	//	m_moving = true;
	//}
	//if (g_pKeyManager->isStayKeyDown('S'))
	//{
	//	vPos = m_vPos + m_vDir * m_fSpeed;
	//	m_moving = true;
	//}

	//if (g_pKeyManager->isStayKeyDown('A'))
	//{
	//	m_fAngle -= 0.1f;
	//	m_moving = true;
	//}

	//if (g_pKeyManager->isStayKeyDown('D'))
	//{
	//	m_fAngle += 0.1f;
	//	m_moving = true;
	//}


	//m_vPos = vPos;

	//m_vDir = D3DXVECTOR3(0, 0, 1);
	//D3DXMATRIX matR;
	//D3DXMatrixRotationY(&matR, m_fAngle);
	//D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matR);

	//D3DXMATRIX matT;
	//D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);

	//m_matWorld = matR * matT;

}

D3DXVECTOR3 * cGameObject::getPointpos(void)
{
	m_pPos = &m_vPos;

	return m_pPos;
}

void cGameObject::setDirection(D3DXVECTOR3 enemy)
{
	m_vDirection = m_vPos - enemy;
	if (D3DXVec3LengthSq(&m_vDirection) <= D3DX_16F_EPSILON)
		return;

	D3DXVec3Normalize(&m_vDirection, &m_vDirection);

	D3DXMATRIX matR;
	D3DXMatrixLookAtLH(&matR,
		&D3DXVECTOR3(0, 0, 0),
		&m_vDirection,
		&D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&matR, &matR);

	D3DXMATRIX matT;
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);

	m_matWorld = matR * matT;
}
