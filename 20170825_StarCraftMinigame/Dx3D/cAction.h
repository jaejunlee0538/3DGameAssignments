#pragma once

class GameObject;
class cAction;

class iActionDelegate
{
public:
	virtual void OnFinish(cAction* pSender) = 0;
};

class cAction
{
protected:
	float m_fPassedActionTime;
	SYNTHESIZE(float, m_fActionTime, ActionTime);
	SYNTHESIZE(GameObject*, m_pTarget, Target);
	SYNTHESIZE(iActionDelegate*, m_pDelegate, Delegate);

public:
	cAction(void);
	virtual ~cAction(void);

	virtual void Start();
	virtual void Update();
};

