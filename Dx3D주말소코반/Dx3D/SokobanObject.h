#pragma once
#include "LinearInterpolater.h"
class cFrame;

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

enum LookDirections {
	DIR_UP = 0,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,

	NUM_DIRS
};

//���ڹ� ���� ������Ʈ���� ���� Ŭ����
class GridObject {
public:
	GridObject();
	virtual ~GridObject() {}

	void SetType(GridObjectTypes objType) { m_objType = objType; }
	GridObjectTypes GetType() { return m_objType; }

	virtual void Setup(POINT initPos, LookDirections lookDir, float yOffset);

	virtual void Update(int time, POINT realPos);

	virtual void Render();

	bool isMoving() const {
		return m_isMoving;
	}

	float GetAngle() const;
	void RotateLocal(float dx, float dz);
	void TurnLeft();
	void TurnRight();
	void SetLookDirection(LookDirections dir);

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
	LookDirections				m_movingDir;
	float						m_angleX;
	float						m_angleZ;
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

class BallObject : public GridObject {
public:
	BallObject(cFrame* pBall);
	void Update(int time, POINT realPos) override;
};
