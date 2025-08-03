#pragma once
#include <vector>
#include <stdexcept>
#include "FieldTypes.h"
#include <cmath>

// Multiply scalar * 2D vector
inline scalarField operator*(double scalar, const scalarField& field)
{
    scalarField result = field;

    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[0].size(); ++j) {
            result[i][j] *= scalar;
        }
    }
    return result;
}

inline scalarField operator*(
    const scalarField& field,
    double scalar)
{
    return scalar * field; // reuse the other overload
}

inline vectorField operator*(double scalar, const vectorField& field)
{
    vectorField result = field;

    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[0].size(); ++j) {
            for (size_t k = 0; k < field[0][0].size(); k++) {
                result[i][j][k] *= scalar;
            }
        }
    }
    return result;
}

// outer product operator
inline tensorField operator&(const vectorField& field1, const vectorField& field2)
{
    tensorField result(field1.size(), std::vector<std::vector<std::vector<double>>>(
        field1[0].size(), std::vector<std::vector<double>>(
            2, std::vector<double>(2, 0.0)
        )
    ));
    
    for (size_t i = 0; i < field1.size(); ++i)
    {
        for (size_t j = 0; j < field1[0].size(); ++j)
        {
            result[i][j][0][0] = field1[i][j][0]*field2[i][j][0];
            result[i][j][0][1] = field1[i][j][0]*field2[i][j][1];
            result[i][j][1][0] = field1[i][j][1]*field2[i][j][0];
            result[i][j][1][1] = field1[i][j][1]*field2[i][j][1];
        }
    }
    return result;
}

// double contraction
inline scalarField operator &&(const tensorField& field1, const tensorField& field2)
{
    scalarField result(field1.size(), std::vector<double>(field1[0].size(), 0.0));
    
    for (size_t i = 0; i < field1.size(); ++i)
    {
        for (size_t j = 0; j < field1[0].size(); ++j)
        {
            result[i][j] = field1[i][j][0][0]*field2[i][j][0][0] + field1[i][j][0][1]*field2[i][j][1][0] + field1[i][j][1][0]*field2[i][j][0][1] + field1[i][j][1][1]*field2[i][j][1][1];
        }
    }
    
    return result;
}

inline vectorField operator/(const vectorField& vectField, const scalarField& scalField)
{
    vectorField result = vectField;

    for (size_t i = 0; i < vectField.size(); ++i)
    {
        for (size_t j = 0; j < vectField[0].size(); ++j)
        {
            result[i][j][0] /= scalField[i][j];
            result[i][j][1] /= scalField[i][j];
        }
    }
    return result;
}

inline vectorField operator/(const vectorField& vectField, double scalar)
{
    vectorField result = vectField;

    for (size_t i = 0; i < vectField.size(); ++i)
    {
        for (size_t j = 0; j < vectField[0].size(); ++j)
        {
            result[i][j][0] /= scalar;
            result[i][j][1] /= scalar;
        }
    }
    return result;
}

scalarField getVectorComponent(vectorField field, int n)
{
    scalarField result(field.size(), std::vector<double>(field[0].size(), 0.0));
    if(n<0 || n>1){
        std::cout << "Index out of bands!" << std::endl;
        return result;
    }

    for (size_t i = 0; i < field.size(); ++i)
    {
        for (size_t j = 0; j < field[0].size(); ++j)
        {
            result[i][j] = field[i][j][n];
        }
    }
    
    return result;
}

scalarField magn(const vectorField& field)
{
    scalarField result(field.size(), std::vector<double>(field[0].size(), 0.0));
    
    for (size_t i = 0; i < field.size(); ++i)
    {
        for (size_t j = 0; j < field[0].size(); ++j)
        {
            result[i][j] = std::pow(std::pow(field[i][j][0],2) + std::pow(field[i][j][1],2), 0.5);
        }
    }
    
    return result;
}

// applicable to 2x2 tensors and vectors of 2 elements
inline vectorField operator*(const tensorField& tens, const vectorField& vect) {
    vectorField result = vect;  // same shape, will be overwritten

    for (size_t i = 0; i < tens.size(); ++i) {
        for (size_t j = 0; j < tens[0].size(); ++j) {
            result[i][j][0] = tens[i][j][0][0] * vect[i][j][0] + tens[i][j][0][1] * vect[i][j][1];
            result[i][j][1] = tens[i][j][1][0] * vect[i][j][0] + tens[i][j][1][1] * vect[i][j][1];
        }
    }
    return result;
}


inline vectorField operator*(const vectorField& field, double scalar)
{
    return scalar * field;
}


inline scalarField operator+(const scalarField& field1, const scalarField& field2)
{
    if (field1.size() != field2.size() || field1[0].size() != field2[0].size()) {
        throw std::invalid_argument("Matrix size mismatch in operator+");
    }

    scalarField result = field1;

    for (size_t i = 0; i < field1.size(); ++i) {
        for (size_t j = 0; j < field1[0].size(); ++j) {
            result[i][j] += field2[i][j];
        }
    }
    return result;
}

inline scalarField operator-(const scalarField& field1, const scalarField& field2)
{
    if (field1.size() != field2.size() || field1[0].size() != field2[0].size()) {
        throw std::invalid_argument("Matrix size mismatch in operator+");
    }

    scalarField result = field1;

    for (size_t i = 0; i < field1.size(); ++i) {
        for (size_t j = 0; j < field1[0].size(); ++j) {
            result[i][j] -= field2[i][j];
        }
    }
    return result;
}

inline vectorField operator+(const vectorField& field1, const vectorField& field2)
{
    if (field1.size() != field2.size() || field1[0].size() != field2[0].size()) {
        throw std::invalid_argument("Matrix size mismatch in operator+");
    }

    vectorField result = field1;

    for (size_t i = 0; i < field1.size(); ++i) {
        for (size_t j = 0; j < field1[0].size(); ++j) {
            for (size_t k = 0; k < field1[0][0].size(); k++)
            {
                result[i][j][k] += field2[i][j][k];
            }
        }
    }

    return result;
}

inline vectorField operator-(const vectorField& field1, const vectorField& field2)
{
    if (field1.size() != field2.size() || field1[0].size() != field2[0].size()) {
        throw std::invalid_argument("Matrix size mismatch in operator+");
    }

    vectorField result = field1;

    for (size_t i = 0; i < field1.size(); ++i) {
        for (size_t j = 0; j < field1[0].size(); ++j) {
            for (size_t k = 0; k < field1[0][0].size(); k++)
            {
                result[i][j][k] -= field2[i][j][k];
            }
        }
    }

    return result;
}
