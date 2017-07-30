#include "Stdafx.h"
#include <stdio.h>

HWND _hWnd;
HINSTANCE _hInstance;



POINT getTopLeftOfGrid(int ix, int iy)
{
	return{ BOARD_LEFT + ix*TILE_SIZE + (ix + 1)*TILE_GAP, BOARD_TOP + iy*TILE_SIZE + (iy + 1)*TILE_GAP };
}

void LogDebugMessage(const TCHAR* fmt, ...) {

	static TCHAR szBuffer[2048]; // this here is a caveat!

	va_list args;

	va_start(args, fmt);
	_vstprintf_s(szBuffer, 2048, fmt, args);
	OutputDebugString(szBuffer);

	va_end(args);
}
