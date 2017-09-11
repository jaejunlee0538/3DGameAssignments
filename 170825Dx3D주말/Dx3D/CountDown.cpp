#include "stdafx.h"
#include "CountDown.h"

CountDown::CountDown()
	:m_counting(false), m_initSeconds(10), m_pFont(NULL)
{
}

CountDown::~CountDown()
{
	SAFE_RELEASE(m_pFont);
}

void CountDown::Init(int seconds, int drawX, int drawY, std::function<void(void)> cbOnZeroSec)
{
	m_drawX = drawX;
	m_drawY = drawY;
	m_initSeconds = seconds;
	m_cbOnZeroSec = cbOnZeroSec;

	m_counting = false;
	D3DXFONT_DESC fd;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
	fd.Height = 300;
	fd.Weight = 150;
	fd.Weight = FW_NORMAL | FW_BOLD;
	fd.Italic = false;
	fd.CharSet = DEFAULT_CHARSET;
	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily = FF_DONTCARE;
	
	D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);
}

void CountDown::Restart()
{
	m_currentCount = m_initSeconds;
	m_counting = true;
}

void CountDown::Update()
{
	if (m_counting == false)
		return;
	m_currentCount -= g_pTimeManager->GetDeltaTime();
	if (m_currentCount <= 0.0f) {
		if (m_cbOnZeroSec) {
			m_cbOnZeroSec();
		}
		m_counting = false;
	}
}

void CountDown::Render()
{
	if (m_counting) {
		int count = ceil(m_currentCount);
		char buffer[20];
		RECT rc;
		rc.left = m_drawX;
		rc.right = m_drawX +1;
		rc.top = m_drawY;
		rc.bottom = m_drawY + 1;
		sprintf_s(buffer, 20, "%d", count);
		m_pFont->DrawTextA(
			NULL, buffer, strlen(buffer), &rc, DT_CENTER | DT_NOCLIP, D3DXCOLOR(1.0f,0.2f, 0.2f, 1.0f));
	}
}
