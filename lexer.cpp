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
    bool inString = false;
    string stringLiteral;

    auto pushWord = [&](string w) {
        if (w.empty()) return;
        Token t;
        t.value = w;

        if (!inString) {
            if (w == "make") t.type = Let;
            else if (w == "=") t.type = Equals;
            else if (w == "(") t.type = OpenParen;
            else if (w == ")") t.type = CloseParen;
            else if (w == "+" || w == "-" || w == "*" || w == "/") t.type = BinaryOperator;
            else if (w == "output:") t.type = Output;
            else if (all_of(w.begin(), w.end(), ::isdigit)) t.type = Number;
            else t.type = Identifier;
        } else {
            t.type = String; // entire captured string
        }

        tokens.push_back(t);
    };

    for (size_t i = 0; i < sourceCode.size(); i++) {
        char c = sourceCode[i];

        if (inString) {
            if (c == '"') {
                // push the captured string while inString is still true
                pushWord(stringLiteral);
                stringLiteral.clear();
                inString = false;
            } else {
                stringLiteral += c;
            }
        }

        else {
            if (c == '"') {
                inString = true;
                if (!word.empty()) {
                    pushWord(word);
                    word.clear();
                }
            }
            else if (isspace(c)) {
                pushWord(word);
                word.clear();
            }
            else if (c=='+'||c=='-'||c=='*'||c=='/'||c=='('||c==')'||c=='=') {
                pushWord(word);
                word.clear();
                string op(1, c);
                pushWord(op);
            }
            else {
                word += c;
            }
        }
    }

    if (!word.empty() && !inString) pushWord(word);

    return tokens;
}



//parser.cpp
Expression* parseExpression(vector<Token>& tokens, size_t &pos);
Expression* parseFactor(vector<Token>& tokens, size_t &pos) {
    Token t = tokens[pos++];

    if (t.type == Number)
        return new NumberExpr(stoi(t.value));

    if (t.type == String)
        return new StringExpr(t.value);

    if (t.type == Identifier)
        return new VariableExpr(t.value);

    if (t.type == OpenParen) {
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

void parseMakeStatement(vector<Token>& tokens, map<string,int>& envNum,map<string,string>& envStr,size_t &pos)
{
    pos++; // skip "make"
    string varName = tokens[pos++].value;
    pos++; // skip "="

    // STRING ASSIGNMENT
    if (tokens[pos].type == String) {
        envStr[varName] = tokens[pos].value;
        pos++;
        return;
    }

    // NUMERIC EXPRESSION
    Expression* expr = parseExpression(tokens, pos);
    envNum[varName] = expr->evaluate(envNum);
    delete expr;
}





void parseOutputStatement(vector<Token>& tokens, size_t &pos, map<string,int>& envNum, map<string,string>& envStr) 
{
    // Expect: Output Identifier
    if (pos < tokens.size() && tokens[pos].type == Output) {
        pos++; // skip "output"

        if (pos < tokens.size() && tokens[pos].type == Identifier) {
            string var = tokens[pos].value;
            pos++;

            // FIRST check string variables
            if (envStr.count(var)) {
                cout << envStr[var] << endl;
                return;
            }

            // THEN check numeric variables
            if (envNum.count(var)) {
                cout << envNum[var] << endl;
                return;
            }

            // Undefined variable
            cout << "Undefined variable: " << var << endl;
        }
    }
}
