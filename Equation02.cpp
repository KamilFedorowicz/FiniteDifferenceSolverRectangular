#include "Equation02.h"
#include "EquationFactory.h"

Equation02::Equation02(
    Grid& grid,
    std::map<std::string, double> constantsMap
)
    : grid(grid),
      EquationBase(grid)
{
    // read constants from the map
    D_director = constantsMap.at("D_director");
    D_temperature = constantsMap.at("D_temperature");
    // Declare fields that we solve for
    vectorFields["director"] = &director;
    scalarFields["temperature"] = &temperature;
}


std::vector<std::string> Equation02::getScalarVariableNames() const {
    return {"temperature"};
}

std::vector<std::string> Equation02::getVectorVariableNames() const {
    return {"director"};
}
 

std::vector<std::string> Equation02::getConstantsNames() {
    return {"D_director", "D_temperature"};
}

void Equation02::step(
    std::map<std::string, const BoundaryCondition*>& scalar_bcs,
    std::map<std::string, const BoundaryCondition*>& vector_bcs,
    double dt
)
{
    // Computing scalar derivatives
    dTemperature_dt = D_temperature * Laplacian::compute(grid, temperature);

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

REGISTER_EQUATION(Equation02);

