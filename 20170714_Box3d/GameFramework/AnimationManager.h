#pragma once
#include "SingletonBase.h"
#include "SpritesAnimation.h"
#include <map>
#include <tstring.h>
#include <list>
namespace SGA {
	class AnimationManager : public SingletonBase<AnimationManager>
	{
		typedef std::map<tstring, SpritesAnimation*> AnimationMap;
	public:
		AnimationManager();
		~AnimationManager();
		void clearAll();
		/*
		configFile을 읽어 SpriteManager에서 스프라이트를 찾아
		애니메이션 객체를 생성한다.
		*/
		void loadFromJSON(const tstring& configFile);
		void addAnimation(const tstring& name, SpritesAnimation* animation);
		void saveToJSON(const tstring& outputConfigFile);
		bool hasAnimation(const tstring& name);

		/*
		복사 생성된 name 애니메이션 객체의 포인터를 반환한다.
		*/
		SpritesAnimation* findAnimation(const tstring& name);
	private:
		AnimationMap _animationMap;
	};
}
#define GET_ANIMATION_MANAGER()	SGA::AnimationManager::getSingleton()