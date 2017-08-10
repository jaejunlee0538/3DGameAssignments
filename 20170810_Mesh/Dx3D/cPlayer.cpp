#include "stdafx.h"
#include "cPlayer.h"
#include "cFrame.h"
#include "cAseLoader.h"
#include "cKeyManager.h"
#define GRAVITY	50.0f
cPlayer::cPlayer()
{
	m_lastHeightOfCollisionPoint = 0;
	m_angularVelocity = D3DXVECTOR3(0, 0, 0);
	m_vecVelocity = D3DXVECTOR3(0, 0, 0);
	m_bRunning = m_bTurning = m_bJumping = false;
	D3DXQuaternionIdentity(&m_rotation);
	m_position = D3DXVECTOR3(0, 0, 0);
	m_prevTime = 0;
	D3DXMatrixIdentity(&m_playerOffset);
	m_rayHeight = 30.0f;
	m_bGravityOn = false;
	D3DXMatrixIdentity(&m_matWorld);
}

cPlayer::~cPlayer()
{
	if (m_pRunning) {
		m_pRunning->Destroy();
	}
	if (m_pIdle) {
		m_pIdle->Destroy();
	}
}

void cPlayer::Init()
{
	{
		cAseLoader aseLoader;
		m_pRunning = aseLoader.Load("woman/woman_01_all.ASE");
	}
	{
		cAseLoader aseLoader;
		m_pIdle = aseLoader.Load("woman/woman_01_all_stand.ASE");
	}

	D3DXMatrixRotationY(&m_playerOffset, D3DX_PI);
}

void cPlayer::Update(DWORD time)
{
	if (m_bGravityOn == false) {
		if (g_pKeyManager->isStayKeyDown('W')) {
			//앞으로 걷기
			SetWalkingSpeed(5.0f);
			m_bRunning = true;
		}
		else if (g_pKeyManager->isStayKeyDown('S')) {
			//뒤로 걷기
			SetWalkingSpeed(-5.0f);
			m_bRunning = true;
		}
		else {
			//그만 걷기
			SetWalkingSpeed(0);
			m_bRunning = false;
		}
		if (g_pKeyManager->isStayKeyDown('A')) {
			//왼쪽으로 회전
			SetAngularVelocity(D3DXVECTOR3(0, -3, 0));
			m_bTurning = true;
		}
		else if (g_pKeyManager->isStayKeyDown('D')) {
			//오른쪽으로 회전
			SetAngularVelocity(D3DXVECTOR3(0, 3, 0));
			m_bTurning = true;
		}
		else {
			//그만 회전
			SetAngularVelocity(D3DXVECTOR3(0, 0, 0));
			m_bTurning = false;
		}

		if (g_pKeyManager->isOnceKeyDown(VK_SPACE)) {
			Jump(20);
			EnableGravity(true);
		}
	}

	float dt = (time - m_prevTime)*0.001f;

	if (m_bGravityOn) {
		//중력에 켜져 있으면 y축 속도를 가속시킨다.
		m_vecVelocity.y -= GRAVITY * dt;
	}

	//충돌 처리를 위해 위치 업데이트 전에 현재 위치를 저장해 놓는다.
	m_prevPosition = m_position;

	//속도/회전 속도를 이용해서 위치와 회전을 업데이트 한다.
	Translate(m_vecVelocity * dt);
	RotateLocal(m_angularVelocity * dt);

	UpdateWorldMatrix();

	//애니메이션용 캐릭터 업데이트
	if (m_pRunning) {
		m_pRunning->Update(time, &(m_playerOffset*m_matWorld));
	}
	if (m_pIdle) {
		m_pIdle->Update(time, &(m_playerOffset*m_matWorld));
	}
	
	m_prevTime = time;
}

void cPlayer::Render()
{
	if (m_bRunning || m_bTurning || m_bJumping) {
		m_pRunning->Render();
	}
	else {
		m_pIdle->Render();
	}
}

void cPlayer::RenderUP()
{
	if (m_bRunning || m_bTurning || m_bJumping) {
		m_pRunning->RenderUP();
	}
	else {
		m_pIdle->RenderUP();
	}
}

void cPlayer::RenderMesh()
{
	if (m_bRunning || m_bTurning || m_bJumping) {
		m_pRunning->RenderMesh();
	}
	else {
		m_pIdle->RenderMesh();
	}
}

void cPlayer::UpdateWorldMatrix()
{
	D3DXMATRIX matT, matR;
	D3DXMatrixTranslation(&matT, m_position.x, m_position.y, m_position.z);
	D3DXMatrixRotationQuaternion(&matR, &m_rotation);
	m_matWorld = matR * matT;
}

