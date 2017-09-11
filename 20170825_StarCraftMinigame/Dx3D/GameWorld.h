#pragma once
class cCamera;
class NavigationGraph;
class HPBar;
class CountDown;
class cActionMoveVelocity;
class GameObject;
class PlayableObject;
class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	bool Init(const char* levelFile);
	void Update();
	void Render();
	bool MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	D3DXVECTOR3 GetGridCenter(int ix, int iz)const;

	void QueryObjectsInRadius(D3DXVECTOR3 pos, float r, std::vector<GameObject*>& objects);
	void BroadcastMessage(int msgType, std::vector<cVariant>& params);
private:
	void InitNavGraph(const std::vector<std::vector<bool>>& gridMap);
	void cbOnCountDownFinished();

	cActionMoveVelocity* CreateVelocityAction(D3DXVECTOR3 from, D3DXVECTOR3 to, float speed, GameObject* target);
private:
	HPBar*					 m_pHPBar[2];
	CountDown*				 m_pCountDown;
	NavigationGraph*		 m_pNavGraph;
	LPD3DXMESH				 m_pGroundMesh;
	D3DMATERIAL9			 m_mtlGround;
	cCamera*				 m_pCamera;
	PlayableObject*				 m_pPlayer;
	PlayableObject*				 m_pEnemy;
	std::vector<GameObject*> m_pObject;
	float					 m_cellSize;
	bool					m_bGameStarted;
};