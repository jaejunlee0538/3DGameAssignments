#pragma once
#include "WorldClock.h"
#include "SingletonBase.h"
#include <Windows.h>
namespace SGA {
	class ClockManager : public SingletonBase<ClockManager>
	{
	public:
		ClockManager();
		~ClockManager();
		
		void reset();


		inline const GameWorldClock* getGameWorldClock() const {
			return _clockGameWorld;
		}

		inline const RealTimeClock* getRealTimeClock() const {
			return _clockRealTime;
		}

		//게임 월드 시계
		void setGameWorldDeltaTime(float dtSec);
		void updateGameWorldClock();
		float getElapsedGameTime() const;
		float getDeltaGameTime() const;

		//실제 시계
		void updateRealTimeClock();
		float getElapsedRealTime() const;
		float getDeltaRealTime() const;

		void showClockInfo(bool show) {
			_showClockInfo = show;
		}

		inline void enableFPSLimit() {
			_limitFPS = true;
		}
		inline void disableFPSLimit() {
			_limitFPS = false;
		}
		inline void setFPSLimit(int fps) {
			_desiredFPS = fps;
		}

		float getFPS() const;
		void renderClockInfo(HDC hdc, int topPixel = 5, int leftPixel = 5);

		void setTextColor(COLORREF color) {
			_textColor = color;
		}
	protected:
		RealTimeClock* _clockRealTime;
		GameWorldClock* _clockGameWorld;
		FPSCalculator * _fpsCalculator;

		bool _limitFPS;
		int _desiredFPS;

		float _gameWorldDeltaTime;//기본값 : 16
		float _lastUpdateTime;
		float _deltaTime;

		bool _showClockInfo;

		COLORREF _textColor;
	};
#define GET_CLOCK_MANAGER()	SGA::ClockManager::getSingleton()
}