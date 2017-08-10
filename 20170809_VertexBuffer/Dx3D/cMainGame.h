#pragma once

class cGrid;
class cCamera;
class cPlayer;
class cFrame;
class cMainGame
{
private:
	cGrid*			m_pGrid;
	cCamera*		m_pCamera;
	cPlayer*		m_pPlayer;

	LPD3DXFONT		m_pFont;

	std::vector<cFrame*>		m_gameMap;
	std::vector<cFrame*>		m_floorCollision;

	int							m_Firstframe;
	int							m_Lastframe;
	float						m_FrameSpeed;
	int							m_TenUnitStage;
	int							m_OneUnitStage;
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	
	//메시지를 받아서 처리했으면 true를 반환하고 그렇지 않으면 false를 반환한다.
	bool MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void UpdateLight();
};

