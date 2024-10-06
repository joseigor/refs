#include <iostream> // File include

// Macros
#define PI 3.14
#define CIRCLE_AREA(radius) (PI * (radius) * (radius))

// Multiline macro with predefined macros __FILE__ and __LINE__
#define DEBUG_FILE_LINE \
    std::cout << __FILE__ \
    << ":" \
    << __LINE__ \
    << " ";

int main() {
    // calculate the area of a circle of radius 10
    float area = CIRCLE_AREA(10);

    #ifdef DEBUG
        DEBUG_FILE_LINE
    #endif

    // std::cout << "Area: " << area << std::endl;

    return 0;
}

// g++ -E main.cpp -o main.i
// g++ -DDEBUG -E main.cpp -o main.i
// g++  -UDEBUG -E main.cpp -o main.i (unset DEBUG )
// g++ -DDEBUG -C -E main.cpp -o main.i (do not discard comments)

// https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html reference for
// all flag