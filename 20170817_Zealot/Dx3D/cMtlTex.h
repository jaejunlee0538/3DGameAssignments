#pragma once

class cMtlTex : public cObject
{
protected:
	SYNTHESIZE_PASS_BY_REF(D3DMATERIAL9, m_stMtl, Mtl);
	SYNTHESIZE(LPDIRECT3DTEXTURE9, m_pTexture, Texture);

public:
	cMtlTex(void);
	~cMtlTex(void);
};

