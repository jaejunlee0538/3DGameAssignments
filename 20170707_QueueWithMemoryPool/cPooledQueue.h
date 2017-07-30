#pragma once
#include <cNode.h>
#include <iostream>

template <typename ValueT>
class cPooledQueue
{
public:
	typedef cNode<ValueT> NodeType;
private:
	NodeType* m_pFront;
	NodeType* m_pRear;

	NodeType* m_pPoolFront;
public:
	cPooledQueue();
	~cPooledQueue();

	void Clear();
	void Release(NodeType* node);
	void Enqueue(ValueT val);
	NodeType* Dequeue();
	void Print();
	int GetNNodePool();
private:
	NodeType* GetNodeFromPool();
};

template<typename ValueT>
cPooledQueue<ValueT>::cPooledQueue()
	:m_pFront(nullptr), m_pRear(nullptr), m_pPoolFront(nullptr)
{
}

template<typename ValueT>
cPooledQueue<ValueT>::~cPooledQueue()
{
	Clear();
}

template<typename ValueT>
void cPooledQueue<ValueT>::Clear()
{
	NodeType* current = m_pFront;
	while (current) {
		NodeType* tmp = current->m_pNext;
		delete current;
		current = tmp;
	}
	current = m_pPoolFront;
	while (current) {
		NodeType* tmp = current->m_pNext;
		delete current;
		current = tmp;
	}
}

template<typename ValueT>
void cPooledQueue<ValueT>::Release(NodeType * node)
{
	if (node)
	{
		node->m_pNext = m_pPoolFront;
		m_pPoolFront = node;
	}
}

template<typename ValueT>
void cPooledQueue<ValueT>::Enqueue(ValueT val)
{
	NodeType* node = GetNodeFromPool();
	node->m_nValue = val;
	if (m_pFront == nullptr) {
		//큐에 아무것도 없는 경우
		m_pRear = m_pFront = node;
	}
	else {
		m_pRear->m_pNext = node;
		m_pRear = node;
	}
}

template<typename ValueT>
typename cPooledQueue<ValueT>::NodeType * cPooledQueue<ValueT>::Dequeue()
{
	if (m_pFront == nullptr) {
		return nullptr;
	}
	cNode<ValueT>* ret = m_pFront;
	if (m_pFront != m_pRear) {
		m_pFront = m_pFront->m_pNext;
	}
	else {
		m_pFront = m_pRear = nullptr;
	}
	return ret;
}

template<typename ValueT>
void cPooledQueue<ValueT>::Print()
{
	NodeType* current = m_pFront;
	while (current) {
		std::cout << current->m_nValue << std::endl;
		current = current->m_pNext;
	}
	//std::cout << "놀고 있는 노드 : " << GetNNodePool() << " 개" << std::endl;
}

template<typename ValueT>
int cPooledQueue<ValueT>::GetNNodePool()
{
	int count = 0;
	NodeType* current = m_pPoolFront;
	while (current) {
		count++;
		current = current->m_pNext;
	}
	return count;
}

template<typename ValueT>
typename cPooledQueue<ValueT>::NodeType * cPooledQueue<ValueT>::GetNodeFromPool()
{
	if (m_pPoolFront == nullptr)
		return new NodeType();
	NodeType* tmp = m_pPoolFront;
	m_pPoolFront = m_pPoolFront->m_pNext;
	return tmp;
}

