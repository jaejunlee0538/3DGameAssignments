#include "StdAfx.h"
#include "cFrame.h"
#include "cMtlTex.h"

cFrame::cFrame(void)
	: m_pMtlTex(NULL)
{
	D3DXMatrixIdentity(&m_matLocal);
	D3DXMatrixIdentity(&m_matWorld);
}

cFrame::~cFrame(void)
{
	SAFE_RELEASE(m_pMtlTex);
}

void cFrame::Update( int nKeyFrame, D3DXMATRIX* pParent )
{
	D3DXMATRIX matR, matT;
	LocalT(nKeyFrame, matT);
	LocalR(nKeyFrame, matR);
	m_matLocal = matR * matT;

	m_matWorld = m_matLocal;
	if (pParent)
	{
		m_matWorld = m_matLocal * *pParent;
	}

	for each(auto p in m_vecChild)
	{
		p->Update(nKeyFrame, &m_matWorld);
	}
}

void cFrame::Render()
{
	if(m_pMtlTex)
	{
		g_pD3DDevice->SetMaterial(&(m_pMtlTex->GetMtl()));
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_PNT_VERTEX));
	}

	for each(auto p in m_vecChild)
	{
		p->Render();
	}
}

void cFrame::AddChild( cFrame* pChild )
{
	m_vecChild.push_back(pChild);
}

void cFrame::Destroy()
{
	for each(auto p in m_vecChild)
	{
		p->Destroy();
	}
	delete this;
}

void cFrame::CalcLocalTM( D3DXMATRIX* pParent )
{
	// W = L * PW
	// L = W * PW^-1
	if(pParent)
	{
		D3DXMATRIX matInvParent;
		D3DXMatrixInverse(&matInvParent, 0, pParent);
		m_matLocal = m_matWorld * matInvParent;
	}
	else
	{
		m_matLocal = m_matWorld;
	}
	
	for each(auto p in m_vecChild)
	{
		p->CalcLocalTM(&m_matWorld);
	}
}

void cFrame::LocalT( IN int nKeyFrame, OUT D3DXMATRIX& matT )
{
	D3DXMatrixIdentity(&matT);
	if (m_vecPosTrack.empty())
	{
		matT._41 = m_matLocal._41;
		matT._42 = m_matLocal._42;
		matT._43 = m_matLocal._43;
	}
	else if (nKeyFrame <= m_vecPosTrack.front().n)
	{
		matT._41 = m_vecPosTrack.front().p.x;
		matT._42 = m_vecPosTrack.front().p.y;
		matT._43 = m_vecPosTrack.front().p.z;
	}
	else if (nKeyFrame >= m_vecPosTrack.back().n)
	{
		matT._41 = m_vecPosTrack.back().p.x;
		matT._42 = m_vecPosTrack.back().p.y;
		matT._43 = m_vecPosTrack.back().p.z;
	}
	else
	{
		int nNext = 0;
		for (size_t i = 0; i < m_vecPosTrack.size(); ++i)
		{
			if (nKeyFrame < m_vecPosTrack[i].n)
			{
				nNext = i;
				break;
			}
		}
		int nPrev = nNext - 1;
		float t = (nKeyFrame - m_vecPosTrack[nPrev].n) / 
			(float)(m_vecPosTrack[nNext].n - m_vecPosTrack[nPrev].n);
		D3DXVECTOR3 p;
		D3DXVec3Lerp(&p, &m_vecPosTrack[nPrev].p, &m_vecPosTrack[nNext].p, t);
		matT._41 = p.x;
		matT._42 = p.y;
		matT._43 = p.z;
	}
}

void cFrame::LocalR( IN int nKeyFrame, OUT D3DXMATRIX& matR )
{
	D3DXMatrixIdentity(&matR);
	if (m_vecRotTrack.empty())
	{
		matR = m_matLocal;
		matR._41 = 0.0f;
		matR._42 = 0.0f;
		matR._43 = 0.0f;
	}
	else if (nKeyFrame <= m_vecRotTrack.front().n)
	{
		D3DXMatrixRotationQuaternion(&matR, &m_vecRotTrack.front().q);
	}
	else if (nKeyFrame >= m_vecRotTrack.back().n)
	{
		D3DXMatrixRotationQuaternion(&matR, &m_vecRotTrack.back().q);
	}
	else
	{
		int nNext = 0;
		for (size_t i = 0; i < m_vecRotTrack.size(); ++i)
		{
			if (nKeyFrame < m_vecRotTrack[i].n)
			{
				nNext = i;
				break;
			}
		}
		int nPrev = nNext - 1;
		float t = (nKeyFrame - m_vecRotTrack[nPrev].n) / 
			(float)(m_vecRotTrack[nNext].n - m_vecRotTrack[nPrev].n);
		D3DXQUATERNION q;
		D3DXQuaternionSlerp(&q, 
			&m_vecRotTrack[nPrev].q,
			&m_vecRotTrack[nNext].q,
			t);
		D3DXMatrixRotationQuaternion(&matR, &q);
	}
}
