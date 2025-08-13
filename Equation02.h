#pragma once

#include <vector>
#include <map>
#include "Grid.h"
#include "Laplacian.h"
#include "SourceTerm.h"
#include "BoundaryCondition.h"
#include "EquationBase.h"
#include "MathOperators.h"

class Equation02 : public EquationBase {
public:
    Equation02(
        Grid& grid,
        double diffusionCoeff_dir,
        double diffusionCoeff_temp,
        const std::map<std::string, scalarSourceTerm>& scalarSourceTerms_,
        const std::map<std::string, vectorSourceTerm>& vectorSourceTerms_
    );

    void step(
        std::map<std::string, const BoundaryCondition*>& scalar_bcs,
        std::map<std::string, const BoundaryCondition*>& vector_bcs,
        double dt
    ) override;

private:
    Grid& grid;
    double D_director;
    double D_temperature;
    double dt;

    vectorField director;
    vectorField dDirector_dt;

    scalarField temperature;
    scalarField dTemperature_dt;

    const std::map<std::string, scalarSourceTerm>& scalarSourceTerms;
    const std::map<std::string, vectorSourceTerm>& vectorSourceTerms;
};
