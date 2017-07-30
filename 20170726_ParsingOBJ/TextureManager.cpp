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

	void TextureManager::init(IDirect3DDevice9 * device, const TCHAR* textureRootPath)
	{
		_device = device;
		if (textureRootPath)
		{
			_textureRootPath = textureRootPath;
			if (_textureRootPath.back() != TEXT('\\') || _textureRootPath.back() != TEXT('/')) {
				if (_textureRootPath.find_first_of(TEXT('\\')) != tstring::npos) {
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
				//텍스쳐를 사용하고 있는 곳이 1곳이면(TextureManager만 사용하고 있음) Release시킨다.
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
			assert(it->second.use_count() == 1 && "아직 텍스쳐를 사용 중입니다.");
			SAFE_RELEASE(it->second);
		}
		_textures.clear();
	}

	D3DTexturePtr TextureManager::createTexture(const TCHAR* textureFile) {
		assert(_device != nullptr && "TextureManager가 초기화되지 않았습니다");
		auto it = _textures.find(textureFile);
		if (it != _textures.end()) {
			return it->second;
		}
		else {
			IDirect3DTexture9* texture = nullptr;
			tstring fullFilePath = _textureRootPath + textureFile;
			D3DXCreateTextureFromFile(_device, fullFilePath.c_str(), &texture);
			//TODO : HRESULT 값확인!
			D3DTexturePtr texturePtr(texture, [](IDirect3DTexture9* texture) { SAFE_RELEASE(texture); });
			if (texture == nullptr) {
				throw ResourceFileMissing(convertToString(fullFilePath).c_str());
			}
			_textures.insert(std::make_pair(tstring(textureFile), texturePtr));
			return texturePtr;
		}
	}
}
