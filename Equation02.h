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
        // declare fields that we solve for
         vectorFields["director"] = &director;
         scalarFields["temperature"] = &temperature;

    }

    void step(std::map<std::string, const BoundaryCondition*>& scalar_bcs, std::map<std::string, const BoundaryCondition*>& vector_bcs) override
    {
        // computing scalar derivatives
        dTemperature_dt = D_temperature * Laplacian::compute(grid, temperature);
        dDirector_dt = D_director * Laplacian::compute(grid, director) + source.compute(grid);

        dScalarFields_dt["temperature"] = &dTemperature_dt;
        dVectorFields_dt["director"] = &dDirector_dt;
        
        //updateFields(scalar_bcs, vector_bcs, dt);
    }

    /*
    void updateFields(std::map<std::string, const BoundaryCondition*>& scalar_bcs, std::map<std::string, const BoundaryCondition*>& vector_bcs, double dt)
    {
        for (auto it = dScalarFields_dt.begin(); it != dScalarFields_dt.end(); it++)
        {
            auto fieldName = it->first;
            auto dField = it->second; // pointer to derivative
            *(scalarFields[fieldName]) = *(scalarFields[fieldName]) + (*dField) * dt;

            // the first argument of apply is the field that will be updated
            scalar_bcs.at(fieldName)->apply(*(scalarFields[fieldName]), grid);
        }

        for (auto it = dVectorFields_dt.begin(); it != dVectorFields_dt.end(); it++)
        {
            auto fieldName = it->first;
            auto dField = it->second; // pointer to derivative
            *(vectorFields[fieldName]) = *(vectorFields[fieldName]) + (*dField) * dt;

            vector_bcs.at(fieldName)->apply(*(vectorFields[fieldName]), grid);
        }
    }
     */



private:
    Grid& grid;
    double D_director;
    double D_temperature;
    double dt;
    std::vector<std::vector<std::vector<double>>> director;
    std::vector<std::vector<std::vector<double>>> dDirector_dt;

    std::vector<std::vector<double>> temperature;
    std::vector<std::vector<double>> dTemperature_dt;

    const SourceTermVector& source;





};
