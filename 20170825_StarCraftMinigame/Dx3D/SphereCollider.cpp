#include "stdafx.h"
#include "SphereCollider.h"

LPD3DXMESH	SphereCollider::m_pSphere = NULL;
SphereCollider::SphereCollider(float radius, D3DXVECTOR3 offsetFromParent)
	:m_radius(radius), m_offsetFromParent(offsetFromParent)
{
	if (m_pSphere==NULL) {
		D3DXCreateSphere(g_pD3DDevice, radius, 10, 10, &m_pSphere, NULL);
	}
}

SphereCollider::~SphereCollider()
{
	SAFE_RELEASE(m_pSphere);
}

bool SphereCollider::CheckRayCollision(D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir, float * distance) const
{
	//광선의 위치를 구의 로컬 좌표계로 변경한다.
	D3DXVECTOR3 sphereWorldPos = m_offsetFromParent;
	if (m_parentObject) {
		sphereWorldPos += m_parentObject->getWorldPosition();
	}
	rayPos = rayPos - sphereWorldPos;
	
	//a*t*t + b*t + c = 0;
	float a, b, c;
	a = D3DXVec3Dot(&rayDir, &rayDir);
	b = 2 * D3DXVec3Dot(&rayPos, &rayDir);
	c = D3DXVec3Dot(&rayPos, &rayPos) - m_radius*m_radius;
	
	//근의 공식
	float test = b*b - 4*a*c;//판별식
	if (test < 0.0f)//판별식이 0보다 작으면 방정식의 해가 존재하지 않는다!
		return false;

	float t = (-b - test) / (2 * a);
	if (t < 0.0f) //t가 0보다 작다는 것은 충돌하는 구가 광선의 뒤쪽에 있다는 뜻!
		return false;
	if (distance) //거리가 필요하면 충돌점까지의 거리를 계산해서 반환해준다.
		*distance = D3DXVec3Length(&(rayDir*t));
	return true;
}

void SphereCollider::Render() const
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pSphere->DrawSubset(0);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
}
