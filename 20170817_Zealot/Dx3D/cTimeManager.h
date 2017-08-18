#pragma once

#define g_pTimeManager cTimeManager::GetInstance()

class cTimeManager
{
private:
	SINGLETON(cTimeManager)

private:
	DWORD m_dwPrevTimeStamp;
	DWORD m_dwDeltaTime;
	DWORD m_dwElapsedTime;
public:
	void Update();
	float GetElapsedTime();
	float GetDeltaTime();
};

