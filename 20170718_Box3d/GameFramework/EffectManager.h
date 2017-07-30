#pragma once
#include <tstring.h>
#include <list>
#include <Windows.h>
#include <SingletonBase.h>

namespace SGA {
	class SpritesAnimation;
	class EffectManager : public SingletonBase<EffectManager>
	{
		struct tagEffect {
			SpritesAnimation* anim;
			float priority;
			tstring targetCam;
			float x, y;
			tagEffect(SpritesAnimation* anim, float x, float y, float priority, tstring targetCam = TEXT(""));
			void release();
		};
		typedef std::list<tagEffect> PlayingAnimations;

	public:
		EffectManager();
		~EffectManager();

		HRESULT init();
		void update();
		void render();
		void release();
		void play(const tstring& animName, float x, float y, float renderPriority, tstring targetCam = TEXT(""));
		void play(const tstring& animName, float x, float y, float renderPriority, int replayCount, tstring targetCam = TEXT(""));
		void stopAll();

	private:
		PlayingAnimations _animPlaying;
	};
}
#define GET_EFFECT_MANAGER() SGA::EffectManager::getSingleton()