#include <iostream>
#include "Case1_Equation01.h"
#include "Case1_Equation02.h"

// Optional: helper to print 2D field
void print(const std::vector<std::vector<double>>& field) {
    for (const auto& row : field) {
        for (double val : row)
            std::cout << val << " ";
        std::cout << "\n";
    }
}

int main() {

    // next to do: BCs, source (vector-dependent?) for a vector, then vector differential equation
    
    runCase1_Equation01();

    runCase1_Equation02();

    return 0;
}
