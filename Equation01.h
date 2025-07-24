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
        : grid(grid), D(diffusionCoeff), dt(dt), source(source)
    {
        // initial field set to 0 by default
        const std::vector<std::vector<double>> initField(grid.get_ny(), std::vector<double>(grid.get_nx(), 0));
        initializeField(initField);
    }

    void initializeField(const std::vector<std::vector<double>>& initialField) {
        temperature = initialField;
        pressure = initialField;
    }
    
    void step(const std::vector<const BoundaryCondition*>& bcs) override {
        auto laplacianTemp = Laplacian::compute(grid, temperature);
        auto laplacianPres = Laplacian::compute(grid, pressure);
        auto sourceFieldTemp = source.compute(grid);
        // std::cout << "Working here" << std::endl;

        auto dTemp_dt = D * laplacianTemp + sourceFieldTemp; // this line does not work
        temperature = temperature + dt * dTemp_dt;
        
        auto dPres_dt = D * laplacianPres; // this line does not work
        pressure = pressure + dt * dPres_dt;
        
        
        bcs[0]->apply(temperature, grid);
        bcs[1]->apply(pressure, grid);
        
    }
    
    // we need this to keep the field public
    const std::vector<std::vector<double>>& getFieldScalar(std::string name) const {
        if(name==fieldName1){
            return temperature;
        } // add here more ifs if more field types
        else if(name==fieldName2){
            return pressure;
        } else{
            std::cout << "Incorrect name, returning zero field!" << std::endl;
            std::vector<std::vector<double>> zeroField(temperature.size(), std::vector<double>(temperature[0].size(), 0.0));
                return zeroField;
        }
        
    }

private:
    Grid& grid;
    double D;
    double dt;
    std::vector<std::vector<double>> temperature;
    std::string fieldName1 = "temperature";
    std::vector<std::vector<double>> pressure;
    std::string fieldName2 = "pressure";
    const SourceTermScalar& source;
    
};
