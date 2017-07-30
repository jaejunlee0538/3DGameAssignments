#pragma once
#include <Windows.h>
#include "SingletonBase.h"
#include <bitset>
#include <map>
#include <tstring.h>
#include <assert.h>
#define KEY_MAX 256
namespace SGA {
	class KeyManager : public SGA::SingletonBase<KeyManager>
	{
	private:
		std::bitset<KEY_MAX> _keyUp; //�ٷ� ������ Key up ����
		std::bitset<KEY_MAX> _keyDown; //�ٷ� ������ Key down ����
		std::bitset<KEY_MAX> _keyToggle; //
		std::bitset<KEY_MAX> _keyToggle2; //
		POINT _currentMousePos;
		std::map<int, std::map<tstring, int>> _keyMap;
		//std::map<tstring, int> _keyMap[MAX_PLAYER_NUM];
	public:
		KeyManager()
		{
		}

		virtual ~KeyManager()
		{
		}

		//
		HRESULT init(void)
		{
			for (int i = 0; i < KEY_MAX; i++) {
				bool isDown = GetAsyncKeyState(i) & 0x8000;
				_keyUp.set(i, !isDown);
				_keyDown.set(i, isDown);
				_keyToggle.set(i, isDown);
				_keyToggle2.set(i, isDown);
			}
			return S_OK;
		}

		//
		void release(void)
		{

		}

		//�ѹ��� ������ ��
		bool isOnceKeyDown(int key)
		{
			//GetAsyncKeyState ==> ���ͱ��� �˻簡��
			//GetKeyState ==> �����͸� �˻簡��
			if (GetAsyncKeyState(key) & 0x8000) {
				if (!_keyDown[key]) {
					_keyDown.set(key, true);
					return true;
				}
			}
			else {
				_keyDown.set(key, false);
			}
			return false;
		}

		//�ѹ��� ������ ��
		bool isOnceKeyUp(int key)
		{
			if (!(GetAsyncKeyState(key) & 0x8000)) {
				if (!_keyUp[key]) {
					_keyUp.set(key, true);
					return true;
				}
			}
			else {
				_keyUp.set(key, false);
			}
			return false;
		}

		//��� ������ ���� ��
		bool isStayKeyDown(int key)
		{
			if (GetAsyncKeyState(key) & 0x8000) return true;
			return false;
		}

		//
		bool isStayKeyUp(int key)
		{
			if (!(GetAsyncKeyState(key) & 0x8000)) return true;
			return false;
		}

		//
		////Ű�� ���°� �ٲ���ٸ� true�� ��ȯ(����->�ȴ���, �ȴ���->����)
		bool isKeyToggled(int key)
		{
			bool current = GetAsyncKeyState(key) & 0x8000;
			bool ret = current != _keyToggle2[key];
			_keyToggle2.set(key, current);
			return ret;
		}

		//����
		bool isToggleKey(int key) {
			//if (isOnceKeyDown(key)) {
			//	_keyToggle[key].flip();
			//}
			//return _keyToggle[key];
			return GetKeyState(key) & 0x0001;//Ű�� �� �� ���� �� ���� 0/1�� �Դ� ���� ��.
		}

		bool isOnceKeyDown(tstring keyName, int playerIdx) {
			return isOnceKeyDown(getKey(keyName, playerIdx));
		}

		bool isOnceKeyUp(tstring keyName, int playerIdx) {
			return isOnceKeyUp(getKey(keyName, playerIdx));
		}

		bool isStayKeyDown(tstring keyName, int playerIdx) {
			return isStayKeyDown(getKey(keyName, playerIdx));
		}

		bool isStayKeyUp(tstring keyName, int playerIdx) {
			return isStayKeyUp(getKey(keyName, playerIdx));
		}

		bool isKeyToggled(tstring keyName, int playerIdx) {
			return isKeyToggled(getKey(keyName, playerIdx));
		}

		bool isToggleKey(tstring keyName, int playerIdx) {
			return isToggleKey(getKey(keyName, playerIdx));
		}

		void setCurrentMousePos(POINT pt) {
			_currentMousePos = pt;
		}

		POINT getCurrentMousePos() const {
			return _currentMousePos;
		}

		void setKeyMap(int playerIdx, const std::map<tstring, int>& keyMap) {
			_keyMap[playerIdx] = keyMap;
		}

		int getKey(tstring keyType, int playerIdx) {
			auto itMap = _keyMap.find(playerIdx);
			assert(itMap != _keyMap.end());
			auto it = itMap->second.find(keyType);
			assert(it != _keyMap[playerIdx].end());
			return it->second;
		}

		const std::map<tstring, int>& getKeyMap(int playerIdx) {
			auto itMap = _keyMap.find(playerIdx);
			assert(itMap != _keyMap.end());
			return itMap->second;
		}
	private:
		bool isKeyUp(int key) const {
			return _keyUp[key];
		}

		bool isKeyDown(int key) const {
			return _keyDown[key];
		}

		void setKeyDown(int key, bool state) {
			_keyDown.set(key, state);
		}

		void setKeyUp(int key, bool state) {
			_keyUp.set(key, state);
		}
	};
#define GET_KEY_MANAGER() SGA::KeyManager::getSingleton()
}