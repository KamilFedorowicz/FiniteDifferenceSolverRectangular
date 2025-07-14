#pragma once
#include <vector>
#include <stdexcept>

// Multiply scalar * 2D vector
inline std::vector<std::vector<double>> operator*(
    double scalar,
    const std::vector<std::vector<double>>& matrix)
{
    std::vector<std::vector<double>> result = matrix;

    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[0].size(); ++j) {
            result[i][j] *= scalar;
        }
    }

    return result;
}

inline std::vector<std::vector<double>> operator*(
    const std::vector<std::vector<double>>& matrix,
    double scalar)
{
    return scalar * matrix; // reuse the other overload
}

inline std::vector<std::vector<double>> operator+(
    const std::vector<std::vector<double>>& matrix1,
    const std::vector<std::vector<double>>& matrix2)
{
    if (matrix1.size() != matrix2.size() || matrix1[0].size() != matrix2[0].size()) {
        throw std::invalid_argument("Matrix size mismatch in operator+");
    }

    std::vector<std::vector<double>> result = matrix1;

    for (size_t i = 0; i < matrix1.size(); ++i) {
        for (size_t j = 0; j < matrix1[0].size(); ++j) {
            result[i][j] += matrix2[i][j];
        }
    }

    return result;
}
