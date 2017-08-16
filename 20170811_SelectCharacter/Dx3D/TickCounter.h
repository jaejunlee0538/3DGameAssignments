#pragma once
class TickCounter
{
public:
	TickCounter();
	~TickCounter();

	void Start();
	DWORD GetElapsedTime();

private:
	double	m_toMillis;
	int64_t m_t0;
};

