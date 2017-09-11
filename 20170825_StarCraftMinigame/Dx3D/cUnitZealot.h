#pragma once
#include "PlayableObject.h"
class ZealotController;
class cUnitZealot :
	public PlayableObject
{
public:
	cUnitZealot(int maxHP, cSkinnedMesh * mesh);
	virtual ~cUnitZealot();

	void update(bool isDirty) override;
	virtual void setup() override;
	void handleMessage(int msgType, const std::vector<cVariant>& params) override;
private:
	void attackImpl() override;
};

