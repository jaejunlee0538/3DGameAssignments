#include "stdafx.h"
#include "PlayScene.h"
#include <KeyManager.h>
#include <functional>
#include <MathUtils.h>
#include <Utils.h>
#include <sstream>
#include <tstring.h>
#include <CommonMacroHeader.h>
#include "UserInputs.h"
PlayScene::PlayScene()
{

}

PlayScene::~PlayScene()
{
}

HRESULT PlayScene::init()
{
	SGA::GameNode::init();
	_box.init(2, 2, 4);
	_box.SetPosition(0, 1.5, 0);
	_box.SetAngles(0, 0.2, 0);

	_grid.init(15, 15,1, 1);
	_grid.SetPosition(0, 0, 0);

	_camera.SetCullMode(Camera3D::CULL_CCW);
	_camera.init(getMemDC(), SGA::deg2rad(80), (float)WIN_SIZE_X / WIN_SIZE_Y, 1, 500, 0, 0, WIN_SIZE_X, WIN_SIZE_Y);
	_camera.SetPosition(4, 15, -10);
	_camera.SetAngles(SGA::deg2rad(45), 0, 0);
	//_camera.SetTextColor(RGB()
	SetBkMode(getMemDC(), TRANSPARENT);
	_camera.SetTextColor(RGB(255, 0, 0));
	//_camera.MakeLookAt(_box.GetPosition());
	return S_OK;
}

void PlayScene::update(void)
{
	GameNode::update();
	SGA::UserInputData input = SGA::generateUserInput();
	if (input.hasInput(SGA::LEFT_KEY_STAYDOWN)) {
		if (input.isVKeyPressed(SGA::VKEY_SHIFT))
			_box.RotateLocal(0, -0.05, 0);
		else
			_box.Rotate(0, -0.05, 0);
	}
	if (input.hasInput(SGA::RIGHT_KEY_STAYDOWN)) {
		if (input.isVKeyPressed(SGA::VKEY_SHIFT))
			_box.RotateLocal(0, 0.05, 0);
		else
			_box.Rotate(0, 0.05, 0);
	}
	if (input.hasInput(SGA::UP_KEY_STAYDOWN)) {
		_box.TranslateLocal(0, 0, 0.5);
	}
	if (input.hasInput(SGA::DOWN_KEY_STAYDOWN)) {
		_box.TranslateLocal(0, 0, -0.5);
	}
	if (input.mouseWheel > 0) {
		_camera.IncrementDistanceToTarget(-0.5);
	}
	if (input.mouseWheel < 0) {
		_camera.IncrementDistanceToTarget(0.5);
	}
	if (input.isVKeyPressed(SGA::VKEY_LBUTTON) && input.hasInput(SGA::MOUSE_MOVE)) {
		float dx=0, dy=0;
		if (input.mouseDelta.x != 0) {
			//World의 y축으로 회전
			dy = input.mouseDelta.x*0.01;
			if (dy > 0.1)
				dy = 0.1;
			if (dy < -0.1)
				dy = -0.1;
		}
		if (input.mouseDelta.y != 0) {
			//World의 x축으로 회전
			dx = input.mouseDelta.y*0.01;
			if (dx > 0.1)
				dx = 0.1;
			if (dx < -0.1)
				dx = -0.1;
		}
		if (input.isVKeyPressed(SGA::VKEY_SHIFT))
			_camera.Rotate(dx, 0, dy);
		else
			_camera.Rotate(dx, dy, 0);
	}
	_camera.FollowTarget(_box.GetPosition());
}

void PlayScene::release(void)
{

}

void PlayScene::render(void)
{
	SGA::GameNode::clearScreen(RGB(0,0,0));
	_camera.SetLineColor(RGB(0, 200, 0));
	_camera.Render(_grid);
	_camera.RenderText(_grid.GetWidth()/2, 1, 0, TEXT("X"));
	_camera.RenderText(0, 1, _grid.GetHeight()/2, TEXT("Z"));
	_camera.SetLineColor(RGB(255, 0, 0));
	_camera.Render(_box);
	_camera.RenderCameraInfo(10, 100);
}
