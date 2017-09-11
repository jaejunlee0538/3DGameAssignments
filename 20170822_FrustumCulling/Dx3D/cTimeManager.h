#pragma once

#define g_pTimeManager cTimeManager::GetInstance()

class cTimeManager
{
private:
	SINGLETON(cTimeManager)

private:
	DWORD m_dwPrevTimeStamp;
	DWORD m_dwDeltaTime;

public:
	void Update();
	float GetDeltaTime();
};

