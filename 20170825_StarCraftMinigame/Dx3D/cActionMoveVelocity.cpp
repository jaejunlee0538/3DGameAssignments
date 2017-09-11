#include "stdafx.h"
#include "cActionMoveVelocity.h"

cActionMoveVelocity::cActionMoveVelocity(D3DXVECTOR3 from, D3DXVECTOR3 to, float speed)
	:m_from(from), m_to(to), m_speed(speed)
{
}

cActionMoveVelocity::~cActionMoveVelocity()
{
}

void cActionMoveVelocity::Start()
{
	assert(m_pTarget);
	m_pTarget->setPosition(m_from);
	cAction::Start();
}

void cActionMoveVelocity::Update()
{
	assert(m_pTarget);
	
	D3DXVECTOR3 vel = m_to - m_from;
	D3DXVec3Normalize(&vel, &vel);
	D3DXVECTOR3 vecToDest = m_to - m_pTarget->getWorldPosition();

	if (D3DXVec3Dot(&vel, &vecToDest) <= 0.0) {
		m_pTarget->setSpeed(0, 0, 0);
		if (m_pDelegate) {
			m_pDelegate->OnFinish(this);
		}
	}
	else {
		vel = vel * m_speed;
		m_pTarget->setSpeed(vel.x, vel.y, vel.z);
	}
}
