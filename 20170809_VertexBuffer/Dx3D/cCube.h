#pragma once
class cCube
{
private:
	vector<ST_PN_VERTEX>	m_vecVertex;
	vector<cCube*>			m_vecChild;
	D3DXMATRIX				m_matWorld;
	float					m_fAngle;
	D3DMATERIAL9			m_stMtl;

	SYNTHESIZE(float, m_fAngleSpeed, AngleSpeed);
	SYNTHESIZE(float, m_fMaxAngle, MaxAngle);
	SYNTHESIZE(D3DXVECTOR3, m_vPostTranslation, PostTranslation);

public:
	cCube(void);
	~cCube(void);

	void Setup(D3DXMATRIX* pmat);
	void Update(D3DXMATRIX* pParent);
	void Render();

	void AddChild(cCube* pChild);
};

