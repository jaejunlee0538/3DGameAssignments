#include <stdio.h>
#include <stack>
#include <queue>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>

void countEarthian(const char* fileName) {
	std::vector<std::vector<int>> interview;
	std::ifstream file(fileName);
	assert(file.good());
	std::string line;
	std::getline(file, line);
	std::istringstream iss(line);

	//���ͺ� ���� ��ŭ ���� ���� �Ҵ�
	int nInterview;
	iss >>nInterview;
	interview.resize(nInterview);
	for (int i = 0; i < nInterview; ++i) {
		std::getline(file, line);
		std::istringstream iss(line);
		
		//���ͺ� ���� �Ľ�
		int id; //������ ���ͺ�����
		int nPeople; //���ͺ� ���뿡 ����� ���ԵǾ� �ִ���
		iss >> id >> nPeople;
		interview[id-1].resize(nPeople);//���ͺ� ���뿡 ���Ե� �ο� �� ��ŭ ���� ũ�� �Ҵ� 
		
		//���ͺ� ���� �Ľ�
		for (int k = 0; k < nPeople; ++k) {
			iss >> interview[id-1][k];
		}
	}

	int count = 0;
	std::vector<bool> checked(nInterview, false);

	std::stack<int> st;
	st.push(1);
	checked[0] = true;
	while (st.empty() == false) {
		int id = st.top();
		st.pop();
		count++;
		for (int i = 0; i < interview[id - 1].size(); ++i) {
			int idToCheck = interview[id - 1][i];
			if (checked[idToCheck - 1] == false) {
				st.push(idToCheck);
				checked[idToCheck - 1] = true;
			}
		}
	}

	std::cout << fileName << " " << count << " ��" << std::endl;
}

int main() {
	countEarthian("����1.txt");
	countEarthian("����2.txt");
	return 0;
}

/*
- ������ ���� ����
- �ܰ��ΰ� �������� ���� ��� �ִ�
- ������ ������ �ֹ� ���� ����ΰ�?
ù��° ���ڴ� ������ �ֹ� ��
�ι��� �ٺ��ʹ� ���ͺ� ���
����� �˰� �ִ� �ֹ� �� �ΰ��� �ֹ��� ����̰� �����Դϱ�?

�ܰ����� �������� �� ���� �ִ�
�������� �������� ���� �ʴ´�.
1�� �ֹ��� �������̴�.
*/