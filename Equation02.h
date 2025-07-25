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

class Equation02 : public EquationBase {
public:
    Equation02(Grid& grid, double diffusionCoeff_dir, double diffusionCoeff_temp, double dt, const SourceTermVector& source)
        : grid(grid), D_director(diffusionCoeff_dir), D_temperature(diffusionCoeff_temp), dt(dt), source(source), EquationBase(grid)
    {
         vectorFields["director"] = &director;
         scalarFields["temperature"] = &temperature;

    }

    void step(const std::vector<const BoundaryCondition*>& scalar_bcs, const std::vector<const BoundaryCondition*>& vector_bcs) override 
    {
        // computing scalar derivatives
        auto laplacianTemperature = Laplacian::compute(grid, temperature);
        auto dTemperature_dt = D_temperature*laplacianTemperature;

        // computing vector derivatives
        auto laplacianDirector = Laplacian::compute(grid, director);
        auto sourceFieldDirector = source.compute(grid);
        auto dDirector_dt = D_director * laplacianDirector + sourceFieldDirector; // this line does not work

        // updating fields
        temperature = temperature + dt * dTemperature_dt;
        scalar_bcs[0]->apply(temperature, grid);

        director = director + dt * dDirector_dt;   
        vector_bcs[0]->apply(director, grid);
    }

private:
    Grid& grid;
    double D_director;
    double D_temperature;
    double dt;
    std::vector<std::vector<std::vector<double>>> director;
    std::string directorName = "director";

    std::vector<std::vector<double>> temperature;
    std::string temperatureName = "temperature";

    const SourceTermVector& source;


};