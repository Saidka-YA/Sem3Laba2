#ifndef POSTFIXCONVERTER_H
#define POSTFIXCONVERTER_H
#pragma once
#include <string>
#include <iomanip>
#include <limits>
#include <sstream>

class postfixConverter {
public:
    std::string convert(const std::string& expression);
    double evaluate(const std::string& postfix);
    
private:
    bool isOperator(char symbol);
    int priority(char operation);
};

#endif