#pragma once
#include "FreqEstimater.h"
#include "TickCounter.h"
class cGrid;
class cCamera;
class cCrtCtrl;
class iMap;
class cSkinnedMesh;

class cMainGame
{
private:
	cGrid*		m_pGrid;
	cCamera*	m_pCamera;
	cCrtCtrl*	m_pCrtCtrl;
	LPD3DXFONT	m_pFont;
	iMap*		m_pMap;

	vector<cSkinnedMesh*>	m_vecSkinnedMesh;
	FreqEstimater			m_fpsCalculator;
	TickCounter				m_tickCounter;
public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetLight();
};

