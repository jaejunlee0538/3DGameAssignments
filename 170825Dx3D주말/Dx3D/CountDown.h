#pragma once
#include <functional>

class CountDown
{
public:
	CountDown();
	~CountDown();

	void Init(int seconds, int drawX, int drawY, std::function<void(void)> cbOnZeroSec);
	void Restart();
	void Update();
	void Render();
private:
	int m_drawX, m_drawY;
	LPD3DXFONT m_pFont;
	int m_initSeconds;
	float m_currentCount;
	std::function<void(void)> m_cbOnZeroSec;
	bool m_counting;
};

