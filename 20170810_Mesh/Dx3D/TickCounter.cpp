#include "stdafx.h"
#include "TickCounter.h"


TickCounter::TickCounter()
{
	int64_t freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	m_toMillis = 1000.0 / freq;
	Start();
}

TickCounter::~TickCounter()
{
}

void TickCounter::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_t0);
}

DWORD TickCounter::GetElapsedTime()
{
	int64_t t1;
	QueryPerformanceCounter((LARGE_INTEGER*)&t1);
	return m_toMillis * (t1 - m_t0);
}
