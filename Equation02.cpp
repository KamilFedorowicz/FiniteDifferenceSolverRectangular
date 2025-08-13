#include "Equation02.h"

Equation02::Equation02(
    Grid& grid,
    double diffusionCoeff_dir,
    double diffusionCoeff_temp,
    const std::map<std::string, scalarSourceTerm>& scalarSourceTerms_,
    const std::map<std::string, vectorSourceTerm>& vectorSourceTerms_
)
    : grid(grid),
      D_director(diffusionCoeff_dir),
      D_temperature(diffusionCoeff_temp),
      EquationBase(grid),
      scalarSourceTerms(scalarSourceTerms_),
      vectorSourceTerms(vectorSourceTerms_)
{
    // Declare fields that we solve for
    vectorFields["director"] = &director;
    scalarFields["temperature"] = &temperature;
}

void Equation02::step(
    std::map<std::string, const BoundaryCondition*>& scalar_bcs,
    std::map<std::string, const BoundaryCondition*>& vector_bcs,
    double dt
)
{
    // Computing scalar derivatives
    dTemperature_dt = D_temperature * Laplacian::compute(grid, temperature)
                    + scalarSourceTerms.at("temperature").compute(grid);

    dDirector_dt = D_director * Laplacian::compute(grid, director)
                 - (director & director) * Laplacian::compute(grid, director) * 0;

    /*
    // The following lines normalise director magnitude
    vectorField directorNew = director + dDirector_dt * dt;
    directorNew = directorNew / magn(directorNew);
    dDirector_dt = (directorNew - director) / dt;
    */

    dScalarFields_dt["temperature"] = &dTemperature_dt;
    dVectorFields_dt["director"]   = &dDirector_dt;

    //updateFields(scalar_bcs, vector_bcs, dt);
}
