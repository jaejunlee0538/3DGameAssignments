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
		//current�� ����� ���� next�� ������ �� ���� ������
		//next�� ������ ���� �����Ѵ�.
		tmp = current->m_pNext;
		//current�� �����Ѵ�.
		delete current;
		//�����ϰ� �ִ� current�� next�� current�� �����Ѵ�.
		current = tmp;
	}
	//����Ʈ�� �ƹ��͵� ������� �ʱ� ������ head�� null�� �����Ѵ�.
	m_pHead = nullptr;
}

template<typename ValueT>
void cLinkedList<ValueT>::Insert(int n)
{
	if (m_pHead == nullptr) {
		//����Ʈ�� �ϰ͵� �ȵ������
		m_pHead = new NodeType();
		m_pHead->m_nValue = n;
		m_pHead->m_pNext = nullptr;
	}
	else {

		NodeType* current = m_pHead;
		//����Ʈ�� tail���� ������
		while (current->m_pNext != nullptr) {
			current = current->m_pNext;
		}
		//tail�� next�� ���ο� ��带 �߰��Ѵ�.
		current->m_pNext = new NodeType();
		current->m_pNext->m_nValue = n;
		//���ο� ��尡 tail�� �Ǿ��� ������ next�� null�� �����Ѵ�.
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
				//ù ��° ��尡 �츮�� ã�� ���� ���� ���
				m_pHead = current->m_pNext;
			}
			else {
				//�߰� �Ǵ� tail�� �ִ� ��尡 �츮�� ã�� ���
				prev->m_pNext = current->m_pNext;
			}
			delete current;
			return;
		}
		//���� ��带 prev�� �����ϰ� �Űܳ���
		prev = current;
		//���� ��带 ��ĭ ���� ��Ų��
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
		//current�� next�� ������ ���� ����
		tmp = current->m_pNext;
		//current�� ����Ű�� ��ũ�� ������ �ݴ�� �����´�.
		current->m_pNext = prev;
		//current�� prev��
		prev = current;
		//�����ϰ� �����ϰ� �ִ� ���� current�� next�� current�� �����Ѵ�.
		current = tmp;
	}
	m_pHead = prev;
}
