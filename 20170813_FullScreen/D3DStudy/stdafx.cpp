// stdafx.cpp : source file that includes just the standard includes
// 20170720_Box3DDirectX.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
D3DXVECTOR3 g_upVector(0, 1, 0);
const float MAX_PITCH_ANGLE = D3DX_PI * 0.49;
D3DXMATRIX Identity4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

void LogDebugMessage(const char* fmt, ...) {

	static char szBuffer[2048]; // this here is a caveat!

	va_list args;
	
	va_start(args, fmt);
	//sprintf_s(szBuffer, 2048, fmt, args);
	_vsnprintf_s(szBuffer, 2048, fmt, args);
	va_end(args);

	OutputDebugStringA(szBuffer);
}