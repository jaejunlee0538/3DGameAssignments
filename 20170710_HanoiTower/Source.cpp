#include <iostream>
/*
하노이 타워
- 한번에 하나의 원반만 이동가능하다
- 작은 원반 위에 큰 원반이 올라갈 수 없다

프로그램 시작되면 원반의 개수를 입력받는다
출력 내용
1 : A -> C
2 : B -> A
....
*/

int actionCount = 0;

void hanoiTower(int from, int to, int N) {
	if (N == 1) {
		std::cout << "(" << ++actionCount << " 번째) ";
		std::cout << " " << N << " : " << static_cast<char>('A' + from) << " -> " << static_cast<char>('A' + to) << std::endl;
		return;
	}
	int middle;
	switch (from + to) {
	case 1: 
		middle = 2; 
		break;
	case 2: 
		middle = 1; 
		break;
	case 3: 
		middle = 0; 
		break;
	}
	hanoiTower(from, middle, N - 1);
	std::cout << "(" << ++actionCount << " 번째) ";
	std::cout <<" "<< N << " : " << static_cast<char>('A' + from) << " -> " << static_cast<char>('A' + to) << std::endl;
	hanoiTower(middle, to, N - 1);
}

int main() {
	int nPlates;
	while (true) {
		std::cout << "원반의 개수를 입력하세요(종료 : -1) : ";
		std::cin >> nPlates;
		if (nPlates <= 0) {
			break;
		}
		actionCount = 0;
		hanoiTower(0, 2, nPlates);
		std::cout << "------------------------------" << std::endl;
	}
	
	return 0;
}