#pragma once
#include <map>
#include <string>
using namespace std;
struct Expression{
    virtual int evaluate(map<string, int>&env) = 0;
    virtual ~Expression() {}
};

struct NumberExpr : Expression {
    int value;
    NumberExpr(int v) : value(v) {}
    int evaluate(map<string,int>& env) override { return value; }
};

class VariableExpr : public Expression {
public:
    string name;

    VariableExpr(string n) : name(n) {}

    int evaluate(map<string,int>& envNum) override {
        return envNum[name];
    }

    string evaluateString(map<string,string>& envStr) {
        return envStr[name];
    }
};


struct BinaryExpr : Expression {
    Expression* left;
    string op;
    Expression* right;
    BinaryExpr(Expression* l, string o, Expression* r) : left(l), op(o), right(r) {}
    int evaluate(map<string,int>& env) override {
        if(op == "+") return left->evaluate(env) + right->evaluate(env);
        if(op == "-") return left->evaluate(env) - right->evaluate(env);
        if(op == "*") return left->evaluate(env) * right->evaluate(env);
        if(op == "/") return left->evaluate(env) / right->evaluate(env);
        return 0;
    }
};

class StringExpr : public Expression {
public:
    string value;

    StringExpr(const string& v) : value(v) {}

    int evaluate(map<string,int>& envNum) override {
        return 0; // numbers only
    }

    string evaluateString(map<string,string>& envStr) {
        return value;
    }
};
