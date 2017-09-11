#pragma once

#define g_pObjectPool cObjectPool::GetInstance()

class cObjectPool
{
private:
	set<cObject*> m_setObject;

private:
	SINGLETON(cObjectPool);

public:
	void AddObject(cObject* pObject);
	void RemoveObject(cObject* pObject);
	void Destroy();
};

