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
	, m_fDistance(30)
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

void cCamera::GetPickingRay(int px, int py, OUT D3DXVECTOR3 & rayDir, OUT D3DXVECTOR3 & rayPos)
{
	D3DVIEWPORT9 viewport;
	g_pD3DDevice->GetViewport(&viewport);
	//ViewSpaceø°º≠¿« ±§º±¿« πÊ«‚ ∫§≈Õ
	D3DXVECTOR3 rayDirection;
	rayDirection.x = ((2.0f*px) / viewport.Width - 1.0f) / m_matProj._11;
	rayDirection.y = ((-2.0f*py) / viewport.Height + 1.0f) /m_matProj._22;
	rayDirection.z = 1.0f;

	//±§º±¿« πÊ«‚ ∫§≈Õ∏¶ World¡¬«•∞Ë∑Œ ∫Ø»Ø
	D3DXVec3TransformNormal(&rayDirection, &rayDirection, &m_matViewInv);
	D3DXVec3Normalize(&rayDirection, &rayDirection);
	
	//
	rayDir = rayDirection;
	rayPos = m_vEye;
}

void cCamera::Setup(D3DXVECTOR3 vTarget /*= NULL*/)
{
	m_ptPrevMouse.x = 0;
	m_ptPrevMouse.y = 0;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	m_vTarget = vTarget;
	UpdateViewMatrix();

	D3DXMatrixPerspectiveFovLH(&m_matProj,
		D3DX_PI / 4.0f, 
		rc.right / (float)rc.bottom, 
		1.0f,
		1000.0f);
}

void cCamera::Update(D3DXVECTOR3 playerPos)
{
	m_vTarget = playerPos;
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
			return false;
		}
		break;
	case WM_LBUTTONUP:
		{
			m_isLButtonDown = false;
			return false;
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

				m_fAngleY = NormalizeAngle(m_fAngleY);

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
	D3DXMatrixLookAtLH(&m_matView,
		&m_vEye,
		&m_vTarget,
		&D3DXVECTOR3(0, 1, 0));
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);
	
	D3DXMatrixInverse(&m_matViewInv, 0, &m_matView);
}

