// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// Dx3D.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

#include "stdafx.h"

// TODO: �ʿ��� �߰� �����
// �� ������ �ƴ� STDAFX.H���� �����մϴ�.

void LogDebugMessage(const char* fmt, ...) {


	static char szBuffer[2048]; // this here is a caveat!

	va_list args;

	va_start(args, fmt);
	//sprintf_s(szBuffer, 2048, fmt, args);
	_vsnprintf_s(szBuffer, 2048, fmt, args);
	va_end(args);

	OutputDebugString(szBuffer);
}