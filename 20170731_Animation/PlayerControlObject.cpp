#include "stdafx.h"
#include "PlayerControlObject.h"
#include "KeyManager.h"
namespace SGA {

	PlayerControlObject::PlayerControlObject()
	{
	}


	PlayerControlObject::~PlayerControlObject()
	{
	}

	void PlayerControlObject::update(bool isDirty)
	{
		if (GET_KEY_MANAGER()->isStayKeyDown('W')) {
			translateLocal(0, 0, 0.05);
		}
		if (GET_KEY_MANAGER()->isStayKeyDown('S')) {
			translateLocal(0, 0, -0.05);
		}
		if (GET_KEY_MANAGER()->isStayKeyDown('A')) {
			rotateLocal(0, -0.1f, 0);
		}
		if (GET_KEY_MANAGER()->isStayKeyDown('D')) {
			rotateLocal(0, 0.1f, 0);
		}
		GameObject::update(isDirty);
	}

}