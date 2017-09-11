#pragma once

#include "cMtlTex.h"

class cGroup
{
private:
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	int						m_nNumTri;
private:
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);
	
public:
	cGroup(void);
	~cGroup(void);

	void Render();
	void SetVertexBuffer(vector<ST_PNT_VERTEX>& vecVertex);
};

