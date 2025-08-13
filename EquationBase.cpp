#include "EquationBase.h"
#include <iostream>
#include <cmath>
#include <stdexcept>

// Constructor
EquationBase::EquationBase(Grid& grid_) : grid(grid_) {}

// solve implementation
void EquationBase::solve(int steps,
                         std::map<std::string, const BoundaryCondition*>& scalar_bcs,
                         std::map<std::string, const BoundaryCondition*>& vector_bcs,
                         double dt)
{
    for (int i = 0; i < steps; ++i) {
        step(scalar_bcs, vector_bcs, dt);
    }
}

// initialise scalar field
void EquationBase::initialiseField(std::string name, const scalarField field) {
    auto it = scalarFields.find(name);

    if (it != scalarFields.end()) {
        if (field.size() != grid.get_ny() || field[0].size() != grid.get_nx()) {
            throw std::runtime_error("Scalar field size does not match grid dimensions.");
        }
        *(it->second) = field;
    } else {
        std::cout << "The name " << name << " is not the right name of a scalar field!" << std::endl;
    }
}

// initialise vector field
void EquationBase::initialiseField(std::string name, const vectorField field) {
    auto it = vectorFields.find(name);
    if (it != vectorFields.end()) {
        if (field.size() != grid.get_ny() || field[0].size() != grid.get_nx()) {
            throw std::runtime_error("Vector field size does not match grid dimensions.");
        }
        *(it->second) = field;
    } else {
        std::cout << "The name " << name << " is not the right name of a vector field!" << std::endl;
    }
}

// check scalar fields
void EquationBase::checkIfScalarFieldsAreInitialised() {
    for (const auto& it : scalarFields) {
        const std::string& name = it.first;
        const auto* fieldPtr = it.second;

        std::cout << "Name: " << name << ", ptr: " << fieldPtr << std::endl;

        if (!fieldPtr) {
            std::cout << "Field " << name << " is not initialised!" << std::endl;
            throw std::runtime_error("Field not initialised!.");
        }
        else if (fieldPtr->empty() || (*fieldPtr)[0].empty()) {
            std::cout << "Field " << name << " is empty!" << std::endl;
            throw std::runtime_error("Field is empty.");
        }
    }
}

// check vector fields
void EquationBase::checkIfVectorFieldsAreInitialsed() {
    for (const auto& it : vectorFields) {
        const std::string& name = it.first;
        const auto* fieldPtr = it.second;

        std::cout << "Name: " << name << ", ptr: " << fieldPtr << std::endl;

        if (!fieldPtr) {
            std::cout << "Field " << name << " is not initialised!" << std::endl;
            throw std::runtime_error("Field not initialised!.");
        }
        else if (fieldPtr->empty() || (*fieldPtr)[0].empty()) {
            std::cout << "Field " << name << " is empty!" << std::endl;
            throw std::runtime_error("Field is empty.");
        }
    }
}

// check all variables
void EquationBase::checkIfAllVariablesAreInitialised() {
    checkIfScalarFieldsAreInitialised();
    checkIfVectorFieldsAreInitialsed();
}

// get scalar field with static fallback
const scalarField& EquationBase::getScalarField(std::string name) const
{
    auto it = scalarFields.find(name);
    if (it != scalarFields.end()) {
        return *(it->second);
    } else {
        static scalarField zeroField(grid.get_ny(), std::vector<double>(grid.get_nx(), 0.0));
        std::cout << "Incorrect name " << name << ", returning zero field!" << std::endl;
        return zeroField;
    }
}

// get vector field with static fallback
const vectorField& EquationBase::getVectorField(std::string name) const
{
    auto it = vectorFields.find(name);
    if (it != vectorFields.end()) {
        return *(it->second);
    } else {
        static vectorField zeroField(grid.get_ny(), std::vector<std::vector<double>>(grid.get_nx(), std::vector<double>(0)));
        std::cout << "Incorrect name, returning zero field!" << std::endl;
        return zeroField;
    }
}

// get vector magnitude (returns by value)
const scalarField EquationBase::getVectorFieldMagnitude(std::string name) const
{
    scalarField result(grid.get_ny(), std::vector<double>(grid.get_nx(), 0));

    auto it = vectorFields.find(name);
    if (it != vectorFields.end()) {
        for (int i = 0; i < grid.get_ny(); i++) {
            for (int j = 0; j < grid.get_nx(); j++) {
                double value_x = (*(it->second))[i][j][0];
                double value_y = (*(it->second))[i][j][1];
                result[i][j] = std::sqrt(value_x * value_x + value_y * value_y);
            }
        }
    } else {
        std::cout << "Incorrect name, returning zero field!" << std::endl;
    }

    return result;
}