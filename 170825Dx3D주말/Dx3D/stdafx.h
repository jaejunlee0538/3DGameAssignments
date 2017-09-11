// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

#include <d3dx9.h>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <assert.h>
#include <list>
#include <iostream>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using namespace std;

extern HWND g_hWnd;
extern POINT _ptMouse;

#define SAFE_ADD_REF(p) if(p) { (p)->AddRef(); }
#define SAFE_RELEASE(p) if(p) { (p)->Release(); p = NULL; }
#define SAFE_DELETE(p) if(p) { delete (p); (p) = NULL; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete [] (p); (p) = NULL; }

#define SINGLETON(class_name)	private:\
	class_name(void);\
	~class_name(void);\
public:\
	static class_name* GetInstance() { static class_name instance; return &instance; }


#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline varType Get##funName(void) const { return varName; }\
public: inline void Set##funName(varType var){ varName = var; }

#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline varType& Get##funName(void) { return varName; }\
public: inline void Set##funName(const varType& var){ varName = var; }

#define SYNTHESIZE_ADD_REF(varType, varName, funName)    \
protected: varType varName; \
public: virtual varType Get##funName(void) const { return varName; } \
public: virtual void Set##funName(varType var){\
	if (varName != var) {\
	SAFE_RELEASE(varName);\
	SAFE_ADD_REF(var);\
	varName = var;\
	}\
}

#define NODE_ROW		16
#define NODE_COL		16

#define NODE_SIZE		1	//정사각형이라는 가정하에 쓸 SIZE
#define NODE_DIAGONAL	sqrt(2.f)


// 1. Position 정점의 좌표 x,y,z(float)			: D3DFVF_XYZ
// 2. RHW (float)                               : D3DFVF_XYZRHW (D3DFVF_XYZ 또는 D3DFVF_NORMAL과 같이 사용불가)
// 3. Blending Weight Data 결합 가중치 (float)	: D3DFVF_XYZB1 ~ D3DFVF_XYZB5
// 4. Vertex Normal 정점의 법선 벡터 x,y,z(float)	: D3DFVF_NORMAL
// 5. Vertex Point Size 정점의 점 크기 (float)	: D3DFVF_PSIZE
// 6. Diffuse Color (DWORD)						: D3DFVF_DIFFUSE
// 7. Specular Color (DWORD)                    : D3DFVF_SPECULAR
// 8. Texture Coordinate Set 1 (float)          : D3DFVF_TEX0 - D3DFVF_TEX8

struct ST_PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DCOLOR	c;

	ST_PC_VERTEX() : p(0, 0, 0), c(D3DCOLOR_XRGB(0, 0, 0)) {}
	ST_PC_VERTEX(D3DXVECTOR3 _p, D3DCOLOR _c) : p(_p), c(_c) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};

struct ST_PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2	t;

	ST_PT_VERTEX() : p(0, 0, 0), t(0, 0) {}
	ST_PT_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR2 _t) : p(_p), t(_t) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
};

struct ST_PN_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3	n;

	ST_PN_VERTEX() : p(0, 0, 0), n(0, 0, 0) {}
	ST_PN_VERTEX(D3DXVECTOR3 _p, D3DXVECTOR3 _n = D3DXVECTOR3(0, 0, 0)) : p(_p), n(_n) {}

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL };
};

struct ST_PNT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3	n;
	D3DXVECTOR2	t;

	ST_PNT_VERTEX(
		D3DXVECTOR3 _p = D3DXVECTOR3(0, 0, 0), 
		D3DXVECTOR3 _n = D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR2 _t = D3DXVECTOR2(0, 0)
		) 
		: p(_p), n(_n) , t(_t) 
	{
	}

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
};

struct ST_POS_SAMPLE
{
	D3DXVECTOR3		p;
	int				n;
};

struct ST_ROT_SAMPLE
{
	D3DXQUATERNION	q;
	int				n;
};

struct ST_SPHERE
{
	D3DXVECTOR3		p;
	float			r;
	bool			isPicked;
};

struct ST_SIZE
{
	float			fWidth;
	float			fHeight;

	ST_SIZE() : fWidth(0.0f), fHeight(0.0f) {}
	ST_SIZE(float _w, float _h) : fWidth(_w), fHeight(_h) {}
};

#pragma pack(push, 1)
struct ST_NODE
{
	enum STATE
	{
		STATE_PURE, STATE_OPEN, STATE_CLOSING, STATE_CLOSED, STATE_MAX
	};
	enum TYPE
	{
		TYPE_EMPTY, TYPE_BLOCK, TYPE_START, TYPE_DEST, TYPE_MAX
	};
	enum DIRECTION
	{
		DIR_NONE = 0, DIR_UP = 1, DIR_DOWN = 2, DIR_LEFT = 4, DIR_RIGHT = 8
	};

	//int Dir; <- 저 구조체 멤버변수 선언이 멤버 어느사이에 선언되느냐에 따라_
	//_에러가 나거나 메모리오류가 나거나 잘되거나 한다._
	//_너무 어처구니가 없다... 이유를 알고 대응하고 싶다.
	//[해결]구조체 바이트 패딩 때문이었고, #pragma pack(push, 1) #pragma pack(pop)으로 관리해준다.

	int			nRow;
	int			nCol;
	int			nNum;
	float		fF;
	float		fG;
	int			nH;
	int			nDir;
	bool		isRoute;	//최단경로에 포함되는가? 기본값 false

	STATE		eState;

	TYPE		eType;

	ST_NODE*	pParent;

};
#pragma pack(pop)

#include "cObject.h"
#include "cUIObject.h"
#include "cObjectPool.h"
#include "cAction.h"
#include "cGameObject.h"
#include "cDeviceManager.h"
#include "cTextureManager.h"
#include "cTimeManager.h"
#include "cAutoReleasePool.h"
#include "cSkinnedMeshManager.h"
#include "cFontManager.h"
#include "cKeyManager.h"