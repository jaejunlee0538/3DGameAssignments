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
public:
	cCamera(void);
	~cCamera(void);

	D3DXVECTOR3 GetLookDir();

	void Setup(D3DXVECTOR3 vTarget);
	void Update(D3DXVECTOR3 playerPos, float angle);
	bool MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void UpdateViewMatrix();
};

