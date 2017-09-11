#pragma once

class cGrid;
class cCamera;
class cMainGame
{
private:
	cGrid*					m_pGrid;
	cCamera*				m_pCamera;
	vector<ST_PC_VERTEX>	m_vecVertex;
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetLight();
};

