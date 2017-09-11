#pragma once
class cActionSeq
	: public cAction
	, public iActionDelegate
{
protected:
	vector<cAction*>	m_vecAction;
	int					m_nCurrActionIndex;

public:
	cActionSeq(void);
	virtual ~cActionSeq(void);

	virtual void AddAction(cAction* pAction);

	// cAction override
	virtual void Start() override;
	virtual void Update() override;

	// iActionDelegate override
	virtual void OnFinish(cAction* pSender) override;
};

