#include "cBST.h"
#include "cNode.h"
#include <iostream>
#include <assert.h>
#include <stack>


cBST::cBST()
	:m_pRoot(nullptr)
{
}

cBST::~cBST()
{
	Clear();
}

#define RECURSIVE_CLEAR 1
void cBST::Clear()
{
#if RECURSIVE_CLEAR
	if(m_pRoot)
		m_pRoot->Destroy();
	m_pRoot = nullptr;
#else
	PostorderSearch([](cNode* node) {delete node; });
#endif
}

void cBST::Insert(int n)
{
	if (m_pRoot == nullptr) {
		m_pRoot = new cNode(n);
		return;
	}
	m_pRoot->Insert(n);
}
#define DELETE_IMPL	2
void cBST::Delete(int n)
{
#if DELETE_IMPL==0
	if (m_pRoot == nullptr)
		return;
	cNode * nodeToDelete = m_pRoot;
	cNode * parentOfNodeToDelete = nullptr;
	//�����ߵ� ��带 ã�´�.
	do {
		if (nodeToDelete->m_nData == n)
			break;
		parentOfNodeToDelete = nodeToDelete;
		if (n < nodeToDelete->m_nData)
			nodeToDelete = nodeToDelete->m_pLChild;
		else
			nodeToDelete = nodeToDelete->m_pRChild;
	} while (nodeToDelete);

	if (nodeToDelete) {
		cNode* newNode = nullptr;
		if (nodeToDelete->m_pLChild && nodeToDelete->m_pRChild) {
			//������ ��尡 �������� �ڽ��� ������ �ִ� ���
			cNode * parentOfReplacingNode = nodeToDelete;
			cNode * replacingNode = nodeToDelete->m_pRChild;//�����ʿ��� ��ü�� ��带 ã�´�.
			//������ ����� ������ ���� ���� ���� �ڽ��� ã�´�.
			while (replacingNode->m_pLChild) {
				parentOfReplacingNode = replacingNode;
				replacingNode = replacingNode->m_pLChild;
			}

			if (parentOfReplacingNode == nodeToDelete) {
				//�����Ϸ��� ����� �ٷ� ������ �ڽ��� ��ü ����̴�.
				replacingNode->m_pLChild = nodeToDelete->m_pLChild;
				//��ü���� ������ ���� Ʈ���� �״�� ������ ��ü�ȴ�.
			}
			else {
				//��ü ���(��� ���)�� ������ ���� Ʈ���� ��ü ����� �θ� ���ʿ� �����Ѵ�.
				//��ü ���� ���� �ڽ��� ������ ���� �ʱ� ������ ���� �ڽ��� �Ű澲�� �ʾƵ� �ȴ�.
				parentOfReplacingNode->m_pLChild = replacingNode->m_pRChild;
				//�����Ϸ��� ����� �¿� �ڽ��� ��ü ��忡�� �����ش�.
				replacingNode->m_pLChild = nodeToDelete->m_pLChild;
				replacingNode->m_pRChild = nodeToDelete->m_pRChild;
			}
			//��ü ��带 �����Ϸ��� ����� �θ�� �����Ѵ�.
			if (parentOfNodeToDelete) {
				if (parentOfNodeToDelete->m_pLChild == nodeToDelete)
					parentOfNodeToDelete->m_pLChild = replacingNode;
				else
					parentOfNodeToDelete->m_pRChild = replacingNode;
			}
			else {
				//�����Ϸ��� ��尡 ��Ʈ ����� ���
				m_pRoot = replacingNode;
			}
		}
		else if (nodeToDelete->m_pLChild) {
			//�����Ϸ��� ��尡 ���� �ڽĸ� ������ �ִ� ���
			if (parentOfNodeToDelete) {
				if (parentOfNodeToDelete->m_pLChild == nodeToDelete)
					parentOfNodeToDelete->m_pLChild = nodeToDelete->m_pLChild;
				else
					parentOfNodeToDelete->m_pRChild = nodeToDelete->m_pLChild;
			}
			else {
				//�����Ϸ��� ��尡 ��Ʈ ����� ���
				m_pRoot = nodeToDelete->m_pLChild;
			}
		}
		else if (nodeToDelete->m_pRChild) {
			//�����Ϸ��� ��尡 ������ �ڽĸ� ������ �ִ� ���
			if (parentOfNodeToDelete) {
				if (parentOfNodeToDelete->m_pLChild == nodeToDelete)
					parentOfNodeToDelete->m_pLChild = nodeToDelete->m_pRChild;
				else
					parentOfNodeToDelete->m_pRChild = nodeToDelete->m_pRChild;
			}
			else {
				//�����Ϸ��� ��尡 ��Ʈ ����� ���
				m_pRoot = nodeToDelete->m_pRChild;
			}
		}
		else {
			//�����Ϸ��� ��尡 ���� ����� ���
			if (parentOfNodeToDelete) {
				if (parentOfNodeToDelete->m_pLChild == nodeToDelete)
					parentOfNodeToDelete->m_pLChild = nullptr;
				else
					parentOfNodeToDelete->m_pRChild = nullptr;
			}
			else {
				//�����Ϸ��� ��尡 ��Ʈ ����� ���
				m_pRoot = nullptr;
			}
		}
		delete nodeToDelete;
	}
#elif DELETE_IMPL == 1
	if (m_pRoot == nullptr)
		return;
	cNode * nodeToDelete = m_pRoot;
	cNode * parentOfNodeToDelete = nullptr;
	//�����ߵ� ��带 ã�´�.
	do {
		if (nodeToDelete->m_nData == n)
			break;
		parentOfNodeToDelete = nodeToDelete;
		if (n < nodeToDelete->m_nData)
			nodeToDelete = nodeToDelete->m_pLChild;
		else
			nodeToDelete = nodeToDelete->m_pRChild;
	} while (nodeToDelete);

	if (nodeToDelete) {
		int countBefore = Count();
		cNode * parentOfReplacingNode = nodeToDelete;
		cNode * replacingNode = nodeToDelete->m_pRChild;//�����ʿ��� ��ü�� ��带 ã�´�.
		if (replacingNode == nullptr) {
			//�����ʿ� �ڽ� ��尡 �ϳ��� ������
			if (parentOfNodeToDelete) {
				//���� �Ϸ��� ����� �θ� �����Ϸ��� ����� ���� ���� Ʈ���� �����Ѵ�.
				if (parentOfNodeToDelete->m_pLChild == nodeToDelete)
					parentOfNodeToDelete->m_pLChild = nodeToDelete->m_pLChild;
				else
					parentOfNodeToDelete->m_pRChild = nodeToDelete->m_pLChild;
			}
			else {
				//nodeToDelete�� ��Ʈ ���
				//������ �ڽ��� ��Ʈ�� ���� �ø���.
				m_pRoot = m_pRoot->m_pLChild;
			}
		}
		else {
			//�����ʿ��� ���� ���� �ڽ��� ã�´�.
			while (replacingNode->m_pLChild) {
				parentOfReplacingNode = replacingNode;
				replacingNode = replacingNode->m_pLChild;
			}
			//��� ����� ������ ���� Ʈ���� �θ��� ���ʿ� �����Ѵ�.
			//��� ���� ���� �ڽ��� ������ �����ϱ� ������ �Ű澵 �ʿ� ����.
			if (parentOfReplacingNode != nodeToDelete)
				parentOfReplacingNode->m_pLChild = replacingNode->m_pRChild;
			else
				parentOfReplacingNode->m_pRChild = replacingNode->m_pRChild;
			//������� ����� ����/������ �ڽ��� ��ü�Ǵ� ��忡�� �����ش�.
			replacingNode->m_pLChild = nodeToDelete->m_pLChild;
			replacingNode->m_pRChild = nodeToDelete->m_pRChild;

			if (parentOfNodeToDelete) {
				//���� �Ϸ��� ����� �θ� ��ü ���� �����Ų��.
				if (parentOfNodeToDelete->m_pLChild == nodeToDelete)
					parentOfNodeToDelete->m_pLChild = replacingNode;
				else
					parentOfNodeToDelete->m_pRChild = replacingNode;
			}
			else {
				m_pRoot = replacingNode;
			}
		}
		assert(countBefore - 1 == Count());
		delete nodeToDelete;
	}
#elif DELETE_IMPL == 2
	if (m_pRoot) 
		m_pRoot->Delete(&m_pRoot, n);
#endif
}

