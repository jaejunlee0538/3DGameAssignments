// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <map>
#include <Windows.h>
#include <d3dx9.h>
#include <assert.h>
#include "UserInputs.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

extern HWND g_hWnd;


#define  SAFE_RELEASE(p) if(p){(p)->Release(); (p) = NULL;}


struct tagVertex {
	D3DXVECTOR3 p;
	D3DCOLOR color;
	tagVertex() {

	}

	//tagVertex(float x, float y, float z, BYTE r, BYTE g, BYTE b) {
	//	p.x = x;
	//	p.y = y;
	//	p.z = z;
	//	color.r = r;
	//	color.g = g;
	//	color.b = b;
	//	color.a = 0xff;
	//}

	tagVertex(float x, float y, float z, D3DCOLOR rgb) {
		p.x = x;
		p.y = y;
		p.z = z;
		color = rgb;
	}
};

extern LPDIRECT3DDEVICE9 g_pD3DDevice;
extern D3DXVECTOR3 g_upVector;
extern const float MAX_PITCH_ANGLE;