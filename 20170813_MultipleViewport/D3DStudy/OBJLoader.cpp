#include "stdafx.h"
#include "OBJLoader.h"
#include <sstream>
#include <fstream>
#include "MeshModel.h"
namespace SGA {
	namespace ModelLoad {
		OBJLoader::OBJLoader()
		{
		}

		OBJLoader::~OBJLoader()
		{

		}

		void OBJLoader::load3DModel(const char * modelFile, MeshModel::Ptr& meshModel)
		{
			_position.clear(); 
			_normal.clear();
			_uv.clear();
			_mtllib.clear();

			std::ifstream file(modelFile);
			if (file.good() == false) {
				throw ResourceFileMissing(modelFile);
			}
			
			float x, y, z, w;
			std::string buffer, token;
			buffer.reserve(100);
			token.reserve(50);
			MeshModel::Ptr model(new MeshModel());
			MeshGroup::Ptr group;
			while (std::getline(file, buffer)) {
				if (buffer.empty())
					continue;
				if (buffer[0] == '#')
					continue;
				std::istringstream iss(buffer);
				iss >> token;
				if (token == "v") {
					iss >> x >> y >> z;
					_position.emplace_back(D3DXVECTOR3(x, y, z));
				}
				else if (token == "vt") {
					iss >> x >> y;
					_uv.emplace_back(D3DXVECTOR2(x, y));
				}
				else if (token == "vn") {
					iss >> x >> y >> z;
					_normal.emplace_back(D3DXVECTOR3(x, y, z));
				}
				else if (token == "f") {
					
					std::string idx;
					idx.reserve(10);
				
					for (int i = 0; i < 3; ++i) {
						iss >> token;
						std::istringstream iss2(token);
						int idxPos, idxUV, idxNormal;
						//XYZ 인덱스
						std::getline(iss2, idx, '/');
						assert(idx.empty() == false && "XYZ 인덱스 파싱 에러");
						idxPos = atoi(idx.c_str()) - 1;
						//텍스쳐 인덱스
						std::getline(iss2, idx, '/');
						assert(idx.empty() == false && "텍스쳐 인덱스 파싱 에러");
						idxUV = atoi(idx.c_str()) - 1;
						//법선 인덱스
						std::getline(iss2, idx, '/');
						assert(idx.empty() == false && "법선 인덱스 파싱 에러");
						idxNormal = atoi(idx.c_str()) - 1;
						group->vertices.emplace_back(VertexPosNormTex(_position[idxPos], _normal[idxNormal], _uv[idxUV]));
					}
				}
				else if (token == "s") {
					//스무스 쉐이딩
					continue;
				}
				else if (token == "g") {
					token.clear();
					iss >> token;
					if (group) {
						//그룹 생성이 끝남.
						//그룹을 모델에 삽입한다.
						model->insertGroup(group->name, group);
						//그룹 포인터를 리셋시킨다.
						group.reset();
					}
					if (token.empty() == false) {
						//그룹 이름이 있는 경우
						//새로운 그룹을 생성한다.
						group.reset(new MeshGroup());
						group->name = token;
					}
				}
				else if (token == "usemtl") {
					assert(group && "MeshGroup이 생성되지 않았습니다.");
					iss >> token;
					auto it = _mtllib.find(token);
					assert(it != _mtllib.end() && "등록되지 않은 Material입니다.");
					group->material = it->second.material;
					group->texture = it->second.texture;
				}
				else if (token == "mtllib") {
					iss >> token;
					try {
						loadMaterialLibrary(token.c_str());
					}
					catch (std::exception& e) {
						throw e;
					}
				}
			}
			//생성된 모델 포인터를 넘겨준다.
			meshModel = model;
		}
	
		void OBJLoader::loadMaterialLibrary(const char * mtlFile){
			std::ifstream file(mtlFile);
			if (file.good() == false) {
				throw ResourceFileMissing(mtlFile);
			}
			std::string buffer, token;
			Material *material = nullptr;

			buffer.reserve(100);
			token.reserve(50);
			while (std::getline(file, buffer)) {
				if (buffer.empty())
					continue;
				if (buffer[0] == '#')
					continue;
				std::istringstream iss(buffer);
				iss >> token;
				if (token == "newmtl") {
					iss >> token;
					material = &_mtllib[token];
				}
				else if (token == "Ka") {
					iss >> material->material->Ambient.r
						>> material->material->Ambient.g
						>> material->material->Ambient.b;
					material->material->Ambient.a = 1.0f;
				}
				else if (token == "Kd") {
					iss >> material->material->Diffuse.r 
						>> material->material->Diffuse.g 
						>> material->material->Diffuse.b;
					material->material->Diffuse.a = 1.0f;
				}
				else if (token == "Ks") {
					iss >> material->material->Specular.r 
						>> material->material->Specular.g 
						>> material->material->Specular.b;
					material->material->Specular.a = 1.0f;
				}
				else if (token == "d") {
					iss >> material->material->Power;
				}
				else if (token == "Ns") {
					//사용안함
				}
				else if (token == "illum") {
					//사용안함
				}
				else if (token == "map_Kd") {
					iss >> token;
					material->texture = GET_TEXTURE_MGR()->createTexture(token.c_str());
					material = nullptr;
				}
				else {
					//???
				}
			}
		}
	}
}
