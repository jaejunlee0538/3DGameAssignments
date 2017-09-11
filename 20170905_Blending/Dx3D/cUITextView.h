#pragma once

class cUITextView : public cUIObject
{
protected:
	SYNTHESIZE(string, m_sText, Text);
	SYNTHESIZE(cFontManager::eFontType, m_eFontType, FontType);
	SYNTHESIZE(DWORD, m_dwDT, DrawTextFormat);
	SYNTHESIZE(D3DCOLOR, m_dwColor, Color);
public:
	cUITextView(void);
	virtual ~cUITextView(void);

	// cUIObject override
	virtual void Render(LPD3DXSPRITE pSprite) override;
};

