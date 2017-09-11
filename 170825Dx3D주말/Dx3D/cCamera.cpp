#include "StdAfx.h"
#include "cCamera.h"

cCamera::cCamera(void)
	: m_pvTarget(NULL)
	, m_fAngleX(D3DX_PI / 3)
	, m_fAngleY(0.0f)
	, m_isLButtonDown(false)
	, m_fDistance(15)
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


	D3DXMatrixLookAtLH(&m_matView,
		&m_vEye,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 1, 0));
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);


	D3DXMatrixPerspectiveFovLH(&m_matProj,
		D3DX_PI / 4.0f,
		rc.right / (float)rc.bottom,
		1.0f,
		1000.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
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
	D3DXMatrixInverse(&m_matViewInv, 0, &m_matView);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);
}

void cCamera::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		_ptMouse.x = static_cast<float>(LOWORD(lParam));
		_ptMouse.y = static_cast<float>(HIWORD(lParam));

		if (m_isLButtonDown)
		{
			POINT ptCurrMouse;
			ptCurrMouse.x = LOWORD(lParam);
			ptCurrMouse.y = HIWORD(lParam);

			float fDeltaX = (ptCurrMouse.x - m_ptPrevMouse.x) / 50.f;
			float fDeltaY = (ptCurrMouse.y - m_ptPrevMouse.y) / 50.f;

			m_fAngleY += fDeltaX;
			m_fAngleX += fDeltaY;

			if (m_fAngleX < -D3DX_PI / 2.0f + 0.0001f)
				m_fAngleX = -D3DX_PI / 2.0f + 0.0001f;

			if (m_fAngleX >  D3DX_PI / 2.0f - 0.0001f)
				m_fAngleX = D3DX_PI / 2.0f - 0.0001f;

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

bool cCamera::getPicking(OUT D3DXVECTOR3& pos)
{
	D3DXVECTOR3 MouserDir;

	D3DVIEWPORT9 vp;
	g_pD3DDevice->GetViewport(&vp);

	POINT win;
	win.x = vp.Width;
	win.y = vp.Height;


	MouserDir.x = ((((2.0 * _ptMouse.x) / win.x) - 1)*(1 / m_matProj._11));
	MouserDir.y = ((((-2.0 * _ptMouse.y) / win.y) + 1)*(1 / m_matProj._22));
	MouserDir.z = 1.0f;

	D3DXVECTOR3 WorldDir;
	D3DXVec3TransformNormal(&WorldDir, &MouserDir, &m_matViewInv);
	D3DXVec3Normalize(&WorldDir, &WorldDir);

	D3DXVECTOR3 laserPos, laserLook;
	laserPos = m_vEye;
	laserLook = WorldDir;


	for (size_t i = 0; i < m_vecSquareVertex.size(); i += 3) {

		m_floorCheck = D3DXIntersectTri(&m_vecSquareVertex[i].p, &m_vecSquareVertex[i + 1].p,
			&m_vecSquareVertex[i + 2].p, &laserPos, &laserLook, NULL, NULL, &m_Distance);

		if (m_floorCheck) {
			break;
		}
	}
	if (m_floorCheck) {
		pos = m_vEye + m_Distance*WorldDir;

		return true;
	}
	return false;
}

