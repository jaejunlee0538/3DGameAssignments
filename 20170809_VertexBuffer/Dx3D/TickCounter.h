#pragma once
class TickCounter
{
public:
	TickCounter();
	~TickCounter();

	void Start();
	DWORD GetElapsedTime();

private:
	DWORD m_t0;
};

