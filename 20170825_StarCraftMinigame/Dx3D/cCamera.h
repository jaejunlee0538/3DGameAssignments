#pragma once
class cCamera
{
private:
	float			m_fAngleX;
	float			m_fAngleY;
	POINT			m_ptPrevMouse;
	bool			m_isLButtonDown;
	D3DXVECTOR3		m_vEye;
	float			m_fDistance;
	D3DXVECTOR3		m_vTarget;

	D3DXMATRIX		m_matProj;
	D3DXMATRIX		m_matView;
	D3DXMATRIX		m_matViewInv;
public:
	cCamera(void);
	~cCamera(void);

	D3DXVECTOR3 GetLookDir();
	D3DXVECTOR3 GetPosition() { return m_vEye; }
	void GetPickingRay(int px, int py, OUT D3DXVECTOR3& rayDir, OUT D3DXVECTOR3& rayPos);
	void Setup(D3DXVECTOR3 vTarget);
	void Update(D3DXVECTOR3 playerPos);
	bool MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void UpdateViewMatrix();
};

