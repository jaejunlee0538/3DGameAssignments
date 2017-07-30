#include "stdafx.h"
#include "cCamera.h"

cCamera::cCamera()
{
	m_distToTarget = 10.0f;
	m_targetPos = D3DXVECTOR3(0, 0, 0);
	m_camPos = D3DXVECTOR3(0, 0, 0);
	m_camAngles = D3DXVECTOR3(0, 0, 0);
}

cCamera::~cCamera()
{
	//D3DXMatrixTranslation()
}

void cCamera::SetAngle(float angleX, float angleY, float angleZ)
{
	m_camAngles = D3DXVECTOR3(angleX, angleY, angleZ);
}

void cCamera::SetLookTargetPosition(D3DXVECTOR3 targetPos)
{
	m_targetPos = targetPos;
	CalculateLookAtMatrix();
}

void cCamera::Update(SGA::UserInputData& input)
{
	bool paramChanged = false;
	if (input.mouseWheel > 0) {
		paramChanged = true;
		m_distToTarget -= 0.5f;
		if (m_distToTarget < 3.0f) {
			m_distToTarget = 3.0f;
		}
	}
	else if(input.mouseWheel < 0){
		paramChanged = true;
		m_distToTarget += 0.5f;
	}

	if (input.isVKeyPressed(SGA::VKEY_LBUTTON) && input.hasInput(SGA::MOUSE_MOVE)) {
		float dx = 0, dy = 0;
		if (input.mouseDelta.x != 0) {
			//World의 y축으로 회전
			dy = input.mouseDelta.x*0.01;
			if (dy > 0.1)
				dy = 0.1;
			if (dy < -0.1)
				dy = -0.1;
			paramChanged = true;
		}
		if (input.mouseDelta.y != 0) {
			//World의 x축으로 회전
			dx = input.mouseDelta.y*0.01;
			if (dx > 0.1)
				dx = 0.1;
			if (dx < -0.1)
				dx = -0.1;
			paramChanged = true;
		}
		m_camAngles.x += dx;
		m_camAngles.y += dy;
		if (m_camAngles.x > MAX_PITCH_ANGLE)
			m_camAngles.x = MAX_PITCH_ANGLE;
		if (m_camAngles.x < -MAX_PITCH_ANGLE)
			m_camAngles.x = -MAX_PITCH_ANGLE;
	}
	if (paramChanged) {
		CalculateLookAtMatrix();
	}
}

void cCamera::CalculateLookAtMatrix()
{
	D3DXMATRIX camRot;
	D3DXVECTOR3 offset(0, 0, m_distToTarget);
	D3DXMatrixRotationYawPitchRoll(&camRot, m_camAngles.y, m_camAngles.x, m_camAngles.z);
	D3DXVec3TransformNormal(&offset, &offset, &camRot);
	m_camPos = m_targetPos - offset;
	D3DXMatrixLookAtLH(&m_matLookAt, &m_camPos, &m_targetPos, &g_upVector);

	//D3DXVECTOR3 offset;
	//offset.y = m_distToTarget * sin(m_camAngles.x);
	//offset.z = m_distToTarget * cos(m_camAngles.x) * sin(m_camAngles.y);
	//offset.x = m_distToTarget * cos(m_camAngles.x) * cos(m_camAngles.y);

	//m_camPos = m_targetPos + offset;
	//D3DXMatrixLookAtLH(&m_matLookAt, &m_camPos, &m_targetPos, &g_upVector);
}
