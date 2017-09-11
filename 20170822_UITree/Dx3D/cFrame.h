#pragma once

#include "cMtlTex.h"

class cFrame
{
private:
	vector<cFrame*> m_vecChild;
	D3DXMATRIX		m_matLocal;
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIX, m_matWorld, WorldTM);
	SYNTHESIZE_PASS_BY_REF(vector<ST_POS_SAMPLE>, m_vecPosTrack, PosTrack);
	SYNTHESIZE_PASS_BY_REF(vector<ST_ROT_SAMPLE>, m_vecRotTrack, RotTrack);

private:
	SYNTHESIZE_PASS_BY_REF(vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	SYNTHESIZE_ADD_REF(cMtlTex*, m_pMtlTex, MtlTex);

public:
	cFrame(void);
	~cFrame(void);

	void Update(int nKeyFrame, D3DXMATRIX* pParent);
	void Render();
	void AddChild(cFrame* pChild);
	void Destroy();
	void CalcLocalTM(D3DXMATRIX* pParent);
	void LocalT(IN int nKeyFrame, OUT D3DXMATRIX& matT);
	void LocalR(IN int nKeyFrame, OUT D3DXMATRIX& matR);
};