#define RECURSIVE_PRINT	0
void cBST::Print()
{
#if RECURSIVE_PRINT
	if (m_pRoot)
		m_pRoot->Print();
#else
	if (m_pRoot == nullptr)
		return;

	InorderSearch([](cNode* node) {std::cout << node->m_nData<< std::endl; });
#endif
}

void cBST::InorderSearch(std::function<void(cNode*)> func)
{
	std::stack<cNode*> nodes;
	//Ʈ���� ���� ������ ��� ���ÿ� �����Ѵ�.
	//���ÿ��� pop�Ǵ� �������� �ش� ����� ���� ���� Ʈ���� ��� �湮�Ǿ�� �Ѵ�.
	cNode* current = m_pRoot;
	while (current) {
		nodes.push(current);
		current = current->m_pLChild;
	}
	while (nodes.empty() == false) {
		cNode* top = nodes.top();
		nodes.pop();
		//������ ���� Ʈ���� ��� �湮�����ϱ� ���� ��带 �湮�Ѵ�.
		func(top);
		if (top->m_pRChild) {
			//������ ���� Ʈ���� ������ ���� ���� Ʈ���� ���� ������ ��� ���ÿ� �����Ѵ�.
			cNode* current = top->m_pRChild;
			while (current) {
				nodes.push(current);
				current = current->m_pLChild;
			}
		}
	}
}

void cBST::PostorderSearch(std::function<void(cNode*)> func)
{
	//std::stack<cNode*> nodes;
	//cNode* root = m_pRoot;
	//while (root != nullptr) {
	//	if (root->m_pRChild) {
	//		nodes.push(root->m_pRChild);
	//	}
	//	nodes.push(root);
	//	root = root->m_pLChild;
	//}

	//while (nodes.empty() == false) {
	//	root = nodes.top();
	//	nodes.pop();
	//}
}

#define RECURSIVE_COUNT 0
int cBST::Count()
{
	if (m_pRoot == nullptr)
		return 0;
#if RECURSIVE_COUNT
	int count = 0;
	m_pRoot->Count(count);
	return count;
#else
	int count = 0;
	InorderSearch([&count](cNode*) {++count; });
	return count;
#endif
}

bool cBST::Find(int n)
{
	return FindNode(n) != nullptr;
}

cNode * cBST::FindNode(int n)
{
	if (m_pRoot == nullptr)
		return nullptr;
	cNode* node = m_pRoot;
	do {
		if (node->m_nData == n) {
			return node;
		}
		if (n < node->m_nData)
			node = node->m_pLChild;
		else
			node = node->m_pRChild;
	} while (node);
	return node;
}