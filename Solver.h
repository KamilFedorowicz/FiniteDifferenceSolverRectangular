#pragma once
#include "EquationBase.h"
#include "BoundaryCondition.h"

// take the solving process away from the equation

class Solver {
public:
    Solver(EquationBase& eq) : equation(eq) {}

    void solve(int steps, const BoundaryCondition& bc) {
        equation.solve(steps, bc);
    }

    const std::vector<std::vector<double>>& getResult() const {
        return equation.getField();
    }

private:
    EquationBase& equation;
};
