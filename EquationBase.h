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
    
    virtual const std::vector<std::vector<double>>& getField(const std::string& name) const {
        throw std::runtime_error("getField(name) not implemented in derived class");
    }

    virtual const std::vector<std::vector<double>>& getField() const = 0;
};
