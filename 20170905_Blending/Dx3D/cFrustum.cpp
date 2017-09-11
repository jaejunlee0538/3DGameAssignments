#include "StdAfx.h"
#include "cFrustum.h"


cFrustum::cFrustum(void)
{
	m_aProjVertex[0] = D3DXVECTOR3(-1, 1, 0);
	m_aProjVertex[1] = D3DXVECTOR3( 1, 1, 0);
	m_aProjVertex[2] = D3DXVECTOR3(-1,-1, 0);
	m_aProjVertex[3] = D3DXVECTOR3( 1,-1, 0);
	m_aProjVertex[4] = D3DXVECTOR3(-1, 1, 1);
	m_aProjVertex[5] = D3DXVECTOR3( 1, 1, 1);
	m_aProjVertex[6] = D3DXVECTOR3(-1,-1, 1);
	m_aProjVertex[7] = D3DXVECTOR3( 1,-1, 1);
}


cFrustum::~cFrustum(void)
{
}

void cFrustum::Update()
{
	D3DXMATRIX matProj, matInvProj, matView, matInvView, matInvProjView;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvProj, 0, &matProj);
	D3DXMatrixInverse(&matInvView, 0, &matView);
	matInvProjView = matInvProj * matInvView;
	
	D3DXVECTOR3 aWorldVertex[8];

	for (size_t i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&aWorldVertex[i], &m_aProjVertex[i], &matInvProjView);
	}
	
	D3DXPlaneFromPoints(&m_aPlane[0], 
		&aWorldVertex[3-1], 
		&aWorldVertex[1-1], 
		&aWorldVertex[2-1]);
	D3DXPlaneFromPoints(&m_aPlane[1], 
		&aWorldVertex[7-1], 
		&aWorldVertex[5-1], 
		&aWorldVertex[1-1]);
	D3DXPlaneFromPoints(&m_aPlane[2], 
		&aWorldVertex[5-1], 
		&aWorldVertex[6-1], 
		&aWorldVertex[1-1]);
	D3DXPlaneFromPoints(&m_aPlane[3], 
		&aWorldVertex[4-1], 
		&aWorldVertex[2-1], 
		&aWorldVertex[8-1]);
	D3DXPlaneFromPoints(&m_aPlane[4], 
		&aWorldVertex[8-1], 
		&aWorldVertex[7-1], 
		&aWorldVertex[3-1]);
	D3DXPlaneFromPoints(&m_aPlane[5], 
		&aWorldVertex[7-1], 
		&aWorldVertex[6-1], 
		&aWorldVertex[5-1]);
}

bool cFrustum::IsIn( ST_SPHERE* pSphere )
{
	for (int i = 0; i < 6; ++i)
	{
		if(D3DXPlaneDotCoord(&m_aPlane[i], &(pSphere->p)) > pSphere->r)
			return false;
	}
	return true;
}
