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
    Solver(EquationBase& eq, Grid& grid) : equation(eq), grid(grid)
    {
        vectorOfMonitoredScalarVariables={};
    }

    void solve(int steps, double dt, std::map<std::string, const BoundaryCondition*>& scalar_bcs, std::map<std::string, const BoundaryCondition*>& vector_bcs)
    {
        equation.checkIfAllVariablesAreInitialised();
        for (int i = 0; i < steps; ++i)
        {
            std::cout << "Step: " << i << std::endl;
            equation.step(scalar_bcs, vector_bcs, dt);  // one time step. function defined in Equation01 etc
            updateFields(scalar_bcs, vector_bcs, dt); // equation.step calculates d/dt, updateFields calculates values after the time step
            updateMonitors();
            
        }
    }
    
    void updateFields(std::map<std::string, const BoundaryCondition*>& scalar_bcs, std::map<std::string, const BoundaryCondition*>& vector_bcs, double dt)
    {
        for (auto it = equation.dScalarFields_dt.begin(); it != equation.dScalarFields_dt.end(); it++)
        {
            auto fieldName = it->first;
            auto dField = it->second; // pointer to derivative
            
            scalarField oldField = *(equation.scalarFields[fieldName]);
            *(equation.scalarFields[fieldName]) = *(equation.scalarFields[fieldName]) + (*dField) * dt;

            // the first argument of apply is the field that will be updated
            scalar_bcs.at(fieldName)->apply(*(equation.scalarFields[fieldName]), grid);
            scalarField newField = *(equation.scalarFields[fieldName]);
            
            double error = computeRootMeanSquaredValueOfScalarField(newField - oldField); // calculate the error
            std::cout<< "Scalar field: " << fieldName << ", error: " << error << std::endl;
        }

        for (auto it = equation.dVectorFields_dt.begin(); it != equation.dVectorFields_dt.end(); it++)
        {
            auto fieldName = it->first;
            auto dField = it->second; // pointer to derivative
            
            vectorField oldField = *(equation.vectorFields[fieldName]); // get the old field
            *(equation.vectorFields[fieldName]) = *(equation.vectorFields[fieldName]) + (*dField) * dt; // calculate the new field
            vectorField newField = *(equation.vectorFields[fieldName]); // value of the new field
            
            vector_bcs.at(fieldName)->apply(*(equation.vectorFields[fieldName]), grid);
            
            double error = computeRootMeanSquaredValueOfVectorField(newField - oldField); // calculate the error
            std::cout<< "Vector field: " << fieldName << ", error: " << error << std::endl;
        }
    }
    
    void addScalarVariableMonitor(VariableMonitor& var)
    {
        vectorOfMonitoredScalarVariables.push_back(&var);
    }
    
    void addVectorVariableMonitor(VariableMonitor& var)
    {
        vectorOfMonitoredVectorVariables.push_back(&var);
    }
    
    void updateMonitors()
    {
        for(VariableMonitor* var: vectorOfMonitoredScalarVariables)
        {
            const std::string tempName = var->getName();
            if(equation.scalarFields.find(tempName) != equation.scalarFields.end())
            {
                const scalarField tempField = equation.getScalarField(tempName);
                var->updateMonitoredScalarVariable(tempField[var->getIx()][var->getIy()]);
            }else
            {
                std::cout << tempName << " string was not found in the variable map keys." << std::endl;
            }
        }
        
        for(VariableMonitor* var: vectorOfMonitoredVectorVariables)
        {
            const std::string tempName = var->getName();
            if(equation.vectorFields.find(tempName) != equation.vectorFields.end())
            {
                const vectorField tempField = equation.getVectorField(tempName);
                var->updateMonitoredVectorVariable(tempField[var->getIx()][var->getIy()]);
            }else
            {
                std::cout << tempName << " string was not found in the variable map keys." << std::endl;
            }
        }
        
        
        
        
        
    }
    
private:
    double dt;
    EquationBase& equation;
    std::vector<std::vector<double>> field;
    Grid& grid;
    std::vector<VariableMonitor*> vectorOfMonitoredScalarVariables;
    std::vector<VariableMonitor*> vectorOfMonitoredVectorVariables;
    std::map<std::string, std::vector<double>*> mapOfMonitoredVariables;
    
};
