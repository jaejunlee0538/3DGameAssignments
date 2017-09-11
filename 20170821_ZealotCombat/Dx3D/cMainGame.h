#pragma once

class cGrid;
class cCamera;
class cSkinnedModel;

class cMainGame
{
private:
	cGrid*						m_pGrid;
	cCamera*					m_pCamera;
	std::vector<cSkinnedModel*> m_pZealots;
	LPD3DXFONT					m_pFont;
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	
	//메시지를 받아서 처리했으면 true를 반환하고 그렇지 않으면 false를 반환한다.
	bool MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	void InitFont();
	void InitZealot();
	void InitCamera();
	void InitGrid();
	void InitLight();

	void DrawTextAnimationList();
};

