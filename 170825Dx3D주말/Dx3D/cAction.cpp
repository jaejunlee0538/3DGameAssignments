#include "StdAfx.h"
#include "cAction.h"


cAction::cAction(void)
	: m_fPassedActionTime(0.0f)
	, m_fActionTime(0.0f)
	, m_pTarget(NULL)
	, m_pDelegate(NULL)
{
}

cAction::~cAction(void)
{
}

void cAction::Start()
{
	m_fPassedActionTime = 0.0f;
}

void cAction::Update()
{

}

bool cAction::IsFinished() const
{
	return m_fPassedActionTime >= m_fActionTime;
}
