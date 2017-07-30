#include "cPooledQueue.h"
#include <Windows.h>
#include <cLinkedListQueue.h>
class TickCounter {
public:
	TickCounter() {
		start();
	}

	void start() {
		_t0 = GetTickCount64();
	}

	void printElapsedTime(const char* msg) {
		std::cout << "[" << msg << "] " << GetTickCount64() - _t0 << " ms" << std::endl;
	}
private:
	ULONGLONG _t0;
};

struct tagBigData {
	tagBigData() 
	:dummy(""){

	}
	char dummy[102400];
};

void testPooledQueue();
void testPerformance();


int main() {
	testPerformance();
	return 0;
}


void testPooledQueue() {
	cPooledQueue<int> PQ;
	for (int i = 0; i < 10; ++i) {
		PQ.Enqueue(i);
	}

	PQ.Print();
	std::cout << "-----------------------------------" << std::endl;

	cPooledQueue<int>::NodeType* node;
	node = PQ.Dequeue(); PQ.Release(node);
	node = PQ.Dequeue(); PQ.Release(node);
	node = PQ.Dequeue(); PQ.Release(node);
	node = PQ.Dequeue(); PQ.Release(node);

	PQ.Print();
	std::cout << "-----------------------------------" << std::endl;

	PQ.Enqueue(5);
	PQ.Enqueue(6);
	PQ.Enqueue(7);
	PQ.Enqueue(8);

	PQ.Print();
	std::cout << "-----------------------------------" << std::endl;

	PQ.Enqueue(9);

	PQ.Print();
	std::cout << "-----------------------------------" << std::endl;
}

void testPerformance(){
	cLinkedListQueue<tagBigData> listQ;
	cPooledQueue<tagBigData> pooledQ;
	TickCounter counter;
	tagBigData dummyData;
	const int NTest = 5000;
	std::cout << "-----------Enqueue---------------" << std::endl;
	counter.start();
	for (int i = 0; i < NTest; ++i) {
		listQ.Enqueue(dummyData);
	}
	counter.printElapsedTime("���Ḯ��Ʈ ť");

	counter.start();
	for (int i = 0; i < NTest; ++i) {
		pooledQ.Enqueue(dummyData);
	}
	counter.printElapsedTime("�޸�Ǯ ť");

	std::cout << "-----------Dequeue---------------" << std::endl;
	counter.start();
	for (int i = 0; i < NTest; ++i) {
		auto* node = listQ.Dequeue();
		if (node) {
			delete node;
		}
	}
	counter.printElapsedTime("���Ḯ��Ʈ ť");

	counter.start();
	for (int i = 0; i < NTest; ++i) {
		auto* node = pooledQ.Dequeue();
		pooledQ.Release(node);
	}
	counter.printElapsedTime("�޸�Ǯ ť");

	std::cout << "-----------Enqueue �ٽ�---------------" << std::endl;
	counter.start();
	for (int i = 0; i < NTest; ++i) {
		listQ.Enqueue(dummyData);
	}
	counter.printElapsedTime("���Ḯ��Ʈ ť");

	counter.start();
	for (int i = 0; i < NTest; ++i) {
		pooledQ.Enqueue(dummyData);
	}
	counter.printElapsedTime("�޸�Ǯ ť");

	std::cout << "-----------Dequeue �ٽ�---------------" << std::endl;
	counter.start();
	for (int i = 0; i < NTest; ++i) {
		auto* node = listQ.Dequeue();
		if (node) {
			delete node;
		}
	}
	counter.printElapsedTime("���Ḯ��Ʈ ť");

	counter.start();
	for (int i = 0; i < NTest; ++i) {
		auto* node = pooledQ.Dequeue();
		pooledQ.Release(node);
	}
	counter.printElapsedTime("�޸�Ǯ ť");
}
