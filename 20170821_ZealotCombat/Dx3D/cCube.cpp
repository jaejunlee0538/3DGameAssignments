#include "StdAfx.h"
#include "cCube.h"

cCube::cCube(void)
	: m_fAngle(0.0f)
	, m_fAngleSpeed(0.0f)
	, m_fMaxAngle(0.0f)
	, m_vPostTranslation(0, 0, 0)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cCube::~cCube(void)
{
	for each(auto p in m_vecChild)
	{
		SAFE_DELETE(p);
	}
}

void cCube::Setup(D3DXMATRIX* pmat)
{
	vector<D3DXVECTOR3>	vecVertex;
	vector<DWORD>		vecIndex;
	vecVertex.push_back(D3DXVECTOR3(-1.0f,-1.0f,-1.0f));
	vecVertex.push_back(D3DXVECTOR3(-1.0f, 1.0f,-1.0f));
	vecVertex.push_back(D3DXVECTOR3( 1.0f, 1.0f,-1.0f));
	vecVertex.push_back(D3DXVECTOR3( 1.0f,-1.0f,-1.0f));
	vecVertex.push_back(D3DXVECTOR3(-1.0f,-1.0f, 1.0f));
	vecVertex.push_back(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	vecVertex.push_back(D3DXVECTOR3( 1.0f, 1.0f, 1.0f));
	vecVertex.push_back(D3DXVECTOR3( 1.0f,-1.0f, 1.0f));

	vector<D3DXVECTOR3> vecNormal;
	vecNormal.push_back(D3DXVECTOR3( 0, 0,-1));
	vecNormal.push_back(D3DXVECTOR3( 0, 0, 1));
	vecNormal.push_back(D3DXVECTOR3(-1, 0, 0));
	vecNormal.push_back(D3DXVECTOR3( 1, 0, 0));
	vecNormal.push_back(D3DXVECTOR3( 0, 1, 0));
	vecNormal.push_back(D3DXVECTOR3( 0,-1, 0));

	//앞
	vecIndex.push_back(0);
	vecIndex.push_back(1);
	vecIndex.push_back(2);
	vecIndex.push_back(0);
	vecIndex.push_back(2);
	vecIndex.push_back(3);
	//뒤
	vecIndex.push_back(4);
	vecIndex.push_back(6);
	vecIndex.push_back(5);
	vecIndex.push_back(4);
	vecIndex.push_back(7);
	vecIndex.push_back(6);
	//좌
	vecIndex.push_back(4);
	vecIndex.push_back(5);
	vecIndex.push_back(1);
	vecIndex.push_back(4);
	vecIndex.push_back(1);
	vecIndex.push_back(0);
	//우
	vecIndex.push_back(3);
	vecIndex.push_back(2);
	vecIndex.push_back(6);
	vecIndex.push_back(3);
	vecIndex.push_back(6);
	vecIndex.push_back(7);
	//상
	vecIndex.push_back(1);
	vecIndex.push_back(5);
	vecIndex.push_back(6);
	vecIndex.push_back(1);
	vecIndex.push_back(6);
	vecIndex.push_back(2);
	//하
	vecIndex.push_back(4);
	vecIndex.push_back(0);
	vecIndex.push_back(3);
	vecIndex.push_back(4);
	vecIndex.push_back(3);
	vecIndex.push_back(7);

	for (size_t i = 0; i < vecIndex.size(); ++i)
	{
		D3DXVECTOR3 p = vecVertex[vecIndex[i]];
		D3DXVECTOR3 n = vecNormal[i / 6];
		m_vecVertex.push_back(ST_PN_VERTEX(p, n));
	}

	if(pmat)
	{
		for (size_t i = 0; i < m_vecVertex.size(); ++i)
		{
			D3DXVec3TransformCoord(&m_vecVertex[i].p,
				&m_vecVertex[i].p,
				pmat);
		}
	}

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = m_stMtl.Diffuse = m_stMtl.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
}

void cCube::Update(D3DXMATRIX* pParent)
{
	m_fAngle += m_fAngleSpeed;
	
	if (m_fAngle <= -m_fMaxAngle)
	{
		m_fAngle = -m_fMaxAngle;
		m_fAngleSpeed *= -1;
	}

	if (m_fAngle >=  m_fMaxAngle)
	{
		m_fAngle =  m_fMaxAngle;
		m_fAngleSpeed *= -1;
	}

	D3DXMATRIX matR, matT;
	D3DXMatrixRotationX(&matR, m_fAngle);
	D3DXMatrixTranslation(&matT, m_vPostTranslation.x,
		m_vPostTranslation.y,
		m_vPostTranslation.z);
	
	m_matWorld  = matR * matT;

	if (pParent)
	{
		m_matWorld *= *pParent;
	}

	for each(auto p in m_vecChild)
	{
		p->Update(&m_matWorld);
	}
}

void cCube::Render()
{
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetFVF(ST_PN_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof(ST_PN_VERTEX));

	for each(auto p in m_vecChild)
	{
		p->Render();
	}
}

void cCube::AddChild( cCube* pChild )
{
	m_vecChild.push_back(pChild);
}

