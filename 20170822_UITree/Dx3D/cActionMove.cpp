#include "StdAfx.h"
#include "cActionMove.h"


cActionMove::cActionMove(void)
	: m_vFrom(0, 0, 0)
	, m_vTo(0, 0, 0)
{
}

cActionMove::~cActionMove(void)
{
}

void cActionMove::Start()
{
	if (m_pTarget)
	{
		m_pTarget->SetPosition(m_vFrom);
	}
	cAction::Start();
}

void cActionMove::Update()
{
	if(!m_pTarget) return;

	if(m_fPassedActionTime < m_fActionTime)
	{
		m_fPassedActionTime += g_pTimeManager->GetDeltaTime();
		
		if (m_fPassedActionTime < m_fActionTime)
		{
			float t = m_fPassedActionTime / m_fActionTime;

			D3DXVECTOR3 p;
			D3DXVec3Lerp(&p, &m_vFrom, &m_vTo, t);
			m_pTarget->SetPosition(p);
		}
		else
		{
			m_pTarget->SetPosition(m_vTo);

			if (m_pDelegate)
			{
				m_pDelegate->OnFinish(this);
			}
		}
	}
}
