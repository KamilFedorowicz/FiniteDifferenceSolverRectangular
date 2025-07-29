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
    Equation01(Grid& grid, double diffusionCoeff, const std::map<std::string, scalarSourceTerm>& scalarSourceTerms_, const std::map<std::string, vectorSourceTerm>& vectorSourceTerms_)
        : grid(grid), D(diffusionCoeff), EquationBase(grid), scalarSourceTerms(scalarSourceTerms_), vectorSourceTerms(vectorSourceTerms_)
    {
        scalarFields["temperature"] = &temperature;
        scalarFields["pressure"] = &pressure;
    }
    
    void step(std::map<std::string, const BoundaryCondition*>& scalar_bcs, std::map<std::string, const BoundaryCondition*>& vector_bcs, double dt) override
    {
        dTemperature_dt = D * Laplacian::compute(grid, temperature) + Laplacian::compute(grid, temperature) + scalarSourceTerms.at("temperature").compute(grid);
        dPressure_dt = D * Laplacian::compute(grid, pressure) ;

        
        dScalarFields_dt["temperature"] = &dTemperature_dt;
        dScalarFields_dt["pressure"] = &dPressure_dt;
    }

private:
    Grid& grid;
    double D;
    scalarField temperature;
    scalarField pressure;
    
    scalarField dTemperature_dt;
    scalarField dPressure_dt;
    
    const std::map<std::string, scalarSourceTerm>& scalarSourceTerms;
    const std::map<std::string, vectorSourceTerm>& vectorSourceTerms;

    
};
