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

	//모든 게임에서 공유할 수 있는 리소스를 로드한다.
	void Init();
	//mapFile을 통해 새로운 게임을 생성한다.
	void LoadGame(const char* mapFile);
	//현재 게임 관련 데이터를 모두 정리한다.
	void Reset();
	//게임 공유 리소스들을 언로드한다.
	void Uninitalize();
	//게임을 클리어하면 true를 반환한다.
	bool IsGameCleared();
	//업데이트
	void Update(int time);
	//렌더
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
	//메시지 프로시저(지금은 아무 기능 없음)
	bool MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//플레이어의 xyz위치를 반환
	D3DXVECTOR3 GetPlayerPosition();
	float GetPlayerLookAngle();
	//총 이동 횟수를 반환
	int GetMovementCount() { return m_nMovements; }
private:
	void InsertAndDoCommand(std::shared_ptr<ICommand> newCommand);
	BoxObject* CreateFloor(int ix, int iy);
	BoxObject* CreateDestination(int ix, int iy);
	BoxObject* CreateWall(int ix, int iy);
	BallObject* CreateBall(int ix, int iy);
	PlayerObject* CreatePlayer(int ix, int iy);
private:
	/////////////////////콜백함수들/////////////////////
	std::function<void(int)>	m_cbMovementUpdate;
	std::function<void(int)>	m_cbGameClear;
	std::function<void(std::string stageName)> m_cbStageLoad;
	//총 이동 횟수
	int m_nMovements;

	/////////////////////맵 데이터/////////////////////
	size_t m_szMapX, m_szMapY;
	std::vector<std::vector<GridObject*>> m_mapFloor1;
	std::vector<std::vector<GridObject*>> m_mapFloor2;
	
	//편의상 만들어 놓은 플레이어 포인터
	PlayerObject* m_pPlayer;

	//Redo/Undo구현을 위한
	//마지막으로 적용된 커맨드의 인덱스를 나타낸다. 
	//현재 적용된 커맨드가 없으면 -1을 가진다.
	int										m_currentCommand;
	std::vector<std::shared_ptr<ICommand>>	m_commandHistory;

	///////////////////GridObject들이 공유하는 cFrame객체////////////////////////////////////////
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


