#include "stdafx.h"
#include "SphereCollider.h"


SphereCollider::SphereCollider(float radius, D3DXVECTOR3 offsetFromParent)
	:m_radius(radius), m_offsetFromParent(offsetFromParent)
{
}

SphereCollider::~SphereCollider()
{
}

bool SphereCollider::CheckRayCollision(D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir, float * distance) const
{
	//������ ��ġ�� ���� ���� ��ǥ��� �����Ѵ�.
	D3DXVECTOR3 sphereWorldPos = m_offsetFromParent;
	if (m_parentObject) {
		sphereWorldPos += m_parentObject->GetPosition();
	}
	rayPos = rayPos - sphereWorldPos;
	
	//a*t*t + b*t + c = 0;
	float a, b, c;
	a = D3DXVec3Dot(&rayDir, &rayDir);
	b = 2 * D3DXVec3Dot(&rayPos, &rayDir);
	c = D3DXVec3Dot(&rayPos, &rayPos) - m_radius*m_radius;

	//���� ����
	float test = b*b - 4*a*c;//�Ǻ���
	if (test < 0.0f)//�Ǻ����� 0���� ������ �������� �ذ� �������� �ʴ´�!
		return false;

	float t = (-b - test) / (2 * a);
	if (t < 0.0f) //t�� 0���� �۴ٴ� ���� �浹�ϴ� ���� ������ ���ʿ� �ִٴ� ��!
		return false;
	if (distance) //�Ÿ��� �ʿ��ϸ� �浹�������� �Ÿ��� ����ؼ� ��ȯ���ش�.
		*distance = D3DXVec3Length(&(rayDir*t));
	return true;
}
