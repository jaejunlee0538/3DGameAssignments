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

		void load(const char* filePath, MeshObject** object, AnimationClip** animationClip);

	public:
		int _firstFrame;
		int _lastFrame;
		int _frameSpeed;
		int _tickPerFrame;
	private:

		bool isClosingBracket(const char* str);
		void parseScene();
		void parseMtllib();
		void parseMaterial(tagMaterial& material);
		void parseMapDiffuse(tagMaterial& material);
		void parseGeomObject(MeshObject& object, BoneAnimation& animation);
		void parseNodeTM(D3DXMATRIX& mat);
		void parseVertexList();
		void parseFaceList();
		void parseTVertexList();
		void parseTFaceList();
		void parseNormals();
		void parseMesh();
		void parseTMAnimation(BoneAnimation& animation, MeshObject& object);
		void parsePosTrack(BoneAnimation& animation, MeshObject& object);
		void parseRotTrack(BoneAnimation& animation, MeshObject& object);
		void parseCtrlRotTCB(BoneAnimation& animation, MeshObject& object);
		void insertObject(std::string objectName, MeshObject* gameObject);
		void nextLine();

		void getLocalTransform(std::string objFrom, std::string objTo, D3DXMATRIX& transform);

	private:
		enum {
			BUFFER_SIZE = 256
		};
		char _buffer[BUFFER_SIZE];
		std::vector<tagMaterial> _mtlLib;
		std::vector<D3DXVECTOR3> _vertices;
		std::vector<D3DXVECTOR3> _uv;
		std::vector<tagFace> _faces;
		int _materialIndex;

		FILE* _file;

		std::map<std::string, MeshObject*> _gameObjects;
		std::map<std::string, D3DXMATRIX> _objectTransforms;
	};
}