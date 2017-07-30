#pragma once
namespace SGA {
	class TextureManager
	{
	public:
		TextureManager();
		~TextureManager();

		void init(IDirect3DDevice9* device, const TCHAR* textureRootPath);

		//현재 사용되지 않고 있는 텍스쳐를 모두 제거한다.
		void releaseUnusedTexture();

		//모든 텍스쳐를 제거한다.
		void release();

		//textureFile 텍스쳐의 인터페이스 포인터를 반환한다.
		//텍스쳐 파일을 생성하지 못한 경우에는 ResourceFileMissing 예외를 throw한다.
		D3DTexturePtr createTexture(const TCHAR* textureFile);
	private:
		IDirect3DDevice9* _device = nullptr;
		tstring _textureRootPath;
		std::map<tstring, D3DTexturePtr> _textures;
	};
}

