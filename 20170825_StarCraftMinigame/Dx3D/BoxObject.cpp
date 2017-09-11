#include "stdafx.h"
#include "BoxObject.h"


BoxObject::BoxObject()
	:m_pMesh(NULL)
{
}


BoxObject::~BoxObject()
{
	SAFE_RELEASE(m_pMesh);
}

void BoxObject::init(D3DXVECTOR3 scale)
{
	D3DXCreateBox(
		g_pD3DDevice,
		scale.x, scale.y, scale.z,
		&m_pMesh, NULL);
	m_mtl.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1);
	m_mtl.Diffuse = D3DXCOLOR(1, 0, 0, 1);
	m_mtl.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1);
	m_mtl.Emissive = D3DXCOLOR(0, 0, 0, 1);
	m_mtl.Power = 20.0f;
}

void BoxObject::render() const
{
	assert(m_pMesh);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &GameObject::getWorldMatrix());
	g_pD3DDevice->SetMaterial(&m_mtl);
	m_pMesh->DrawSubset(0);
	GameObject::render();
}

void BoxObject::update(bool isDirty)
{
	GameObject::update(isDirty);
}
