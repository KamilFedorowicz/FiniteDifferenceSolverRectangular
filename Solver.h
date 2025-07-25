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

    void solve(int steps, const std::vector<const BoundaryCondition*>& scalar_bcs, const std::vector<const BoundaryCondition*>& vector_bcs) {
        equation.checkIfAllVariablesAreInitialised();
        for (int i = 0; i < steps; ++i) {
            std::cout << "Step: " << i << std::endl;
            equation.step(scalar_bcs, vector_bcs);  // one time step. function defined in Equation01 etc
        }
    }

    void updateVariables() 
    {

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
