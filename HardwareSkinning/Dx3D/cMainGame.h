#pragma once

class cGrid;
class cCamera;
class cSkinnedModel;

class cMainGame
{
private:
	cGrid*						m_pGrid;
	cCamera*					m_pCamera;
	cSkinnedModel*				m_pZealot;
	LPD3DXFONT					m_pFont;
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	
	//�޽����� �޾Ƽ� ó�������� true�� ��ȯ�ϰ� �׷��� ������ false�� ��ȯ�Ѵ�.
	bool MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	void InitFont();
	void InitZealot();
	void InitCamera();
	void InitGrid();
	void InitLight();

	void DrawTextAnimationList();
};

