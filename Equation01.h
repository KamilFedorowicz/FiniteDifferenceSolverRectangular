#pragma once
#include <vector>
#include "Grid.h"
#include "Laplacian.h"
#include "SourceTerm.h"
#include "BoundaryCondition.h"
#include "EquationBase.h"
#include "MathOperators.h"
#include <map>
#include "MathOperators.h"

// detailed implementation of the equation

class Equation01 : public EquationBase {
public:
    Equation01(Grid& grid, double diffusionCoeff, double dt, const SourceTermScalar& source)
        : grid(grid), D(diffusionCoeff), dt(dt), source(source), EquationBase(grid)
    {
        scalarFields["temperature"] = &temperature;
        scalarFields["pressure"] = &pressure;
    }


    
    void step(std::map<std::string, const BoundaryCondition*>& scalar_bcs, std::map<std::string, const BoundaryCondition*>& vector_bcs) override {
        auto laplacianTemp = Laplacian::compute(grid, temperature);
        auto laplacianPres = Laplacian::compute(grid, pressure);
        auto sourceFieldTemp = source.compute(grid);
        // std::cout << "Working here" << std::endl;


        auto dTemp_dt = D * laplacianTemp + sourceFieldTemp; // this line does not work        
        auto dPres_dt = D * laplacianPres; // this line does not work

        /*
        temperature = temperature + dt * dTemp_dt;
        pressure = pressure + dt * dPres_dt;
        */

        dScalarFields_dt["temperature"] = &dTemp_dt;
        dScalarFields_dt["pressure"] = &dPres_dt;

        for (auto it = dScalarFields_dt.begin(); it != dScalarFields_dt.end(); it++) 
        {
            auto fieldName = it->first;
            auto dField = it->second; // pointer to derivative
            *(scalarFields[fieldName]) = *(scalarFields[fieldName]) + (*dField) * dt;

            // the first argument of apply is the field that will be updated
            scalar_bcs.at(fieldName)->apply(*(scalarFields[fieldName]), grid);
        }
        
    }
    

private:
    Grid& grid;
    double D;
    double dt;
    std::vector<std::vector<double>> temperature;
    std::vector<std::vector<double>> pressure;
    const SourceTermScalar& source;

    
};
