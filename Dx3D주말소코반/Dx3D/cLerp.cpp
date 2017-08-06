#include "StdAfx.h"
#include "cLerp.h"
#include "cPyramid.h"
#include "cActionMove.h"
#include "cActionSeq.h"
#include "cActionRepeat.h"

cLerp::cLerp(void)
	: m_pPyramid(NULL)
{
}

cLerp::~cLerp(void)
{
	SAFE_RELEASE(m_pPyramid);
}

void cLerp::Setup()
{
	float fRadius = 10.f;
	for (int i = 0; i < 7; ++i)
	{
		float x = cosf(i * D3DX_PI / 3) * fRadius;
		float z = sinf(i * D3DX_PI / 3) * fRadius;
		D3DXVECTOR3 p = D3DXVECTOR3(x, i * 2, z);
		D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 0);
		m_vecVertex.push_back(ST_PC_VERTEX(p, c));
	}

	D3DXMATRIX mat, matS, matR, matT;
	D3DXMatrixScaling(&matS, 0.2f, 1.0f, 0.2f);
	D3DXMatrixRotationX(&matR, D3DX_PI / 2.0f);
	D3DXMatrixTranslation(&matT, 0.0f, 0.5f, 0.0f);
	mat = matS * matT * matR;
	m_pPyramid = new cPyramid;
	m_pPyramid->Setup(&mat, D3DCOLOR_XRGB(255, 255, 255));
	m_pPyramid->SetIsDirection(true);

	cActionSeq* pActionSeq = new cActionSeq;

	for (size_t i = 0; i < m_vecVertex.size() - 1; ++i)
	{
		cActionMove* pActionMove = new cActionMove;
		pActionMove->SetFrom(m_vecVertex[i].p);
		pActionMove->SetTo(m_vecVertex[i + 1].p);
		pActionMove->SetActionTime(1);
		pActionMove->SetTarget(m_pPyramid);

		pActionSeq->AddAction(pActionMove);

		SAFE_RELEASE(pActionMove);
	}
	
	cActionRepeat* pActionRepeat = new cActionRepeat;
	pActionSeq->SetDelegate(pActionRepeat);
	pActionRepeat->SetAction(pActionSeq);

	m_pPyramid->SetAction(pActionRepeat);

	SAFE_RELEASE(pActionRepeat);
	SAFE_RELEASE(pActionSeq);
}

void cLerp::Update()
{
	if (m_pPyramid) m_pPyramid->Update();
}

void cLerp::Render()
{
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP,
		m_vecVertex.size() - 1,
		&m_vecVertex[0],
		sizeof(ST_PC_VERTEX));
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	m_pPyramid->Render();
}


