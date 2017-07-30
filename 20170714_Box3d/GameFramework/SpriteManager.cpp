#include "SpriteManager.h"
#include "json.hpp"
#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <sstream>

namespace SGA {

	SpriteManager::SpriteManager()
	{
	}


	SpriteManager::~SpriteManager()
	{
		release();
	}

	void SpriteManager::release()
	{
		//스프라이트 객체 모두 삭제
		for (auto it = _spriteMap.begin(); it != _spriteMap.end(); ++it) {
			delete it->second;
		}
		_spriteMap.clear();
		//아틀라스 객체 모두 삭제
		for (auto it = _atlasImageMap.begin(); it != _atlasImageMap.end(); ++it) {
			delete it->second.source;
		}
		_atlasImageMap.clear();
	}

	void SpriteManager::loadFromJSON(const tstring & configFile)
	{
		using json = nlohmann::json;
		std::ifstream file(configFile.c_str());
		assert(file.good());

		json jsonFile;
		file >> jsonFile;
		tagAtlasImage atlas = loadAtlasImage(
			convertToTString(jsonFile["meta"]["image"].get<std::string>()),
			convertToTString(jsonFile["meta"]["format"].get<std::string>()));
		
		int x, y, w, h;
		float px, py;
		//tstring spriteName;
		for (auto it = jsonFile["frames"].begin(); it != jsonFile["frames"].end(); ++it) {
			tstring spriteName = convertToTString((*it)["filename"].get<std::string>());
			x = (*it)["frame"]["x"];
			y = (*it)["frame"]["y"];
			w = (*it)["frame"]["w"];
			h = (*it)["frame"]["h"];
			px = (*it)["pivot"]["x"];
			py = (*it)["pivot"]["y"];
			insertSprite(spriteName, new Sprite(atlas.source, x, y, w, h, px, py));
		}
	}

	void SpriteManager::addSprite(const tstring & imageFile, const tstring & spriteName, BitmapImage::TransparentModes transparentMode, float pivotX, float pivotY)
	{
		tagAtlasImage atlas = loadAtlasImage(imageFile, transparentMode);
		Sprite* sprite = new Sprite(atlas.source, 0, 0,
			atlas.source->getWidth(), atlas.source->getHeight(), pivotX, pivotY);
		insertSprite(spriteName, sprite);
	}

	void SpriteManager::addSprite(const tstring & imageFile, const tstring & spriteName, int clipX, int clipY, int clipW, int clipH)
	{
		assert(false);
	}

	void SpriteManager::addSprite(const tstring & imageFile, const tstring & spriteName, int clipX, int clipY, int clipW, int clipH, int pivotX, int pivotY)
	{
		assert(false);
	}

	void SpriteManager::addFrameSprite(const tstring & imageFile, const tstring & spriteNamePrefix, int beginX, int beginY, int clipW, int clipH, int nFrameX, int nFrameY, bool horizontalFirst)
	{
		//tagAtlasImage atlas = loadAtlasImage(imageFile);
		assert(false);
	}

	void SpriteManager::findSpriteList(tstring namePrefix, std::vector<const Sprite*>& sprites)
	{
		int count = 0;
		if (findSprite(namePrefix + TEXT("_") + to_tstring(0)) == NULL)
			count = 1;
		while (true) {
			tstring name = namePrefix +TEXT("_")+ to_tstring(count);
			const Sprite * sprite = findSprite(name);
			if (sprite == NULL) {
				break;
			}
			sprites.push_back(sprite);
			count++;
		}
		assert(sprites.empty() == false);

	}

	void SpriteManager::findSpritesStartWith(tstring namePrefix, std::vector<const Sprite*>& sprites)
	{
		for (auto it = _spriteMap.begin(); it != _spriteMap.end(); ++it) {
			if (it->first.compare(0, namePrefix.size(), namePrefix.c_str()) == 0) {
				sprites.push_back(it->second);
			}
		}
	}

	void SpriteManager::findSpritesStartWith(tstring namePrefix, std::vector<std::pair<tstring, const Sprite*>>& sprites)
	{
		for (auto it = _spriteMap.begin(); it != _spriteMap.end(); ++it) {
			if (it->first.compare(0, namePrefix.size(), namePrefix.c_str()) == 0) {
				sprites.push_back(*it);
			}
		}
	}

	Sprite * SpriteManager::findSprite(const tstring & key)
	{
		TileSpriteMap::iterator it = _spriteMap.find(key);
		if (it == _spriteMap.end()) {
			return NULL;
		}
		return it->second->clone();
	}

	SpriteManager::tagAtlasImage SpriteManager::loadAtlasImage(const tstring & imageFile, const tstring& colorFormat)
	{
		tagAtlasImage atlas;
		auto it = _atlasImageMap.find(imageFile);
		if (it == _atlasImageMap.end()) {
			atlas.source = new BitmapImage();
			assert(atlas.source != NULL);
			if (colorFormat == TEXT("RGBA8888")) {
				assert(atlas.source->init(imageFile.c_str(), BitmapImage::TRANSPARENT_MODE_ALPHA) == S_OK);
			}
			else if (colorFormat == TEXT("RGB888")) {
				assert(atlas.source->init(imageFile.c_str(), BitmapImage::TRANSPARENT_MODE_KEYCOLOR, RGB(255, 0, 255)) == S_OK);
			}
			
			_atlasImageMap.insert(std::make_pair(imageFile, atlas));//맵에 저장
		}
		else {
			atlas = it->second;
		}
		return atlas;
	}

	SpriteManager::tagAtlasImage SpriteManager::loadAtlasImage(const tstring & imageFile, BitmapImage::TransparentModes transparentMode)
	{
		tagAtlasImage atlas;
		auto it = _atlasImageMap.find(imageFile);
		if (it == _atlasImageMap.end()) {
			atlas.source = new BitmapImage();
			assert(atlas.source != NULL);
			assert(atlas.source->init(imageFile.c_str(), transparentMode, RGB(255, 0, 255)) == S_OK);
			_atlasImageMap.insert(std::make_pair(imageFile, atlas));//맵에 저장
		}
		else {
			atlas = it->second;
		}
		return atlas;
	}

	void SpriteManager::insertSprite(const tstring & key, Sprite * sprite)
	{
		assert(sprite != NULL);
		auto result = _spriteMap.insert(std::make_pair(key, sprite));
		assert(result.second == true);
	}
}