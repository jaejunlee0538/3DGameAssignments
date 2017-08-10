#include "StdAfx.h"
#include "cTimeManager.h"


cTimeManager::cTimeManager(void)
	: m_dwPrevTimeStamp(GetTickCount())
	, m_dwDeltaTime(0)
{
}


cTimeManager::~cTimeManager(void)
{
}

void cTimeManager::Update()
{
	DWORD dwCurrTimeStamp = GetTickCount();
	m_dwDeltaTime = dwCurrTimeStamp - m_dwPrevTimeStamp;
	m_dwPrevTimeStamp = dwCurrTimeStamp;
}

float cTimeManager::GetDeltaTime()
{
	return (float)m_dwDeltaTime / 1000.f;
}
