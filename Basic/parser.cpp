/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include "parser.h"
#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"

#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/tokenscanner.h"
using namespace std;

/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */

Expression *parseExp(TokenScanner &scanner) {
    Expression *exp = readE(scanner);
    if (scanner.hasMoreTokens()) {
        error("parseExp: Found extra token: " + scanner.nextToken());
    }
    return exp;
}

/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */

Expression *readE(TokenScanner &scanner, int prec) {
    Expression *exp = readT(scanner);
    string token;
    while (true) {
        token = scanner.nextToken();
        int newPrec = precedence(token);
        if (newPrec <= prec) break;
        Expression *rhs = readE(scanner, newPrec);
        exp = new CompoundExp(token, exp, rhs);
    }
    scanner.saveToken(token);
    return exp;
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */

Expression *readT(TokenScanner &scanner) {
    string token = scanner.nextToken();
    TokenType type = scanner.getTokenType(token);
    if (type == WORD) return new IdentifierExp(token);
    if (type == NUMBER) return new ConstantExp(stringToInteger(token));
    if (token != "(") error("syntaxErr: Illegal term in expression");
    Expression *exp = readE(scanner);
    if (scanner.nextToken() != ")") {
        error("syntaxErr: Unbalanced parentheses in expression");
    }
    return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

int precedence(string token) {
    if (token == "+" || token == "-") return 2;
    if (token == "*" || token == "/") return 3;
    return 0;
}

bool isComparision(const string& op) {
	return op == ">" || op == "<" || op == "=";
}

bool isValidVariableName(const string& name) {
	if (TokenScanner().getTokenType(name) != TokenType(WORD)) return false;
	if (name == "REM" ||
		name == "LET" ||
		name == "PRINT" ||
		name == "INPUT" ||
		name == "END" ||
		name == "GOTO" ||
		name == "IF")
		return false;
	return true;
}

string readVar(TokenScanner& scanner) {
    string varName = scanner.nextToken();
    if (!isValidVariableName(varName)) error("syntaxErr: Illegal variable name");
	return varName;
}

int readInt(TokenScanner& scanner) {
	string value = scanner.nextToken();
	if (scanner.getTokenType(value) != TokenType(NUMBER))
		error("syntaxErr: Not an integer : " + value);
	return stringToInteger(value);
}

void checkEOLN(TokenScanner& scanner) {
    if (scanner.hasMoreTokens())
        error("syntaxErr: Extra infomation at end of line");
}

Statement* readStatement(const string &cmd, TokenScanner &scanner) {
    if (cmd == "REM") return readREM(scanner);
    if (cmd == "LET") return readLET(scanner);
    if (cmd == "PRINT") return readPRINT(scanner);
    if (cmd == "INPUT") return readINPUT(scanner);
    if (cmd == "END") return readEND(scanner);
    if (cmd == "GOTO") return readGOTO(scanner);
    if (cmd == "IF") return readIF(scanner);
	error("syntaxErr: Invalid command : " + cmd);
	return NULL;
}

Statement* readREM(TokenScanner &scanner) { return new REM(); }

Statement* readLET(TokenScanner &scanner) {
	string varName = readVar(scanner);
    if (scanner.nextToken() != "=") error("syntanErr: Illegal assignment");
    Expression *Expr = readE(scanner);
	checkEOLN(scanner);
    return new LET(varName, Expr);
}

Statement* readPRINT(TokenScanner &scanner) {
    Expression *Expr = readE(scanner);
	checkEOLN(scanner);
    return new PRINT(Expr);
}

Statement* readINPUT(TokenScanner &scanner) {
    string varName = readVar(scanner);
	checkEOLN(scanner);
	return new INPUT(varName);
}

Statement* readEND(TokenScanner& scanner) {
	checkEOLN(scanner);
	return new END();
}

Statement* readGOTO(TokenScanner& scanner) {
	int lineNumber = readInt(scanner);
	checkEOLN(scanner);
	return new GOTO(lineNumber);
}

Statement* readIF(TokenScanner& scanner) {
	Expression *lhs = readE(scanner);
	string op = scanner.nextToken();
	if (op != "=" && op != ">" && op != "<") error("syntaxErr: Expected a comparision operator, recieved " + op);
	Expression *rhs = readE(scanner);
	if (scanner.nextToken() != "THEN") error("syntaxErr: Expected THEN");
	int lineNumber = readInt(scanner);
	checkEOLN(scanner);
	return new IF(lhs, op, rhs, lineNumber);
}