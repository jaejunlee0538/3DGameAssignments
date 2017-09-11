#pragma once

class iMap;

class cCrtCtrl
{
private:
	D3DXVECTOR3				m_vDir;
	D3DXVECTOR3				m_vPos;
	float					m_fSpeed;
	float					m_fAngle;

protected:
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIX, m_matWorld, WorldTM);

public:
	cCrtCtrl(void);
	~cCrtCtrl(void);

	void Update(iMap* pMap = NULL);
	D3DXVECTOR3* GetPosition();

};

