#pragma once
namespace SGA {
	namespace __ASE_private {
		class Node;
	}
	class MeshObject;
	class MeshModel;
	class MeshGroup;
	class ASEParser
	{
		struct tagMaterial {
			std::string mtlName;
			D3DMaterialPtr material;
			D3DTexturePtr texture;
			tagMaterial():
			material(new D3DMATERIAL9){
				material->Emissive = D3DXCOLOR(0, 0, 0, 0);
				material->Power = 20.0f;
			}
		};
		struct tagFace {
			int vertexIndices[3];
			int tVertexIndices[3];
			D3DXVECTOR3 normals[3];
		};
	public:
		ASEParser();
		~ASEParser();

		void load(const char* filePath, MeshObject** object);
	private:
		bool isClosingBracket(const char* str);
		void parseScene(FILE * file);
		void parseMtllib(FILE * file);
		void parseMaterial(tagMaterial& material, FILE * file);
		void parseMapDiffuse(tagMaterial& material, FILE * file);
		void parseGeomObject(MeshObject& object, FILE * file);
		void parseNodeTM(MeshObject& object, FILE* file);
		void parseVertexList(FILE* file);
		void parseFaceList(FILE* file);
		void parseTVertexList(FILE* file);
		void parseTFaceList(FILE* file);
		void parseNormals(FILE* file);
		void parseMesh(FILE* file);
		void parseTMAnimation(FILE* file);
		void parsePosTrack(FILE* file);
		void parseRotTrack(FILE* file);
		void parseCtrlRotTCB(FILE* file);
		void insertObject(std::string objectName, MeshObject* gameObject);
		void nextLine(FILE* file);

		enum {
			BUFFER_SIZE = 256
		};
		char _buffer[BUFFER_SIZE];
		std::vector<tagMaterial> _mtlLib;
		std::vector<D3DXVECTOR3> _vertices;
		std::vector<D3DXVECTOR3> _uv;
		std::vector<tagFace> _faces;
		int _materialIndex;

		std::map<std::string, MeshObject*> _gameObjects;
		
	};
}