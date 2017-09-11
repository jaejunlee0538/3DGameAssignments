#pragma once
#include "cAction.h"
class cActionMoveVelocity :
	public cAction
{
	D3DXVECTOR3 m_from, m_to;
	float m_speed;
public:
	cActionMoveVelocity(D3DXVECTOR3 from, D3DXVECTOR3 to, float speed);
	virtual ~cActionMoveVelocity();

	virtual void Start() override;
	virtual void Update() override;
};

