// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// Dx3D.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

// TODO: 필요한 추가 헤더는
// 이 파일이 아닌 STDAFX.H에서 참조합니다.

void LogDebugMessage(const char* fmt, ...) {


	static char szBuffer[2048]; // this here is a caveat!

	va_list args;

	va_start(args, fmt);
	//sprintf_s(szBuffer, 2048, fmt, args);
	_vsnprintf_s(szBuffer, 2048, fmt, args);
	va_end(args);

	OutputDebugString(szBuffer);
}