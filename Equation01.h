#pragma once

#include <vector>
#include <map>
#include "Grid.h"
#include "Laplacian.h"
#include "SourceTerm.h"
#include "BoundaryCondition.h"
#include "EquationBase.h"
#include "MathOperators.h"

// detailed implementation of the equation

class Equation01 : public EquationBase {
public:
    Equation01(Grid& grid,
               double diffusionCoeff,
               const std::map<std::string, scalarSourceTerm>& scalarSourceTerms_,
               const std::map<std::string, vectorSourceTerm>& vectorSourceTerms_);

    void step(std::map<std::string, const BoundaryCondition*>& scalar_bcs,
              std::map<std::string, const BoundaryCondition*>& vector_bcs,
              double dt) override;

private:
    Grid& grid;
    double D;

    scalarField temperature;
    scalarField pressure;

    scalarField dTemperature_dt;
    scalarField dPressure_dt;

    const std::map<std::string, scalarSourceTerm>& scalarSourceTerms;
    const std::map<std::string, vectorSourceTerm>& vectorSourceTerms;
};
