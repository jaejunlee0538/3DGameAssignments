#pragma once

class cPyramid : public cGameObject
{
private:
	vector<ST_PN_VERTEX>	m_vecVertex;
	D3DMATERIAL9			m_stMtl;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
public:
	cPyramid(void);
	~cPyramid(void);
	
	void Setup(D3DXMATRIX* pmat, D3DCOLOR c);
	void RenderUP();
	void Render();
};

