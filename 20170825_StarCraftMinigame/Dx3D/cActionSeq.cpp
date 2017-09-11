#include "StdAfx.h"
#include "cActionSeq.h"


cActionSeq::cActionSeq(void)
	: m_nCurrActionIndex(0)
{
}

cActionSeq::~cActionSeq(void)
{
	for each(auto p in m_vecAction){
		SAFE_DELETE(p);
	}
	m_vecAction.clear();
}

void cActionSeq::AddAction( cAction* pAction )
{
	if(pAction){
		pAction->SetDelegate(this);
		m_vecAction.push_back(pAction);
	}
}

void cActionSeq::Start()
{
	m_nCurrActionIndex = 0;

	if (m_vecAction.empty()) return;

	m_vecAction[0]->Start();
}

void cActionSeq::Update()
{
	if(m_nCurrActionIndex >= m_vecAction.size()) return;
	m_vecAction[m_nCurrActionIndex]->Update();
}

void cActionSeq::OnFinish( cAction* pSender )
{
	m_nCurrActionIndex++;
	if(m_nCurrActionIndex < m_vecAction.size()){
		m_vecAction[m_nCurrActionIndex]->Start();
	}
	else
	{
		if (m_pDelegate){
			m_pDelegate->OnFinish(this);
		}
	}
}