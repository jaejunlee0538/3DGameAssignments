#pragma once

#include <Windows.h>
namespace SGA {
	class ClockIface {
	public:
		virtual ~ClockIface(){}
		virtual float getCurrentTime() const = 0;
		virtual float getDeltaTime() const = 0;
		virtual void reset() = 0;
	};

	class GameWorldClock : public ClockIface
	{
	public:
		GameWorldClock();

		inline float getCurrentTime() const {
			return _currentTime;
		}

		inline float getDeltaTime() const {
			return _deltaTime;
		}

		void updateClock(float dt);

		void reset();
	private:
		float _deltaTime;
		float _currentTime;
	};

	class RealTimeClock : public ClockIface
	{
	public:
		RealTimeClock();

		inline float getCurrentTime() const {
			return _currentTime;
		}

		inline float getDeltaTime() const {
			return _deltaTime;
		}

		void updateClock();

		void reset();
	private:
		__int64 _periodFreq;
		float _timeScale;

		__int64 _t0;
		float _currentTime;
		float _deltaTime;

		bool _highResolution;
	};

	class TimeoutChecker {
	public:
		TimeoutChecker(float timeout)
		:_timeout(timeout){
			reset();
		}

		void reset() {
			_lastSampleTime = -1;
		}

		void setDuration(float duration) {
			_timeout = duration;
		}

		bool isTimeout(float currentTime) {
			if (_lastSampleTime < 0) {
				_lastSampleTime = currentTime;
				return false;
			}
			if ((currentTime - _lastSampleTime) >= _timeout) {
				_lastSampleTime = currentTime;
				return true;
			}
			return false;
		}
	private:
		float _lastSampleTime;
		float _timeout;
		bool _isTimeout;
	};

	class FPSCalculator {
	public:
		FPSCalculator(float samplingTime)
			:_samplePeriod(samplingTime) {
			reset();
		}

		void reset() {
			_lastSampleTime = -1;
			_sampleCount = 0;
			_fps = 0.0f;
			_frameTime = 999999;
		}

		/*
		프레임률을 반환한다.
		*/
		inline float getFPS() const {
			return _fps;
		}

		/*
		평균 프레임 시간을 반환한다.
		*/
		inline int getFramTime() const {
			return _frameTime;
		}

		void update(float currentTime) {
			_sampleCount++;
			if (_lastSampleTime < 0) {
				_lastSampleTime = currentTime;
				return;
			}
			float dt = currentTime - _lastSampleTime;
			if (dt >= _samplePeriod) {
				_fps = _sampleCount / dt;
				_frameTime = dt / _sampleCount;
				_lastSampleTime = currentTime;
				_sampleCount = 0;
			}
		}
	private:
		float _lastSampleTime;
		float _samplePeriod;
		float _fps;
		int _frameTime;
		int _sampleCount;
	};

}