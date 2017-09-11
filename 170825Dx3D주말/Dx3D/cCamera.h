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
	D3DXMATRIX		m_matProj;
	D3DXMATRIX		m_matView;
	D3DXMATRIX		m_matViewInv;
	vector<ST_PC_VERTEX>	m_vecSquareVertex;
	bool			m_floorCheck;
	float			m_Distance;

public:
	cCamera(void);
	~cCamera(void);

	void Setup(D3DXVECTOR3* pvTarget = NULL);
	void Update();
	void MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool getPicking(OUT D3DXVECTOR3& pos);
	void setSquarevertex(vector<ST_PC_VERTEX> SquareVertex) { m_vecSquareVertex = SquareVertex; }
	D3DXVECTOR3 getEyePos() { return m_vEye; }
};

