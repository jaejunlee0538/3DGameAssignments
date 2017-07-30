#include "GameLoop.h"
#include <assert.h>
#include <algorithm>
#include <string>
#include <BitmapImage.h>
#include <WorldClock.h>
#include <SpriteManager.h>
#include <AnimationManager.h>
#include <ClockManager.h>
#include "Stdafx.h"
#include <KeyManager.h>
#include <functional>
#include <SceneManager.h>
#include "PlayScene.h"
#include <SoundManager.h>

GameLoop::GameLoop()
{
	
}

GameLoop::~GameLoop()
{

}

HRESULT GameLoop::init(){
	SGA::GameNode::init();
	GET_CLOCK_MANAGER()->setTextColor(RGB(255,255,255));
	GET_CLOCK_MANAGER()->showClockInfo(true);
	GET_SOUND_MANAGER()->init();

	GET_SCENE_MANAGER()->addScene(TEXT("PlayScene"), new PlayScene());
	GET_SCENE_MANAGER()->changeScene(TEXT("PlayScene"));
	return S_OK;
}

void GameLoop::release(void){
	SGA::GameNode::release();
	GET_CLOCK_MANAGER()->releaseSingleton();
	GET_SOUND_MANAGER()->release();
	GET_SOUND_MANAGER()->releaseSingleton();
}

void GameLoop::update(void){
	SGA::GameNode::update();
	GET_CLOCK_MANAGER()->updateGameWorldClock();
	GET_SCENE_MANAGER()->update();
	GET_SOUND_MANAGER()->update();
}

void GameLoop::render(void){
	//SGA::GameNode::clearScreen(RGB(0,0,0));
	SGA::GameNode::clearScreen(RGB(255, 255, 255));
	GET_SCENE_MANAGER()->render();
}

void GameLoop::controlMessage(int message)
{

}

