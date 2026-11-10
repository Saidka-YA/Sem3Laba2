#include "postfixConverter.h"
#include "stack.h"
using namespace std;

bool postfixConverter::isOperator(char symbol){
    if(symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/') return true;
    return false;
}

int postfixConverter::priority(char operation){
    if(operation == '+' || operation == '-') return 1;
    else if(operation == '*' || operation == '/') return 2;
    return 0;
}

string postfixConverter::convert(const string& expression){
    string result;
    StackData operators;
    SINIT(&operators);

    for(char symbol : expression) {
        if(!isOperator(symbol) && symbol != '(' && symbol != ')') {
            result += symbol;
        } else if(symbol == '(') {
            SPUSH(&operators, {symbol});
        } else if(symbol == ')') {
            while(!SISEMPTY(&operators) && SGET(&operators) != "(") {
                result += SPOP(&operators);
            }
            SPOP(&operators);
        } else if (isOperator(symbol)) {
            while(!SISEMPTY(&operators)) {
                if(isOperator(SGET(&operators)[0]) && 
                   priority(SGET(&operators)[0]) >= priority(symbol)) {
                    result += SPOP(&operators);
                } else break;
            }
            SPUSH(&operators, {symbol});
        }
    }
    while(!SISEMPTY(&operators)) {
        result += SPOP(&operators);
    }

    SCLEAR(&operators);
    return result;
}

#include <iomanip>
#include <limits>
#include <sstream>

double postfixConverter::evaluate(const std::string& postfix)
{
    StackData numbers;
    SINIT(&numbers);

    for (char symbol : postfix) {
        if (symbol >= '0' && symbol <= '9') {
            SPUSH(&numbers, string(1, symbol));
        } else if (isOperator(symbol)) {
            double right = stod(SPOP(&numbers));
            double left = stod(SPOP(&numbers));

            double value = 0;
            switch (symbol) {
            case '+': value = left + right; break;
            case '-': value = left - right; break;
            case '*': value = left * right; break;
            case '/': value = left / right; break;
            }

            ostringstream text;
            text << setprecision(
                numeric_limits<double>::max_digits10
            ) << value;

            SPUSH(&numbers, text.str());
        }
    }

    double answer = stod(SPOP(&numbers));
    SCLEAR(&numbers);
    return answer;
}