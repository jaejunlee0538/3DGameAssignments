#include "StdAfx.h"
#include "cGroup.h"
#include "cMtlTex.h"

cGroup::cGroup(void)
	: m_pMtlTex(NULL)
	, m_pVB(NULL)
	, m_nNumTri(0)
{
}

cGroup::~cGroup(void)
{
	SAFE_RELEASE(m_pMtlTex);
	SAFE_RELEASE(m_pVB);
}

void cGroup::Render()
{
	if(m_pMtlTex)
	{
		g_pD3DDevice->SetMaterial(&(m_pMtlTex->GetMtl()));
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		//g_pD3DDevice->SetTexture(0, NULL);
	}
	
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNT_VERTEX));
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_nNumTri);
}

void cGroup::SetVertexBuffer( vector<ST_PNT_VERTEX>& vecVertex )
{
	g_pD3DDevice->CreateVertexBuffer(
		vecVertex.size() * sizeof(ST_PNT_VERTEX),
		0,
		ST_PNT_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL);

	ST_PNT_VERTEX* pV = 0;
	m_pVB->Lock(0, 0, (LPVOID*)&pV, 0);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pVB->Unlock();

	m_nNumTri = vecVertex.size() / 3;
}
