// stdafx.cpp : source file that includes just the standard includes
// 20170723_Mirror.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

LPDIRECT3DDEVICE9 g_pD3DDevice;
HWND g_hWnd;



void LogDebugMessage(const TCHAR* fmt, ...) {

	static TCHAR szBuffer[2048]; // this here is a caveat!

	va_list args;

	va_start(args, fmt);
	_vstprintf_s(szBuffer, 2048, fmt, args);
	OutputDebugString(szBuffer);

	va_end(args);
}