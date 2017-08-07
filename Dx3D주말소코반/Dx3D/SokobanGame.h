#pragma once
#include "LinearInterpolater.h"
#include <functional>
#include <vector>
#include <memory>
#include "SokobanObject.h"
#include "SokobanCommands.h"

class SokobanGame
{
public:
	SokobanGame();
	~SokobanGame();

	//��� ���ӿ��� ������ �� �ִ� ���ҽ��� �ε��Ѵ�.
	void Init();
	//mapFile�� ���� ���ο� ������ �����Ѵ�.
	void LoadGame(const char* mapFile);
	//���� ���� ���� �����͸� ��� �����Ѵ�.
	void Reset();
	//���� ���� ���ҽ����� ��ε��Ѵ�.
	void Uninitalize();
	//������ Ŭ�����ϸ� true�� ��ȯ�Ѵ�.
	bool IsGameCleared();
	//������Ʈ
	void Update(int time);
	//����
	void Render();

	void Redo();
	void Undo();

	GridObject* GetPlayer() { return m_pPlayer; }
	GridObject* GetGridObject(int ix, int iy) { return m_mapFloor2[iy][ix]; }
	void SetGridObject(int ix, int iy, GridObject* obj) {
		m_mapFloor2[iy][ix] = obj;
	}

	void SetStageLoadCallback(std::function<void(std::string stageName)> cbStageLoad) {
		m_cbStageLoad = cbStageLoad;
	}
	//
	void SetGameClearCallback(std::function<void(int totalMoveCount)> cbGameClear) {
		m_cbGameClear = cbGameClear;
	}
	//
	void SetMovecountUpdateCallback(std::function<void(int count)> cbMovementUpdate) {
		m_cbMovementUpdate = cbMovementUpdate;
	}
	//�޽��� ���ν���(������ �ƹ� ��� ����)
	bool MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//�÷��̾��� xyz��ġ�� ��ȯ
	D3DXVECTOR3 GetPlayerPosition();
	float GetPlayerLookAngle();
	//�� �̵� Ƚ���� ��ȯ
	int GetMovementCount() { return m_nMovements; }
private:
	void InsertAndDoCommand(std::shared_ptr<ICommand> newCommand);
	BoxObject* CreateFloor(int ix, int iy);
	BoxObject* CreateDestination(int ix, int iy);
	BoxObject* CreateWall(int ix, int iy);
	BallObject* CreateBall(int ix, int iy);
	PlayerObject* CreatePlayer(int ix, int iy);
private:
	/////////////////////�ݹ��Լ���/////////////////////
	std::function<void(int)>	m_cbMovementUpdate;
	std::function<void(int)>	m_cbGameClear;
	std::function<void(std::string stageName)> m_cbStageLoad;
	//�� �̵� Ƚ��
	int m_nMovements;

	/////////////////////�� ������/////////////////////
	size_t m_szMapX, m_szMapY;
	std::vector<std::vector<GridObject*>> m_mapFloor1;
	std::vector<std::vector<GridObject*>> m_mapFloor2;
	
	//���ǻ� ����� ���� �÷��̾� ������
	PlayerObject* m_pPlayer;

	//Redo/Undo������ ����
	//���������� ����� Ŀ�ǵ��� �ε����� ��Ÿ����. 
	//���� ����� Ŀ�ǵ尡 ������ -1�� ������.
	int										m_currentCommand;
	std::vector<std::shared_ptr<ICommand>>	m_commandHistory;

	///////////////////GridObject���� �����ϴ� cFrame��ü////////////////////////////////////////
	cFrame* m_pPlayerRunning;
	cFrame* m_pPlayerIdle;
	cFrame* m_pDest;
	cFrame* m_pFloor;
	cFrame* m_pObject;
	cFrame* m_pWall;
public:
	int							m_Firstframe;
	int							m_Lastframe;
	float						m_FrameSpeed;
};


