#pragma once

#define g_pTimeManager cTimeManager::GetInstance()

class cTimeManager
{
private:
	SINGLETON(cTimeManager)

private:

	float m_deltaTimeFixed;
	float m_currentTime;
public:
	void Update();
	float GetDeltaTime();
	float GetTime();
};

