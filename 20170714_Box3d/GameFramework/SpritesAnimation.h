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
		
		//Clock, maxReplayCount, sprite목록, duration을 복사하는 복제생성자.
		SpritesAnimation(const SpritesAnimation& other);
		virtual ~SpritesAnimation();

		//Clock, maxReplayCount, sprite목록, duration 데이터를 복제한 애니메이션 객체를 반환한다.
		SpritesAnimation* clone() const;

		virtual void release();

		//마지막 스프라이트 교체 시각이후로 _duration만큼의 시간이 지나면
		//현재 스프라이트를 다음 스프라이트로 교체한다.
		void update();

		virtual void reset();

		//애니메이션을 일시 중지한다. 
		inline void setPaused(bool paused) {
			_paused = paused;
		}

		//현재 스프라이트를 drawPosX, drawPosY에 렌더링한다.
		virtual void render(HDC hdc, int drawPosX, int drawPosY) const;

		virtual void setFlipX(bool flipped);
		virtual bool getFlipX() const { return _flipX; }
		virtual void setClipXFactor(float factor);
		virtual void setClipYFactor(float factor);
		//현재 재생 중인 스프라이트의 픽셀 폭을 반환한다.
		virtual int getWidth() const;
		//현재 재생 중인 스프라이트의 픽셀 높이을 반환한다.
		virtual int getHeight() const;
		//현재 재생 중인 스프라이트의 Left좌표를 반환한다.
		virtual int getLeft() const;
		//현재 재생 중인 스프라이트의 Right좌표를 반환한다.
		virtual int getRight() const;
		//현재 재생 중인 스프라이트의 Top좌표를 반환한다.
		virtual int getTop() const;
		//현재 재생 중인 스프라이트의 Bottom좌표를 반환한다.
		virtual int getBottom() const;

		//누적 데이터를 리셋하고 첫번재 스프라이트부터 재생한다.
		void restart();

		//애니메이션에 사용할 시계를 설정한다.
		//초기값 : GameWorldClock
		inline void setClock(const ClockIface* clock) {
			_clock = clock;
		}
		
		//재생을 시작할 스프라이트의 인덱스를 설정한다.
		void setCurrentFrameIdx(int spriteIdx);

		//현재 출력되고 있는 프레임 인덱스를 반환한다.
		inline int getCurrentFrameIdx() const {
			return _currentSprite;
		}

		//스프라이트 하나당 재생시간(millisec)을 세팅한다.
		inline void setDuration(float duration) {
			_duration = duration;
		}

		//한 프레임당 출력 시간을 설정한다.
		inline float getDuration() const {
			return _duration;
		}

		//애니메이션 반복 재생 횟수를 설정한다.
		//0으로 설정하면 애니메이션을 무한정 반복한다.
		inline void setMaxReplayCount(int maxReplayCount) {
			_maxReplayCount = maxReplayCount;
		}

		//애니메이션 반복 회수를 반환한다.
		inline int getMaxReplayCount() const{
			return _maxReplayCount;
		}

		//지금까지 총 애니메이션 반복 횟수를 반환한다.
		//cbRestart()메서드를 호출하면 리셋된다.
		inline int getPlayCount() const {
			return _replayCount;
		}

		//정해진 횟수만큼 애니메이션이 플레이되면 true를 반환.
		//_maxReplayCount가 0이면 무한반복이기 때문에 항상 false를 반환.
		inline bool isPlayDone() const {
			if (_maxReplayCount == 0)
				return false;
			if (_replayCount == _maxReplayCount)
				return true;
			return false;
		}

		//애니메이션이 무한 반복으로 재생된다면 true를 반환한다.
		inline bool isInfinite() const {
			return _maxReplayCount == 0;
		}

		//애니메이션에 사용되는 총 스프라이트 개수를 반환한다.
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
		AnimationCallback _cbPlayDone;//maxPlayCount횟수만큼  애니메이션이 플레이된 후에 호출
		int _customCallbackFrame;
		AnimationCallback _cbCustomFrameBegin;
		AnimationCallback _cbCustomFrameEnd;
		AnimationCallback _cbFirstFrameBegin;
		AnimationCallback _cbFirstFrameEnd;
		AnimationCallback _cbLastFrameBegin;
		AnimationCallback _cbLastFrameEnd;

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
	};

#define GET_ANIMATION_MANAGER() SGA::AnimationManager::getSingleton()
}