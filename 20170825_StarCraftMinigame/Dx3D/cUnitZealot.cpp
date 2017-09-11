#include "stdafx.h"
#include "cUnitZealot.h"
#include "MessageTypes.h"
#include "cKeyManager.h"
cUnitZealot::cUnitZealot(int maxHP, cSkinnedMesh * mesh)
	:PlayableObject(maxHP, mesh)
{

}

cUnitZealot::~cUnitZealot()
{

}

void cUnitZealot::update(bool isDirty)
{
	PlayableObject::update(isDirty);

	if (g_pKeyManager->isOnceKeyDown('A')) {
		attack();
	}
}

void cUnitZealot::setup()
{
	setAnimations("Stand", "Attack", "Walk");
	setState(STATE_IDLE);
}

void cUnitZealot::handleMessage(int msgType, const std::vector<cVariant>& params)
{
	switch (msgType) {
	case Messages::GIVE_DAMAGE:
	{
		//int damage = params[0].AsInt32;
		takeDamage(params[0].AsInt32);
	}
		break;
	}
}

void cUnitZealot::attackImpl()
{
	if (m_pWorld) {
		std::vector<GameObject*> objects;
		m_pWorld->QueryObjectsInRadius(getWorldPosition(), 0.8, objects);
		for each(auto obj in objects) {
			if (obj == this)
				continue;
			obj->handleMessage(Messages::GIVE_DAMAGE, {cVariant(2)});
		}
	}
}
