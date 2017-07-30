// 20170712_MyMatrix.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Matrix.h"

void printMatrix(const cMatrix& m) {
	m.Print();
	std::cout << "-------------------------" << std::endl;
}

void code1();

void inverseTest();

int main()
{
	code1();
	
	return 0;
}

void inverseTest() {
	int N = 3;
	for (int dim = N; dim < 15; ++dim) {
		cMatrix m;
		m.init(dim);
		for (int row = 0; row < dim; row++) {
			for (int col = 0; col < dim; col++) {
				m(row, col) = rand() % 10 - 10;
			}
		}
		//std::cout << "Det = " << m.GetDeterminant() << std::endl;
		printMatrix(m * m.Inverse());
	}
}


int readLine(FILE* file, char* buffer, const char* delim) {
	int cnt = 0;
	char c = fgetc(file);
	bool delimFound = false;
	while (c != EOF) {
		if (strchr(delim, c) == nullptr) {
			if (delimFound) {
				fseek(file, -1, SEEK_CUR);//파일 포인터를 1칸 뒤로 이동시킨다
				goto STOP_PARSE;
			}
		}
		else {
			if (delimFound == false) {
				delimFound = true;
			}
		}
		if (delimFound == false) {
			buffer[cnt++] = c;
		}
		c = fgetc(file);
	}
STOP_PARSE:
	buffer[cnt] = 0;
	return cnt;
}

void calculate(char op, char m1, char m2, cMatrix& matA, cMatrix& matB) {
	cMatrix* mat1, *mat2;
	
	mat1 = m1 == 'A' ? &matA : &matB;
	mat2 = m2 == 'A' ? &matA : &matB;
	switch (op) {
	case '+':
		printMatrix(*mat1 + *mat2);
		break;
	case '-':
		printMatrix(*mat1 - *mat2);
		break;
	case '*':
		printMatrix(*mat1 * *mat2);
		break;
	}
}

void code1() {
	FILE * file = nullptr;
	fopen_s(&file, "matrix.txt", "r");
	char buffer[1024];

	std::vector<float> data;

	int dim;
	cMatrix matA, matB;
	readLine(file, buffer, "\n");
	dim = atoi(buffer);

	matA.init(dim);
	matB.init(dim);
	
	for (int row = 0; row < dim; ++row) {
		for (int col = 0; col < dim; ++col) {
			readLine(file, buffer, " \n");
			matA(row, col) = atof(buffer);
		}
	}
	for (int row = 0; row < dim; ++row) {
		for (int col = 0; col < dim; ++col) {
			readLine(file, buffer, " \n");
			matB(row, col) = atof(buffer);
		}
	}

	while (feof(file) == false) {
		readLine(file, buffer, ": ");
		if (strcmp(buffer, "plus") == 0) {
			char op1, op2;
			readLine(file, buffer, ", "); op1 = buffer[0];
			readLine(file, buffer, "\n "); op2 = buffer[0];
			calculate('+', op1, op2, matA, matB);
		}
		else if (strcmp(buffer, "minus") == 0) {
			char op1, op2;
			readLine(file, buffer, ", "); op1 = buffer[0];
			readLine(file, buffer, " \n"); op2 = buffer[0];
			calculate('-', op1, op2, matA, matB);
		}
		else if (strcmp(buffer, "scalar") == 0) {
			char op1;
			float scalar;
			readLine(file, buffer, ", ");
			scalar = atof(buffer);
			readLine(file, buffer, " \n");
			op1 = buffer[0];
			if (op1 == 'A') {
				printMatrix(matA * scalar);
			}
			else {
				printMatrix(matB * scalar);
			}
		}
		else if (strcmp(buffer, "mul") == 0) {
			char op1, op2;
			readLine(file, buffer, ",");
			op1 = buffer[0];
			readLine(file, buffer, " \n");
			op2 = buffer[0];
			calculate('*', op1, op2, matA, matB);
		}
		else if (strcmp(buffer, "inverse") == 0) {
			char op1;
			readLine(file, buffer, " \n");
			op1 = buffer[0];
			if (op1 == 'A') {
				printMatrix(matA.Inverse());
				std::cout << "A*A_inv = " << std::endl;
				printMatrix(matA*matA.Inverse());
			}
			else {
				printMatrix(matB.Inverse());
				std::cout << "B*B_inv = " << std::endl;
				printMatrix(matB*matB.Inverse());
			}

		}
		else if (strcmp(buffer, "transpose") == 0) {
			char op1;
			readLine(file, buffer, " \n");
			op1 = buffer[0];
			if (op1 == 'A') {
				printMatrix(matA.Transpose());
			}
			else {
				printMatrix(matB.Transpose());
			}
		}
		else if (strcmp(buffer, "equal") == 0) {
			char op1, op2;
			readLine(file, buffer, ",");
			op1 = buffer[0];
			readLine(file, buffer, " \n");
			op2 = buffer[0];
			if (op1 == 'A') {
				std::cout << ((matA == matB) ? "true" : "false") << std::endl;
			}
			else {
				std::cout << ((matB == matA) ? "true" : "false") << std::endl;
			}
		}
		else if (strcmp(buffer, "nequal") == 0) {
			char op1, op2;
			readLine(file, buffer, ",");
			op1 = buffer[0];
			readLine(file, buffer, "\n");
			op2 = buffer[0];
			if (op1 == 'A') {
				std::cout << ((matA != matB) ? "true" : "false") << std::endl;
			}
			else {
				std::cout << ((matB != matA) ? "true" : "false") << std::endl;
			}
		}
	}
}

void test1() {

	cMatrix m1, m2;
	float data[] = { 8,2,3,4,5,6,7,8,9 };
	m1.init(3, 0);
	m1.SetDataFrom(data);
	m2 = cMatrix::Identity(3);

	printMatrix(cMatrix::Identity(5));
	printMatrix(m1);
	printMatrix(m2);
	printMatrix(m1 + m2);
	printMatrix(m1 - m2);
	printMatrix(m1 * m2);
	printMatrix(m2 * m1);
	printMatrix(m1 * 5);
	printMatrix(m1.Transpose());
	printMatrix(m1.GetPartialMatrix(2, 2));
	printMatrix(m1.GetPartialMatrix(1, 2));
	printMatrix(m1.GetPartialMatrix(0, 2));
	std::cout << "행렬식 : " << m1.GetDeterminant() << std::endl;
	printMatrix(m1.Inverse() * m1);
}
