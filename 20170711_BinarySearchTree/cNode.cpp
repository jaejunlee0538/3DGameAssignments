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
		//�����Ϸ��� ���� this ����� ������ ������
		//���ʿ� ����
		if (m_pLChild)
			m_pLChild->Insert(val);//���ʿ� �ڽ��� ������ �ڽ����� �ñ��
		else
			m_pLChild = new cNode(val); //���ʿ� �ڽ��� ������ ���ο� �ڽ��� �����.
	}
	else {
		if (m_pRChild)
			m_pRChild->Insert(val);//�����ʿ� �ڽ��� ������ �ڽ����� �ñ��
		else
			m_pRChild = new cNode(val);//�����ʿ� �ڽ��� ������ ���ο� �ڽ��� �����.
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
		//���� ���� ��� �߰�! ���� ����
		if (m_pLChild && m_pRChild) {
			//�����Ϸ��� ��尡 ���� ��� �ڽ��� ������ �ִ� ���
			//�����ʿ��� ���� ���� ��带 �߶�´�.
			cNode* replacingNode = m_pRChild->SplitLeftMostNode(&m_pRChild);
			//�����Ϸ��� ����� ����/������ �ڽ��� ��ü��忡�� �����ش�.
			replacingNode->m_pLChild = m_pLChild;
			replacingNode->m_pRChild = m_pRChild;
			//�θ� ����� �ڽ� �����͸� �ٲ��ش�.
			*parentLink = replacingNode;
		}
		else if (m_pLChild) {
			//�����Ϸ��� ��尡 ���ʿ��� �ڽ� ��� ������ �ִ� ���
			*parentLink = m_pLChild;
		}
		else if (m_pRChild) {
			//�����Ϸ��� ��尡 �����ʿ��� �ڽ� ��� ������ �ִ� ���
			*parentLink = m_pRChild;
		}
		else {
			//�����Ϸ��� ��尡 ���� ����� ���
			*parentLink = nullptr;
		}
		delete this;
	}
	else if (n < m_nData) {
		//�����Ϸ��� �����Ͱ� ���� ��� �����ͺ��� �۴�
		if (m_pLChild)
			m_pLChild->Delete(&m_pLChild, n);
	}
	else {
		//�����Ϸ��� �����Ͱ� ���� ��� �����ͺ��� ũ��
		if (m_pRChild)
			m_pRChild->Delete(&m_pRChild, n);
	}
}

cNode * cNode::SplitLeftMostNode(cNode ** parentLink)
{
	if (m_pLChild) {
		return m_pLChild->SplitLeftMostNode(&m_pLChild);
	}
	//�θ�� this�� ������ ���� Ʈ���� �������ش�.
	*parentLink = m_pRChild;
	//������ ���� ���� �ش�.
	m_pRChild = nullptr;
	return this;
}
