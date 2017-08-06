#pragma once
#include "LinearInterpolater.h"
#include <functional>
class cFrame;
class cCamera;

extern const float BOX_SCALE;
extern const float BOX_SIZE;

enum GridObjectTypes {
	OBJ_NONE = -1,
	OBJ_FLOOR = 1,
	OBJ_BALL = 2,
	OBJ_DESTINATION = 3,
	OBJ_WALL = 0,
	OBJ_PLAYER = 4
};

//소코반 게임 오브젝트들의 상위 클래스
class GridObject {
public:
	enum LookDirections {
		DIR_UP = 0,
		DIR_RIGHT,
		DIR_DOWN,
		DIR_LEFT,

		NUM_DIRS
	};

	GridObject();
	virtual ~GridObject(){}

	void SetType(GridObjectTypes objType) { m_objType = objType; }
	GridObjectTypes GetType() { return m_objType; }

	virtual void Setup(POINT initPos, LookDirections lookDir, float yOffset);

	virtual void Update(int time, POINT realPos);

	virtual void Render();

	bool isMoving() const {
		return m_isMoving;
	}

	float GetAngle() const;
	void TurnLeft();
	void TurnRight();
	LookDirections GetLookDirection() const { return m_lookDir; }
	//오브젝트의 XYZ위치를 반환한다.
	D3DXVECTOR3 GetCurrentXYZ() { return m_currentXYZ; }
	//오브젝트의 현재 그리드 위치(인덱스)를 반환한다.
	POINT GetCurrentIdx() { return m_currentIdx; }
protected:
	void SetCurrentObject(cFrame* currentObj);
	cFrame* GetCurrentObject();
protected:
	
	float						m_movePhase;
	int							m_currentTime;
	GridObjectTypes				m_objType;
	D3DXMATRIX					m_matWorld;
	cFrame*						m_currentObj;
	LookDirections				m_lookDir;
	D3DXVECTOR3					m_currentXYZ;
	POINT						m_currentIdx;
	LinearInterpolator			m_lerp;
	bool						m_isMoving;
};

//플레이어 클래스
class PlayerObject :public GridObject {
public:
	PlayerObject(cFrame* pRunning, cFrame* pIdle);
	virtual ~PlayerObject();
	void Update(int time, POINT realPos) override;
protected:
	cFrame* m_pRunning;
	cFrame* m_pIdle;
};

//바닥, 목적지, 벽, 움직이는물체를 위한 클래스
class BoxObject : public GridObject {
public:
	BoxObject(cFrame* pBox);
};

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
	BoxObject* CreateFloor(int ix, int iy);
	BoxObject* CreateDestination(int ix, int iy);
	BoxObject* CreateWall(int ix, int iy);
	BoxObject* CreateBall(int ix, int iy);
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

