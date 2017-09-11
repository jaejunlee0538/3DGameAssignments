#pragma once
#include "GameObject.h"
namespace SGA {
	class MeshModel;
	class MeshObject :
		public GameObject
	{
	public:
		MeshObject();
		virtual ~MeshObject();

		void render(LPDIRECT3DDEVICE9 device) const override;
		void update(bool isDirty = false) override;
		void init(MeshModel* mesh);
	private:
		MeshModel* _mesh;
	};

}