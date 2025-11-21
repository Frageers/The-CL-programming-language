#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "ast.h"
#include "lexer.h"
#include <algorithm>
using namespace std;

vector<string> splitString(const string &sourceCode) {
    vector<string> words;
    string word;

    for (char ch : sourceCode) {
        if (ch != ' ') {
            word += ch;
        } else if (!word.empty()) {
            words.push_back(word);
            word.clear();
        }
    }

    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

vector<Token> tokenize(string sourceCode) {
    vector<Token> tokens;
    string word;

    auto pushWord = [&](string w) {
        if (w.empty()) return;
        Token t;

        t.value = w;

        if (w == "make"){
            t.type = Let;
        } 
        else if (w == "="){
            t.type = Equals;
        } 
        else if (w == "("){
            t.type = OpenParen;
        } 
        else if (w == ")"){
            t.type = CloseParen;
        } 
        else if (w == "+" || w == "-" || w == "*" || w == "/")
            t.type = BinaryOperator;
        else if (w == "output:" || w == "advika:"){
            t.type = Output;
        }
        else if (all_of(w.begin(), w.end(), ::isdigit)){
             t.type = Number;
        }
        else{
            t.type = Identifier;
        }
        tokens.push_back(t);
    };

    for (char c : sourceCode) {
        if (c == ' ') {
            pushWord(word);
            word.clear();
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' ||
                c == '(' || c == ')' || c == '=') {
            pushWord(word);
            word.clear();
            string op(1, c);
            pushWord(op);
        }
        else {
            word += c;
        }
    }

    pushWord(word);
    return tokens;
}


//parser.cpp
Expression* parseExpression(vector<Token>& tokens, size_t &pos);
Expression* parseFactor(vector<Token>& tokens, size_t &pos) {
    Token t = tokens[pos++];
    if(t.type == Number) return new NumberExpr(stoi(t.value));
    if(t.type == Identifier) return new VariableExpr(t.value);
    if(t.type == OpenParen) {
        Expression* expr = parseExpression(tokens, pos);
        pos++; // skip CloseParen
        return expr;
    }
    return nullptr;
}

Expression* parseExpression(vector<Token>& tokens, size_t &pos) {
    Expression* left = parseFactor(tokens, pos);
    while(pos < tokens.size() && tokens[pos].type == BinaryOperator) {
        string op = tokens[pos++].value;
        Expression* right = parseFactor(tokens, pos);
        left = new BinaryExpr(left, op, right);
    }
    return left;
}

void parseMakeStatement(vector<Token>& tokens, map<string,int>& env, size_t &pos) {
    pos++; // skip "make"
    string varName = tokens[pos++].value;
    pos++; // skip "="
    Expression* expr = parseExpression(tokens, pos);
    env[varName] = expr->evaluate(env);
    delete expr;
}


void parseOutputStatement(vector<Token>& tokens, size_t& pos, map<string,int>& env) {
    pos++; // skip "output:"
    Expression* expr = parseExpression(tokens, pos);
    cout << expr->evaluate(env) << endl;
    delete expr;
}


