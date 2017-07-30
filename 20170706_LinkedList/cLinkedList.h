#pragma once
#include "cNode.h"

template <typename ValueT>
class cLinkedList
{
private:
	typedef cNode<ValueT> NodeType;
	NodeType* m_pHead;

public:
	cLinkedList(void);
	~cLinkedList(void);

	void Clear();
	void Insert(int n);
	void Delete(int n);
	void Print();
	void Reverse();
};
/*
-3*1.3+(-1+10.1-3/(3*4))-2

*/

template<typename ValueT>
cLinkedList<ValueT>::cLinkedList(void)
	: m_pHead(nullptr)
{
}

template<typename ValueT>
cLinkedList<ValueT>::~cLinkedList(void)
{
	Clear();
}

template<typename ValueT>
void cLinkedList<ValueT>::Clear()
{
	NodeType* current = m_pHead;
	NodeType * tmp;
	while (current != nullptr) {
		//current를 지우고 나면 next에 접근할 수 없기 때문에
		//next를 안전한 곳에 보관한다.
		tmp = current->m_pNext;
		//current를 삭제한다.
		delete current;
		//보관하고 있던 current의 next를 current로 설정한다.
		current = tmp;
	}
	//리스트에 아무것도 들어있지 않기 때문에 head를 null로 설정한다.
	m_pHead = nullptr;
}

template<typename ValueT>
void cLinkedList<ValueT>::Insert(int n)
{
	if (m_pHead == nullptr) {
		//리스트에 암것도 안들어있음
		m_pHead = new NodeType();
		m_pHead->m_nValue = n;
		m_pHead->m_pNext = nullptr;
	}
	else {

		NodeType* current = m_pHead;
		//리스트의 tail까지 간다음
		while (current->m_pNext != nullptr) {
			current = current->m_pNext;
		}
		//tail의 next에 새로운 노드를 추가한다.
		current->m_pNext = new NodeType();
		current->m_pNext->m_nValue = n;
		//새로운 노드가 tail이 되었기 때문에 next를 null로 설정한다.
		current->m_pNext->m_pNext = nullptr;
	}
}

template<typename ValueT>
void cLinkedList<ValueT>::Delete(int n)
{
	NodeType* current = m_pHead;
	NodeType* prev = nullptr;
	while (current != nullptr) {
		if (current->m_nValue == n) {
			if (current == m_pHead) {
				//첫 번째 노드가 우리가 찾는 값을 가진 노드
				m_pHead = current->m_pNext;
			}
			else {
				//중간 또는 tail에 있는 노드가 우리가 찾는 노드
				prev->m_pNext = current->m_pNext;
			}
			delete current;
			return;
		}
		//현재 노드를 prev에 안전하게 옮겨놓고
		prev = current;
		//현재 노드를 한칸 전진 시킨다
		current = current->m_pNext;
	}
}

template<typename ValueT>
void cLinkedList<ValueT>::Print()
{
	NodeType* current = m_pHead;
	while (current != nullptr) {
		std::cout << current->m_nValue << std::endl;
		current = current->m_pNext;
	}
}

template<typename ValueT>
void cLinkedList<ValueT>::Reverse()
{
	NodeType * current = m_pHead;
	NodeType * prev = nullptr;
	NodeType * tmp;
	while (current != nullptr) {
		//current의 next를 안전한 곳에 보관
		tmp = current->m_pNext;
		//current가 가리키는 링크의 방향을 반대로 뒤집는다.
		current->m_pNext = prev;
		//current를 prev로
		prev = current;
		//안전하게 보관하고 있던 원래 current의 next를 current로 설정한다.
		current = tmp;
	}
	m_pHead = prev;
}
