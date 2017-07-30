#pragma once
namespace SGA {

	class TextureManager
	{
	private:
		TextureManager();
		~TextureManager();
	public:
		static TextureManager* getInstance();

		void init(IDirect3DDevice9* device, const char* textureRootPath);

		//���� ������ �ʰ� �ִ� �ؽ��ĸ� ��� �����Ѵ�.
		void releaseUnusedTexture();

		//��� �ؽ��ĸ� �����Ѵ�.
		void release();

		std::weak_ptr<IDirect3DTexture9> createTexture(const char* textureFile);
		//textureFile �ؽ����� �������̽� �����͸� ��ȯ�Ѵ�.
		//�ؽ��� ������ �������� ���� ��쿡�� ResourceFileMissing ���ܸ� throw�Ѵ�.
	private:
		IDirect3DDevice9* _device = nullptr;
		std::string _textureRootPath;
		std::map<std::string, std::shared_ptr<IDirect3DTexture9>> _textures;
	};
}

#define GET_TEXTURE_MGR()	SGA::TextureManager::getInstance()