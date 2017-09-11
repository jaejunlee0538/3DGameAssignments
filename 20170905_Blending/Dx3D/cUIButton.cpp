#include "StdAfx.h"
#include "cUIButton.h"


cUIButton::cUIButton(void)
	: m_eButtonStatus(E_NORMAL)
	, m_pDelegate(NULL)
{
}

cUIButton::~cUIButton(void)
{
}

void cUIButton::SetTexture( string sNor, string sOvr, string sSel )
{
	m_aTexture[E_NORMAL]	= sNor;
	m_aTexture[E_MOUSEOVER] = sOvr;
	m_aTexture[E_SELECTED]	= sSel;

	D3DXIMAGE_INFO stImageInfo;
	g_pTextureManager->GetTextureEx(sNor, &stImageInfo);
	m_stSize.fWidth = stImageInfo.Width;
	m_stSize.fHeight = stImageInfo.Height;
}

void cUIButton::Update()
{
	RECT rc;
	GetRect(&rc);

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&rc, ptMouse))
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_eButtonStatus == E_MOUSEOVER)
			{
				m_eButtonStatus = E_SELECTED;
			}
		}
		else
		{
			if (m_eButtonStatus == E_SELECTED)
			{
				if(m_pDelegate)
					m_pDelegate->OnClick(this);
			}
			m_eButtonStatus = E_MOUSEOVER;
		}
	}
	else
	{
		m_eButtonStatus = E_NORMAL;
	}

	cUIObject::Update();
}

void cUIButton::Render( LPD3DXSPRITE pSprite )
{
	D3DXIMAGE_INFO stImageInfo;
	LPDIRECT3DTEXTURE9 pTexture = g_pTextureManager->GetTextureEx(m_aTexture[m_eButtonStatus], &stImageInfo);
	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	pSprite->SetTransform(&m_matWorld);
	RECT rc;
	SetRect(&rc, 0, 0, stImageInfo.Width, stImageInfo.Height);
	pSprite->Draw(pTexture,
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(255, 255, 255, 255));

	pSprite->End();

	cUIObject::Render(pSprite);
}

