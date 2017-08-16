#include "StdAfx.h"
#include "cTimeManager.h"


cTimeManager::cTimeManager(void)
	: m_dwDeltaTime(13)
	, m_dwElapsedTime(0)
{
}

cTimeManager::~cTimeManager(void)
{
}

void cTimeManager::Update()
{
	m_dwElapsedTime += m_dwDeltaTime;
}

float cTimeManager::GetElapsedTime()
{
	return m_dwElapsedTime * 0.001f;
}

float cTimeManager::GetDeltaTime()
{
	return m_dwDeltaTime*0.001f;
}
