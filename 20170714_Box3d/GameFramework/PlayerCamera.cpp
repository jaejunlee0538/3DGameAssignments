#include "PlayerCamera.h"
#include "GameNode.h"
#include <ClockManager.h>
namespace SGA {
	PlayerCamera::PlayerCamera()
		:_target(NULL)
	{
		_K = 10;
		_D = 10;
		_eX = 0.0;
		_eY = 0.0;
		_maxSpeed = 6000;
		enableSpring(false);
		enableWorldLimit(false, NULL);
		_followX = true;
		_followY = true;
	}


	PlayerCamera::~PlayerCamera()
	{
	}

	void PlayerCamera::update() {
		follow();
	}

	void PlayerCamera::follow() {
		if (_target) {
			if (isSpringEnabled() == false) {
				setPosition(
					_followX ? _target->getPositionX() : getPositionX(),
					_followY ? _target->getPositionY() : getPositionY());
			}
			else {
				updateSpring();
			}

			if (isWorldLimitEnabled()) {
				limitInWorld();
			}
		}
	}

	void PlayerCamera::updateSpring()
	{
		float targetX, targetY;
		float velX, velY;
		float eX, eY;
		targetX = _target->getPositionX();
		targetY = _target->getPositionY();
		eX = targetX - getPositionX();
		eY = targetY - getPositionY();
		if (_followX) {
			velX = eX * _K - (eX - _eX) * _D;
		}
		else {
			velX = 0.0f;
		}

		if (_followY) {
			velY = eY * _K - (eY - _eY) * _D;
		}
		else {
			velY = 0.0f;
		}

		float speed = sqrt(velX*velX + velY*velY);
		if (speed > _maxSpeed) {
			float s = _maxSpeed / speed;
			velX *= s;
			velY *= s;
		}

		float dt = GET_CLOCK_MANAGER()->getDeltaGameTime();
		setPosition(getPositionX() + dt*velX, getPositionY() + dt*velY);

		_eX = eX;
		_eY = eY;
	}

	void PlayerCamera::limitInWorld()
	{
		if (_pos.x < _worldRECT.left + _width / 2) {
			_pos.x = _worldRECT.left + _width / 2;
		}
		if (_pos.x > _worldRECT.right - _width / 2) {
			_pos.x = _worldRECT.right - _width / 2;
		}
		if (_pos.y < _worldRECT.top + _height / 2) {
			_pos.y = _worldRECT.top + _height / 2;
		}
		if (_pos.y > _worldRECT.bottom - _height / 2) {
			_pos.y = _worldRECT.bottom - _height / 2;
		}
	}
}