#pragma once

#define WIN32_LEAN_AND_MEAN             
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string>
#include <tchar.h>

#define WIN_NAME	(TEXT("APIWindow"))
#define WIN_START_X	0
#define WIN_START_Y	0
#define WIN_SIZE_X	1280
#define WIN_SIZE_Y	720
#define WIN_STYLE	CS_HREDRAW | CS_VREDRAW | WS_SYSMENU | WS_CAPTION | WS_THICKFRAME

#define TILE_SIZE	114
#define TILE_GAP	20
#define BOARD_TOP	310
#define BOARD_LEFT	40

POINT getTopLeftOfGrid(int ix, int iy);

void LogDebugMessageA(const char* fmt, ...);
void LogDebugMessage(const TCHAR* fmt, ...);

extern HWND _hWnd;
extern HINSTANCE _hInstance;