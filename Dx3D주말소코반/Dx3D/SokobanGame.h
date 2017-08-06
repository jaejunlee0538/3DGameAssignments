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

//���ڹ� ���� ������Ʈ���� ���� Ŭ����
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
	//������Ʈ�� XYZ��ġ�� ��ȯ�Ѵ�.
	D3DXVECTOR3 GetCurrentXYZ() { return m_currentXYZ; }
	//������Ʈ�� ���� �׸��� ��ġ(�ε���)�� ��ȯ�Ѵ�.
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

//�÷��̾� Ŭ����
class PlayerObject :public GridObject {
public:
	PlayerObject(cFrame* pRunning, cFrame* pIdle);
	virtual ~PlayerObject();
	void Update(int time, POINT realPos) override;
protected:
	cFrame* m_pRunning;
	cFrame* m_pIdle;
};

//�ٴ�, ������, ��, �����̴¹�ü�� ���� Ŭ����
class BoxObject : public GridObject {
public:
	BoxObject(cFrame* pBox);
};

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
	BoxObject* CreateFloor(int ix, int iy);
	BoxObject* CreateDestination(int ix, int iy);
	BoxObject* CreateWall(int ix, int iy);
	BoxObject* CreateBall(int ix, int iy);
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

