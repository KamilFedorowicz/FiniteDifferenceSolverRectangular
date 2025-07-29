#pragma once
#include "EquationBase.h"
#include "BoundaryCondition.h"
#include "Grid.h"
#include "VariableMonitor.h"
#include <map>
#include "FieldTypes.h"

// take the solving process away from the equation

class Solver {
public:
    Solver(EquationBase& eq, Grid& grid) : equation(eq), grid(grid) {
        monitoredVariablesVector={};
    }

    void solve(int steps, double dt, std::map<std::string, const BoundaryCondition*>& scalar_bcs, std::map<std::string, const BoundaryCondition*>& vector_bcs) {
        equation.checkIfAllVariablesAreInitialised();
        for (int i = 0; i < steps; ++i) {
            std::cout << "Step: " << i << std::endl;
            equation.step(scalar_bcs, vector_bcs, dt);  // one time step. function defined in Equation01 etc
            updateFields(scalar_bcs, vector_bcs, dt);
            
        }
    }
    
    void updateFields(std::map<std::string, const BoundaryCondition*>& scalar_bcs, std::map<std::string, const BoundaryCondition*>& vector_bcs, double dt)
    {
        for (auto it = equation.dScalarFields_dt.begin(); it != equation.dScalarFields_dt.end(); it++)
        {
            auto fieldName = it->first;
            auto dField = it->second; // pointer to derivative
            *(equation.scalarFields[fieldName]) = *(equation.scalarFields[fieldName]) + (*dField) * dt;

            // the first argument of apply is the field that will be updated
            scalar_bcs.at(fieldName)->apply(*(equation.scalarFields[fieldName]), grid);
        }

        for (auto it = equation.dVectorFields_dt.begin(); it != equation.dVectorFields_dt.end(); it++)
        {
            auto fieldName = it->first;
            auto dField = it->second; // pointer to derivative
            *(equation.vectorFields[fieldName]) = *(equation.vectorFields[fieldName]) + (*dField) * dt;

            vector_bcs.at(fieldName)->apply(*(equation.vectorFields[fieldName]), grid);
        }
    }
    
    

    
    void addVariableMonitor(VariableMonitor& var){
        monitoredVariablesVector.push_back(&var);
    }
    
private:
    EquationBase& equation;
    std::vector<std::vector<double>> field;
    Grid& grid;
    std::vector<VariableMonitor*> monitoredVariablesVector;
    double dt;
    
};
