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

void REM::execute(EvalState& state) {}

LET::LET() {}

LET::LET(const string& var, Expression* expr) {
    Var = var;
    Expr = expr;
}

LET::~LET() {
    delete Expr;
}

void LET::execute(EvalState& state) {
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

void PRINT::execute(EvalState & state) {
    int value = Expr->eval(state);
    cout << value << endl;
}

INPUT::INPUT () {}

INPUT::INPUT (const string & var) {
    Var = var;
}

INPUT::~INPUT () {}

void INPUT::execute(EvalState & state) {
    int value;
    cin >> value;
    state.setValue(Var, value);
}