#pragma once

class cUIButton;

class iButtonDelegate
{
public:
	virtual void OnClick(cUIButton* pSender) PURE;
};

class cUIButton : public cUIObject
{
protected:
	enum eButtonStatus
	{
		E_NORMAL,
		E_MOUSEOVER,
		E_SELECTED,
		E_STATUS_COUNT,
	};

	eButtonStatus	m_eButtonStatus;
	string			m_aTexture[E_STATUS_COUNT];
	SYNTHESIZE(iButtonDelegate*, m_pDelegate, Delegate);

public:
	cUIButton(void);
	virtual ~cUIButton(void);

	virtual void SetTexture(string sNor, string sOvr, string sSel);

	// cUIObject override
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE pSprite) override;
};


