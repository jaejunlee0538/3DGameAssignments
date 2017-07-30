#pragma once
#include <functional>
class cNode;

class cBST
{
private:
	cNode* m_pRoot;
public:
	cBST(void);
	~cBST(void);

	void Clear();
	void Insert(int n);
	void Delete(int n);
	void Print();
	void InorderSearch(std::function<void(cNode*)> func);
	void PostorderSearch(std::function<void(cNode*)> func);
	int Count();
	bool Find(int n);
private:
	cNode* FindNode(int n);
};