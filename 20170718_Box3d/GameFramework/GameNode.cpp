#include "Stdafx.h"
#include "GameNode.h"
#include "RandomFunction.h"
#include "ClockManager.h"
#include "BitmapImage.h"
#include <KeyManager.h>
namespace SGA {
	DoubleBuffering GameNode::_backBuffer;
	POINT GameNode::_ptMouse;
	RECT GameNode::_screenRect;
	short GameNode::_mouseWheel = 0;
	POINT GameNode::_screenCenter;
	HDC GameNode::_hdc = NULL;
	HWND GameNode::_hWnd = NULL;
	HINSTANCE GameNode::_hInstance = NULL;
	GameNode::GameNode()
	{
	}


	GameNode::~GameNode()
	{
		if (_hdc) {
			ReleaseDC(GameNode::_hWnd, GameNode::_hdc);
			_hdc = NULL;
		}
	}

	HRESULT GameNode::init()
	{
		//SetTimer(_hWnd, 1, UPDATE_DELTA_TIME, 0);
		return S_OK;
	}

	void GameNode::release(void)
	{
		//KillTimer(_hWnd, 1);
	}

	void GameNode::update(void)
	{
		InvalidateRect(_hWnd, NULL, false);
	}

	void GameNode::render(void)
	{
	}

	void GameNode::renderBackbuffer()
	{
		render();
		GET_CLOCK_MANAGER()->renderClockInfo(getMemDC(), 10, 10);
		_backBuffer.drawBuffer();
	}

	HINSTANCE GameNode::getHINSTANCE()
	{
		return GameNode::_hInstance;
	}

	HWND GameNode::getHWND()
	{
		return GameNode::_hWnd;
	}

	POINT GameNode::getMousePosition() {
		return _ptMouse;
	}

	void GameNode::clearScreen(COLORREF color) {
		_backBuffer.clearBuffer(color);
	}

	HDC GameNode::getHDC() {
		return _hdc;
	}

	const RECT & GameNode::getScreenSize() {
		return _screenRect;
	}

	const int GameNode::getScreenWidth()
	{
		return _screenRect.right;
	}

	const int GameNode::getScreenHeight()
	{
		return _screenRect.bottom;
	}

	const POINT & GameNode::getScreenCenter() {
		return _screenCenter;
	}

	int GameNode::getMouseWheel(bool resetAfterRead)
	{
		int wheel = GameNode::_mouseWheel;
		if (resetAfterRead) {
			GameNode::_mouseWheel = 0;
		}
		return wheel;
	}

	HDC GameNode::getMemDC() {
		return _backBuffer.getBufferDC();
	}

	LRESULT GameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		switch (iMessage)
		{
		case WM_MOUSEMOVE:
		{
			GameNode::_ptMouse.x = static_cast<float>(LOWORD(lParam));
			GameNode::_ptMouse.y = static_cast<float>(HIWORD(lParam));
			GET_KEY_MANAGER()->setCurrentMousePos(GameNode::_ptMouse);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			_mouseWheel = HIWORD(wParam);
		}
		break;
		case WM_TIMER:
		{
			//GET_CLOCK_MANAGER()->updateRealTimeClock();
			this->update();
		}
		break;
		case WM_CREATE:
		{
			GET_RANDOM_FUNCTION();//seed value
			GET_KEY_MANAGER()->init();
			GetClientRect(hWnd, &_screenRect);
			_screenCenter.x = (_screenRect.right + _screenRect.left) / 2;
			_screenCenter.y = (_screenRect.top + _screenRect.bottom) / 2;
			GameNode::_hWnd = hWnd;
			SGA::BitmapImage::setHWND(hWnd);
			GameNode::_hdc = GetDC(hWnd);
			_backBuffer.init(GameNode::_hdc, _screenRect.right, _screenRect.bottom);
			GameNode::_hInstance = (HINSTANCE)GetModuleHandle(NULL);
			init();
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			//this->render();
			renderBackbuffer();
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_KEYDOWN:
		{
			switch (wParam) {
			case VK_ESCAPE:
				PostMessage(hWnd, WM_DESTROY, wParam, lParam);
			}
		}
		break;
		case WM_COMMAND:
		{
			if (HIWORD(wParam) == 0)
				controlMessage(LOWORD(wParam));
		}
		break;
		case WM_SIZE:
		{
			int w, h;
			w = LOWORD(lParam);
			h = HIWORD(lParam);
			_backBuffer.resize(w, h);
			const int offset = 0;
			_screenRect.left = 0 + offset;
			_screenRect.right = w - offset;
			_screenRect.top = 0 + offset;
			_screenRect.bottom = h - offset;
			_screenCenter.x = w / 2;
			_screenCenter.y = h / 2;

		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			GET_RANDOM_FUNCTION()->releaseSingleton();
			break;
		default:
			break;
		}
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
}