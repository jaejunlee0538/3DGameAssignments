#pragma once
#include "NavigationGraph.h"
class cGrid;
class cCamera;
class cPlayer;
class cFrame;
class cActionSeq;
class cActionMove;
class cMainGame
{
private:
	cGrid*		m_pGrid;
	cCamera*	m_pCamera;
	cPlayer*	m_pPlayer;

	LPD3DXFONT	m_pFont;

	cFrame*		m_pFloor;
	std::vector<cFrame*>		m_vecObstacle;

	bool						m_navGraphLoaded;
	NavigationGraph				m_navGraph;
	NavigationGraphRenderer	    m_graphRender;
	bool						m_connectingEdge;
	int							m_pickedNode[2];
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
	bool GetPickingPoint(int px, int py, const std::vector<ST_PNT_VERTEX>& triangles, D3DXVECTOR3& pickingPoint);
	void movePlayerToHere(D3DXVECTOR3 pickingPos);
	cActionMove* CreateActionMove(D3DXVECTOR3 from, D3DXVECTOR3 to);
	bool CheckRayCollisionWithObstacles(D3DXVECTOR3 fromPos, D3DXVECTOR3 toPos, float* distance=nullptr);
	tagNavNode* GetNearestVisibleNavNode(D3DXVECTOR3 pos);
};

