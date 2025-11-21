#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std;

enum TokenType {
    Number, 
    Identifier,
    Equals,
    OpenParen,
    CloseParen,
    BinaryOperator,
    Let,
    Output,
    String
};

struct Token {
    string value;
    TokenType type;
};

vector<string> splitString(const string &sourceCode);

vector<Token> tokenize(string sourceCode);