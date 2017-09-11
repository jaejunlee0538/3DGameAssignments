#include "StdAfx.h"
#include "cCamera.h"

cCamera::cCamera(void)
	: m_pvTarget(NULL)
	, m_fAngleX(0.3f)
	, m_fAngleY(0.0f)
	, m_isLButtonDown(false)
	, m_fDistance(5)
	, m_vEye(0, 5, -m_fDistance),
	m_bFixViewFrustum(false)
{
	
}

cCamera::~cCamera(void)
{
}

void cCamera::Setup(D3DXVECTOR3* pvTarget /*= NULL*/)
{
	m_ptPrevMouse.x = 0;
	m_ptPrevMouse.y = 0;

	m_pvTarget = pvTarget;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	
	D3DXMatrixLookAtLH(&m_matView,
		&m_vEye,
		&D3DXVECTOR3(0, 0, 0), 
		&D3DXVECTOR3(0, 1, 0));
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);
	D3DXMatrixInverse(&m_matViewInv, NULL, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj,
		D3DX_PI / 4.0f, 
		rc.right / (float)rc.bottom, 
		1.0f,
		1000.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
	D3DXMatrixInverse(&m_matProjInv, NULL, &m_matProj);
	CalculateFrustumPlanes();
}

void cCamera::Update()
{
	D3DXMATRIX matRX;
	D3DXMatrixRotationX(&matRX, m_fAngleX);
	D3DXMATRIX matRY;
	D3DXMatrixRotationY(&matRY, m_fAngleY);
	D3DXMATRIX matR = matRX * matRY;

	m_vEye = D3DXVECTOR3(0, 0, -m_fDistance);
	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matR);
	
	D3DXVECTOR3 vTarget = D3DXVECTOR3(0, 0, 0);
	if (m_pvTarget)
	{
		vTarget = *m_pvTarget;
	}
	
	m_vEye = m_vEye + vTarget;
	
	D3DXMatrixLookAtLH(&m_matView,
		&m_vEye,
		&vTarget, 
		&D3DXVECTOR3(0, 1, 0));
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);
	D3DXMatrixInverse(&m_matViewInv, NULL, &m_matView);

	if (m_bFixViewFrustum == false) {
		CalculateFrustumPlanes();
	}
}

void cCamera::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_SPACE) {
			m_bFixViewFrustum = true;
		}
	}
	break;
	case WM_KEYUP:
	{
		if (wParam == VK_SPACE) {
			m_bFixViewFrustum = false;
		}
	}
	break;
	case WM_LBUTTONDOWN:
		{
			m_isLButtonDown = true;
			m_ptPrevMouse.x = LOWORD(lParam);
			m_ptPrevMouse.y = HIWORD(lParam);
		}
		break;
	case WM_LBUTTONUP:
		{
			m_isLButtonDown = false;
		}
		break;
	case WM_MOUSEMOVE:
		{
			if(m_isLButtonDown)
			{
				POINT ptCurrMouse;
				ptCurrMouse.x = LOWORD(lParam);
				ptCurrMouse.y = HIWORD(lParam);

				float fDeltaX = (ptCurrMouse.x - m_ptPrevMouse.x) / 50.f;
				float fDeltaY = (ptCurrMouse.y - m_ptPrevMouse.y) / 50.f;

				m_fAngleY += fDeltaX;
				m_fAngleX += fDeltaY;

				if(m_fAngleX < -D3DX_PI / 2.0f + 0.0001f)
					m_fAngleX = -D3DX_PI / 2.0f + 0.0001f;
				
				if(m_fAngleX >  D3DX_PI / 2.0f - 0.0001f)
					m_fAngleX =  D3DX_PI / 2.0f - 0.0001f;

				m_ptPrevMouse = ptCurrMouse;
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			int nWheel = GET_WHEEL_DELTA_WPARAM(wParam);
			m_fDistance -= (nWheel / 120.f);
		}
		break;
	}
}

bool cCamera::CheckFrustumCulling(const D3DXVECTOR3 & center, float radius)
{
	for (size_t i = 0; i < 6; ++i) {
		float distance = D3DXPlaneDotCoord(&m_frustumPlanes[i], &center);
		if (distance - radius > 0.0) {
			return false;
		}
	}
	return true;
}

void cCamera::CalculateFrustumPlanes()
{
	D3DXVECTOR3 cube[8] = {
		{-1, 1, 0},{1, 1, 0},{1, -1, 0},{-1, -1,0},
		{ -1,1,1 },{ 1,1,1 },{ 1,-1,1 },{ -1,-1,1 } };

	D3DXMATRIX viewProjInv = m_matProjInv * m_matViewInv;
	for (size_t i = 0; i < 8; ++i) {
		D3DXVec3TransformCoord(&cube[i], &cube[i], &viewProjInv);
	}

	D3DXPlaneFromPoints(&m_frustumPlanes[0], &cube[0], &cube[1], &cube[2]);
	D3DXPlaneFromPoints(&m_frustumPlanes[1], &cube[0], &cube[4], &cube[5]);
	D3DXPlaneFromPoints(&m_frustumPlanes[2], &cube[1], &cube[5], &cube[6]);
	D3DXPlaneFromPoints(&m_frustumPlanes[3], &cube[3], &cube[2], &cube[6]);
	D3DXPlaneFromPoints(&m_frustumPlanes[4], &cube[3], &cube[7], &cube[4]);
	D3DXPlaneFromPoints(&m_frustumPlanes[5], &cube[4], &cube[7], &cube[6]);

	//assert(D3DXVec3LengthSq(&D3DXVECTOR3(m_frustumPlanes[0].a, m_frustumPlanes[1].b, m_frustumPlanes[2].c)) - 1.0 < 0.0000001);
}

