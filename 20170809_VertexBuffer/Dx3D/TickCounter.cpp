#include "stdafx.h"
#include "TickCounter.h"


TickCounter::TickCounter()
{
	Start();
}

TickCounter::~TickCounter()
{
}

void TickCounter::Start()
{
	m_t0 = GetTickCount();
}

DWORD TickCounter::GetElapsedTime()
{
	return GetTickCount() - m_t0;
}
