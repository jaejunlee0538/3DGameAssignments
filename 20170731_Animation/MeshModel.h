#pragma once
namespace SGA {
	struct MeshGroup {
		typedef std::shared_ptr<MeshGroup> Ptr;

		void render(IDirect3DDevice9* device);

		D3DMaterialPtr material;
		D3DTexturePtr texture;
		std::vector<VertexPosNormTex> vertices;
		std::string name;
	};

	class MeshModel
	{
	public:
		typedef std::shared_ptr<MeshModel> Ptr;
	public:
		MeshModel();
		~MeshModel();
		
		void clear();
		void render(IDirect3DDevice9* device);
		void renderGroup(IDirect3DDevice9* device, std::string groupName);
		void insertGroup(std::string groupName, MeshGroup::Ptr group);
	private:
		std::map<std::string, MeshGroup::Ptr> _groups;
	};
}