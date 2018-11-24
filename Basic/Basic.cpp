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
void runImmediately(Program& program, EvalState& state, const string& cmd,
                    TokenScanner& scanner);
void addStatement(Program& program, EvalState& state, const int lineNumber,
                  const string& line, TokenScanner& scanner);
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
        } catch (ErrorException& ex) {
            showError(ex.getMessage());
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
    string cmd = scanner.nextToken();
    switch (scanner.getTokenType(cmd)) {
        case TokenType(EOF):
            break;
        case TokenType(WORD):
            runImmediately(program, state, cmd, scanner);
            break;
        case TokenType(NUMBER):
            addStatement(program, state, stringToInteger(cmd), line, scanner);
            break;
        default:
            error("syntaxErr: Unrecognized input : " + cmd);
    }
}

void runImmediately(Program& program, EvalState& state, const string& cmd,
                    TokenScanner& scanner) {
    if (cmd == "RUN") {
        checkEOLN(scanner);
        program.run(state);
    } else if (cmd == "LIST") {
        checkEOLN(scanner);
        program.list();
    } else if (cmd == "CLEAR") {
        checkEOLN(scanner);
        program.clear();
        state.clear();
    } else if (cmd == "QUIT") {
        checkEOLN(scanner);
        exit(0);
    } else if (cmd == "HELP") {
        checkEOLN(scanner);
        showHelp();
    } else if (cmd == "PRINT" || cmd == "INPUT" || cmd == "LET") {
        Statement* line = readStatement(cmd, scanner);
        int tmpProgramNextLine = -1;
        line->execute(state, tmpProgramNextLine);
    } else {
        error("syntaxErr:  Invalid command: " + cmd);
    }
}

void addStatement(Program& program, EvalState& state, const int lineNumber,
                  const string& line, TokenScanner& scanner) {
	Statement* stmt = readStatement(scanner.nextToken(), scanner);
	program.addSourceLine(lineNumber, line);
	program.setParsedStatement(lineNumber, stmt);
}

void showError(const string& msg) {
	TokenScanner scanner;
    scanner.setInput(msg);
	string errType = scanner.nextToken(), errMsg = "";
		 if (errType == "arithmeticErr") errMsg = "DIVIDE BY ZERO";
	else if (errType == "invalidNumErr") errMsg = "INVALID NUMBER";
	else if (errType == "undefinedVariable") errMsg = "VARIABLE NOT DEFINED";
	else if (errType == "lineNumErr") errMsg = "LINE NUMBER ERROR";
	else errMsg = "SYNTAX ERROR";

//	cerr << msg << endl;
	cout << errMsg << endl;
}

void showHelp() { cout << help_msg << endl; }