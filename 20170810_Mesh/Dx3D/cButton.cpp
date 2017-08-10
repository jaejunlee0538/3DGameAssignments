#include "stdafx.h"
#include "cButton.h"

cButton::cButton()
{
	m_bWatingButtonUp = false;
	D3DXFONT_DESC fd;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
	fd.Height = 50;
	fd.Width = 25;
	fd.Weight = FW_NORMAL;
	fd.Italic = false;
	fd.CharSet = DEFAULT_CHARSET;
	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily = FF_DONTCARE;
	//AddFontResource("umberto.ttf");
	strcpy_s(fd.FaceName, "고딕");	//글꼴 스타일
	D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);
}


cButton::~cButton()
{
	SAFE_RELEASE(m_pFont);
}

void cButton::Init(RECT rect, const char * buttonText, std::function<void(void)> cbClicked)
{
	m_rectBtn = rect;
	m_buttonStr = buttonText;
	m_cbClicked = cbClicked;
}

void cButton::SetText(std::string str)
{
	m_buttonStr = str;
}

void cButton::Render()
{
	assert(m_pFont);
	m_pFont->DrawText(NULL,
		m_buttonStr.c_str(),
		m_buttonStr.length(),
		&m_rectBtn,
		DT_LEFT | DT_TOP,
		D3DCOLOR_XRGB(255, 255, 255));
}

bool cButton::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!m_cbClicked) {
		return false;
	}
	switch (message) {
	case WM_LBUTTONDOWN:
	{
		POINT ptMouse;
		ptMouse.x = LOWORD(lParam);
		ptMouse.y = HIWORD(lParam);
		if (PtInRect(&m_rectBtn, ptMouse)) {
			m_bWatingButtonUp = true;
			return true;
		}
	}
		break;
	case WM_LBUTTONUP:
	{
		if (m_bWatingButtonUp) {
			POINT ptMouse;
			ptMouse.x = LOWORD(lParam);
			ptMouse.y = HIWORD(lParam);
			m_bWatingButtonUp = false;
			if (PtInRect(&m_rectBtn, ptMouse)) {
				m_cbClicked();
			}
			return true;
		}
	}
		break;
	}
	return false;
}
