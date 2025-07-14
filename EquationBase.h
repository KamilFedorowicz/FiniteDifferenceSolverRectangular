#pragma once
#include <vector>
#include "Grid.h"
#include "BoundaryCondition.h"

// base for the equation class. only has the solve function which is the same for each equation

class EquationBase {
public:
    virtual ~EquationBase() = default;

    virtual void step(const std::vector<const BoundaryCondition*>& bcs) = 0;
    virtual void solve(int steps, const std::vector<const BoundaryCondition*>& bcs) {
        for (int i = 0; i < steps; ++i) {
            step(bcs);
        }
    }
    
    // we use string name as the name for the field to distinguish different fields that we have
    virtual const std::vector<std::vector<double>> getField(std::string name) const = 0;
};
