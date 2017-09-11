#include "StdAfx.h"
#include "cCamera.h"

cCamera::cCamera(void)
	: m_pvTarget(NULL)
	, m_fAngleX(0.0f)
	, m_fAngleY(0.0f)
	, m_isLButtonDown(false)
	, m_fDistance(100)
	, m_vEye(0, 0, -m_fDistance)
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

	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
		&m_vEye,
		&D3DXVECTOR3(0, 0, 0), 
		&D3DXVECTOR3(0, 1, 0));
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj,
		D3DX_PI / 4.0f, 
		rc.right / (float)rc.bottom, 
		1.0f,
		1000.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
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
	
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
		&m_vEye,
		&vTarget, 
		&D3DXVECTOR3(0, 1, 0));
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}

void cCamera::MsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
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

