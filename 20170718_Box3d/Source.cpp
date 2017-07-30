#include "stdafx.h"
#include "GameLoop.h"
#include "ClockManager.h"
#define MAX_LOADSTRING 100

GameLoop game;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void setWindowSize(HWND hWnd, int x, int y, int width, int height);
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPSTR lpszCmdParam,
	int cmdShow) {
	setlocale(LC_ALL, "");
	MSG message;
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = WIN_NAME;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wndClass);

	_hInstance = hInstance;
	//ChildWindow깜빡임 막기 위해서 WS_CLIPCHILDREN 비트 추가
	//https://www.gamedev.net/topic/595488-how-do-i-show-a-child-window-on-top-of-a-backbuffer/?setlanguage=1&langurlbits=topic/595488-how-do-i-show-a-child-window-on-top-of-a-backbuffer/&langid=2
	_hWnd = CreateWindow(
		WIN_NAME,
		WIN_NAME,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		WIN_START_X,
		WIN_START_Y,
		WIN_SIZE_X,
		WIN_SIZE_Y,
		NULL,
		NULL,
		hInstance,
		NULL);

	setWindowSize(_hWnd, WIN_START_X, WIN_START_Y, WIN_SIZE_X, WIN_SIZE_Y);
	ShowWindow(_hWnd, cmdShow);

	//게임 스터디 초기화
	//if (FAILED(game.init())) {
	//	return 0;
	//}
	while (true) {
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			//초당 60으로 고정한다
			GET_CLOCK_MANAGER()->updateRealTimeClock();
			game.update();
			game.renderBackbuffer();
			//InvalidateRect(_hWnd, NULL, FALSE);
			//SendMessage(_hWnd, WM_PAINT, 0, 0);
		}
	}
	
	//while (GetMessage(&message, 0, 0, 0)) {
	//	TranslateMessage(&message);
	//	DispatchMessage(&message);
	//}

	game.release();
	UnregisterClass(WIN_NAME, hInstance);
	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return game.MainProc(hWnd, message, wParam, lParam);
}

void setWindowSize(HWND hWnd, int x, int y, int width, int height) {
	RECT rect = { 0, 0, width, height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(hWnd, NULL, x, y, rect.right - rect.left,
		rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE);
}

