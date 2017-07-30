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

	//인터뷰 개수 만큼 벡터 개수 할당
	int nInterview;
	iss >>nInterview;
	interview.resize(nInterview);
	for (int i = 0; i < nInterview; ++i) {
		std::getline(file, line);
		std::istringstream iss(line);
		
		//인터뷰 정보 파싱
		int id; //누구의 인터뷰인지
		int nPeople; //인터뷰 내용에 몇명이 포함되어 있는지
		iss >> id >> nPeople;
		interview[id-1].resize(nPeople);//인터뷰 내용에 포함된 인원 수 만큼 벡터 크기 할당 
		
		//인터뷰 내용 파싱
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

	std::cout << fileName << " " << count << " 명" << std::endl;
}

int main() {
	countEarthian("마을1.txt");
	countEarthian("마을2.txt");
	return 0;
}

/*
- 마을에 대한 정보
- 외계인과 지구인이 같이 살고 있다
- 지구인 마을의 주민 수는 몇명인가?
첫번째 숫자는 마을의 주민 수
두번재 줄부터는 인터뷰 결과
당신이 알고 있는 주민 중 인간인 주민은 몇명이고 누구입니까?

외계인은 거짓말을 할 수도 있다
지구인은 거짓말을 하지 않는다.
1번 주민은 지구인이다.
*/