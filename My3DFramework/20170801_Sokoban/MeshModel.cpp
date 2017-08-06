#include "stdafx.h"
#include "MeshModel.h"
namespace SGA {

	void MeshGroup::render(IDirect3DDevice9 * device)
	{
		if (material) {
			device->SetMaterial(material.get());
			device->SetTexture(0, texture.lock().get());
			//device->SetTexture(0, NULL);

			device->SetFVF(VertexPosNormTex::FVF);
			device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, vertices.size() / 3, vertices.data(), sizeof(VertexPosNormTex));
		}
	}

	MeshModel::MeshModel()
	{
		
	}

	MeshModel::~MeshModel()
	{
		clear();
	}

	void MeshModel::clear()
	{
		_groups.clear();
	}

	void MeshModel::render(IDirect3DDevice9 * device)
	{
		for (auto it = _groups.begin(); it != _groups.end(); ++it) {
			it->second->render(device);
		}
	}

	void MeshModel::renderGroup(IDirect3DDevice9 * device, std::string groupName)
	{
		auto it = _groups.find(groupName);
		assert(it != _groups.end() && "존재하지 않는 group입니다");
		it->second->render(device);
	}

	void MeshModel::insertGroup(std::string groupName, MeshGroup::Ptr group)
	{
		auto it = _groups.find(groupName);
		if (it != _groups.end()) {
			//이미 같은 이름의 그룹이 있는 경우
			//기존의 그룹을 삭제하고 새로운 그룹을 삽입한다.
			it->second = group;
		}
		else {
			//새로운 그룹을 추가한다.
			_groups[groupName] = group;
		}
	}

}