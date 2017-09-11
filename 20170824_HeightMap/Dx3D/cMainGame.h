#pragma once
#include "FreqEstimater.h"
#include "TickCounter.h"
class cCamera;
class cCrtCtrl;
class iMap;
class cSkinnedMesh;
class HeightMap;
class ZealotPlayer;
class Grid;
class cMainGame
{
private:
	cCamera*	m_pCamera;
	HeightMap*				m_pHeightMap;
	ZealotPlayer*			m_pZealot;
	Grid*					m_pGrid;
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetLight();
};

