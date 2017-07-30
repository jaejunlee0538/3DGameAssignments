#include "stdafx.h"
#include "UserInputs.h"
#include "KeyManager.h"
namespace SGA {
	UserInputData InputManager::GenerateUserInput()
	{
		static int prevX=0, prevY = 0;
		UserInputData input;
		input.clear();
		input.mousePos = _mousePos;
		input.mouseWheel = _mouseWheel;
		_mouseWheel = 0;
		
		if (prevX != input.mousePos.x || prevY != input.mousePos.y) {
			input.setInput(MOUSE_MOVE);
			input.mouseDelta.x = input.mousePos.x - prevX;
			input.mouseDelta.y = input.mousePos.y - prevY;
		}
		else {
			input.mouseDelta.x = 0;
			input.mouseDelta.y = 0;
		}
		
		if (GET_KEY_MANAGER()->isOnceKeyDown(VK_LBUTTON)) {
			input.setInput(LBUTTON_DOWN);
		}
		if (GET_KEY_MANAGER()->isOnceKeyUp(VK_LBUTTON)) {
			input.setInput(LBUTTON_UP);
		}
		if (GET_KEY_MANAGER()->isStayKeyDown(VK_LBUTTON)) {
			input.setInput(LBUTTON_STAYDOWN);
		}
		
		if (GET_KEY_MANAGER()->isOnceKeyDown(VK_RBUTTON)) {
			input.setInput(RBUTTON_DOWN);
		}
		else if (GET_KEY_MANAGER()->isOnceKeyUp(VK_RBUTTON)) {
			input.setInput(RBUTTON_UP);
		}
		if (GET_KEY_MANAGER()->isStayKeyDown(VK_RBUTTON)) {
			input.setInput(RBUTTON_STAYDOWN);
		}

		if (GET_KEY_MANAGER()->isOnceKeyDown(VK_RETURN)) {
			input.setInput(OK);
		}
		if (GET_KEY_MANAGER()->isOnceKeyDown(VK_ESCAPE)) {
			input.setInput(CANCEL);
		}

		if (GET_KEY_MANAGER()->isOnceKeyDown('W')) {
			input.setInput(UP_KEY_DOWN);
		}
		if (GET_KEY_MANAGER()->isStayKeyDown('W')) {
			input.setInput(UP_KEY_STAYDOWN);
		}
		if (GET_KEY_MANAGER()->isOnceKeyUp('W')) {
			input.setInput(UP_KEY_UP);
		}
		if (GET_KEY_MANAGER()->isOnceKeyDown('A')) {
			input.setInput(LEFT_KEY_DOWN);
		}
		if (GET_KEY_MANAGER()->isStayKeyDown('A')) {
			input.setInput(LEFT_KEY_STAYDOWN);
		}
		if (GET_KEY_MANAGER()->isOnceKeyUp('A')) {
			input.setInput(LEFT_KEY_UP);
		}
		if (GET_KEY_MANAGER()->isOnceKeyDown('S')) {
			input.setInput(DOWN_KEY_DOWN);
		}
		if (GET_KEY_MANAGER()->isStayKeyDown('S')) {
			input.setInput(DOWN_KEY_STAYDOWN);
		}
		if (GET_KEY_MANAGER()->isOnceKeyUp('S')) {
			input.setInput(DOWN_KEY_UP);
		}
		if (GET_KEY_MANAGER()->isOnceKeyDown('D')) {
			input.setInput(RIGHT_KEY_DOWN);
		}
		if (GET_KEY_MANAGER()->isStayKeyDown('D')) {
			input.setInput(RIGHT_KEY_STAYDOWN);
		}
		if (GET_KEY_MANAGER()->isOnceKeyUp('D')) {
			input.setInput(RIGHT_KEY_UP);
		}
		if (GET_KEY_MANAGER()->isStayKeyDown(VK_CONTROL)) {
			input.setVKey(VKEY_CONTROL);
		}
		if (GET_KEY_MANAGER()->isStayKeyDown(VK_LBUTTON)) {
			input.setVKey(VKEY_LBUTTON);
		}
		if (GET_KEY_MANAGER()->isStayKeyDown(VK_RBUTTON)) {
			input.setVKey(VKEY_RBUTTON);
		}
		if (GET_KEY_MANAGER()->isStayKeyDown(VK_SHIFT)) {
			input.setVKey(VKEY_SHIFT);
		}
		prevX = input.mousePos.x;
		prevY = input.mousePos.y;
		return input;
	}

	InputManager::InputManager()
	{
		_mousePos = { 0,0 };
		_mouseWheel = 0;
	}

	void InputManager::setMouseWheel(short wheel)
	{
		_mouseWheel = wheel;
	}
	void InputManager::setMousePosition(POINT pos)
	{
		_mousePos = pos;
	}
}