#pragma once
#include "cNode.h"
#include <iostream>
template <typename ValueT>
class cStack
{
public:
	typedef cNode<ValueT> NodeType;
private:
	NodeType* m_pTop;
public:
	cStack();
	~cStack();

	bool Empty();
	void Clear();
	void Push(ValueT val);
	NodeType* Pop();
	NodeType* Top();
	void Print();
};

template<typename ValueT>
cStack<ValueT>::cStack()
	:m_pTop(nullptr)
{
}

template<typename ValueT>
cStack<ValueT>::~cStack()
{
	Clear();
}

template<typename ValueT>
bool cStack<ValueT>::Empty() {
	return m_pTop == nullptr;
}

template<typename ValueT>
void cStack<ValueT>::Clear()
{
	while (m_pTop) {
		Pop();
	}
}

template<typename ValueT>
void cStack<ValueT>::Push(ValueT val)
{
	NodeType* tmp = m_pTop;
	m_pTop = new NodeType(val);
	m_pTop->m_pNext = tmp;
}

template<typename ValueT>
typename cStack<ValueT>::NodeType * cStack<ValueT>::Pop()
{
	if (m_pTop == nullptr) {
		return nullptr;
	}
	NodeType* tmp = m_pTop;
	m_pTop = m_pTop->m_pNext;
	return tmp;
}

template<typename ValueT>
typename cStack<ValueT>::NodeType * cStack<ValueT>::Top()
{
	return m_pTop;
}

template<typename ValueT>
void cStack<ValueT>::Print()
{
	NodeType* current = m_pTop;
	while (current) {
		std::cout << current->m_nValue << std::endl;
		current = current->m_pNext;
	}
}
