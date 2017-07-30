#pragma once
#include <iostream>
#define QUEUE_SIZE 5

template <typename ValueT>
class cCircularQueue
{
private:
	int m_nFront;
	int m_nRear;
	ValueT m_aData[QUEUE_SIZE];

public:
	cCircularQueue(void);
	~cCircularQueue(void);
	void Clear();
	void Enqueue(ValueT val);
	void Dequeue();
	void Print();
};

template<typename ValueT>
cCircularQueue<ValueT>::cCircularQueue(void)
	:m_nFront(0), m_nRear(0)
{
}

template<typename ValueT>
cCircularQueue<ValueT>::~cCircularQueue(void)
{
}

template<typename ValueT>
void cCircularQueue<ValueT>::Clear()
{
	m_nFront = m_nRear = 0;
}

template<typename ValueT>
void cCircularQueue<ValueT>::Enqueue(ValueT val)
{
	int nextRear = (m_nRear + 1) % QUEUE_SIZE;
	if (nextRear == m_nFront) {
		return;
	}
	m_aData[m_nRear] = val;
	m_nRear = nextRear;
}

template<typename ValueT>
void cCircularQueue<ValueT>::Dequeue()
{
	if (m_nFront == m_nRear) {
		return;
	}
	m_nFront = (m_nFront + 1) % QUEUE_SIZE;
}



template<typename ValueT>
void cCircularQueue<ValueT>::Print()
{
	for (int i = m_nFront; i != m_nRear;) {
		std::cout << m_aData[i]<<std::endl;
		if (++i == QUEUE_SIZE) {
			i = 0;
		}
	}
}
