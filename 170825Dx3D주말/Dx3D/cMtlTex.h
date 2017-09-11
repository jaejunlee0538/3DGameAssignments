#pragma once

class cMtlTex : public cObject
{
protected:
	SYNTHESIZE_PASS_BY_REF(D3DMATERIAL9, m_stMtl, Mtl);
	SYNTHESIZE(LPDIRECT3DTEXTURE9, m_pTexture, Texture);
	SYNTHESIZE(DWORD, m_dwAttrID, AttrID);

public:
	cMtlTex(void);
	~cMtlTex(void);
};

