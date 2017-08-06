#pragma once
#include "GameObject.h"
namespace SGA {
	class MeshModel;
	class MeshObject :
		public GameObject
	{
	public:
		MeshObject();
		MeshObject(MeshModel* mesh);
		virtual ~MeshObject();

		void render() const override;
		void update(bool isDirty = false) override;
		void init(MeshModel* mesh);
	private:
		MeshModel* _mesh;
	};

}