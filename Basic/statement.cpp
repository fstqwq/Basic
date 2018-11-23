/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.h"
#include <string>
using namespace std;

Statement::Statement() {}

Statement::~Statement() {}

REM::REM() {}

REM::~REM() {}

void REM::execute(EvalState& state, int & nextline) {}

LET::LET() {}

LET::LET(const string& var, Expression* expr) {
    Var = var;
    Expr = expr;
}

LET::~LET() {
    delete Expr;
}

void LET::execute(EvalState& state, int & nextline) {
    int value = Expr->eval(state);
    state.setValue(Var, value);
}

PRINT::PRINT() {}

PRINT::PRINT(Expression *exp) {
    Expr = exp;
}

PRINT::~PRINT() {
    delete Expr;
}

void PRINT::execute(EvalState & state, int & nextline) {
    int value = Expr->eval(state);
    cout << value << endl;
}

INPUT::INPUT () {}

INPUT::INPUT (const string & var) {
    Var = var;
}

INPUT::~INPUT () {}

void INPUT::execute(EvalState & state, int & nextline) {
    int value;
    cin >> value;
    state.setValue(Var, value);
}

END::END () {}

END::~END () {}

void END::execute(EvalState & state, int & nextline) {
    nextline = -1;
}

GOTO::GOTO () {}

GOTO::GOTO (int lineNumber) {
    line = lineNumber;
}

GOTO::~GOTO () {}

void GOTO::execute(EvalState & state, int & nextline) {
    nextline = line;
}

IF::IF() {}

IF::IF (Expression *a, const string & op, Expression *b, int lineNumber) {
    A = a;
    B = b;
    OP = op;
    line = lineNumber;
}

IF::~IF () {
    delete A;
    delete B;
}

void IF::execute(EvalState & state, int & nextline) {
    int value_A = A->eval(state);
    int value_B = B->eval(state);
    bool value;

         if (OP == "=") value = (value_A == value_B);
    else if (OP == ">") value = (value_A > value_B);
    else if (OP == "<") value = (value_A < value_B);
    
    if (value) nextline = line;
}