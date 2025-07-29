#include <iostream>
#include "Case1_Equation01.h"
#include "Case1_Equation02.h"

// Optional: helper to print 2D field
void print(const scalarField& field) {
    for (const auto& row : field) {
        for (double val : row)
            std::cout << val << " ";
        std::cout << "\n";
    }
}

void print(const vectorField& field) {
    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[i].size(); ++j) {
            std::cout << "(" << field[i][j][0] << ", " << field[i][j][1] << ") ";
        }
        std::cout << std::endl;
    }
}

void print(const tensorField& field) {
    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[i].size(); ++j) {
            std::cout << "Tensor at (" << i << ", " << j << "):" << std::endl;
            for (size_t m = 0; m < field[i][j].size(); ++m) {
                std::cout << "  [ ";
                for (size_t n = 0; n < field[i][j][m].size(); ++n) {
                    std::cout << field[i][j][m][n] << " ";
                }
                std::cout << "]" << std::endl;
            }
        }
    }
}

int main() {

    // tracking variables in time
    // source files in txt files like in openfoam - that would substitute Case1_Equation01 etc
    // how to make it compile etc?
    
    vectorField vect1(1, std::vector<std::vector<double>>(1, std::vector<double>{1,2}));
    vectorField vect2(1, std::vector<std::vector<double>>(1, std::vector<double>{2,3}));
    
    
    std::vector<std::vector<std::vector<std::vector<double>>>> tens1(1, std::vector<std::vector<std::vector<double>>>(1, std::vector<std::vector<double>>{
                                                                                                                                  {1.0, 3.0},
                                                                                                                                  {2.0, 4.0}
                                                                                                                              }
                                                                                                                          )); // the inner tensor goes row by row
    
    
    vectorField test = vect1/magn(vect1);
    print(test);
    
    //runCase1_Equation01();

    runCase1_Equation02();

    return 0;
}
