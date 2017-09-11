#pragma once
#include "FreqEstimater.h"
#include "TickCounter.h"
class cCamera;
class cCrtCtrl;
class iMap;
class cSkinnedMesh;
class HeightMap;
class PlayableObject;
class Grid;
class GameWorld;
class cMainGame
{
private:
	GameWorld*				m_pGameWorld;
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetLight();
};

