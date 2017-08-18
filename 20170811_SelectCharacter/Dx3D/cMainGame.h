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
	
	//�޽����� �޾Ƽ� ó�������� true�� ��ȯ�ϰ� �׷��� ������ false�� ��ȯ�Ѵ�.
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

	//���������� �÷��̾ ����.
	bool TryPickPlayer(D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir);
	bool GetPickingPoint(int px, int py, const std::vector<ST_PNT_VERTEX>& triangles, D3DXVECTOR3& pickingPoint);

	//�÷��̾ destPos�� ������ ���� ��θ� �����ϰ� Action�� �����Ѵ�.
	void movePlayerToHere(cPlayer* pPlayer, D3DXVECTOR3 destPos);
	void moveSelectedPlayerToHere(D3DXVECTOR3 destPos);
	//from���� to�� ���� cActionMove�� �����Ѵ�.
	cActionMove* CreateActionMove(cGameObject* target, D3DXVECTOR3 from, D3DXVECTOR3 to);
	//fromPos�� toPos ���̿� ��ֹ��� �ִ��� ������ ���� Ȯ���Ѵ�.
	bool CheckRayCollisionWithObstaclesFromTo(D3DXVECTOR3 fromPos, D3DXVECTOR3 toPos, float* distance=nullptr);
	//pos���� ���̴� ��� �� ���� ����� ��带 ��ȯ�Ѵ�.
	tagNavNode* GetNearestVisibleNavNode(D3DXVECTOR3 pos);
	//pos���� ���� ����� ��带 ��ȯ�Ѵ�.
	tagNavNode* GetNearestNavNode(D3DXVECTOR3 pos);
};

