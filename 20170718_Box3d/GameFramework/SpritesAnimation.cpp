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
		//���� ������� ��������Ʈ
		int _currentSprite;
		//�� ��� Ƚ��
		int _replayCount;
		//������ ��������Ʈ ��ȯ �ð�
		float _lastSwitchTime;
		//��������Ʈ ��ȯ �ֱ�
		float _duration;
		//��������Ʈ�� �ٽ� ����ϴ� Ƚ��
		int _maxReplayCount;
		//�ִϸ��̼� ����� ����� �ð�
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
			//���� �ݺ����� �ʰ�, replayCount�� max�� �����ϸ�
			return;
		}
		if ((t - _lastSwitchTime) >= _duration) {
			_currentSprite++;
			if (_currentSprite == _sprites.size()) {
				_replayCount++;
				if (_replayCount == _maxReplayCount) {
					//������ ��������Ʈ���� ����� �����.
					_currentSprite -= 1;
				}
				else {
					//�ٽ� ó�� ��������Ʈ���� ���
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

	//���� ��� ���� ��������Ʈ�� �ȼ� ���� ��ȯ�Ѵ�.

	int SpritesAnimation::getWidth() const
	{
		return _sprites[_currentSprite]->getWidth();
	}

	//���� ��� ���� ��������Ʈ�� �ȼ� ������ ��ȯ�Ѵ�.

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