#pragma once
#include "ColliderBase.h"
class SphereCollider :
	public ColliderBase
{
public:
	SphereCollider(float radius, D3DXVECTOR3 offsetFromParent);
	~SphereCollider();

	virtual bool CheckRayCollision(D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir, float* distance) const override;
	virtual void Render() const;
private:
	D3DXVECTOR3 m_offsetFromParent;
	float m_radius;
	static LPD3DXMESH	m_pSphere;
};

