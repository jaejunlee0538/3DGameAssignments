#include "SpritesAnimation.h"
#include "Sprite.h"
#include "ClockManager.h"
#include <assert.h>
#include <algorithm>
namespace SGA {
	SpritesAnimation::SpritesAnimation(const std::vector<SGA::Sprite*>& sprites, 
		float duration, int maxReplayCount)
		:_sprites(sprites), _duration(duration), _maxReplayCount(maxReplayCount), _paused(false)
	{
		setClock(GET_CLOCK_MANAGER()->getGameWorldClock());
		assert(std::all_of(sprites.begin(), sprites.end(),
			[](SGA::Sprite* sprite) {return sprite != NULL; }));
		restart();
	}

	SpritesAnimation::SpritesAnimation(const SpritesAnimation & other)
	{
		/*
		AnimationCallback _cbPlayDone;
		AnimationCallback _cbCustomFrame;
		AnimationCallback _cbLoopOnce;

		std::vector<SGA::Sprite*> _sprites;
		//현재 재생중인 스프라이트
		int _currentSprite;
		//총 재생 횟수
		int _replayCount;
		//마지막 스프라이트 전환 시각
		float _lastSwitchTime;
		//스프라이트 전환 주기
		float _duration;
		//스프라이트를 다시 재생하는 횟수
		int _maxReplayCount;
		//애니메이션 재생에 사용할 시계
		//default : GameWorldClock
		const ClockIface* _clock;

		bool _paused;

		bool _flipX;
		*/
		_sprites.resize(other.getNSprites());
		for (size_t i = 0; i < _sprites.size(); ++i) {
			_sprites[i] = other._sprites[i]->clone();
		}
		_currentSprite = other._currentSprite;
		_replayCount = other._replayCount;
		_lastSwitchTime = other._lastSwitchTime;
		_duration = other._duration;
		_maxReplayCount = other._maxReplayCount;
		setClock(other._clock);
		_paused = other._paused;
		_flipX = other._flipX;
	}

	SpritesAnimation::~SpritesAnimation()
	{
		release();
	}

	SpritesAnimation * SpritesAnimation::clone() const
	{
		return new SpritesAnimation(*this);
	}

	void SpritesAnimation::release()
	{
		for (int i = 0; i < _sprites.size(); ++i) {
			_sprites[i]->release();
			delete _sprites[i];
		}
		_sprites.clear();
	}

	void SpritesAnimation::update()
	{
		float t = _clock->getCurrentTime();
		if (_paused) {
			_lastSwitchTime = t;
			return;
		}
		if (_maxReplayCount != 0 && _replayCount == _maxReplayCount) {
			//무한 반복하지 않고, replayCount가 max에 도달하면
			return;
		}
		if ((t - _lastSwitchTime) >= _duration) {
			_currentSprite++;
			if (_currentSprite == _sprites.size()) {
				_replayCount++;
				if (_replayCount == _maxReplayCount) {
					//마지막 스프라이트에서 재생을 멈춘다.
					_currentSprite -= 1;
				}
				else {
					//다시 처음 스프라이트부터 재생
					_currentSprite = 0;
				}
			}
			_lastSwitchTime = t;
		}
	}

	void SpritesAnimation::reset()
	{
		restart();
	}	
	
	void SpritesAnimation::render(HDC hdc, int drawPosX, int drawPosY) const
	{
		_sprites[_currentSprite]->render(hdc, drawPosX, drawPosY);
	}

	void SpritesAnimation::setFlipX(bool flipped)
	{
		_flipX = flipped;
		for (size_t i = 0; i < _sprites.size(); ++i) {
			_sprites[i]->setFlipX(flipped);
		}
	}

	void SpritesAnimation::setClipXFactor(float factor)
	{
		for (size_t i = 0; i < _sprites.size(); ++i) {
			_sprites[i]->setClipXFactor(factor);
		}
	}

	void SpritesAnimation::setClipYFactor(float factor)
	{
		for (size_t i = 0; i < _sprites.size(); ++i) {
			_sprites[i]->setClipYFactor(factor);
		}
	}

	//현재 재생 중인 스프라이트의 픽셀 폭을 반환한다.

	int SpritesAnimation::getWidth() const
	{
		return _sprites[_currentSprite]->getWidth();
	}

	//현재 재생 중인 스프라이트의 픽셀 높이을 반환한다.

	int SpritesAnimation::getHeight() const
	{
		return _sprites[_currentSprite]->getHeight();
	}

	/*void SpritesAnimation::renderMirrorX(HDC hdc, int drawPosX, int drawPosY) const
	{
		_sprites[_currentSprite]->renderMirrorX(hdc, drawPosX, drawPosY);
	}*/

	//void SpritesAnimation::renderRot(HDC hdc, int drawPosX, int drawPosY, float angleRad) const
	//{
	//	_sprites[_currentSprite]->renderRot(hdc, drawPosX, drawPosY, angleRad);
	//}

	int SpritesAnimation::getLeft() const
	{
		return _sprites[_currentSprite]->getLeft();
	}

	int SpritesAnimation::getRight() const
	{
		return _sprites[_currentSprite]->getRight();
	}

	int SpritesAnimation::getTop() const
	{
		return _sprites[_currentSprite]->getTop();
	}

	int SpritesAnimation::getBottom() const
	{
		return _sprites[_currentSprite]->getBottom();
	}

	void SpritesAnimation::restart()
	{
		_currentSprite = 0;
		_replayCount = 0;
		_lastSwitchTime = _clock->getCurrentTime();
	}

	void SpritesAnimation::setCurrentFrameIdx(int spriteIdx) {
		if (spriteIdx >= getNSprites()) {
			spriteIdx = getNSprites() - 1;
		}
		_currentSprite = spriteIdx;
		_lastSwitchTime = _clock->getCurrentTime();
	}
}