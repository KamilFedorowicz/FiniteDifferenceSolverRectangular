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


    
    void step(const std::vector<const BoundaryCondition*>& scalar_bcs, const std::vector<const BoundaryCondition*>& vector_bcs) override {
        auto laplacianTemp = Laplacian::compute(grid, temperature);
        auto laplacianPres = Laplacian::compute(grid, pressure);
        auto sourceFieldTemp = source.compute(grid);
        // std::cout << "Working here" << std::endl;

        auto dTemp_dt = D * laplacianTemp + sourceFieldTemp; // this line does not work
        temperature = temperature + dt * dTemp_dt;
        
        auto dPres_dt = D * laplacianPres; // this line does not work
        pressure = pressure + dt * dPres_dt;

        scalar_bcs[0]->apply(temperature, grid);
        scalar_bcs[1]->apply(pressure, grid);
        
    }
    

private:
    Grid& grid;
    double D;
    double dt;
    std::vector<std::vector<double>> temperature;
    std::vector<std::vector<double>> pressure;
    const SourceTermScalar& source;

    
};
