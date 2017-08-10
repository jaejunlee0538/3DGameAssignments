#pragma once

#include "cMtlTex.h"

class cGroup
{
private:
	SYNTHESIZE_PASS_BY_REF(vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);
	
public:
	cGroup(void);
	~cGroup(void);

	void Render();
	void ScaleVertices(float scale);
// 	void SetMtlTex(cMtlTex* pMtlTex)
// 	{
// 		if (pMtlTex != m_pMtlTex)
// 		{
// 			SAFE_RELEASE(m_pMtlTex);
// 			SAFE_ADDREF(pMtlTex);
// 			m_pMtlTex = pMtlTex;
// 		}
// 	}
};

