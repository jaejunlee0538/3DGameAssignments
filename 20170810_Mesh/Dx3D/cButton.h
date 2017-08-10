#pragma once
#include <functional>
class cButton
{
public:
	cButton();
	~cButton();
	
	//버튼을 초기화한다.
	//cbclicked에 NULL을 넣으면 클릭하여도 아무런 반응을 하지 않는 단순 Text가 된다.
	void Init(RECT rect, const char* buttonText, std::function<void(void)> cbClicked);

	//버튼의 텍스트를 교체한다.
	void SetText(std::string str);
	
	//버튼을 그린다
	void Render();
	
	//메시지를 받아서 처리했으면 true를 반환하고 그렇지 않으면 false를 반환한다.
	bool MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	bool m_bWatingButtonUp;
	RECT m_rectBtn;
	std::string m_buttonStr;
	std::function<void(void)> m_cbClicked;

	LPD3DXFONT m_pFont;
};

