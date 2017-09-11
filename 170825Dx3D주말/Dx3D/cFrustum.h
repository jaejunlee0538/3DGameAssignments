#pragma once
class cFrustum
{
private:
	D3DXVECTOR3	m_aProjVertex[8];
	D3DXPLANE	m_aPlane[6];

public:
	cFrustum(void);
	~cFrustum(void);

	void Update();
	bool IsIn(ST_SPHERE* pSphere);
};

