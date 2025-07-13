#pragma once
#include "EquationBase.h"
#include "BoundaryCondition.h"
#include "Grid.h"
#include "VariableMonitor.h"

// take the solving process away from the equation

class Solver {
public:
    Solver(EquationBase& eq, Grid& grid) : equation(eq), grid(grid) {
        monitoredVariablesVector={};
    }

    void solve(int steps, const BoundaryCondition& bc) {
        for (int i = 0; i < steps; ++i) {
            equation.step(bc);  // one time step. function defined in Equation01 etc
            for(VariableMonitor* var:monitoredVariablesVector){
                int ix = var->getIx();
                int iy = var->getIy();
                var->update(getResult()[ix][iy]);
            }

        }
    }
    
    const std::vector<std::vector<double>>& getResult() const {
        return equation.getField();
    }
    
    void addVariableMonitor(VariableMonitor& var){
        monitoredVariablesVector.push_back(&var);
    }
    
private:
    EquationBase& equation;
    std::vector<std::vector<double>> field;
    Grid& grid;
    std::vector<VariableMonitor*> monitoredVariablesVector;
};
