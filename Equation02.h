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
        : grid(grid), D_director(diffusionCoeff_dir), D_temperature(diffusionCoeff_temp), dt(dt), source(source)
    {
        const std::vector<std::vector<std::vector<double>>> initFieldDirector(grid.get_ny(), std::vector<std::vector<double>>(grid.get_nx(), std::vector<double>{1,0}));
        initialiseField(initFieldDirector);

         const std::vector<std::vector<double>> initFieldTemperature(grid.get_ny(), std::vector<double>(grid.get_nx(), 0));
         initialiseField(initFieldTemperature);

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

    void initialiseField(const std::vector<std::vector<std::vector<double>>>& initialField) {
        director = initialField;
    }

    void initialiseField(const std::vector<std::vector<double>>& initialField) {
        temperature = initialField;
    }

    const std::vector<std::vector<std::vector<double>>>& getVectorField(std::string name) const
    {  
        auto it = vectorFields.find(name);
        if (it!=vectorFields.end()) {
            return *(it->second); // dereference the pointer
        }else{
            std::cout << "Incorrect name, returning zero field!" << std::endl;
            std::vector<std::vector<std::vector<double>>> zeroField(grid.get_ny(), std::vector<std::vector<double>>(grid.get_nx(), std::vector<double>(0)));
                return zeroField;
        }
    }

    const std::vector<std::vector<double>> getVectorFieldMagnitude(std::string name) const
    {
        std::vector<std::vector<double>> result(grid.get_ny(), std::vector<double>(grid.get_nx(), 0));

        auto it = vectorFields.find(name);
        if (it != vectorFields.end()) 
        {
            for (int i = 0; i < grid.get_ny(); i++) 
            {
                for (int j = 0; j < grid.get_nx(); j++)
                {
                    double value_x = (*(it->second))[i][j][0];
                    double value_y = (*(it->second))[i][j][1];
                    result[i][j] = std::sqrt( std::pow(value_x,2) + std::pow(value_y,2) ); // if a vector field is found, update the result
                }
            }

        } else
        {
            std::cout << "Incorrect name, returning zero field!" << std::endl;
        }

        return result;
    }

    const std::vector<std::vector<double>>& getScalarField(std::string name) const 
    {  
        auto it = scalarFields.find(name);
        if (it!=scalarFields.end()) {
            return *(it->second); // dereference the pointer
        }else{
            std::cout << "Incorrect name, returning zero field!" << std::endl;
            std::vector<std::vector<double>> zeroField(grid.get_ny(), std::vector<double>(grid.get_nx(), 0.0));
                return zeroField;
        }
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