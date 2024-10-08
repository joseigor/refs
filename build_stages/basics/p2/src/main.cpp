#include <iostream>
#include "calculator.h"

int main() {
    double a = 10.0;
    double b = 5.0;
    char op = '+';
    double result = calculator(a, b, op);
    std::cout << "Result (a + b): " << result << std::endl;

    char op2 = '-';
    double result2 = calculator(a, b, op2);
    std::cout << "Result (a - b) : " << result2 << std::endl;

    char op3 = 'o';
    double result3 = calculator(a, b, op3);
    std::cout << "Result (a + b + 10): " << result3 << std::endl;

    return 0;
}