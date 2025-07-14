#pragma once
#include <vector>
#include "Grid.h"
#include "BoundaryCondition.h"

// base for the equation class. only has the solve function which is the same for each equation

class EquationBase {
public:
    virtual ~EquationBase() = default;

    virtual void step(const BoundaryCondition& bc) = 0;
    virtual void solve(int steps, const BoundaryCondition& bc) {
        
        for (int i = 0; i < steps; ++i) {
            step(bc);
        }
    }
    
    // we use string name as the name for the field to distinguish different fields that we have
    virtual const std::vector<std::vector<double>> getField(std::string name) const = 0;
};
