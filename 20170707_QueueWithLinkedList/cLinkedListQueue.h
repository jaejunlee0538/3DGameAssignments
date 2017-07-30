#pragma once
#include <cNode.h>
#include <iostream>
template <typename ValueT>
class cLinkedListQueue
{
public:
	typedef cNode<ValueT> NodeType;
private:

	NodeType* m_pFront;
	NodeType* m_pRear;
public:
	cLinkedListQueue();
	~cLinkedListQueue();

	void Clear();
	void Enqueue(ValueT val);
	NodeType* Dequeue();
	void Print();
};

template<typename ValueT>
cLinkedListQueue<ValueT>::cLinkedListQueue()
	:m_pFront(nullptr), m_pRear(nullptr)
{
}

template<typename ValueT>
cLinkedListQueue<ValueT>::~cLinkedListQueue()
{
	Clear();
}

template<typename ValueT>
void cLinkedListQueue<ValueT>::Clear()
{
	NodeType* current = m_pFront;
	while (current) {
		NodeType* tmp = current->m_pNext;
		delete current;
		current = tmp;
	}
}

template<typename ValueT>
void cLinkedListQueue<ValueT>::Enqueue(ValueT val)
{
	cNode<ValueT>* node = new cNode<ValueT>(val);
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
typename cLinkedListQueue<ValueT>::NodeType * cLinkedListQueue<ValueT>::Dequeue()
{
	if (m_pFront == nullptr) {
		return nullptr;
	}
	cNode<ValueT>* ret = m_pFront;
	if (m_pFront != m_pRear) {
		m_pFront = m_pFront->m_pNext;
	}else{
		m_pFront = m_pRear = nullptr;
	}
	return ret;
}

template<typename ValueT>
void cLinkedListQueue<ValueT>::Print()
{
	cNode<ValueT>* current = m_pFront;
	while (current) {
		std::cout << current->m_nValue << std::endl;
		current = current->m_pNext;
	}
}
