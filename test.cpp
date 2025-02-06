#include <gtest/gtest.h>
#include "main.h"

//=====================================
// 25 Evaluation tests
//=====================================

TEST(CalculatorTest, Addition) {
    std::string expression = "3 + 5";
    std::vector<Token> tokens = tokenizer(expression);
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, NUMBER);
    EXPECT_EQ(tokens[0].value, "3");
    EXPECT_EQ(tokens[1].type, OPERATOR);
    EXPECT_EQ(tokens[1].value, "+");
    EXPECT_EQ(tokens[2].type, NUMBER);
    EXPECT_EQ(tokens[2].value, "5");

    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 8.0);
}

TEST(CalculatorTest, Subtraction) {
    std::string expression = "10 - 4";
    std::vector<Token> tokens = tokenizer(expression);
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, NUMBER);
    EXPECT_EQ(tokens[0].value, "10");
    EXPECT_EQ(tokens[1].type, OPERATOR);
    EXPECT_EQ(tokens[1].value, "-");
    EXPECT_EQ(tokens[2].type, NUMBER);
    EXPECT_EQ(tokens[2].value, "4");

    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 6.0);
}

TEST(CalculatorTest, Multiplication) {
    std::string expression = "7 * 6";
    std::vector<Token> tokens = tokenizer(expression);
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, NUMBER);
    EXPECT_EQ(tokens[0].value, "7");
    EXPECT_EQ(tokens[1].type, OPERATOR);
    EXPECT_EQ(tokens[1].value, "*");
    EXPECT_EQ(tokens[2].type, NUMBER);
    EXPECT_EQ(tokens[2].value, "6");

    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 42.0);
}

TEST(CalculatorTest, Division) {
    std::string expression = "8 / 2";
    std::vector<Token> tokens = tokenizer(expression);
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, NUMBER);
    EXPECT_EQ(tokens[0].value, "8");
    EXPECT_EQ(tokens[1].type, OPERATOR);
    EXPECT_EQ(tokens[1].value, "/");
    EXPECT_EQ(tokens[2].type, NUMBER);
    EXPECT_EQ(tokens[2].value, "2");

    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 4.0);
}

TEST(CalculatorTest, Parentheses) {
    std::string expression = "(3 + 5) * 2";
    std::vector<Token> tokens = tokenizer(expression);
    ASSERT_EQ(tokens.size(), 7);
    EXPECT_EQ(tokens[0].type, OPEN_PAREN);
    EXPECT_EQ(tokens[0].value, "(");
    EXPECT_EQ(tokens[1].type, NUMBER);
    EXPECT_EQ(tokens[1].value, "3");
    EXPECT_EQ(tokens[2].type, OPERATOR);
    EXPECT_EQ(tokens[2].value, "+");
    EXPECT_EQ(tokens[3].type, NUMBER);
    EXPECT_EQ(tokens[3].value, "5");
    EXPECT_EQ(tokens[4].type, CLOSE_PAREN);
    EXPECT_EQ(tokens[4].value, ")");
    EXPECT_EQ(tokens[5].type, OPERATOR);
    EXPECT_EQ(tokens[5].value, "*");
    EXPECT_EQ(tokens[6].type, NUMBER);
    EXPECT_EQ(tokens[6].value, "2");

    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 16.0);
}

// Add more tests for other operators and combinations
TEST(CalculatorTest, ComplexExpression1) {
    std::string expression = "3 + 5 * 2 - 8 / 4";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 11.0);
}

TEST(CalculatorTest, ComplexExpression2) {
    std::string expression = "10 + (2 * 3) - (4 / 2)";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 14.0);
}

TEST(CalculatorTest, ComplexExpression3) {
    std::string expression = "((3 + 5) * 2) / 4";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 4.0);
}

TEST(CalculatorTest, ComplexExpression4) {
    std::string expression = "3 + 4 * 2 / (1 - 5)";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 1.0);
}

TEST(CalculatorTest, ComplexExpression5) {
    std::string expression = "7 + 3 * (10 / (12 / (3 + 1) - 1))";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 22.0);
}

// Continue adding more tests to reach at least 20
TEST(CalculatorTest, ComplexExpression6) {
    std::string expression = "5 + ((1 + 2) * 4) - 3";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 14.0);
}

TEST(CalculatorTest, ComplexExpression7) {
    std::string expression = "10 / 2 + 3 * 4 - 5";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 12.0);
}

TEST(CalculatorTest, ComplexExpression8) {
    std::string expression = "8 + 2 * 5 - (3 / 1)";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 15.0);
}

TEST(CalculatorTest, ComplexExpression9) {
    std::string expression = "7 - 3 + 2 * (5 / 1)";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 14.0);
}

TEST(CalculatorTest, ComplexExpression10) {
    std::string expression = "6 / 2 * (1 + 2)";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 9.0);
}

TEST(CalculatorTest, ComplexExpression11) {
    std::string expression = "3 + 4 * 2 / (1 - 5) + 6";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 7.0);
}

TEST(CalculatorTest, ComplexExpression12) {
    std::string expression = "10 - 3 + 2 * (8 / 4)";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 11.0);
}

TEST(CalculatorTest, ComplexExpression13) {
    std::string expression = "5 * (3 + 2) - 4 / 2";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 23.0);
}

TEST(CalculatorTest, ComplexExpression14) {
    std::string expression = "8 / (4 - 2) + 6 * 3";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 22.0);
}

TEST(CalculatorTest, ComplexExpression15) {
    std::string expression = "7 + 3 * (10 / (5 - 3))";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 22.0);
}

TEST(CalculatorTest, ComplexExpression16) {
    std::string expression = "5 + 2 * (3 + 4) - 6 / 2";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 16.0);
}

TEST(CalculatorTest, ComplexExpression17) {
    std::string expression = "10 / (2 + 3) * 4 - 1";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 7.0);
}

TEST(CalculatorTest, ComplexExpression18) {
    std::string expression = "8 + 2 * (5 - 3) / 1";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 12.0);
}

TEST(CalculatorTest, ComplexExpression19) {
    std::string expression = "7 - 3 + 2 * (4 / 2)";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 8.0);
}

TEST(CalculatorTest, ComplexExpression20) {
    std::string expression = "6 ^ 2 * (1 + 3)";
    std::vector<Token> tokens = tokenizer(expression);
    Node* root = parser(tokens);
    double result = evaluate(root);
    EXPECT_DOUBLE_EQ(result, 144.0);
}

//=========================================
// 25 Error handling tests
//=========================================

