#pragma once

class cGameObject : public cObject
{
protected:
	D3DXMATRIX m_matWorld;
	D3DXVECTOR3	m_vPrevPosition;
	SYNTHESIZE(D3DXVECTOR3, m_vPos, Position);
	SYNTHESIZE_ADD_REF(cAction*, m_pAction, Action);
	SYNTHESIZE(bool, m_isDirection, IsDirection);
public:
	cGameObject(void);
	virtual ~cGameObject(void);

	virtual void Update();
};

