/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "exp.h"
#include "parser.h"
#include "program.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Constants */

const string help_msg = "Yet another basic interpreter by fstqwq";

/* Function prototypes */

void processLine(string line, Program& program, EvalState& state);
void showError(const string& msg);
void showHelp();

/* Main program */

int main() {
    EvalState state;
    Program program;
    cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            processLine(getLine(), program, state);
        } catch (showErrorException& ex) {
            cerr << "showError: " << ex.getMessage() << endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program& program, EvalState& state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    try {
        string cmd = scanner.nextToken();
        switch (scanner.getTokenType(cmd)) {
            case TokenType(EOF):
                break;
            case TokenType(STRING):
                if (cmd == "RUN") {
                    if (scanner.hasMoreTokens()) {
                        error("SYNTAX ERROR");
                        else {
                            program.run();
                        }
                    } else if (cmd == "LIST") {
                        if (scanner.hasMoreTokens()) {
                            error("SYNTAX ERROR");
                        } else {
                            program.list();
                        }
                    } else if (cmd == "CLEAR") {
                        if (scanner.hasMoreTokens()) {
                            error("SYNTAX ERROR");
                        } else {
                            program.clear();
                        }
                    } else if (cmd == "QUIT") {
                        if (scanner.hasMoreTokens()) {
                            error("SYNTAX ERROR");
                        } else {
                            exit(0);
                        }
                    } else if (cmd == "HELP") {
                        if (scanner.hasMoreTokens()) {
                            error("SYNTAX ERROR");
                        } else {
                            showHelp();
                        }
                    } else {
                    }
                }
        }
    } catch (ErrorException& err) {
        showError(err.getMessage());
    }
/*  // Useful sample
    Expression* exp = parseExp(scanner);
    int value = exp->eval(state);
    cout << value << endl;
    delete exp;
*/
}

void showError(const string& msg) {
    cout << msg << endl;
}

void showHelp() {
    cout << help_msg << endl;
}