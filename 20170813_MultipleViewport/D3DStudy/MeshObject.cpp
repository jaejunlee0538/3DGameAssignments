#include "stdafx.h"
#include "MeshObject.h"
#include "MeshModel.h"

namespace SGA {
	MeshObject::MeshObject()
		:_mesh(NULL)
	{
	}

	MeshObject::~MeshObject()
	{
		SAFE_DELETE(_mesh);
	}

	void MeshObject::render(LPDIRECT3DDEVICE9 device) const
	{
		device->SetTransform(D3DTS_WORLD, &getWorldMatrix());
		if (_mesh) {
			_mesh->render(device);
		}
		GameObject::render(device);
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