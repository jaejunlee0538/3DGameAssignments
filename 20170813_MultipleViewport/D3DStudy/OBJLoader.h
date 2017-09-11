#pragma once
#include "ModelLoader.h"

namespace SGA{
	namespace ModelLoad {
		class OBJLoader : public I3DModelLoader
		{
			struct Material {
				D3DMaterialPtr material;
				D3DTexturePtr texture;
				Material()
				:material(new D3DMATERIAL9){
					material->Ambient = D3DXCOLOR(1, 1, 1, 1);
					material->Diffuse = D3DXCOLOR(1, 1, 1, 1);
					material->Specular = D3DXCOLOR(1, 1, 1, 1);
					material->Emissive = D3DXCOLOR(0, 0, 0, 1);
					material->Power = 5;
				}
			};
		public:
			OBJLoader();
			virtual ~OBJLoader();
			void load3DModel(const char* modelFile, MeshModel::Ptr& meshModel) override;
		private:
			void loadMaterialLibrary(const char* mtlFile);
		private:
			std::vector<D3DXVECTOR2> _uv;
			std::vector<D3DXVECTOR3> _normal;
			std::vector<D3DXVECTOR3> _position;
			std::map<std::string, Material> _mtllib;
		};
	}
}
