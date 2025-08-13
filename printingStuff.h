#pragma once
#include "FieldTypes.h"
#include <iostream>

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