#include "EffectManager.h"
#include <AnimationManager.h>
#include <SpritesAnimation.h>
#include <CameraManager.h>
namespace SGA {

	EffectManager::tagEffect::tagEffect(SpritesAnimation * anim, float x, float y, float priority, tstring targetCam)
		:anim(anim), priority(priority), x(x), y(y), targetCam(targetCam) {

	}
	void EffectManager::tagEffect::release() {
		if (anim) {
			anim->release();
			delete anim;
		}
	}

	EffectManager::EffectManager()
	{
	}


	EffectManager::~EffectManager()
	{
		release();
	}

	HRESULT EffectManager::init()
	{
		return S_OK;
	}

	void EffectManager::update()
	{
		for (auto it = _animPlaying.begin(); it != _animPlaying.end(); ) {
			if (it->anim->isPlayDone()) {
				it->release();
				it = _animPlaying.erase(it);
			}
			else {
				it->anim->update();
				++it;
			}
		}
	}

	void EffectManager::render()
	{
		for (auto it = _animPlaying.begin(); it != _animPlaying.end(); ++it) {
			if (it->targetCam.empty()) {
				GET_CAMERA_MANAGER()->render(it->anim, it->x, it->y, it->priority);
			}
			else {
				GET_CAMERA_MANAGER()->render(it->anim, it->x, it->y, it->priority, it->targetCam);
			}
		}
	}

	void EffectManager::release()
	{
		stopAll();
	}

	void EffectManager::play(const tstring & animName, float x, float y, float renderPriority, tstring targetCam)
	{
		SpritesAnimation* anim = GET_ANIMATION_MANAGER()->findAnimation(animName);
		if (anim) {
			_animPlaying.push_back(tagEffect(anim, x,y, renderPriority, targetCam));
		}
	}

	void EffectManager::play(const tstring & animName, float x, float y, float renderPriority, int replayCount, tstring targetCam)
	{
		SpritesAnimation* anim = GET_ANIMATION_MANAGER()->findAnimation(animName);
		if (anim) {
			anim->setMaxReplayCount(replayCount);
			_animPlaying.push_back(tagEffect(anim, x, y, renderPriority, targetCam));
		}
	}

	void EffectManager::stopAll()
	{
		for (auto it = _animPlaying.begin(); it != _animPlaying.end(); ++it) {
			it->release();
		}
		_animPlaying.clear();
	}

}