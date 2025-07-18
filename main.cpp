#include <iostream>
#include "Case1_Equation01.h"

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

    return 0;
}
