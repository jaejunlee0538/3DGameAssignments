#include "StdAfx.h"
#include "cGrid.h"
#include "cPyramid.h"

cGrid::cGrid(void)
	:m_vecOrigin(0,0,0), m_pVB(NULL)
{
}

cGrid::~cGrid(void)
{
	for each(auto p in m_vecPyramid)
	{
		SAFE_RELEASE(p);
	}
	SAFE_RELEASE(m_pVB);
}


void cGrid::Setup( int nNumLine /*= 20*/, float fInterval /*= 1.0f*/ )
{
	int nHalfNumLine = nNumLine / 2;

	float fMax = fabs(nHalfNumLine * fInterval);

	D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 255);
	for (int i = 1; i <= nHalfNumLine; ++i)
	{
		if( i % 5 == 0 )
			c = D3DCOLOR_XRGB(255, 255, 255);
		else 
			c = D3DCOLOR_XRGB(155, 155, 155);

		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0,  i * fInterval), c));
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( fMax, 0,  i * fInterval), c));

		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0, -i * fInterval), c));
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( fMax, 0, -i * fInterval), c));
		
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( i * fInterval, 0, -fMax), c));
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( i * fInterval, 0,  fMax), c));
		
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-i * fInterval, 0, -fMax), c));
		m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-i * fInterval, 0,  fMax), c));
	}

	c = D3DCOLOR_XRGB(255, 0, 0);
	m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-fMax, 0, 0), c));
	m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( fMax, 0, 0), c));

	c = D3DCOLOR_XRGB(0, 255, 0);
	m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0,-fMax, 0), c));
	m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0, fMax, 0), c));

	c = D3DCOLOR_XRGB(0, 0, 255);
	m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0, 0,-fMax), c));
	m_vecVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3( 0, 0, fMax), c));

	//VertexBuffer
	g_pD3DDevice->CreateVertexBuffer(
		m_vecVertex.size() * sizeof(ST_PC_VERTEX),
		D3DUSAGE_WRITEONLY,
		ST_PC_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL);
	ST_PC_VERTEX * vertices;
	m_pVB->Lock(0, 0, (void**)&vertices, 0);
	for (size_t i = 0; i < m_vecVertex.size(); ++i) {
		vertices[i] = m_vecVertex[i];
	}
	m_pVB->Unlock();

	//피라미드
	D3DXMATRIX matS, matR, mat;
	D3DXMatrixScaling(&matS, 0.1f, 2.0f, 0.1f);
	
	cPyramid* pPyramid = new cPyramid;
	D3DXMatrixRotationZ(&matR, D3DX_PI / 2.0f);
	mat = matS * matR;
	pPyramid->Setup(&mat, D3DCOLOR_XRGB(255, 0, 0));
	m_vecPyramid.push_back(pPyramid);

	pPyramid = new cPyramid;
	D3DXMatrixRotationZ(&matR, D3DX_PI);
	mat = matS * matR;
	pPyramid->Setup(&mat, D3DCOLOR_XRGB(0, 255, 0));
	m_vecPyramid.push_back(pPyramid);

	pPyramid = new cPyramid;
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);
	mat = matS * matR;
	pPyramid->Setup(&mat, D3DCOLOR_XRGB(0, 0, 255));
	m_vecPyramid.push_back(pPyramid);
}

void cGrid::RenderUP()
{
	DWORD dwPrev;
	g_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &dwPrev);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, m_vecOrigin.x, m_vecOrigin.y, m_vecOrigin.z);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST,
		m_vecVertex.size() / 2,
		&m_vecVertex[0],
		sizeof(ST_PC_VERTEX));
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	for each(auto p in m_vecPyramid)
	{
		p->RenderUP();
	}
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, (bool)dwPrev);
}

void cGrid::Render()
{
	DWORD dwPrev;
	g_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &dwPrev);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, m_vecOrigin.x, m_vecOrigin.y, m_vecOrigin.z);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PC_VERTEX));
	g_pD3DDevice->DrawPrimitive(D3DPT_LINELIST,
		0, m_vecVertex.size() / 2);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	for each(auto p in m_vecPyramid)
	{
		p->Render();
	}
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, (bool)dwPrev);
}

void cGrid::SetOrigin(float x, float y, float z)
{
	m_vecOrigin = D3DXVECTOR3(x, y, z);
	for each(auto p in m_vecPyramid) {
		p->SetPosition(m_vecOrigin);
	}
}
