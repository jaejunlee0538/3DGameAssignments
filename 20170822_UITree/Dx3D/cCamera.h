#pragma once
class cCamera
{
private:
	D3DXVECTOR3*	m_pvTarget;
	float			m_fAngleX;
	float			m_fAngleY;
	POINT			m_ptPrevMouse;
	bool			m_isLButtonDown;
	D3DXVECTOR3		m_vEye;
	
	float			m_fDistance;

	D3DXPLANE		m_frustumPlanes[6];
	D3DXMATRIX		m_matProj;
	D3DXMATRIX		m_matProjInv;
	D3DXMATRIX		m_matView;
	D3DXMATRIX		m_matViewInv;
	bool			m_bFixViewFrustum;
public:
	cCamera(void);
	~cCamera(void);

	void Setup(D3DXVECTOR3* pvTarget = NULL);
	void Update();
	void MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool CheckFrustumCulling(const D3DXVECTOR3& center, float radius);

private:
	void CalculateFrustumPlanes();
};

