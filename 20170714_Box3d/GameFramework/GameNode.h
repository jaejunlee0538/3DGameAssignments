#pragma once
#pragma once
#include <Windows.h>
#include "DoubleBuffering.h"
#include <GameObject.h>
#include <vector>
namespace SGA {
	class GameNode
	{
	public:
		GameNode();
		virtual ~GameNode();

		/*
		WM_CREATE
		*/
		virtual HRESULT init();

		/*
		WM_DESTROY
		*/
		virtual void release(void);

		/*
		WM_TIMER
		*/
		virtual void update(void);

		/*
		WM_PAINT
		*/
		virtual void render(void);

		void renderBackbuffer();

		static HINSTANCE getHINSTANCE();
		static HWND getHWND();
		static POINT getMousePosition();
		static void clearScreen(COLORREF color);
		static HDC getHDC();
		static HDC getMemDC();
		static const RECT& getScreenSize();
		static const int getScreenWidth();
		static const int getScreenHeight();
		static const POINT& getScreenCenter();

		virtual void controlMessage(int message) {}

		virtual std::vector<GameObject*> getPlayers() { 
			return std::vector<SGA::GameObject*>(); 
		}
		/*
		*/
		LRESULT MainProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	private:
		static SGA::DoubleBuffering _backBuffer;
		static POINT _ptMouse;
		static RECT _screenRect;
		static POINT _screenCenter;
		static HDC _hdc;
		static HWND _hWnd;
		static HINSTANCE _hInstance;
	};

}