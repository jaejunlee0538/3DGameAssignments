#include "GameObject.h"

namespace SGA {

	GameObject::GameObject()
	{
	}


	GameObject::~GameObject()
	{
	}

	float GameObject::getPositionX() const
	{
		return _x;
	}

	float GameObject::getPositionY() const
	{
		return _y;
	}

	void GameObject::setPositionX(float x)
	{
		_x = x;
	}

	void GameObject::setPositionY(float y)
	{
		_y = y;
	}
}