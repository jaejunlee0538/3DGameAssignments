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
#include <list>
#include <memory>
#include <Windows.h>
#include <d3dx9.h>
#include <assert.h>
#include "Exceptions.h"
#include "tstring.h"
#include "UserInputs.h"
#include "FrameAnimation.h"
#include "VertexTypes.h"
#include "LightHelper.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

typedef std::shared_ptr<IDirect3DTexture9> D3DTexturePtr;

extern HWND g_hWnd;

void LogDebugMessage(const TCHAR* fmt, ...);

#undef min(a,b)
#undef max(a,b)

#define SAFE_RELEASE(p) do{if(p){p->Release(); p = NULL;}} while(0)
#define SAFE_DELETE(p) do{if(p){delete p; p = NULL;}} while(0)
#define RETURN_IF_NULL(p) if(p==NULL) return
#define RETURN_IF_NULL(p, ret) if(p==NULL) return ret
#define ASSERT_IF_NULL(p) assert(p!=NULL && ##p"가 NULL입니다")


extern LPDIRECT3DDEVICE9 g_pD3DDevice;
extern D3DXVECTOR3 g_upVector;
extern const float MAX_PITCH_ANGLE;
extern D3DXMATRIX Identity4X4;