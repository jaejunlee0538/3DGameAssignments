#include "Calculator.h"
#include "cStack.h"
#include <assert.h>

bool isNumber(char c) {
	return c == '.' || (c >= '0'&&c <= '9');
}

int getPriority(const tagToken& op) {
	switch (op.token[0]) {
	case '(': case ')':
		return 0;
	case '-':case '+':
		return 1;
	case '/':case '*':
		return 2;
	}
}

void parseToken(const char* equation, std::vector<tagToken>& tokens) {
	char buffer[100];
	int count1 = 0;
	int count2 = 0;
	while (equation[count2]) {
		buffer[count1++] = equation[count2];
		switch (equation[count2]) {
		case '-':
		{
			if (tokens.empty()){
				//ó���� -������ 0�� �տ� ���δ�.
				tokens.push_back(tagToken(OPERAND, "0"));
			}
			else {
				if (tokens.back().type == OPERATOR) {
					if (tokens.back().token[0] == '(') {
						//�ٷ� ���� '('�� ������ 0�� �ִ´�.
						tokens.push_back(tagToken(OPERAND, "0"));
					}
					else if(tokens.back().token[0]!=')'){
						//')'�ڿ� ������ -�� ��ȣ�� ó���Ѵ�.
						break;
					}
					else {
						
					}
				}
			}
			buffer[count1] = 0;
			tokens.push_back(tagToken(OPERATOR, buffer));
			count1 = 0;
		}
		break;
		case '(':
		{
			if (count1 > 1) {
				buffer[1] = 0;
				tokens.push_back(tagToken(OPERATOR, buffer));
				tokens.push_back(tagToken(OPERATOR, "("));
			}
			else {
				buffer[count1] = 0;
				tokens.push_back(tagToken(OPERATOR, buffer));
			}
			count1 = 0;
		}
		break;
		case '+':case '/':case '*':case ')':
		{
			buffer[count1] = 0;
			tokens.push_back(tagToken(OPERATOR, buffer));
			count1 = 0;
		}
		break;
		default:
		{
			if (isNumber(equation[count2 + 1]) == false) {
				//���� ��
				buffer[count1] = 0;
				tokens.push_back(tagToken(OPERAND, buffer));
				count1 = 0;
			}
		}
		break;
		}
		count2++;
	}
}

void toPostfixNotation(const char * equationInfix, std::vector<tagToken>& postfixTokens)
{
	std::vector<tagToken> tokens;
	parseToken(equationInfix, tokens);
	toPostfixNotation(tokens, postfixTokens);
}

void toPostfixNotation(const std::vector<tagToken>& tokensInfix, std::vector<tagToken>& postfix) {
	cStack<tagToken> operators;
	char prevOperator = 0;
	for (int i = 0; i < tokensInfix.size(); ++i) {
		switch (tokensInfix[i].type) {
		case OPERATOR:
		{
			char opCurrent = tokensInfix[i].token[0];
			switch (tokensInfix[i].token[0]) {
			case '(':
				//'('�� ������ Push
				operators.Push(tokensInfix[i]);
				break;
			case ')':
				//'('���� ������ stack���� Pop�ؼ� postfix�� Push�Ѵ�
				while (operators.Top()->m_nValue.token[0] != '(') {
					auto* node = operators.Pop();
					postfix.push_back(node->m_nValue);
					delete node;
				}
				//'('�� ������ stack���� ����
				delete operators.Pop();
				break;
			default:
				while (operators.Empty() == false//������ ������ ��ų�
					&& getPriority(tokensInfix[i]) <= getPriority(operators.Top()->m_nValue))//���� �������� �켱������ ������ ������ Top���� Ŀ����
				{
					auto* node = operators.Pop();
					postfix.push_back(node->m_nValue);
					delete node;
				}
				//���� �����ڸ� ������ ���ÿ� ����
				operators.Push(tokensInfix[i]);
				break;
			}
			//���� �����ڷ� ���� �����ڸ� ����
			prevOperator = tokensInfix[i].token[0];
		}
		break;
		case OPERAND:
		{
			//�ǿ����ڸ� �ٷ� 
			postfix.push_back(tokensInfix[i]);
			prevOperator = 0;
		}
		break;
		}
	}

	while (operators.Empty() == false) {
		auto* node = operators.Pop();
		postfix.push_back(node->m_nValue);
		delete node;
	}
}

double evaluatePostfix(std::vector<tagToken>& posfixTokens)
{
	cStack<double> operands;
	for (int i = 0; i < posfixTokens.size(); ++i) {
		switch (posfixTokens[i].type) {
		case OPERAND:
		{
			operands.Push(atof(posfixTokens[i].token.c_str()));
		}
		break;
		case OPERATOR:
		{
			double op1, op2, result;
			op2 = operands.Top()->m_nValue;
			delete operands.Pop();
			op1 = operands.Top()->m_nValue;
			delete operands.Pop();
			switch (posfixTokens[i].token[0]) {
			case '+':
				result = op1 + op2;
				break;
			case '-':
				result = op1 - op2;
				break;
			case '*':
				result = op1 * op2;
				break;
			case '/':
				result = op1 / op2;
				break;
			}
			operands.Push(result);
		}
		break;
		}
	}
	double result;
	result = operands.Top()->m_nValue;
	delete operands.Pop();
	return result;
}

