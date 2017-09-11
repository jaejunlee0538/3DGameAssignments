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
						//XYZ �ε���
						std::getline(iss2, idx, '/');
						assert(idx.empty() == false && "XYZ �ε��� �Ľ� ����");
						idxPos = atoi(idx.c_str()) - 1;
						//�ؽ��� �ε���
						std::getline(iss2, idx, '/');
						assert(idx.empty() == false && "�ؽ��� �ε��� �Ľ� ����");
						idxUV = atoi(idx.c_str()) - 1;
						//���� �ε���
						std::getline(iss2, idx, '/');
						assert(idx.empty() == false && "���� �ε��� �Ľ� ����");
						idxNormal = atoi(idx.c_str()) - 1;
						group->vertices.emplace_back(VertexPosNormTex(_position[idxPos], _normal[idxNormal], _uv[idxUV]));
					}
				}
				else if (token == "s") {
					//������ ���̵�
					continue;
				}
				else if (token == "g") {
					token.clear();
					iss >> token;
					if (group) {
						//�׷� ������ ����.
						//�׷��� �𵨿� �����Ѵ�.
						model->insertGroup(group->name, group);
						//�׷� �����͸� ���½�Ų��.
						group.reset();
					}
					if (token.empty() == false) {
						//�׷� �̸��� �ִ� ���
						//���ο� �׷��� �����Ѵ�.
						group.reset(new MeshGroup());
						group->name = token;
					}
				}
				else if (token == "usemtl") {
					assert(group && "MeshGroup�� �������� �ʾҽ��ϴ�.");
					iss >> token;
					auto it = _mtllib.find(token);
					assert(it != _mtllib.end() && "��ϵ��� ���� Material�Դϴ�.");
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
			//������ �� �����͸� �Ѱ��ش�.
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
					//������
				}
				else if (token == "illum") {
					//������
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