void cPlayer::SetWalkingSpeed(float velocity)
{
	D3DXVECTOR3 vel(0, 0, velocity);
	D3DXVec3TransformNormal(&vel, &vel, &m_matWorld);

	SetVelocity(D3DXVECTOR3(vel.x, m_vecVelocity.y, vel.z));
}

void cPlayer::SetVelocityLocal(D3DXVECTOR3 velocityLocal)
{
	D3DXVec3TransformNormal(&velocityLocal, &velocityLocal, &m_matWorld);
	SetVelocity(velocityLocal);
}

void cPlayer::Translate(D3DXVECTOR3 displace)
{
	m_position = m_position + displace;
}

void cPlayer::RotateLocal(D3DXVECTOR3 displaceAngle)
{
	D3DXQUATERNION quatRot;
	D3DXQuaternionRotationYawPitchRoll(&quatRot, displaceAngle.y, displaceAngle.x, displaceAngle.z);
	m_rotation = m_rotation * quatRot;
}

void cPlayer::ResolveCollisionByLiftingUp(std::vector<cFrame*>& objects)
{
	float dist = GetCollisionDistance(objects, 0.4f);
	if (dist < 0) {
		dist = m_rayHeight - m_lastHeightOfCollisionPoint;
	}
	SetPosition({ m_position.x ,m_rayHeight- dist, m_position.z });
}

void cPlayer::ResolveCollision(std::vector<cFrame*>& objects)
{
	bool gravity = m_bGravityOn;
	float heightOfCollisionPoint = GetCollisionDistance(objects, 0.4f);
	float heightOfCenterCollisionPoint = GetCollisionDistance(objects, 0.0f);

	if (heightOfCollisionPoint > 0) {
		heightOfCollisionPoint = m_rayHeight - heightOfCollisionPoint;
		heightOfCenterCollisionPoint = m_rayHeight - heightOfCenterCollisionPoint;
		if (heightOfCollisionPoint < m_position.y - 0.2f ) {
			//떨어져야함
			gravity = true;
			m_lastHeightOfCollisionPoint = heightOfCollisionPoint;
		}
		else if (heightOfCollisionPoint > m_position.y + 0.2f) {
			//벽이 있음
			m_position.x = m_prevPosition.x;
			m_position.z = m_prevPosition.z;
		}
		else {
			//캐릭터의 발과 땅이 가까울때
			if (fabs(m_position.y - heightOfCenterCollisionPoint) < fabs(m_position.y - heightOfCollisionPoint)) {
				m_position.y = heightOfCenterCollisionPoint;
				gravity = false;
				m_lastHeightOfCollisionPoint = heightOfCenterCollisionPoint;
			}
			else {
				m_position.y = heightOfCollisionPoint;
				gravity = false;
				m_lastHeightOfCollisionPoint = heightOfCollisionPoint;
			}
		}
	}
	else {
		////충돌 없음
		m_position.x = m_prevPosition.x;
		m_position.z = m_prevPosition.z;
	}
	if (m_bGravityOn && m_position.y <= m_lastHeightOfCollisionPoint) {
		gravity = false;
	}
	if (gravity != m_bGravityOn) {
		EnableGravity(gravity);
		if (gravity == false) {
			ResolveCollisionByLiftingUp(objects);
		}
	}
}

float cPlayer::GetCollisionDistance(std::vector<cFrame*>& objects, float center)
{
	D3DXVECTOR3 moveDir(0, 0, 1);
	D3DXVec3TransformNormal(&moveDir, &moveDir, &m_matWorld);
	float projected = D3DXVec3Dot(&moveDir, &m_vecVelocity) >= 0 ? center : -center;
	moveDir *= projected;
	return ShootCollisionRay(D3DXVECTOR3(m_position.x + moveDir.x, m_rayHeight, m_position.z+ moveDir.z), D3DXVECTOR3(0,-1,0), objects);
}

void cPlayer::EnableGravity(bool enable)
{
	m_bGravityOn = enable;
	if (enable==false) {
		m_vecVelocity.y = 0;
		if (m_bJumping) {
			m_bJumping = false;
		}
	}
}

void cPlayer::Jump(float jumpSpeed)
{
	m_vecVelocity.y = jumpSpeed;
	EnableGravity(true);
	m_bJumping = true;
}

float cPlayer::ShootCollisionRay(D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir, std::vector<cFrame*>& objects)
{
	for (auto pFrame : objects) {
		auto& vertices = pFrame->GetVertex();
		float dist;
		for (size_t i = 0; i < vertices.size(); i += 3) {
			if (D3DXIntersectTri(
				&vertices[i].p,
				&vertices[i + 1].p,
				&vertices[i + 2].p,
				&rayPos,
				&rayDir,
				NULL, NULL,
				&dist)) {
				return dist;
			}
		}
	}
	return -1.0f;
}
