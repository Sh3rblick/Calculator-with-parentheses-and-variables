#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <vector>

enum TokenType {
	NUMBER,
	OPERATOR,
	OPEN_PAREN,
	CLOSE_PAREN,
	VARIABLE,
	LITERAL,
	ASSIGNMENT
   };

enum Priority {
	P=4,
	E=3,
	MD=2,
	AS=1,
	DEFAULT=0
};

struct Token {
	TokenType type;
	std::string value;
	Priority priority=DEFAULT;
};

struct Node {
	Token token;
	Node* left{};
	Node* right{};
};

std::vector<Token> tokenizer(const std::string& exp);
Node *parser(const std::vector<Token>& tokens);
double evaluate(Node* root);

#endif //MAIN_H