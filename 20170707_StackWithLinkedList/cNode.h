#pragma once
template <typename ValueT>
class cNode
{
public:
	cNode(ValueT value = ValueT());
	~cNode(void);

	ValueT	m_nValue;
	cNode*	m_pNext;
};


template<typename ValueT>
cNode<ValueT>::cNode(ValueT value)
	:m_pNext(nullptr), m_nValue(value)
{
}

template<typename ValueT>
cNode<ValueT>::~cNode(void)
{
}
