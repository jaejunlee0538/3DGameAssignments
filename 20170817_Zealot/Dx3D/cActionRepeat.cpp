#include "StdAfx.h"
#include "cActionRepeat.h"


cActionRepeat::cActionRepeat(void)
	: m_pAction(NULL)
{
}


cActionRepeat::~cActionRepeat(void)
{
	SAFE_RELEASE(m_pAction);
}

void cActionRepeat::Start()
{
	if(m_pAction) m_pAction->Start();
}

void cActionRepeat::Update()
{
	if (m_pAction) m_pAction->Update();
}

void cActionRepeat::OnFinish( cAction* pSender )
{
	if(m_pAction) m_pAction->Start();
}
