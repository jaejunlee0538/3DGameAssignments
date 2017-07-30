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
	//지워야될 노드를 찾는다.
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
			//삭제할 노드가 양쪽으로 자식을 가지고 있는 경우
			cNode * parentOfReplacingNode = nodeToDelete;
			cNode * replacingNode = nodeToDelete->m_pRChild;//오른쪽에서 교체할 노드를 찾는다.
			//삭제할 노드의 오른쪽 편에서 가장 왼쪽 자식을 찾는다.
			while (replacingNode->m_pLChild) {
				parentOfReplacingNode = replacingNode;
				replacingNode = replacingNode->m_pLChild;
			}

			if (parentOfReplacingNode == nodeToDelete) {
				//삭제하려는 노드의 바로 오른쪽 자식이 교체 노드이다.
				replacingNode->m_pLChild = nodeToDelete->m_pLChild;
				//교체노드는 오른쪽 서브 트리를 그대로 가지고 교체된다.
			}
			else {
				//교체 노드(떼어낸 노드)의 오른쪽 서브 트리를 교체 노드의 부모 왼쪽에 연결한다.
				//교체 노드는 왼쪽 자식을 가지고 있지 않기 때문에 왼쪽 자식은 신경쓰지 않아도 된다.
				parentOfReplacingNode->m_pLChild = replacingNode->m_pRChild;
				//삭제하려는 노드의 좌우 자식을 교체 노드에게 물려준다.
				replacingNode->m_pLChild = nodeToDelete->m_pLChild;
				replacingNode->m_pRChild = nodeToDelete->m_pRChild;
			}
			//교체 노드를 삭제하려는 노드의 부모와 연결한다.
			if (parentOfNodeToDelete) {
				if (parentOfNodeToDelete->m_pLChild == nodeToDelete)
					parentOfNodeToDelete->m_pLChild = replacingNode;
				else
					parentOfNodeToDelete->m_pRChild = replacingNode;
			}
			else {
				//삭제하려는 노드가 루트 노드인 경우
				m_pRoot = replacingNode;
			}
		}
		else if (nodeToDelete->m_pLChild) {
			//삭제하려는 노드가 왼쪽 자식만 가지고 있는 경우
			if (parentOfNodeToDelete) {
				if (parentOfNodeToDelete->m_pLChild == nodeToDelete)
					parentOfNodeToDelete->m_pLChild = nodeToDelete->m_pLChild;
				else
					parentOfNodeToDelete->m_pRChild = nodeToDelete->m_pLChild;
			}
			else {
				//삭제하려는 노드가 루트 노드인 경우
				m_pRoot = nodeToDelete->m_pLChild;
			}
		}
		else if (nodeToDelete->m_pRChild) {
			//삭제하려는 노드가 오른쪽 자식만 가지고 있는 경우
			if (parentOfNodeToDelete) {
				if (parentOfNodeToDelete->m_pLChild == nodeToDelete)
					parentOfNodeToDelete->m_pLChild = nodeToDelete->m_pRChild;
				else
					parentOfNodeToDelete->m_pRChild = nodeToDelete->m_pRChild;
			}
			else {
				//삭제하려는 노드가 루트 노드인 경우
				m_pRoot = nodeToDelete->m_pRChild;
			}
		}
		else {
			//삭제하려는 노드가 리프 노드인 경우
			if (parentOfNodeToDelete) {
				if (parentOfNodeToDelete->m_pLChild == nodeToDelete)
					parentOfNodeToDelete->m_pLChild = nullptr;
				else
					parentOfNodeToDelete->m_pRChild = nullptr;
			}
			else {
				//삭제하려는 노드가 루트 노드인 경우
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
	//지워야될 노드를 찾는다.
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
		cNode * replacingNode = nodeToDelete->m_pRChild;//오른쪽에서 교체할 노드를 찾는다.
		if (replacingNode == nullptr) {
			//오른쪽에 자식 노드가 하나도 없으면
			if (parentOfNodeToDelete) {
				//삭제 하려는 노드의 부모에 삭제하려는 노드의 왼쪽 서브 트리를 연결한다.
				if (parentOfNodeToDelete->m_pLChild == nodeToDelete)
					parentOfNodeToDelete->m_pLChild = nodeToDelete->m_pLChild;
				else
					parentOfNodeToDelete->m_pRChild = nodeToDelete->m_pLChild;
			}
			else {
				//nodeToDelete가 루트 노드
				//왼쪽의 자식을 루트로 끌어 올린다.
				m_pRoot = m_pRoot->m_pLChild;
			}
		}
		else {
			//오른쪽에서 가장 왼쪽 자식을 찾는다.
			while (replacingNode->m_pLChild) {
				parentOfReplacingNode = replacingNode;
				replacingNode = replacingNode->m_pLChild;
			}
			//떼어낸 노드의 오른쪽 서브 트리를 부모의 왼쪽에 연결한다.
			//떼어낸 노드는 왼쪽 자식을 가지지 않으니까 왼쪽은 신경쓸 필요 없다.
			if (parentOfReplacingNode != nodeToDelete)
				parentOfReplacingNode->m_pLChild = replacingNode->m_pRChild;
			else
				parentOfReplacingNode->m_pRChild = replacingNode->m_pRChild;
			//지우려는 노드의 왼쪽/오른쪽 자식을 교체되는 노드에게 물려준다.
			replacingNode->m_pLChild = nodeToDelete->m_pLChild;
			replacingNode->m_pRChild = nodeToDelete->m_pRChild;

			if (parentOfNodeToDelete) {
				//삭제 하려는 노드의 부모를 교체 노드와 연결시킨다.
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
	//트리의 좌측 노드들을 모두 스택에 삽입한다.
	//스택에서 pop되는 시점에서 해당 노드의 좌측 서브 트리는 모두 방문되어야 한다.
	cNode* current = m_pRoot;
	while (current) {
		nodes.push(current);
		current = current->m_pLChild;
	}
	while (nodes.empty() == false) {
		cNode* top = nodes.top();
		nodes.pop();
		//좌측의 서브 트리를 모두 방문헀으니까 현재 노드를 방문한다.
		func(top);
		if (top->m_pRChild) {
			//우측에 서브 트리가 있으면 우측 서브 트리의 좌측 노드들을 모두 스택에 삽입한다.
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