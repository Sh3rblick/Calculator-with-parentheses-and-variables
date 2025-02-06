#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
#include <unordered_map>
#include <cmath>
#include <gtest/gtest.h>
#include "main.h"

using namespace std;

// Vracia pointer na prvy znak, ktory nie je medzera
int skip_spaces(const string& exp, int pointer) {
	while (pointer < exp.size() && exp[pointer] == ' ') {
		pointer++;
	}
	return pointer;
}

// Mapa premennych
unordered_map<string, string> variables;

// Vracia vektor tokenov
vector<Token> tokenizer(const string& exp) {
	vector<Token> tokens;
	int bg_pointer = 0;
	int current_pointer = 0;

	// Ak sa nachadza '=' v exp, tak pridame do vektoru tokenov premennu a operator '=' (TokenType::ASSIGNMENT)
	if (exp.find('=') != string::npos) {
		Token token;
		token.type = VARIABLE;
		bg_pointer = skip_spaces(exp, current_pointer);
		current_pointer = bg_pointer;
		if (exp.at(bg_pointer) == '0') {
			throw std::runtime_error("Invalid variable name");
		}
		while (current_pointer < exp.size()
			&& exp.at(current_pointer) != ' ') {
			if (exp.at(current_pointer) != '_'
				&& !isalnum(exp.at(current_pointer))) {
				throw std::runtime_error("Invalid variable name");
				}
			current_pointer++;
			}
		token.value = exp.substr(bg_pointer, current_pointer - bg_pointer);
		if (token.value == "e") {
			throw std::runtime_error("Character 'e' is reserved");
		}
		tokens.push_back(token);

		current_pointer = skip_spaces(exp, current_pointer);
		token.type = ASSIGNMENT;
		token.value = '=';
		tokens.push_back(token);
		current_pointer++;
	}


	while (current_pointer < exp.size()) {
		Token token;
		current_pointer = skip_spaces(exp, current_pointer);
		bg_pointer = current_pointer;

		if (current_pointer >= exp.size()) {
			break;
		}

		// Rozhodovanie o type tokenu a jeho hodnote
		switch (const char c = exp.at(current_pointer)) {
			case '+':
			case '-': {
				token.type = OPERATOR;
				token.value = c;
				token.priority = AS;
				current_pointer++;
				break;
			}
			case '*':
			case '/': {
				token.type = OPERATOR;
				token.value = c;
				token.priority = MD;
				current_pointer++;
				break;
			}
			case '^': {
				token.type = OPERATOR;
				token.value = c;
				token.priority = E;
				current_pointer++;
				break;
			}
			case '(': {
				token.type = OPEN_PAREN;
				token.value = c;
				token.priority = P;
				current_pointer++;
				break;
			}
			case ')': {
				token.type = CLOSE_PAREN;
				token.value = c;
				token.priority = P;
				current_pointer++;
				break;
			}
			default: {
				// Handling cisla
				if (c >= '1' && c <= '9') {
					int dot_count = 0;
					bool is_literal = false;

					while (current_pointer < exp.size()
						&& exp.at(current_pointer) != ' '
						&& exp.at(current_pointer) != ')') {
						if (exp.at(current_pointer) == '.'
							|| exp.at(current_pointer) == ',') {
							dot_count++;
						}

						if (!isdigit(exp.at(current_pointer))
							&& exp.at(current_pointer) != '.'
							&& exp.at(current_pointer) != ',') {
							is_literal = true;
						}
						current_pointer++;
					}

					if (dot_count <= 1 && !is_literal) {
						token.type = NUMBER;
						token.value = exp.substr(bg_pointer, current_pointer - bg_pointer);
					} else {
						token.type = LITERAL;
						token.value = exp.substr(bg_pointer, current_pointer - bg_pointer);
					}
				}

				// Handling literalu
				else {
					while (current_pointer < exp.size()
						&& exp.at(current_pointer) != ' '
						&& exp.at(current_pointer) != ')') {
						current_pointer++;
					}
					token.type = LITERAL;
					token.value = exp.substr(bg_pointer, current_pointer - bg_pointer);
				}
				break;
			}
		}
		tokens.push_back(token);
	}
	return tokens;
}

