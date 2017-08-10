#include "StdAfx.h"
#include "cObjectPool.h"


cObjectPool::cObjectPool(void)
{
}

cObjectPool::~cObjectPool(void)
{
}

void cObjectPool::AddObject( cObject* pObject )
{
	m_setObject.insert(pObject);
}

void cObjectPool::RemoveObject( cObject* pObject )
{
	m_setObject.erase(pObject);
}

void cObjectPool::Destroy()
{
	assert(m_setObject.empty() && "cObject Memory Leak");
}
