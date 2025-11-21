#pragma once
#include <vector>
#include <map>
#include <iostream>
#include "lexer.h"   // your lexer with Token, TokenType, and tokenize()
#include "ast.h"
using namespace std;

Expression* parseExpression(vector<Token>& tokens, size_t &pos);
Expression* parseFactor(vector<Token>& tokens, size_t &pos);

Expression* parseExpression(vector<Token>& tokens, size_t &pos);
void parseMakeStatement(vector<Token>& tokens, map<string,int>& envNum, map<string,string>& envStr, size_t &pos);


void parseOutputStatement(vector<Token>& tokens, size_t &pos, map<string,int>& envNum, map<string,string>& envStr);