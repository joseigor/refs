#include "calculator.h"
#include "math/add.h"
#include "math/sub.h"

double calculator(double a, double b, char op) {
    switch (op) {
        case '+':
            return add(a, b);
        case 'o': // new operation, add 10 to the result
            return add3(a, b, 10.0);
        case '-':
            return sub(a, b);
        default:
            return 0.0;
    }
}