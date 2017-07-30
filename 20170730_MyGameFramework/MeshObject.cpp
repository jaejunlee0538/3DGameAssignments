#include "stdafx.h"
#include "MeshObject.h"
#include "MeshModel.h"

namespace SGA {
	MeshObject::MeshObject()
	{
	}

	MeshObject::~MeshObject()
	{
		SAFE_DELETE(_mesh);
	}

	void MeshObject::render() const
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &getWorldMatrix());
		_mesh->render(g_pD3DDevice);
		GameObject::render();
	}

	void MeshObject::update(bool isDirty)
	{
		GameObject::update(isDirty);
	}

	void MeshObject::init(MeshModel * mesh)
	{
		_mesh = mesh;
	}
}