#include "StdAfx.h"
#include "cRay.h"


cRay::cRay(void)
	: m_vRayOrg(0, 0, 0)
	, m_vRayDir(0, 0, 0)
	, m_eRaySpace(E_NONE)
{
}


cRay::~cRay(void)
{
}

cRay cRay::RayAtViewSpace( int nScreenX, int nScreenY )
{
	D3DVIEWPORT9 vp;
	g_pD3DDevice->GetViewport(&vp);

	D3DXMATRIXA16 matProj;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	cRay r;
	r.m_vRayDir.x = (( 2.0f * nScreenX) / vp.Width  - 1) / matProj._11;
	r.m_vRayDir.y = ((-2.0f * nScreenY) / vp.Height + 1) / matProj._22;
	r.m_vRayDir.z = 1.0f;

	r.m_eRaySpace = E_VIEW;

	return r;
}

cRay cRay::RayAtWorldSpace( int nScreenX, int nScreenY )
{
	cRay r = cRay::RayAtViewSpace(nScreenX, nScreenY);

	D3DXMATRIXA16 matView, matInvView;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matInvView, 0, &matView);

	D3DXVec3TransformCoord(&r.m_vRayOrg, &r.m_vRayOrg, &matInvView);
	D3DXVec3TransformNormal(&r.m_vRayDir, &r.m_vRayDir, &matInvView);

	r.m_eRaySpace = E_WORLD;

	return r;
}

bool cRay::IsPicked( const ST_SPHERE* pSphere )
{
	assert(m_eRaySpace == E_WORLD && pSphere);

	D3DXVECTOR3 vLocalRayPos = m_vRayOrg - pSphere->p;

	// (Q.V)^2 - V.V * (Q.Q - r*r)
	float qv = D3DXVec3Dot(&vLocalRayPos, &m_vRayDir);
	float vv = D3DXVec3Dot(&m_vRayDir, &m_vRayDir);
	float qq = D3DXVec3Dot(&vLocalRayPos, &vLocalRayPos);
	float rr = pSphere->r * pSphere->r;

	return qv * qv - vv * (qq - rr) >= 0;
}
