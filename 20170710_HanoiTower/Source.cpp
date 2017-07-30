#include <iostream>
/*
�ϳ��� Ÿ��
- �ѹ��� �ϳ��� ���ݸ� �̵������ϴ�
- ���� ���� ���� ū ������ �ö� �� ����

���α׷� ���۵Ǹ� ������ ������ �Է¹޴´�
��� ����
1 : A -> C
2 : B -> A
....
*/

int actionCount = 0;

void hanoiTower(int from, int to, int N) {
	if (N == 1) {
		std::cout << "(" << ++actionCount << " ��°) ";
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
	std::cout << "(" << ++actionCount << " ��°) ";
	std::cout <<" "<< N << " : " << static_cast<char>('A' + from) << " -> " << static_cast<char>('A' + to) << std::endl;
	hanoiTower(middle, to, N - 1);
}

int main() {
	int nPlates;
	while (true) {
		std::cout << "������ ������ �Է��ϼ���(���� : -1) : ";
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