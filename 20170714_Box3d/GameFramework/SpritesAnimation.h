#pragma once
#include <vector>
#include <Windows.h>
#include "SpriteIface.h"
#include <functional>
namespace SGA {
	class Sprite;
	class ClockIface;
	class SpritesAnimation : public SpriteIface
	{
	public:
		typedef std::function<void(void)> AnimationCallback;

		SpritesAnimation(const std::vector<SGA::Sprite*>& sprites, float duration, int maxReplayCount = 0);
		
		//Clock, maxReplayCount, sprite���, duration�� �����ϴ� ����������.
		SpritesAnimation(const SpritesAnimation& other);
		virtual ~SpritesAnimation();

		//Clock, maxReplayCount, sprite���, duration �����͸� ������ �ִϸ��̼� ��ü�� ��ȯ�Ѵ�.
		SpritesAnimation* clone() const;

		virtual void release();

		//������ ��������Ʈ ��ü �ð����ķ� _duration��ŭ�� �ð��� ������
		//���� ��������Ʈ�� ���� ��������Ʈ�� ��ü�Ѵ�.
		void update();

		virtual void reset();

		//�ִϸ��̼��� �Ͻ� �����Ѵ�. 
		inline void setPaused(bool paused) {
			_paused = paused;
		}

		//���� ��������Ʈ�� drawPosX, drawPosY�� �������Ѵ�.
		virtual void render(HDC hdc, int drawPosX, int drawPosY) const;

		virtual void setFlipX(bool flipped);
		virtual bool getFlipX() const { return _flipX; }
		virtual void setClipXFactor(float factor);
		virtual void setClipYFactor(float factor);
		//���� ��� ���� ��������Ʈ�� �ȼ� ���� ��ȯ�Ѵ�.
		virtual int getWidth() const;
		//���� ��� ���� ��������Ʈ�� �ȼ� ������ ��ȯ�Ѵ�.
		virtual int getHeight() const;
		//���� ��� ���� ��������Ʈ�� Left��ǥ�� ��ȯ�Ѵ�.
		virtual int getLeft() const;
		//���� ��� ���� ��������Ʈ�� Right��ǥ�� ��ȯ�Ѵ�.
		virtual int getRight() const;
		//���� ��� ���� ��������Ʈ�� Top��ǥ�� ��ȯ�Ѵ�.
		virtual int getTop() const;
		//���� ��� ���� ��������Ʈ�� Bottom��ǥ�� ��ȯ�Ѵ�.
		virtual int getBottom() const;

		//���� �����͸� �����ϰ� ù���� ��������Ʈ���� ����Ѵ�.
		void restart();

		//�ִϸ��̼ǿ� ����� �ð踦 �����Ѵ�.
		//�ʱⰪ : GameWorldClock
		inline void setClock(const ClockIface* clock) {
			_clock = clock;
		}
		
		//����� ������ ��������Ʈ�� �ε����� �����Ѵ�.
		void setCurrentFrameIdx(int spriteIdx);

		//���� ��µǰ� �ִ� ������ �ε����� ��ȯ�Ѵ�.
		inline int getCurrentFrameIdx() const {
			return _currentSprite;
		}

		//��������Ʈ �ϳ��� ����ð�(millisec)�� �����Ѵ�.
		inline void setDuration(float duration) {
			_duration = duration;
		}

		//�� �����Ӵ� ��� �ð��� �����Ѵ�.
		inline float getDuration() const {
			return _duration;
		}

		//�ִϸ��̼� �ݺ� ��� Ƚ���� �����Ѵ�.
		//0���� �����ϸ� �ִϸ��̼��� ������ �ݺ��Ѵ�.
		inline void setMaxReplayCount(int maxReplayCount) {
			_maxReplayCount = maxReplayCount;
		}

		//�ִϸ��̼� �ݺ� ȸ���� ��ȯ�Ѵ�.
		inline int getMaxReplayCount() const{
			return _maxReplayCount;
		}

		//���ݱ��� �� �ִϸ��̼� �ݺ� Ƚ���� ��ȯ�Ѵ�.
		//cbRestart()�޼��带 ȣ���ϸ� ���µȴ�.
		inline int getPlayCount() const {
			return _replayCount;
		}

		//������ Ƚ����ŭ �ִϸ��̼��� �÷��̵Ǹ� true�� ��ȯ.
		//_maxReplayCount�� 0�̸� ���ѹݺ��̱� ������ �׻� false�� ��ȯ.
		inline bool isPlayDone() const {
			if (_maxReplayCount == 0)
				return false;
			if (_replayCount == _maxReplayCount)
				return true;
			return false;
		}

		//�ִϸ��̼��� ���� �ݺ����� ����ȴٸ� true�� ��ȯ�Ѵ�.
		inline bool isInfinite() const {
			return _maxReplayCount == 0;
		}

		//�ִϸ��̼ǿ� ���Ǵ� �� ��������Ʈ ������ ��ȯ�Ѵ�.
		inline int getNSprites() const {
			return _sprites.size();
		}

		void setCallbackPlayDone(AnimationCallback cb) {
			_cbPlayDone = cb;
		}
		void setCallbackCustomFrameBegin(int frameIdx, AnimationCallback cbBegin, AnimationCallback cbEnd) {
			_customCallbackFrame = frameIdx;
			_cbCustomFrameBegin = cbBegin;
			_cbCustomFrameEnd = cbEnd;
		}
		void setCallbackFirstFrameBegin(AnimationCallback cb) {
			_cbFirstFrameBegin = cb;
		}
		void setCallbackFirstFrameEnd(AnimationCallback cb) {
			_cbFirstFrameEnd = cb;
		}
		void setCallbackLastFrameBegin(AnimationCallback cb) {
			_cbLastFrameBegin = cb;
		}
		void setCallbackLastFrameEnd(AnimationCallback cb) {
			_cbLastFrameEnd = cb;
		}
	private:
		AnimationCallback _cbPlayDone;//maxPlayCountȽ����ŭ  �ִϸ��̼��� �÷��̵� �Ŀ� ȣ��
		int _customCallbackFrame;
		AnimationCallback _cbCustomFrameBegin;
		AnimationCallback _cbCustomFrameEnd;
		AnimationCallback _cbFirstFrameBegin;
		AnimationCallback _cbFirstFrameEnd;
		AnimationCallback _cbLastFrameBegin;
		AnimationCallback _cbLastFrameEnd;

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
	};

#define GET_ANIMATION_MANAGER() SGA::AnimationManager::getSingleton()
}