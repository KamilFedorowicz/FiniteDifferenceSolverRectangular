#pragma once
#include <vector>
#include <stdexcept>
#include "FieldTypes.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <array>

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

inline scalarField operator/(const scalarField& field, double scalar)
{
    scalarField result = field;

    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[0].size(); ++j) {
            result[i][j] /= scalar;
        }
    }
    return result;
}

inline scalarField operator/(const scalarField& field, scalarField divider)
{
    scalarField result = field;

    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[0].size(); ++j) {
            result[i][j] /= divider[i][j];
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


inline vectorField operator*(tensor tens, const vectorField& field)
{
    vectorField result = 0*field;

    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[0].size(); ++j) {
            for (size_t k = 0; k < field[0][0].size(); k++) {
                result[i][j][k] = tens[k][0]*field[i][j][0] + tens[k][1]*field[i][j][1];
            }
        }
    }
    return result;
}

inline vectorField operator*(scalarField scalar, const vectorField& field)
{
    vectorField result = 0*field;

    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[0].size(); ++j) {
            for (size_t k = 0; k < field[0][0].size(); k++) {
                result[i][j][k] = scalar[i][j]*field[i][j][k];
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

/*
inline vectorField operator*(const vectorField& vectField, const scalarField& scalField)
{
    vectorField result = vectField;

    for (size_t i = 0; i < vectField.size(); ++i)
    {
        for (size_t j = 0; j < vectField[0].size(); ++j)
        {
            result[i][j][0] *= scalField[i][j];
            result[i][j][1] *= scalField[i][j];
        }
    }
    return result;
}

inline vectorField operator*(const scalarField& scalField, const vectorField& vectField)
{
    return vectField * scalField;
}
*/

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

std::vector<double> getVectorComponent(const std::vector<std::vector<double>>& vectField, int n){
    std::vector<double> result(vectField.size(), 0);
    for(int i=0; i<vectField.size(); i++){
        result[i] = vectField[i][n];
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

double computeRootMeanSquaredValueOfVectorField(const vectorField& vectField){
    double sumSquares = 0.0;
    const scalarField vectorFieldMagnitude = magn(vectField);

    const size_t ny = vectorFieldMagnitude.size();
    const size_t nx = vectorFieldMagnitude[0].size();

    for (size_t i = 0; i < ny; ++i) {
        for (size_t j = 0; j < nx; ++j) {
            sumSquares += std::pow(vectorFieldMagnitude[i][j], 2);
        }
    }

    return std::sqrt(sumSquares / static_cast<double>(ny * nx));
}

double computeRootMeanSquaredValueOfScalarField(const scalarField& scalField){
    double sumSquares = 0.0;

    const size_t ny = scalField.size();
    const size_t nx = scalField[0].size();

    for (size_t i = 0; i < ny; ++i) {
        for (size_t j = 0; j < nx; ++j) {
            sumSquares += std::pow(scalField[i][j], 2);
        }
    }

    return std::sqrt(sumSquares / static_cast<double>(ny * nx));
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

inline scalarField operator-(const scalarField& field1, const double& val)
{
    scalarField result = field1;

    for (size_t i = 0; i < field1.size(); ++i) {
        for (size_t j = 0; j < field1[0].size(); ++j) {
            result[i][j] -= val;
        }
    }
    return result;
}

inline scalarField operator-(const double& val, const scalarField& field1)
{
    scalarField result = scalarField(field1.size(), std::vector<double>(field1[0].size(), 0.0));

    for (size_t i = 0; i < field1.size(); ++i) {
        for (size_t j = 0; j < field1[0].size(); ++j) {
            result[i][j] = val - field1[i][j];
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

inline scalarField operator^(const scalarField& field, const double& n) 
{
    scalarField result = scalarField(field.size(), std::vector<double>(field[0].size(), 1.0));

    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[0].size(); ++j) 
        {
            result[i][j] = std::pow(field[i][j], n);
        }
    }

    return result;
}

inline scalarField minField(const scalarField& field, double value)
{
    scalarField result(field.size(), std::vector<double>(field[0].size(), 0.0));

    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[i].size(); ++j) {
            result[i][j] = std::min(field[i][j], value);
        }
    }

    return result;
}

inline scalarField maxField(const scalarField& field, double value)
{
    scalarField result(field.size(), std::vector<double>(field[0].size(), 0.0));

    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[i].size(); ++j) {
            result[i][j] = std::max(field[i][j], value);
        }
    }

    return result;
}