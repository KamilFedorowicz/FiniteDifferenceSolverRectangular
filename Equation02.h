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
    Equation02(Grid& grid, double diffusionCoeff, double dt, const SourceTermVector& source)
        : grid(grid), D(diffusionCoeff), dt(dt), source(source)
    {
         const std::vector<std::vector<std::vector<double>>> initField(grid.get_ny(), std::vector<std::vector<double>>(grid.get_nx(), std::vector<double>(0)));
         initializeField(initField);
    }

    void initializeField(const std::vector<std::vector<std::vector<double>>>& initialField) {
        director = initialField;
    }

    const std::vector<std::vector<std::vector<double>>>& getFieldVector(std::string name) const {
    if(name==directorName){
        return director;
    } else{
        std::cout << "Incorrect name, returning zero field!" << std::endl;
        std::vector<std::vector<std::vector<double>>> zeroField(grid.get_ny(), std::vector<std::vector<double>>(grid.get_nx(), std::vector<double>(0)));
            return zeroField;
    }
        
    }

    void step(const std::vector<const BoundaryCondition*>& bcs) override {
        auto laplacianDirector = Laplacian::compute(grid, director);
        auto sourceFieldDirector = source.compute(grid);
        // std::cout << "Working here" << std::endl;

        auto dDirector_dt = D * laplacianDirector + sourceFieldDirector; // this line does not work
        director = director + dt * dDirector_dt;
               
        
        //bcs[0]->apply(director, grid);

    }


private:
    Grid& grid;
    double D;
    double dt;
    std::vector<std::vector<std::vector<double>>> director;
    std::string directorName = "director";

    const SourceTermVector& source;


};