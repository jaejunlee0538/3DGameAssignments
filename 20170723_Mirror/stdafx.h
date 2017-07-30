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
#include <assert.h>

#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

extern LPDIRECT3DDEVICE9 g_pD3DDevice;
extern HWND g_hWnd;

void LogDebugMessage(const TCHAR* fmt, ...);

#define SAFE_RELEASE(p) do{if(p){p->Release(); p = NULL;}} while(0)
#define SAFE_DELETE(p) do{if(p){delete p; p = NULL;}} while(0)
#define RETURN_IF_NULL(p) if(p==NULL) return
#define RETURN_IF_NULL(p, ret) if(p==NULL) return ret


struct tagVertexXYZ {
	D3DXVECTOR3 xyz;
	enum {
		FVF = D3DFVF_XYZ
	};

	tagVertexXYZ(float x, float y, float z)
	:xyz(x,y,z){
		
	}
};