// Parser podľa Shunting-yard algoritmu vracia pointer na koren stromu
Node* parser(const std::vector<Token>& tokens) {
	std::vector<Node*> operandStack;
	std::vector<Token> operatorStack;

	auto apply_operator = [&]() {
		if (operatorStack.empty() || operandStack.size() < 2) {
			throw std::runtime_error("Invalid expression");
		}

		Token op = operatorStack.back();
		operatorStack.pop_back();

		Node* right = operandStack.back();
		operandStack.pop_back();
		Node* left = operandStack.back();
		operandStack.pop_back();

		Node* newNode = new Node{op};
		newNode->left = left;
		newNode->right = right;
		operandStack.push_back(newNode);
	};

	for (const Token& token : tokens) {
		if (token.type == TokenType::NUMBER
			|| token.type == TokenType::LITERAL
			|| token.type == TokenType::VARIABLE) {
			operandStack.push_back(new Node{token});
		} else if (token.type == TokenType::OPERATOR) {
			while (!operatorStack.empty() &&
				   static_cast<int>(operatorStack.back().priority) >= static_cast<int>(token.priority) &&
				   operatorStack.back().type != TokenType::OPEN_PAREN) {
				apply_operator();
				   }
			operatorStack.push_back(token);
		} else if (token.type == TokenType::OPEN_PAREN
			|| token.type == TokenType::ASSIGNMENT) {
			operatorStack.push_back(token);
		} else if (token.type == TokenType::CLOSE_PAREN) {
			while (!operatorStack.empty()
				&& operatorStack.back().type != TokenType::OPEN_PAREN) {
				apply_operator();
			}
			if (operatorStack.empty()
				|| operatorStack.back().type != TokenType::OPEN_PAREN) {
				throw std::runtime_error("Unmatched parenthesis");
			}
			operatorStack.pop_back();
		} else {
			throw std::runtime_error("Invalid token");
		}
	}

	while (!operatorStack.empty()) {
		apply_operator();
	}

	if (operandStack.size() != 1) {
		throw std::runtime_error("Invalid expression");
	}

	return operandStack.back();
}

// Evaluator (Postfix) vracia vysledok stromu (double), pripadne error message pre nevalidne, alebo chybove vyrazy
double evaluate(Node* root) {
	if (root->token.type == TokenType::NUMBER) {
		return std::stod(root->token.value);
	} else if (root->token.type == TokenType::LITERAL) {
		if (variables.find(root->token.value) == variables.end()) {
			throw std::runtime_error("Undefined variable");
		}
		return std::stod(variables[root->token.value]);
	} else if (root->token.type == TokenType::ASSIGNMENT) {
		if (root->left->token.type != TokenType::VARIABLE) {
			throw std::runtime_error("Invalid assignment");
		}
		double result = evaluate(root->right);
		variables[root->left->token.value] = std::to_string(result);
		return result;
	} else if (root->token.type == TokenType::OPERATOR) {
		double leftValue = evaluate(root->left);
		double rightValue = evaluate(root->right);

		if (root->token.value == "+") return leftValue + rightValue;
		if (root->token.value == "-") return leftValue - rightValue;
		if (root->token.value == "*") return leftValue * rightValue;
		if (root->token.value == "/") {
			if (rightValue == 0) {
				throw std::runtime_error("Division by zero");
			}
			return leftValue / rightValue;
		}
		if (root->token.value == "^") return std::pow(leftValue, rightValue);

		throw std::runtime_error("Unknown operator: " + root->token.value);
	}

	throw std::runtime_error("Unsupported token type in evaluation");
}

int main(int argc, char **argv) {
	while (true) {
		cout << "If you want to read the expression from the file, press 1" << endl;
		cout << "If you want to enter the expression manually, press 2" << endl;
		cout << "If you want to run tests, press 3" << endl;
		cout << "If you want to quit, press 'q'" << endl;

		string choice;
		getline(cin, choice);
		if (choice == "1") {
			cout << "Enter the file name: " << endl;
			// Nacitanie suboru
		} else if (choice == "2") {
			while (true) {
				cout << "Enter the expression (or press 'e' to exit): " << endl;
				string exp;
				getline(cin, exp);
				if (exp == "e") {
					break;
				}
				try {
					vector<Token> tokens = tokenizer(exp);
					Node* root = parser(tokens);
					double result = evaluate(root);
					cout << "Result: " << result << endl;
				} catch (const std::exception& e) {
					cout << "Error: " << e.what() << endl;
				}
			}
		} else if (choice == "3") {
			::testing::InitGoogleTest(&argc, argv);
			int test_result = RUN_ALL_TESTS();
			if (test_result != 0) {
				return test_result;
			}
			cout << "\n" << endl;
		} else if (choice == "q") {
			cout << "Goodbye!" << endl;
			return 0;
		} else {
			cout << "Invalid input. Please try again." << endl;
		}
	}
}