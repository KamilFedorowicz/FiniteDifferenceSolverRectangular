#pragma once
#include <vector>
#include "Grid.h"
#include "Laplacian.h"
#include "Divergence.h"
#include "SourceTerm.h"
#include "BoundaryCondition.h"
#include "EquationBase.h"
#include "MathOperators.h"
#include <map>
#include "MathOperators.h"

class PressureEquation01 : public EquationBase {
public:
    PressureEquation01(Grid& grid, double compressibility_, double rho_, double mu_, tensor k_, const std::map<std::string, scalarSourceTerm>& scalarSourceTerms_, const std::map<std::string, vectorSourceTerm>& vectorSourceTerms_)
        : grid(grid), compressibility(compressibility_), rho(rho_), mu(mu_), k(k_), EquationBase(grid), scalarSourceTerms(scalarSourceTerms_), vectorSourceTerms(vectorSourceTerms_)
    {
        // declare fields that we solve for
         scalarFields["pressure"] = &pressure;

    }

    void step(std::map<std::string, const BoundaryCondition*>& scalar_bcs, std::map<std::string, const BoundaryCondition*>& vector_bcs, double dt) override
    {
        // computing scalar derivatives
        dPressure_dt = (1 / compressibility) * Divergence::compute(grid, (rho / mu) * (k * Gradient::compute(grid, pressure))) - 1e-3*pressure ;
        
        dScalarFields_dt["pressure"] = &dPressure_dt;

    }




private:
    Grid& grid;
    double compressibility, rho, mu;
    tensor k;

    scalarField pressure;
    scalarField dPressure_dt;
    
    const std::map<std::string, scalarSourceTerm>& scalarSourceTerms;
    const std::map<std::string, vectorSourceTerm>& vectorSourceTerms;






};
