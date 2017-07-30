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
	����
	void Insert(int n)
	void Delete(int n)
	void Print()
	void Reverse()
	
	����!
		-������� �� �̻� �߰����� ����
		-�迭 ���� �ȵ�!
		-stl���� ����
		-�������� �߰��ص� ��
		-reverse�� �� ���� �ٲ��� ���� link�� �ٲܰ�
		-����Լ��� �ᵵ ��
		-Delete�� �� �ߺ��Ǵ� �� ������ ù��° �ָ� �����
		-����� �ٲ��� ����
		-����Լ��� �߰��ص� ��
		-������ ������ �� �Ű� �� ��

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