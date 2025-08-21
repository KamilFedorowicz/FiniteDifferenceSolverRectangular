#include "Equation01.h"

Equation01::Equation01(Grid& grid,
                       double diffusionCoeff,
                       const std::map<std::string, scalarSourceTerm>& scalarSourceTerms_,
                       const std::map<std::string, vectorSourceTerm>& vectorSourceTerms_)
    : grid(grid),
      D(diffusionCoeff),
      EquationBase(grid),
      scalarSourceTerms(scalarSourceTerms_),
      vectorSourceTerms(vectorSourceTerms_)
{
    scalarFields["temperature"] = &temperature;
    scalarFields["pressure"] = &pressure;
}

void Equation01::step(std::map<std::string, const BoundaryCondition*>& scalar_bcs,
                      std::map<std::string, const BoundaryCondition*>& vector_bcs,
                      double dt)
{
    dTemperature_dt = D * Laplacian::compute(grid, temperature)
                    + Laplacian::compute(grid, temperature)
                    + scalarSourceTerms.at("temperature").compute(grid);

    dPressure_dt = D * Laplacian::compute(grid, pressure);

    dScalarFields_dt["temperature"] = &dTemperature_dt;
    dScalarFields_dt["pressure"] = &dPressure_dt;
}
