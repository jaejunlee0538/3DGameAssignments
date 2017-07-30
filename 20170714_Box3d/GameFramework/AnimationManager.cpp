#include "AnimationManager.h"
#include <assert.h>
#include "json.hpp"
#include "SpriteManager.h"
#include <fstream>
#include <tstring.h>
namespace SGA {
	AnimationManager::AnimationManager()
	{

	}


	AnimationManager::~AnimationManager()
	{

	}

	void AnimationManager::clearAll()
	{
		for (auto it = _animationMap.begin(); it != _animationMap.end(); ++it) {
			delete it->second;
		}
		_animationMap.clear();
	}

	void AnimationManager::loadFromJSON(const tstring & configFile)
	{
		using json = nlohmann::json;
		std::ifstream file(configFile.c_str());
		assert(file.good() && "Opening input json file failed");
		json jsonDoc;
		file >> jsonDoc;
		//tstring spriteName = convertToTString((*it)["filename"].get<std::string>());
		for (auto it = jsonDoc["animation"].begin(); it != jsonDoc["animation"].end(); ++it) {
			//tstring animationName = (*it)["name"];
			tstring animationName = convertToTString((*it)["name"].get<std::string>());
			float duration = (*it)["duration"];
			int maxReplayCount = (*it)["maxReplayCount"];
			std::vector<Sprite*> sprites;
			if ((*it)["sprites"].count("count") != 0) {
				int spriteCount = (*it)["sprites"]["count"];
				tstring spriteNamePrefix = convertToTString((*it)["sprites"]["prefix"].get<std::string>());
				sprites.reserve(spriteCount);
				for (int i = 0; i < spriteCount; ++i) {
					tstring spriteName = spriteNamePrefix + TEXT("_") + to_tstring(i);
					Sprite* sprite = GET_SPRITE_MANAGER()->findSprite(spriteName);
					assert(sprite != NULL && "Cannot find sprite for animation");
					sprites.push_back(sprite);
				}
			}
			else {
				sprites.reserve((*it)["sprites"].size());
				for (auto it2 = (*it)["sprites"].begin(); it2 != (*it)["sprites"].end(); ++it2) {
					SGA::tstring spriteName = convertToTString((*it2).get<std::string>());
					Sprite* sprite = GET_SPRITE_MANAGER()->findSprite(spriteName);
					assert(sprite != NULL && "Cannot find sprite for animation");
					sprites.push_back(sprite);
				}
			}
			addAnimation(animationName, new SpritesAnimation(sprites, duration, maxReplayCount));
		}
	}

	void AnimationManager::addAnimation(const tstring& name, SpritesAnimation * animation)
	{
		AnimationMap::iterator it = _animationMap.find(name);
		assert(it == _animationMap.end() && "Animation with same name already exists");
		_animationMap.insert(std::make_pair(name, animation));
	}

	void AnimationManager::saveToJSON(const tstring & outputConfigFile)
	{
		std::ofstream fileOut(outputConfigFile.c_str());
		assert(fileOut.good() && "Opening output file failed");
		nlohmann::json json;
		for (auto it = _animationMap.begin(); it != _animationMap.end(); ++it) {
			nlohmann::json anim;
			anim["name"] = it->first;
			anim["sprites"]["prefix"] = it->first;
			anim["sprites"]["count"] = it->second->getNSprites();
			anim["duration"] = it->second->getDuration();
			anim["maxReplayCount"] = it->second->getMaxReplayCount();
			json["animation"].push_back(anim);
		}
		fileOut << json << std::endl;
		fileOut.close();
	}

	bool AnimationManager::hasAnimation(const tstring& name) {
		return _animationMap.count(name) == 1;
	}

	SpritesAnimation * AnimationManager::findAnimation(const tstring& name)
	{
		AnimationMap::iterator it = _animationMap.find(name);
		assert(it != _animationMap.end() && "Finding animation failed");
		return it->second->clone();
	}
}