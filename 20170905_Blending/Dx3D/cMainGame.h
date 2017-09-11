#pragma once

class cGrid;
class cCamera;

class cMainGame
{
private:
	cGrid*					m_pGrid;
	cCamera*				m_pCamera;
	vector<ST_PCT_VERTEX>	m_vecVertex;
	vector<ST_PCT_VERTEX>	m_vecVertex1;
	vector<ST_PCT_VERTEX>	m_vecVertex2;

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetLight();
};

