#pragma once
#include <algorithm>
#include <vector>
template <typename T, typename Comp = std::less<T>>
class UpdatableHeap
{
public:
	typedef int32_t ElementHandle;
private:
	struct HeapNode {
		HeapNode(ElementHandle h, T v)
			:handle(h), val(v) {
		}
		ElementHandle handle;
		T val;
	};

	Comp	m_comp;
	std::vector<HeapNode> m_tree;
	std::vector<int> m_handleToPos;
	ElementHandle m_handleCount;

public:
	UpdatableHeap()
	{
		m_handleCount = 0;
	}

	~UpdatableHeap()
	{
	}

	ElementHandle Push(T val) {
		ElementHandle handle = m_handleCount++;
		m_handleToPos.push_back(m_tree.size());
		m_tree.emplace_back(handle, val);
		AlignUpward(m_tree.size() - 1);
		return handle;
	}

	void Pop() {
		m_tree[0] = m_tree.back();
		m_tree.pop_back();
		AlignDownward(0);
	}

	T Top()
	{
		assert(m_tree.empty() == false);
		return m_tree[0].val;
	}

	bool IsEmpty() const {
		return m_tree.empty();
	}

	void UpdateValue(ElementHandle eleHandle, T newVal)
	{
		int elePos = m_handleToPos[eleHandle];
		T prevVal = m_tree[elePos].val;
		m_tree[elePos].val = newVal;
		if (m_comp(newVal, prevVal))
			AlignUpward(elePos);
		else if (m_comp(prevVal, newVal))
			AlignDownward(elePos);
		//else 같은 경우
	}
private:
	//Swap후 child가 옮겨간 기존 부모의 위치를 반환한다.
	int SwapWithParent(int childPos) {
		int parent = GetParentIndex(childPos);

		HeapNode tmp = m_tree[childPos];
		m_tree[childPos] = m_tree[parent]; //parent가 child가 되고
		m_tree[parent] = tmp;//child가 parent가 된다.
							 //핸들 업데이트
		m_handleToPos[m_tree[childPos].handle] = childPos;
		m_handleToPos[m_tree[parent].handle] = parent;
		return parent;
	}

	void AlignUpward(int child) {
		if (child == 0)
			return;
		int parent = GetParentIndex(child);
		while (parent >= 0 && m_comp(m_tree[child].val, m_tree[parent].val)) {
			SwapWithParent(child);
			child = parent;
			parent = GetParentIndex(parent);
		}
	}

	void AlignDownward(int elePos)
	{
		if (!HasChild(elePos))
			return;
		int parent = elePos;
		int child = GetSmallestChildIdx(parent);
		while (m_comp(m_tree[child].val, m_tree[parent].val)) {
			SwapWithParent(child);
			parent = child;
			if (!HasChild(parent))
				return;
			child = GetSmallestChildIdx(parent);
		}
	}

	int GetLeftChildIdx(int parent) {
		return parent * 2 + 1;
	}
	int GetRightChildIdx(int parent) {
		return parent * 2 + 2;
	}
	int GetParentIndex(int child) {
		return (child - 1) / 2;
	}

	bool HasChild(int parentPos) {
		return m_tree.size() >= GetLeftChildIdx(parentPos) + 1;
	}

	int GetSmallestChildIdx(int parentPos) {
		assert(HasChild(parentPos));
		int left = GetLeftChildIdx(parentPos);
		int right = GetRightChildIdx(parentPos);

		if (right >= m_tree.size()) {
			//child가 하나인 경우
			return left;
		}

		if (m_comp(m_tree[left].val, m_tree[right].val))
			return left;
		return right;
	}
};

