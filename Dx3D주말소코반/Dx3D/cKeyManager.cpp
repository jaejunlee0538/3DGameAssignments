#include "stdafx.h"
#include "cKeyManager.h"


cKeyManager::cKeyManager(void)
{
}

cKeyManager::~cKeyManager(void)
{
}

HRESULT cKeyManager::init(void)
{
	for (int i = 0; i < KEYMAX; i++)
	{
		this->getKeyUp().set(i, false);
		this->getKeyDown().set(i, false);
	}
	return S_OK;
}

void cKeyManager::release(void)
{
}

//한번만 눌렀을때..
bool cKeyManager::isOnceKeyDown(int key)
{
	//GetKeyState : 누른것만 검사 가능
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!this->getKeyDown()[key])
		{
			this->setKeyDown(key, true);
			return true;
		}
	}
	else this->setKeyDown(key, false);
	return false;
}

//한번만 떼었을때..
bool cKeyManager::isOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) this->setKeyUp(key, true);
	else
	{
		if (this->getKeyUp()[key])
		{
			this->setKeyUp(key, false);
			return true;
		}
	}
	return false;
}

//계속 누르고 있을때..
bool cKeyManager::isStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

//딸깍 on 딸깍 off ~~~~~ (전등 스위치 개념...)
bool cKeyManager::isToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001) return true;
	return false;
}
