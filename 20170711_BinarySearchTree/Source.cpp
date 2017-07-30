#include "stdafx.h"
#include "cBST.h"
#include <iostream>
#include <assert.h>

std::set<cNode*> g_setPool;

int _tmain(int argc, _TCHAR* argv[])
{
	cBST bst;


	int arr[100];

	for (int k = 0; k < 100; ++k)
	{
		for (int i = 0; i < 100; ++i)
		{
			arr[i] = rand() % 100;
			bst.Insert(arr[i]);
		}

		for (int i = 0; i < 1000; ++i)
		{
			int n1 = rand() % 100;
			int n2 = rand() % 100;
			int n = arr[n1];
			arr[n1] = arr[n2];
			arr[n2] = n;
		}

		for (int i = 0; i < 100; ++i)
		{
			bst.Delete(arr[i]);
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		arr[i] = rand() % 10;
		bst.Insert(arr[i]);
	}
	bst.Print();
	for (int i = 0; i < 10; ++i)
	{
		bst.Delete(arr[i]);
	}

	assert(g_setPool.empty() && "삭제되지 않은 노드가 있음.");

	return 0;
}

//#include "cBST.h"
//#include <random>
//#include <iostream>
//#include <vector>
//#include <time.h>
//#include <algorithm>
//#include <assert.h>
//#include <crtdbg.h>
//
//void test1();
//void test2();
//int main() {
//	srand(time(0));
//	for(int i=0;i<500;++i)
//		test1();
//	for (int i = 0; i<500; ++i)
//		test2();
//	_CrtDumpMemoryLeaks();
//	return 0;
//}
//
//void test1() {
//	cBST bst;
//	std::vector<int> samples;
//	int nSamples = rand() % 500 + 200;
//	for (int i = 0; i < nSamples; ++i) {
//		int val = rand() % 1000 - 500;
//		if (bst.Find(val) == false) {
//			samples.push_back(val);
//			bst.Insert(val);
//		}
//	}
//	int count1, count2;
//	count1 = bst.Count();
//	std::cout << "삭제 전 : " << count1 << std::endl;	
//	std::random_shuffle(samples.begin(), samples.end());
//
//	int nDelete = rand() % 600 + 100;
//	nDelete %= samples.size() + 1;
//	for (int i = 0; i < nDelete; ++i) {
//		bst.Delete(samples[i]);
//	}
//	count2 = bst.Count();
//	std::cout << nDelete << " 개 삭제" << std::endl;
//	std::cout << "삭제 후 : " << count2 << std::endl;
//	std::cout << "-------------" << std::endl;
//	assert(count1 == count2 + nDelete);
//}
//
//void test2()
//{
//	cBST bst;
//	int nSamples = rand() % 500 + 200;
//	for (int i = 0; i < nSamples; ++i) {
//		int val = rand() % 1000 - 500;
//		if (bst.Find(val) == false) {
//			bst.Insert(val);
//		}
//	}
//	for (int i = 0; i < nSamples; ++i) {
//		int val = rand() % 1000 - 500;
//		bst.Delete(val);
//	}
//}
