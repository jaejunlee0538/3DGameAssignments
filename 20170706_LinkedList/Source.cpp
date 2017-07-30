#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "cLinkedList.h"

enum Commands {
	INSERT,
	DELETE,
	REVERSE,
	PRINT
};

int readLine(FILE* file, char*buffer, char untilChar) {
	char c = fgetc(file);
	int cnt = 0;
	while (c != untilChar && c !=EOF) {
		buffer[cnt++] = c;
		c = fgetc(file);
	}
	buffer[cnt] = NULL;
	return cnt;
}

bool parseCommand(FILE* file, Commands* cmd, int* param) {
	char cmdChar;
	char buffer[100];
	cmdChar = fgetc(file);
	if (cmdChar == EOF)
		return false;
	switch (cmdChar) {
	case 'i':case 'I':
		*cmd = INSERT;
		fgetc(file);
		readLine(file, buffer, '\n');
		*param = atoi(buffer);
		break;
	case 'd':case 'D':
		*cmd = DELETE;
		fgetc(file);
		readLine(file, buffer, '\n');
		*param = atoi(buffer);
		break;
	case 'r': case 'R':
		*cmd = REVERSE;
		readLine(file, buffer, '\n');
		break;
	case 'p':case 'P':
		*cmd = PRINT;
		readLine(file, buffer, '\n');
		break;
	}
	return true;
}

int main() {

	/*
	숙제
	void Insert(int n)
	void Delete(int n)
	void Print()
	void Reverse()
	
	조건!
		-멤버변수 더 이상 추가하지 말것
		-배열 쓰면 안됨!
		-stl쓰지 말것
		-지역번수 추가해도 됨
		-reverse할 때 값을 바꾸지 말고 link를 바꿀것
		-재귀함수는 써도 됨
		-Delete할 때 중복되는 값 있으면 첫번째 애를 지울것
		-모양은 바꾸지 말것
		-멤버함수는 추가해도 됨
		-데이터 삭제할 때 신경 쓸 것

	i:1
	i:3
	d:2
	r
	p
	*/


	FILE * file = fopen("data.txt", "r");
	cLinkedList<int> list;
	while (1) {
		Commands cmd;
		int param;
		bool good = parseCommand(file, &cmd, &param);
		if (good == false)
			break;
		switch (cmd) {
		case INSERT:
			//std::cout << "INSERT : " << param << std::endl;
			list.Insert(param);
			break;
		case DELETE:
			list.Delete(param);
			//std::cout << "DELETE : " << param << std::endl;
			break;
		case REVERSE:
			//std::cout << "REVERSE : " << std::endl;
			list.Reverse();
			break;
		case PRINT:
			//std::cout << "PRINT : " << std::endl;
			list.Print();
			break;
		}
	}
	fclose(file);
	return 0;
}

bool readFile(FILE* fileName, char* arr, int* dataNum)
{
	char text;
	int num = 0;
	text = fgetc(fileName);
	while (text != '\n' && text != 'EOF')
	{

		text = fgetc(fileName);

		arr[num] = text;
		num += 1;

	}
	arr[num] = NULL;
	*dataNum = atoi(arr);
	return 1;
}