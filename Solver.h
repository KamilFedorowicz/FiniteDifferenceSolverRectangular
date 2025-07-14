#pragma once
#include "EquationBase.h"
#include "BoundaryCondition.h"
#include "Grid.h"
#include "VariableMonitor.h"
#include <map>

// take the solving process away from the equation

class Solver {
public:
    Solver(EquationBase& eq, Grid& grid) : equation(eq), grid(grid) {
        monitoredVariablesVector={};
    }

    void solve(int steps, const std::vector<const BoundaryCondition*>& bcs) {
        for (int i = 0; i < steps; ++i) {
            equation.step(bcs);  // one time step. function defined in Equation01 etc
            
            // monitoring variable evolution
            for(VariableMonitor* var: monitoredVariablesVector){
                int ix = var->getIx();
                int iy = var->getIy();
                var->update(getResult(var->getName())[ix][iy]);
            }

        }
    }
    
    const std::vector<std::vector<double>> getResult(std::string name) const {
        return equation.getField(name);
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
