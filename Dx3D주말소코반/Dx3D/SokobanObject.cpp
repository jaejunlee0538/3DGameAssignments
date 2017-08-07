#include "stdafx.h"
#include "SokobanObject.h"
#include "SokobanGame.h"
#include "cFrame.h"

const float BOX_SCALE = 0.5f;
const float BOX_SIZE = 2.0 * BOX_SCALE;

GridObject::GridObject()
	:m_currentObj(NULL), m_angleX(0), m_angleZ(0)
{

}

void GridObject::Setup(POINT initPos, LookDirections lookDir, float yOffset)
{
	m_currentIdx.x = initPos.x;
	m_currentIdx.y = initPos.y;

	m_currentXYZ.x = initPos.x * BOX_SIZE;
	m_currentXYZ.y = yOffset;
	m_currentXYZ.z = initPos.y * BOX_SIZE;

	m_lookDir = lookDir;

	m_isMoving = false;
}

void GridObject::Update(int time, POINT realPos) {
	if (isMoving() == false && (realPos.x != m_currentIdx.x || realPos.y != m_currentIdx.y)) {
		int deltaIx, deltaIy;
		deltaIx = realPos.x - m_currentIdx.x;
		deltaIy = realPos.y - m_currentIdx.y;
		if (deltaIx > 0) m_movingDir = DIR_RIGHT;
		else if (deltaIx < 0) m_movingDir = DIR_LEFT;
		else if (deltaIy > 0) m_movingDir = DIR_UP;
		else m_movingDir = DIR_DOWN;
		m_lerp.init(
			m_currentIdx.x * BOX_SIZE, m_currentXYZ.y, m_currentIdx.y * BOX_SIZE,
			realPos.x * BOX_SIZE, m_currentXYZ.y, realPos.y * BOX_SIZE);
		m_currentIdx = realPos;
		m_isMoving = true;
		m_movePhase = 0.0f;
	}

	if (isMoving()) {
		m_movePhase += 0.045f;
		if (m_movePhase >= 1.0f) {
			//다 움직임!
			m_movePhase = 1.0f;
			m_isMoving = false;
		}
		//이동용 애니메이션
		m_lerp.calculateWithPhase(m_movePhase);
		m_currentXYZ.x = m_lerp.getX();
		m_currentXYZ.y = m_lerp.getY();
		m_currentXYZ.z = m_lerp.getZ();
	}

	m_currentTime = time;
}

void GridObject::Render()
{
	if (m_currentObj) {
		//월드 행렬 업데이트
		D3DXMATRIX matR, matT;
		//D3DXMatrixRotationY(&matR, GetAngle());
		D3DXMatrixRotationYawPitchRoll(&matR, GetAngle(), m_angleX, m_angleZ);
		D3DXMatrixTranslation(&matT, m_currentXYZ.x, m_currentXYZ.y, m_currentXYZ.z);
		m_matWorld = matR * matT;

		m_currentObj->Update(m_currentTime, &m_matWorld);
		m_currentObj->Render();
	}
}

float GridObject::GetAngle() const
{
	switch (m_lookDir) {
	case DIR_UP:
		return D3DX_PI;
	case DIR_RIGHT:
		return D3DX_PI * 3 / 2;
	case DIR_DOWN:
		return 0;
	case DIR_LEFT:
		return D3DX_PI / 2;
	}
	assert(false);
}

void GridObject::RotateLocal(float dx, float dz)
{
	m_angleX += dx;
	m_angleZ += dz;
}

void GridObject::TurnLeft()
{
	int dir = m_lookDir;
	dir--;
	if (dir < 0) {
		dir = NUM_DIRS - 1;
	}
	m_lookDir = static_cast<LookDirections>(dir);
}

void GridObject::TurnRight()
{
	int dir = m_lookDir;
	dir++;
	if (dir == NUM_DIRS) {
		dir = 0;
	}
	m_lookDir = static_cast<LookDirections>(dir);
}

void GridObject::SetLookDirection(LookDirections dir)
{
	m_lookDir = dir;
}

void GridObject::SetCurrentObject(cFrame * currentObj)
{
	m_currentObj = currentObj;
}

cFrame * GridObject::GetCurrentObject()
{
	return m_currentObj;
}

PlayerObject::PlayerObject(cFrame* pRunning, cFrame* pIdle)
	:m_pRunning(pRunning), m_pIdle(pIdle)
{
}

PlayerObject::~PlayerObject()
{
}

void PlayerObject::Update(int time, POINT realPos)
{
	GridObject::Update(time, realPos);
	if (isMoving() == true && GetCurrentObject() != m_pRunning) {
		SetCurrentObject(m_pRunning);
	}
	else if (isMoving() == false && GetCurrentObject() != m_pIdle) {
		SetCurrentObject(m_pIdle);
	}
}

BoxObject::BoxObject(cFrame * pBox)
{
	SetCurrentObject(pBox);
}

BallObject::BallObject(cFrame * pBall)
{
	SetCurrentObject(pBall);
}

void BallObject::Update(int time, POINT realPos)
{
	GridObject::Update(time, realPos);
	if (isMoving()) {
		switch (m_movingDir) {
		case DIR_UP:
			RotateLocal(0, -0.05);
			break;
		case DIR_DOWN:
			RotateLocal(0, 0.05);
			break;
		case DIR_LEFT:
			RotateLocal(0.05, 0);
			break;
		case DIR_RIGHT:
			RotateLocal(-0.05, 0);
			break;
		}
	}
}