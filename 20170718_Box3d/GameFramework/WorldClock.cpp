#include "WorldClock.h"

namespace SGA {
	GameWorldClock::GameWorldClock() {
		reset();
	}

	void GameWorldClock::updateClock(float dt)
	{
		_deltaTime = dt;
		_currentTime += dt;
	}

	void GameWorldClock::reset() {
		_deltaTime = 0;
		_currentTime = 0;
	}

	RealTimeClock::RealTimeClock() {
		if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFreq))
		{
			_highResolution = true;
			_timeScale = 1.0 / _periodFreq;
		}
		else
		{
			_highResolution = false;
			_timeScale = 0.001;
		}
		reset();
	}

	void RealTimeClock::updateClock() {
		__int64 currentTick;
		if (_highResolution) {
			QueryPerformanceCounter((LARGE_INTEGER*)&currentTick);
		}
		else {
			currentTick = GetTickCount64();
		}
		float currentTime = (currentTick - _t0)*_timeScale;
		_deltaTime = currentTime - _currentTime;
		_currentTime = currentTime;
	}

	void RealTimeClock::reset() {
		if (_highResolution) {
			QueryPerformanceCounter((LARGE_INTEGER*)&_t0);
		}
		else {
			_t0 = GetTickCount64();
		}
		
		_currentTime = 0;
		_deltaTime = 0;
	}
}