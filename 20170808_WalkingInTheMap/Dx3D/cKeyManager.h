#pragma once
#include <bitset>


#define KEYMAX 256
#define g_pKeyManager cKeyManager::GetInstance()

class cKeyManager
{

private:
	SINGLETON(cKeyManager)

private:
	bitset<KEYMAX> _keyUp;
	bitset<KEYMAX> _keyDown;

public:
	HRESULT init(void);
	void release(void);

	//ÇÑ¹ø¸¸ ´­·¶À»¶§..
	bool isOnceKeyDown(int key);

	//ÇÑ¹ø¸¸ ¶¼¾úÀ»¶§..
	bool isOnceKeyUp(int key);

	//°è¼Ó ´©¸£°í ÀÖÀ»¶§..
	bool isStayKeyDown(int key);

	//µþ±ï on µþ±ï off ~~~~~ (Àüµî ½ºÀ§Ä¡ °³³ä...)
	bool isToggleKey(int key);

	bitset<KEYMAX> getKeyUp() { return _keyUp; }
	bitset<KEYMAX> getKeyDown() { return _keyDown; }

	void setKeyDown(int key, bool state) { _keyDown.set(key, state); }
	void setKeyUp(int key, bool state) { _keyUp.set(key, state); }

};

