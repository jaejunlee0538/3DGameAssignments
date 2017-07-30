#include <stdio.h>
#include "Calculator.h"
#include <iostream>

int readLine(FILE* file, char*buffer, const char* untilChar, int nUntilChar) {
	char c = fgetc(file);
	int cnt = 0;

	while (true) {
		if (c == EOF)
			goto End_Of_While;
		for (int i = 0; i < nUntilChar; ++i) {
			if (c == untilChar[i])
				goto End_Of_While;
		}
		buffer[cnt++] = c;
		c = fgetc(file);
	}
End_Of_While:
	buffer[cnt] = NULL;
	return cnt;
}

int main() {
	FILE * file = 0;
	fopen_s(&file, "Input.txt", "r");

	char buffer[100] = "";
	int count = 0;

	while (feof(file)==false) {
		readLine(file, buffer, "\n", 1);
		std::vector<tagToken> postFix;
		toPostfixNotation(buffer, postFix);
		std::cout << evaluatePostfix(postFix) << std::endl;
	}

	fclose(file);
	return 0;
}
