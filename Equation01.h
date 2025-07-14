#pragma once
#include <vector>
#include "Grid.h"
#include "Laplacian.h"
#include "SourceTerm.h"
#include "BoundaryCondition.h"
#include "EquationBase.h"
#include "MathOperators.h"
#include <map>

// detailed implementation of the equation

class Equation01 : public EquationBase {
public:
    Equation01(Grid& grid, double diffusionCoeff, double dt, const SourceTerm& source)
        : grid(grid), D(diffusionCoeff), dt(dt), source(source)
    {
        // initial field set to 0 by default
        const std::vector<std::vector<double>> initField(grid.ny, std::vector<double>(grid.nx, 0));
        initializeField(initField);
    }

    void initializeField(const std::vector<std::vector<double>>& initialField) {
        field = initialField;
    }

    void step(const BoundaryCondition& bc) override {
        auto laplacian = Laplacian::compute(grid, field);
        auto sourceField = source.compute(grid);
        // std::cout << "Working here" << std::endl;

        auto dField_dt = D * laplacian + sourceField; // this line does not work
        
        field = field + dt * dField_dt;
        bc.apply(field, grid);
    }

    // we need this to keep the field public
    const std::vector<std::vector<double>> getField(std::string name) const override {
        if(name==fieldName1){
            return field;
        } // add here more ifs if more field types
        else
        {
            std::vector<std::vector<double>> zeroField(field.size(), std::vector<double>(field[0].size(), 0.0));
                return zeroField;
        }
        
    }

private:
    Grid& grid;
    double D;
    double dt;
    std::vector<std::vector<double>> field;
    std::string fieldName1 = "field01";
    const SourceTerm& source;
    
};
