#include "Stdafx.h"
#include "cNode.h"
#include <iostream>
#include <assert.h>
cNode::cNode(int data)
	:m_pLChild(nullptr), m_pRChild(nullptr)
	, m_nData(data)
{
	g_setPool.insert(this);
}

cNode::~cNode(void) {
	g_setPool.erase(this);
}

void cNode::Insert(int val) {
	if (val == m_nData)
		return;
	if (val < m_nData) {
		//삽입하려는 값이 this 노드의 값보다 작으면
		//왼쪽에 삽입
		if (m_pLChild)
			m_pLChild->Insert(val);//왼쪽에 자식이 있으면 자식한테 맡긴다
		else
			m_pLChild = new cNode(val); //왼쪽에 자식이 없으면 새로운 자식을 만든다.
	}
	else {
		if (m_pRChild)
			m_pRChild->Insert(val);//오른쪽에 자식이 있으면 자식한테 맡긴다
		else
			m_pRChild = new cNode(val);//오른쪽에 자식이 없으면 새로운 자식을 만든다.
	}
}

void cNode::Print() {
	if(m_pLChild)
		m_pLChild->Print();
	std::cout << m_nData << std::endl;
	if (m_pRChild)
		m_pRChild->Print();
}

void cNode::Count(int & count)
{
	count++;
	if (m_pLChild)
		m_pLChild->Count(count);
	if (m_pRChild)
		m_pRChild->Count(count);
}

void cNode::Destroy()
{
	if (m_pLChild)
		m_pLChild->Destroy();
	if (m_pRChild)
		m_pRChild->Destroy();
	delete this;
}

void cNode::Delete(cNode** parentLink, int n)
{
	if (n == m_nData) {
		//값이 같은 노드 발견! 삭제 시작
		if (m_pLChild && m_pRChild) {
			//삭제하려는 노드가 양쪽 모두 자식을 가지고 있는 경우
			//오른쪽에서 가장 왼쪽 노드를 잘라온다.
			cNode* replacingNode = m_pRChild->SplitLeftMostNode(&m_pRChild);
			//삭제하려는 노드의 왼쪽/오른쪽 자식을 교체노드에게 물려준다.
			replacingNode->m_pLChild = m_pLChild;
			replacingNode->m_pRChild = m_pRChild;
			//부모 노드의 자식 포인터를 바꿔준다.
			*parentLink = replacingNode;
		}
		else if (m_pLChild) {
			//삭제하려는 노드가 왼쪽에만 자식 노드 가지고 있는 경우
			*parentLink = m_pLChild;
		}
		else if (m_pRChild) {
			//삭제하려는 노드가 오른쪽에만 자식 노드 가지고 있는 경우
			*parentLink = m_pRChild;
		}
		else {
			//삭제하려는 노드가 리프 노드인 경우
			*parentLink = nullptr;
		}
		delete this;
	}
	else if (n < m_nData) {
		//삭제하려는 데이터가 현재 노드 데이터보다 작다
		if (m_pLChild)
			m_pLChild->Delete(&m_pLChild, n);
	}
	else {
		//삭제하려는 데이터가 현재 노드 데이터보다 크다
		if (m_pRChild)
			m_pRChild->Delete(&m_pRChild, n);
	}
}

cNode * cNode::SplitLeftMostNode(cNode ** parentLink)
{
	if (m_pLChild) {
		return m_pLChild->SplitLeftMostNode(&m_pLChild);
	}
	//부모와 this의 오른족 서브 트리를 연결해준다.
	*parentLink = m_pRChild;
	//오른쪽 연결 끊어 준다.
	m_pRChild = nullptr;
	return this;
}
