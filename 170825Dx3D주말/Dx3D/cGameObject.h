#pragma once

class cGameObject : public cObject
{
protected:
	D3DXVECTOR3	m_vPrevPosition;
	SYNTHESIZE(D3DXVECTOR3, m_vPos, Position);
	SYNTHESIZE_ADD_REF(cAction*, m_pAction, Action);
	SYNTHESIZE(bool, m_isDirection, IsDirection);
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_matWorld, WorldTM);

	bool					m_moving;
	D3DXVECTOR3				m_vDirection;
private:
	D3DXVECTOR3*			m_pPos;
	D3DXVECTOR3				m_vDir;
	float					m_fSpeed;
	float					m_fAngle;

public:
	cGameObject(void);
	virtual ~cGameObject(void);

	virtual void Update();
	bool getMoving(void) { return m_moving; }
	void setMoving(bool move) { m_moving = move; }
	D3DXVECTOR3 getcurrentDir(void) { return m_vDirection; }
	D3DXVECTOR3* getPointpos(void);
	void setDirection(D3DXVECTOR3 enemy);

};

