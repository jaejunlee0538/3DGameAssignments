#pragma once
#include "LinearInterpolater.h"
#include "SokobanGame.h"

class cGrid;
class cCamera;
class cCrtCtrl;
class cGroup;
class cLerp;
class cFrame;
class cAseLoader;
class cMapLoader;
class cButton;
class cMainGame
{
private:
	std::vector<cButton*> m_vecButtons;

	SokobanGame m_sokoban;
	cGrid*		m_pGrid;
	cCamera*	m_pCamera;

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
	
	//�޽����� �޾Ƽ� ó�������� true�� ��ȯ�ϰ� �׷��� ������ false�� ��ȯ�Ѵ�.
	bool MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void UpdateLight();
private:
	void StartNextGame();
	void StartPrevGame();
	void RestartGame();
	void GameCleared(int totalMovements);
	void GoToFirstStage();
};

