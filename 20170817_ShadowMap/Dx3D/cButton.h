#pragma once
#include <functional>
class cButton
{
public:
	cButton();
	~cButton();
	
	//��ư�� �ʱ�ȭ�Ѵ�.
	//cbclicked�� NULL�� ������ Ŭ���Ͽ��� �ƹ��� ������ ���� �ʴ� �ܼ� Text�� �ȴ�.
	void Init(RECT rect, const char* buttonText, std::function<void(void)> cbClicked);

	//��ư�� �ؽ�Ʈ�� ��ü�Ѵ�.
	void SetText(std::string str);
	
	//��ư�� �׸���
	void Render();
	
	//�޽����� �޾Ƽ� ó�������� true�� ��ȯ�ϰ� �׷��� ������ false�� ��ȯ�Ѵ�.
	bool MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	bool m_bWatingButtonUp;
	RECT m_rectBtn;
	std::string m_buttonStr;
	std::function<void(void)> m_cbClicked;

	LPD3DXFONT m_pFont;
};

