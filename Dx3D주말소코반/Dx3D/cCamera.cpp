#include "StdAfx.h"
#include "cCamera.h"

float NormalizeAngle(float angle) {
	if (angle < 0.0)
		return D3DX_PI * 2 - angle;
	else if (angle >= 2*D3DX_PI)
		return angle - 2 * D3DX_PI;
	return angle;
}

cCamera::cCamera(void)
	:m_fAngleX(D3DX_PI / 4)
	, m_fAngleY(D3DX_PI)
	, m_isLButtonDown(false)
	, m_fDistance(15)
	, m_vEye(0, 0, -m_fDistance)
{
}

cCamera::~cCamera(void)
{
}

D3DXVECTOR3 cCamera::GetLookDir()
{
	D3DXVECTOR3 vLook = m_vTarget - m_vEye;
	D3DXVec3Normalize(&vLook, &vLook);
	return vLook;
}

void cCamera::Setup(D3DXVECTOR3 vTarget /*= NULL*/)
{
	m_ptPrevMouse.x = 0;
	m_ptPrevMouse.y = 0;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	m_vTarget = vTarget;
	UpdateViewMatrix();

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj,
		D3DX_PI / 4.0f, 
		rc.right / (float)rc.bottom, 
		1.0f,
		1000.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void cCamera::Update(D3DXVECTOR3 playerPos, float angle)
{
	m_vTarget = playerPos;
	
	float deltaAngle = angle - m_fAngleY;
	
	if (deltaAngle > D3DX_PI)
		deltaAngle = deltaAngle - 2 * D3DX_PI;
	else if (deltaAngle < -D3DX_PI)
		deltaAngle = 2 * D3DX_PI + deltaAngle;
	m_fAngleY += deltaAngle*0.1f;
	m_fAngleY = NormalizeAngle(m_fAngleY);
	//LogDebugMessage("PlayerAngle : %.1f, CameraAngle : %.1f\n", D3DXToDegree(angle), D3DXToDegree(m_fAngleY));

	UpdateViewMatrix();
}


bool cCamera::MsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		{
			m_isLButtonDown = true;
			m_ptPrevMouse.x = LOWORD(lParam);
			m_ptPrevMouse.y = HIWORD(lParam);
			return true;
		}
		break;
	case WM_LBUTTONUP:
		{
			m_isLButtonDown = false;
			return true;
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

				//m_fAngleY += fDeltaX;
				m_fAngleX += fDeltaY;

				//m_fAngleY = NormalizeAngle(m_fAngleY);

				if(m_fAngleX < -D3DX_PI / 2.0f + 0.0001f)
					m_fAngleX = -D3DX_PI / 2.0f + 0.0001f;
				
				if(m_fAngleX >  D3DX_PI / 2.0f - 0.0001f)
					m_fAngleX =  D3DX_PI / 2.0f - 0.0001f;

				m_ptPrevMouse = ptCurrMouse;
				return true;
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			int nWheel = GET_WHEEL_DELTA_WPARAM(wParam);
			m_fDistance -= (nWheel / 120.f);
			return true;
		}
		break;
	}
	return false;
}

void cCamera::UpdateViewMatrix()
{
	m_vEye.y = m_vTarget.y + m_fDistance * sin(m_fAngleX);
	m_vEye.x = m_vTarget.x + m_fDistance * cos(m_fAngleX) * sin(m_fAngleY);
	m_vEye.z = m_vTarget.z + m_fDistance * cos(m_fAngleX) * cos(m_fAngleY);
	//LogDebugMessage("x y z : %.3f %.3f %.3f\n", m_vEye.x, m_vEye.y, m_vEye.z);
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
		&m_vEye,
		&m_vTarget,
		&D3DXVECTOR3(0, 1, 0));
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}

