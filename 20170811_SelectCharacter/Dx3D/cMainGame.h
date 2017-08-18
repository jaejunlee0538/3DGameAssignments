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
	cGrid*						m_pGrid;
	cCamera*					m_pCamera;
	std::vector<cPlayer*>		m_pPlayers;
	std::set<cPlayer*>			m_selectedPlayers;

	D3DMATERIAL9				m_mtlSelectionCone;
	ID3DXMesh*					m_pMeshSelectionCone;

	cFrame*						m_pFloor;
	std::vector<cFrame*>		m_vecObstacle;

	bool						m_navGraphLoaded;
	NavigationGraph	*			m_navGraph;
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
private:
	void InitGraph();
	void InitFloor();
	void InitObstacles();
	void InitPlayer();
	void InitCamera();
	void InitGrid();
	void InitLight();

	void DrawSelectCones();

	//광선가지고 플레이어를 선택.
	bool TryPickPlayer(D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir);
	bool GetPickingPoint(int px, int py, const std::vector<ST_PNT_VERTEX>& triangles, D3DXVECTOR3& pickingPoint);

	//플레이어를 destPos로 보내기 위해 경로를 생성하고 Action을 생성한다.
	void movePlayerToHere(cPlayer* pPlayer, D3DXVECTOR3 destPos);
	void moveSelectedPlayerToHere(D3DXVECTOR3 destPos);
	//from에서 to로 가는 cActionMove를 생성한다.
	cActionMove* CreateActionMove(cGameObject* target, D3DXVECTOR3 from, D3DXVECTOR3 to);
	//fromPos와 toPos 사이에 장애물이 있는지 광선을 쏴서 확인한다.
	bool CheckRayCollisionWithObstaclesFromTo(D3DXVECTOR3 fromPos, D3DXVECTOR3 toPos, float* distance=nullptr);
	//pos에서 보이는 노드 중 가장 가까운 노드를 반환한다.
	tagNavNode* GetNearestVisibleNavNode(D3DXVECTOR3 pos);
	//pos에서 가장 가까운 노드를 반환한다.
	tagNavNode* GetNearestNavNode(D3DXVECTOR3 pos);
};

