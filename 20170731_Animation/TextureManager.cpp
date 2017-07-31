#include "stdafx.h"
#include "TextureManager.h"

namespace SGA {
	TextureManager::TextureManager()
	{
	}

	TextureManager::~TextureManager()
	{
		release();
	}

	TextureManager * TextureManager::getInstance()
	{
		static TextureManager textMgr;
		return &textMgr;
	}

	void TextureManager::init(IDirect3DDevice9 * device, const char* textureRootPath)
	{
		_device = device;
		if (textureRootPath)
		{
			_textureRootPath = textureRootPath;
			if (_textureRootPath.back() != TEXT('\\') || _textureRootPath.back() != TEXT('/')) {
				if (_textureRootPath.find_first_of(TEXT('\\')) != std::string::npos) {
					_textureRootPath.push_back(TEXT('\\'));
				}
				else {
					_textureRootPath.push_back(TEXT('/'));
				}
			}
		}
		else {
			_textureRootPath = TEXT("");
		}
	}

	void TextureManager::releaseUnusedTexture()
	{
		for (auto it = _textures.begin(); it != _textures.end();) {
			if (it->second.use_count() == 1) {
				//�ؽ��ĸ� ����ϰ� �ִ� ���� 1���̸�(TextureManager�� ����ϰ� ����) Release��Ų��.
				it = _textures.erase(it);
			}
			else {
				++it;
			}
		}
	}

	void TextureManager::release()
	{
		for (auto it = _textures.begin(); it != _textures.end(); ++it) {
			assert(it->second.use_count() == 1 && "���� �ؽ��ĸ� ��� ���Դϴ�.");
			SAFE_RELEASE(it->second);
		}
		_textures.clear();
	}

	D3DTexturePtr TextureManager::createTexture(const char* textureFile) {
		assert(_device != nullptr && "TextureManager�� �ʱ�ȭ���� �ʾҽ��ϴ�");
		auto it = _textures.find(textureFile);
		if (it != _textures.end()) {
			return it->second;
		}
		else {
			IDirect3DTexture9* texture = nullptr;
			std::string fullFilePath = _textureRootPath + textureFile;
			HRESULT ret = D3DXCreateTextureFromFile(_device, fullFilePath.c_str(), &texture);
			assert(ret == D3D_OK);
			//TODO : HRESULT ��Ȯ��!
			std::shared_ptr<IDirect3DTexture9> texturePtr(texture, [](IDirect3DTexture9* texture) { 
				SAFE_RELEASE(texture);
			});
			if (texture == nullptr) {
				throw ResourceFileMissing(fullFilePath.c_str());
			}
			_textures.insert(std::make_pair(std::string(textureFile), texturePtr));
			return texturePtr;
		}
	}
}
