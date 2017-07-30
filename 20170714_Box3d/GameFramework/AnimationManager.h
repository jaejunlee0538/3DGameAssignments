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
		configFile�� �о� SpriteManager���� ��������Ʈ�� ã��
		�ִϸ��̼� ��ü�� �����Ѵ�.
		*/
		void loadFromJSON(const tstring& configFile);
		void addAnimation(const tstring& name, SpritesAnimation* animation);
		void saveToJSON(const tstring& outputConfigFile);
		bool hasAnimation(const tstring& name);

		/*
		���� ������ name �ִϸ��̼� ��ü�� �����͸� ��ȯ�Ѵ�.
		*/
		SpritesAnimation* findAnimation(const tstring& name);
	private:
		AnimationMap _animationMap;
	};
}
#define GET_ANIMATION_MANAGER()	SGA::AnimationManager::getSingleton()