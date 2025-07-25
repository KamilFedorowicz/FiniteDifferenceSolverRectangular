#pragma once
#include <vector>
#include "Grid.h"
#include "BoundaryCondition.h"
#include <variant>
#include <map>



// base for the equation class. only has the solve function which is the same for each equation

class EquationBase {
public:
    virtual ~EquationBase() = default;

    virtual void step(const std::vector<const BoundaryCondition*>& scalar_bcs, const std::vector<const BoundaryCondition*>& vector_bcs) =0;
    virtual void solve(int steps, const std::vector<const BoundaryCondition*>& scalar_bcs, const std::vector<const BoundaryCondition*>& vector_bcs) {
        for (int i = 0; i < steps; ++i) {
            step(scalar_bcs, vector_bcs);
        }
    }
    
    // we use string name as the name for the field to distinguish different fields that we have
    virtual const std::vector<std::vector<double>>& getScalarField(std::string name) const {
        static std::vector<std::vector<double>> dummy;
        return dummy;
    }

    virtual const std::vector<std::vector<std::vector<double>>>& getVectorField(std::string name) const {
        static std::vector<std::vector<std::vector<double>>> dummy;
        return dummy;
    }

    std::map< std::string, std::vector<std::vector<double>>* > scalarFields; // the second variable is a reference to the field
    std::map< std::string, std::vector<std::vector<std::vector<double>>>* > vectorFields; // the second variable is a reference to the field
};
