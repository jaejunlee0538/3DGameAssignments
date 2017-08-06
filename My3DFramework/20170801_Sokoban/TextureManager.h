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

		//현재 사용되지 않고 있는 텍스쳐를 모두 제거한다.
		void releaseUnusedTexture();

		//모든 텍스쳐를 제거한다.
		void release();

		std::weak_ptr<IDirect3DTexture9> createTexture(const char* textureFile);
		//textureFile 텍스쳐의 인터페이스 포인터를 반환한다.
		//텍스쳐 파일을 생성하지 못한 경우에는 ResourceFileMissing 예외를 throw한다.
	private:
		IDirect3DDevice9* _device = nullptr;
		std::string _textureRootPath;
		std::map<std::string, std::shared_ptr<IDirect3DTexture9>> _textures;
	};
}

#define GET_TEXTURE_MGR()	SGA::TextureManager::getInstance()