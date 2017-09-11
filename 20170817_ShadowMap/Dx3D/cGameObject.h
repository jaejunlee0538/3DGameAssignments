#pragma once
class ColliderBase;
class cGameObject : public cObject
{
protected:
	//D3DXMATRIX m_matWorld;
	SYNTHESIZE(D3DXMATRIX, m_matWorld, WorldMatrix);
	SYNTHESIZE(D3DXVECTOR3, m_vPos, Position);
	SYNTHESIZE(bool, m_isDirection, IsDirection);

	ColliderBase* m_pCollider;
	D3DXVECTOR3	m_vPrevPosition;
	D3DXVECTOR3 m_vMoveDirection;

	bool m_bMoving;
public:
	cGameObject(void);
	virtual ~cGameObject(void);

	virtual void Update(DWORD time);
	void SetCollider(ColliderBase* collider);
	bool CheckRayCollision(D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir, float* distance) const;
};

