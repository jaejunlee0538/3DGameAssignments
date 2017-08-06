#pragma once

class cPyramid;

class cGrid
{
private:
	vector<ST_PC_VERTEX>	m_vecVertex;
	vector<cPyramid*>		m_vecPyramid;
	D3DXVECTOR3				m_vecOrigin;
public:
	cGrid(void);
	~cGrid(void);

	void Update();
	void Setup(int nNumLine = 30, float fInterval = 1.0f);
	void Render();

	void SetOrigin(float x, float y, float z);
};

