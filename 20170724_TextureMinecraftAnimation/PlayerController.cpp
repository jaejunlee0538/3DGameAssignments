#include "stdafx.h"
#include "PlayerController.h"
#include "KeyManager.h"
#include "MinecraftCharacter.h"
namespace SGA {

	PlayerController::PlayerController()
	{
	}


	PlayerController::~PlayerController()
	{
	}

	void PlayerController::setPlayer(MinecraftCharacter * player)
	{
		_player = player;
	}

	void PlayerController::update(UserInputData & input)
	{
		if (_player) {
			bool walk = false;
			if (input.hasInput(SGA::UP_KEY_STAYDOWN)) {
				_player->translateLocal(0, 0, 0.5);
				walk = true;
			}
			if (input.hasInput(SGA::DOWN_KEY_STAYDOWN)) {
				_player->translateLocal(0, 0, -0.5);
				walk = true;
			}
			if (input.hasInput(SGA::LEFT_KEY_STAYDOWN)) {
				_player->rotateLocal(0, -0.1, 0);
				walk = true;
			}
			if (input.hasInput(SGA::RIGHT_KEY_STAYDOWN)) {
				_player->rotateLocal(0, 0.1, 0);
				walk = true;
			}
			if (walk) {
				if (_player->getState() == SGA::MinecraftCharacter::STATE_SPINATTACK) {

				}
				else {
					_player->setState(SGA::MinecraftCharacter::STATE_WALK);
				}
			}
			else if (GET_KEY_MANAGER()->isStayKeyDown('J')) {
				_player->setState(SGA::MinecraftCharacter::STATE_SPINATTACK);
			}
			else if (GET_KEY_MANAGER()->isOnceKeyDown('K')) {
				_player->setState(SGA::MinecraftCharacter::STATE_KICKATTACK);
			}
			else{
				if (_player->getState() != SGA::MinecraftCharacter::STATE_KICKATTACK) {
					_player->setState(SGA::MinecraftCharacter::STATE_IDLE);
				}
			}
		}
	}
}