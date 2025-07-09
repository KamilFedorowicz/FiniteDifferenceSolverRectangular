#pragma once
#include <vector>

// Multiply scalar * 2D vector
std::vector<std::vector<double>> operator*(double scalar, 
    const std::vector<std::vector<double>>& matrix) 
{
    std::vector<std::vector<double>> result = matrix; // copy
    
    for (auto& row : result) {
        for (auto& val : row) {
            val *= scalar;
        }
    }
    
    return result;
}

std::vector<std::vector<double>> operator*(const std::vector<std::vector<double>>& matrix, double scalar) 
{
    std::vector<std::vector<double>> result = matrix; // copy
    
    for (auto& row : result) {
        for (auto& val : row) {
            val *= scalar;
        }
    }
    
    return result;
}

std::vector<std::vector<double>> operator+(const std::vector<std::vector<double>>& matrix1, const std::vector<std::vector<double>>& matrix2) 
{
    std::vector<std::vector<double>> result = matrix1; // copy
    
    for (int i = 0; i < matrix1.size(); i++) {
        for (int j = 0; j < matrix1.size(); j++) {
            result[i][j] += matrix2[i][j];
        }
    }
    
    return result;
}

