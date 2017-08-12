#pragma once

#define g_pAutoReleasePool cAutoReleasePool::GetInstance()

class cAutoReleasePool
{
protected:
	set<cObject*> m_setPool;

private:
	SINGLETON(cAutoReleasePool);

public:
	void AddObject(cObject* pObject)
	{
		m_setPool.insert(pObject);
	}
	void Drain()
	{
		for each(auto p in m_setPool)
		{
			p->Release();
		}
		m_setPool.clear();
	}
};

