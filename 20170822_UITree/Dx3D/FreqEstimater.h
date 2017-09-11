#pragma once
class FreqEstimater {
public:
	FreqEstimater(float samplingTime)
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
	float getFPS() const {
		return _fps;
	}

	/*
	평균 프레임 시간을 반환한다.
	*/
	int getFramTime() const {
		return _frameTime;
	}

	bool update(float currentTime) {
		_sampleCount++;
		if (_lastSampleTime < 0) {
			_lastSampleTime = currentTime;
			return false;
		}
		float dt = currentTime - _lastSampleTime;
		if (dt >= _samplePeriod) {
			_fps = _sampleCount / dt;
			_frameTime = dt / _sampleCount;
			_lastSampleTime = currentTime;
			_sampleCount = 0;
			return true;
		}
		return false;
	}
private:
	float _lastSampleTime;
	float _samplePeriod;
	float _fps;
	int _frameTime;
	int _sampleCount;
};