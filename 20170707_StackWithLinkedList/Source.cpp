#include "cStack.h"
#include <stdio.h>

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
	cStack<int> stack;
	char buffer[1024];

	FILE * file;
	fopen_s(&file, "Input.txt", "r");

	while (feof(file) == false) {
		readLine(file, buffer, ":\n", 2);
		if (strcmp(buffer, "push") == 0) {
			readLine(file, buffer, "\n", 1);
			stack.Push(atoi(buffer));
		}
		else if (strcmp(buffer, "pop") == 0) {
			auto* node = stack.Pop();
			if (node) {
				delete node;
			}
		}
		else if (strcmp(buffer, "print") == 0) {
			stack.Print();
		}
	}
	fclose(file);
	return 0;
}
