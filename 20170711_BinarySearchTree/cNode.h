#pragma once

class cNode
{
public:
	cNode(int data);

	~cNode(void);

	void Insert(int val);

	void Print();

	void Count(int& count);

	void Destroy();

	void Delete(cNode** parentLink, int n);
private:
	cNode* SplitLeftMostNode(cNode** parentLink);
public:
	int		m_nData;
	cNode*	m_pLChild;
	cNode*	m_pRChild;
};

