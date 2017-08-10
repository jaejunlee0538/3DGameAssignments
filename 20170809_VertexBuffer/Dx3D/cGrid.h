#pragma once

class cPyramid;

class cGrid
{
private:
	vector<ST_PC_VERTEX>	m_vecVertex;
	vector<cPyramid*>		m_vecPyramid;
	D3DXVECTOR3				m_vecOrigin;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
public:
	cGrid(void);
	~cGrid(void);

	void Update();
	void Setup(int nNumLine = 30, float fInterval = 1.0f);
	void Render();
	void RenderUP();

	void SetOrigin(float x, float y, float z);
};

