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

struct VariableExpr : Expression {
    string name;
    VariableExpr(const string &n) : name(n) {}
    int evaluate(map<string,int>& env) override { return env[name]; }
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
