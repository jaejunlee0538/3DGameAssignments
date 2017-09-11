#include "StdAfx.h"
#include "cTimeManager.h"


cTimeManager::cTimeManager(void)
	:m_deltaTimeFixed(0.013f)
	, m_currentTime(0.0f)
{
}


cTimeManager::~cTimeManager(void)
{
}

void cTimeManager::Update()
{
	m_currentTime += m_deltaTimeFixed;
}

float cTimeManager::GetDeltaTime()
{
	return m_deltaTimeFixed;
}

float cTimeManager::GetTime()
{
	return m_currentTime;
}
