#pragma once
#include <vector>
#include <string>
enum TokenTypes {
	OPERAND,
	OPERATOR
};

struct tagToken {
	tagToken(TokenTypes type, std::string token) {
		this->type = type;
		this->token = token;
	}

	TokenTypes type;
	std::string token;
};

bool isNumber(char c);
void parseToken(const char* equation, std::vector<tagToken>& tokens);
void toPostfixNotation(const char* equationInfix, std::vector<tagToken>& postfixTokens);
void toPostfixNotation(const std::vector<tagToken>& tokensInfix, std::vector<tagToken>& posfix);
double evaluatePostfix(std::vector<tagToken>& posfixTokens);