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
	
	//메시지를 받아서 처리했으면 true를 반환하고 그렇지 않으면 false를 반환한다.
	bool MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void UpdateLight();
private:
	void StartNextGame();
	void StartPrevGame();
	void RestartGame();
	void GameCleared(int totalMovements);
	void GoToFirstStage();
};

