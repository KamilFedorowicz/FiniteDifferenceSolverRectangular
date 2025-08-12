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

class PressureEquation02 : public EquationBase {
public:
    PressureEquation02(Grid& grid, double compressibility_, tensor k_, const std::map<std::string, scalarSourceTerm>& scalarSourceTerms_, const std::map<std::string, vectorSourceTerm>& vectorSourceTerms_)
        : grid(grid), compressibility(compressibility_), k(k_), EquationBase(grid), scalarSourceTerms(scalarSourceTerms_), vectorSourceTerms(vectorSourceTerms_)
    {
        // declare fields that we solve for
         scalarFields["Po"] = &Po;
         scalarFields["So"] = &So;

    }

    void step(std::map<std::string, const BoundaryCondition*>& scalar_bcs, std::map<std::string, const BoundaryCondition*>& vector_bcs, double dt) override
    {

        scalarField SwNormalised = calculateSwNormalised(So);
        kro = calculateKro(SwNormalised);
        krw = calculateKrw(SwNormalised);

        // equation 70. Bo and Bw assumed to be 1 and hence not written
        dPo_dt = (1 / (phi * compressibility)) * Divergence::compute(grid, (kro / mu_o) * (k * Gradient::compute(grid, Po))) + (1 / (phi * compressibility)) * Divergence::compute(grid, (krw / mu_w) * (k * Gradient::compute(grid, Po)));
        dScalarFields_dt["Po"] = &dPo_dt;

        // equation 73
        dSo_dt = (1/phi)*Divergence::compute(grid, (kro / mu_o) * (k * Gradient::compute(grid, Po)));
        // these steps ensure that the saturation does not exceed critical value
        scalarField So_new = So + dt * dSo_dt;
        So_new = maxField(minField(So_new, 1-Swr), Sor);
        dSo_dt = (So_new - So) / dt;
        dScalarFields_dt["So"] = &dSo_dt;

    }

    scalarField calculateSwNormalised(const scalarField& So) {
        scalarField ones = scalarField(grid.get_ny(), std::vector<double>(grid.get_nx(), 1.0));
        scalarField Sw = ones - So;

        scalarField SwNormalised = (Sw - Swr) / (ones - Sor - Swr);
        return SwNormalised;
    }

    scalarField calculateKro(scalarField SwNormalised) {
        scalarField result = scalarField(grid.get_ny(), std::vector<double>(grid.get_nx(), 0.0));

        result = (1 - SwNormalised) ^ (no);

        return result;
    }

    scalarField calculateKrw(scalarField SwNormalised) {
        scalarField result = scalarField(grid.get_ny(), std::vector<double>(grid.get_nx(), 0.0));

        result = SwNormalised ^ (nw);

        return result;
    }


private:
    Grid& grid;
    double compressibility, rho, mu;
    scalarField kro = scalarField(grid.get_ny(), std::vector<double>(grid.get_nx(), 1.0));
    scalarField krw = scalarField(grid.get_ny(), std::vector<double>(grid.get_nx(), 1.0));
    double phi = 1; // we can initialise it later
    double mu_o = 1;
    double mu_w = 1;
    double Swr = 0.2;
    double Sor = 0.25;
    double no = 2;
    double nw = 3;
    tensor k;

    scalarField Po;
    scalarField dPo_dt;
    scalarField So;
    scalarField dSo_dt;
    
    const std::map<std::string, scalarSourceTerm>& scalarSourceTerms;
    const std::map<std::string, vectorSourceTerm>& vectorSourceTerms;






};
