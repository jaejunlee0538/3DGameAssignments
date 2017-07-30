#include "cLinkedListQueue.h"

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
	cLinkedListQueue<int> listQ;
	char buffer[1024];

	FILE * file;
	fopen_s(&file, "Input.txt", "r");

	while (feof(file) == false) {
		readLine(file, buffer, ":\n", 2);
		if (strcmp(buffer, "enqueue") == 0) {
			readLine(file, buffer, "\n", 1);
			listQ.Enqueue(atoi(buffer));
			//std::cout << "Enque : " << atoi(buffer) << std::endl;
		}
		else if (strcmp(buffer, "dequeue") == 0) {
			auto* node = listQ.Dequeue();
			if (node) {
				delete node;
			}
			//std::cout << "Dequeue" << std::endl;
		}
		else if (strcmp(buffer, "print") == 0) {
			listQ.Print();
		}
	}
	fclose(file);
	return 0;
}
