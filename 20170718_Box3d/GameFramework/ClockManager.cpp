#include "ClockManager.h"
#include "WorldClock.h"
#include <tchar.h>
#include <stdio.h>

namespace SGA {
	ClockManager::ClockManager()
		:_desiredFPS(60), _limitFPS(true), _gameWorldDeltaTime(1.0 / _desiredFPS)
	{
		_textColor = RGB(255, 255, 255);
		_clockGameWorld = new GameWorldClock;
		_clockRealTime = new RealTimeClock;
		_fpsCalculator = new FPSCalculator(1.5);
		reset();
	}

	ClockManager::~ClockManager()
	{
		delete _clockGameWorld;
		delete _clockRealTime;
		delete _fpsCalculator;
	}

	void ClockManager::reset()
	{
		_clockGameWorld->reset();
		_clockRealTime->reset();
		_fpsCalculator->reset();
		_lastUpdateTime = _clockRealTime->getCurrentTime();
	}

	void ClockManager::setGameWorldDeltaTime(float dtSec)
	{
		_gameWorldDeltaTime = dtSec;
	}

	void ClockManager::updateGameWorldClock()
	{
		_clockGameWorld->updateClock(_gameWorldDeltaTime);
	}

	float ClockManager::getElapsedGameTime() const
	{
		return _clockGameWorld->getCurrentTime();
	}

	float ClockManager::getDeltaGameTime() const
	{
		return _clockGameWorld->getDeltaTime();
	}

	void ClockManager::updateRealTimeClock()
	{
		 _clockRealTime->updateClock();
		 float currentUpdate = _clockRealTime->getCurrentTime();
		 float deltaTime = currentUpdate - _lastUpdateTime;
		 
		 if (_limitFPS) {
			 float desDeltaTime = 1.0 / _desiredFPS;
			 while (desDeltaTime >= deltaTime) {
				 if (desDeltaTime - deltaTime > 0.002) {
					 //::Sleep(1);
				 }
				_clockRealTime->updateClock();
				currentUpdate = _clockRealTime->getCurrentTime();
				deltaTime = currentUpdate - _lastUpdateTime;
			 }
			 //LogDebugMessage(TEXT("dt : %.3lf %.3lf\n"), desDeltaTime, deltaTime);
		 }
		 _fpsCalculator->update(currentUpdate);
		 _lastUpdateTime = currentUpdate;
		 _deltaTime = deltaTime;
	}

	float ClockManager::getElapsedRealTime() const
	{
		return _lastUpdateTime;
	}

	float ClockManager::getDeltaRealTime() const
	{
		return _deltaTime;
	}

	float ClockManager::getFPS() const
	{
		return _fpsCalculator->getFPS();
	}

	void ClockManager::renderClockInfo(HDC hdc, int topPixel, int leftPixel)
	{
		if (_showClockInfo) {
			TCHAR infoStr[256];
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, _textColor);
			_stprintf(infoStr, TEXT("FrameRate : %.0f fps"), getFPS());
			TextOut(hdc, leftPixel, topPixel, infoStr, _tcslen(infoStr));
			topPixel += 20;
			_stprintf(infoStr, TEXT("FrameTime : %.4f"), getDeltaRealTime());
			TextOut(hdc, leftPixel, topPixel, infoStr, _tcslen(infoStr));
			topPixel += 20;
			_stprintf(infoStr, TEXT("ElapsedTime(RealTime) : %.3f"), getElapsedRealTime());
			TextOut(hdc, leftPixel, topPixel, infoStr, _tcslen(infoStr));
			topPixel += 20;
			_stprintf(infoStr, TEXT("ElapsedTime(GameWorld) : %.3f"), getElapsedGameTime());
			TextOut(hdc, leftPixel, topPixel, infoStr, _tcslen(infoStr));
		}
	}
}