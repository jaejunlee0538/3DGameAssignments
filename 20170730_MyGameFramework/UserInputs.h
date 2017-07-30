#pragma once
#include <Windows.h>
#include "SingletonBase.h"
namespace SGA {
	enum ControlInputTypes {
		LBUTTON_STAYDOWN = 0,
		LBUTTON_UP,
		LBUTTON_DOWN,
		RBUTTON_STAYDOWN,
		RBUTTON_UP,
		RBUTTON_DOWN,
		LEFT_KEY_STAYDOWN,
		LEFT_KEY_UP,
		LEFT_KEY_DOWN,
		RIGHT_KEY_STAYDOWN,
		RIGHT_KEY_UP,
		RIGHT_KEY_DOWN,
		UP_KEY_STAYDOWN,
		UP_KEY_UP,
		UP_KEY_DOWN,
		DOWN_KEY_STAYDOWN,
		DOWN_KEY_UP,
		DOWN_KEY_DOWN,

		MOUSE_MOVE,
		CANCEL,
		OK,

		N_INPUTS
	};
	enum VirtualKeyTypes {
		VKEY_CONTROL = N_INPUTS+1,
		VKEY_LBUTTON,
		VKEY_RBUTTON,
		VKEY_SHIFT
	};

	class UserInputData {
		int _flag;

	public:
		POINT mousePos;
		POINT mouseDelta;
		int mouseWheel;

		void setInput(ControlInputTypes inputType) {
			_flag |= (1 << inputType);
		}

		void clearInput(ControlInputTypes inputType) {
			_flag &= ~(1 << inputType);
		}

		bool hasInput(ControlInputTypes inputType) {
			return _flag & (1 << inputType);
		}

		void clear() {
			mousePos.x = mousePos.y = 0;
			mouseWheel = 0;
			_flag = 0;
		}

		void setVKey(VirtualKeyTypes vk) {
			_flag |= (1 << vk);
		}

		bool isVKeyPressed(VirtualKeyTypes vk) {
			return _flag & (1 << vk);
		}
	};

	class InputManager :public SGA::SingletonBase<InputManager>{
	public:
		InputManager();
		void setMouseWheel(short wheel);
		void setMousePosition(POINT pos);
		UserInputData GenerateUserInput();
	private:
		short _mouseWheel;
		POINT _mousePos;
	};
}
#define GET_INPUT_MANAGER() SGA::InputManager::getSingleton()	