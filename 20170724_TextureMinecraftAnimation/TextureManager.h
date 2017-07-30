#pragma once
namespace SGA {
	class TextureManager
	{
	public:
		TextureManager();
		~TextureManager();

		void init(IDirect3DDevice9* device, const TCHAR* textureRootPath);

		//���� ������ �ʰ� �ִ� �ؽ��ĸ� ��� �����Ѵ�.
		void releaseUnusedTexture();

		//��� �ؽ��ĸ� �����Ѵ�.
		void release();

		//textureFile �ؽ����� �������̽� �����͸� ��ȯ�Ѵ�.
		//�ؽ��� ������ �������� ���� ��쿡�� ResourceFileMissing ���ܸ� throw�Ѵ�.
		D3DTexturePtr createTexture(const TCHAR* textureFile);
	private:
		IDirect3DDevice9* _device = nullptr;
		tstring _textureRootPath;
		std::map<tstring, D3DTexturePtr> _textures;
	};
}

