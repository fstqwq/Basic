/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.h"
#include <string>
#include "statement.h"
using namespace std;

Program::Program() {
    // Empty
}

Program::~Program() {
    clear();
}

void Program::clear() {
    for (auto& i : src) {
        delete i.second.second;
    }
    src.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
    removeSourceLine(lineNumber);
    src[lineNumber] = std::make_pair(line, (Statement*)NULL);
}

void Program::removeSourceLine(int lineNumber) {
    auto it = src.find(lineNumber);
    if (it != src.end()) {
        src.erase(it);
    }
}

string Program::getSourceLine(int lineNumber) {
    return src[lineNumber].first;
}

void Program::setParsedStatement(int lineNumber, Statement* stmt) {
    auto it = src.find(lineNumber);
    if (it != src.end()) {
        delete it->second.second;
        it->second.second = stmt;
    }
}

Statement* Program::getParsedStatement(int lineNumber) {
    return src[lineNumber].second;
}

int Program::getFirstLineNumber() {
    if (src.empty())
        return -1;
    return src.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
    auto now = src.find(lineNumber);
    if (now == src.end())
        return -1;
    now++;
    if (now == src.end())
        return -1;
    return now->first;
}

void Program::run() {
    int line = getFirstLineNumber();
    while (line != -1) {
        if (src.find(line) == src.end()) {
            error("LINE NUMBER ERROR");
        }
        int nextline = getNextLineNumber(line);
        src.find(line)->second.second->execute(state, nextline);
        line = nextline;
    }
}

void Program::list() {
    for (auto& i : src) {
        cout << i.second.first << endl;
    }
}