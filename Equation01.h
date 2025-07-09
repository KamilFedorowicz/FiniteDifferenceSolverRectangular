#pragma once
#include <vector>
#include "Grid.h"
#include "Laplacian.h"
#include "SourceTerm.h"
#include "BoundaryCondition.h"
#include "EquationBase.h"
#include "MathOperators.h"

// detailed implementation of the equation

class Equation01 : public EquationBase {
public:
    Equation01(Grid& grid, double diffusionCoeff, double dt, const SourceTerm& source)
        : grid(grid), D(diffusionCoeff), dt(dt), source(source) {}

    void initializeField(const std::vector<std::vector<double>>& initialField) {
        field = initialField;
    }

    void step(const BoundaryCondition& bc) override {
        auto laplacian = Laplacian::compute(grid, field);
        auto sourceField = source.compute(grid);

        // main equation of the model
        auto dField_dt = D * laplacian + sourceField;

        field = field + dt * dField_dt;

        bc.apply(field, grid);
    }

    // we need this to keep the field public
    const std::vector<std::vector<double>>& getField() const override {
        return field;
    }

private:
    Grid& grid;
    double D;
    double dt;
    std::vector<std::vector<double>> field;
    const SourceTerm& source;
};
