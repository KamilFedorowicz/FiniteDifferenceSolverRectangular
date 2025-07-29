#pragma once
#include "Grid.h"
#include "Laplacian.h"
#include "Gradient.h"
#include "Equation02.h"
#include "SourceTerm.h"
#include "CSVExporter.h"
#include "MyBoundaryCondition.h"
#include "Solver.h"
#include "MathOperators.h"
#include "VariableMonitor.h"

void runCase1_Equation02() {
    const int nx = 40;
    const int ny = 50;
    Grid grid(0, 0, 1, 1, nx, ny);

    std::map<std::string, scalarSourceTerm> scalarSourceTerms;
    std::map<std::string, vectorSourceTerm> vectorSourceTerms;
    
    scalarSourceTerms["temperature"] = scalarSourceTerm(0);
    vectorSourceTerms["director"] = vectorSourceTerm({ 0,1 });
    
    Equation02 eq(grid, 0.1, 0.1, scalarSourceTerms, vectorSourceTerms); // arguments: grid, diff coeff for for director, diff coeff for temperature

    scalarField initialTemperatureField(ny, std::vector<double>(nx, 0.0));
    vectorField initialDirectorField(ny, std::vector<std::vector<double>>(nx, std::vector<double>{1,0}));
    eq.initialiseField("temperature", initialTemperatureField);
    eq.initialiseField("director", initialDirectorField);

    MyBoundaryCondition bc_director;
    bc_director.setNorthType(BCType::FixedValue);
    bc_director.setSouthType(BCType::FixedValue);
    bc_director.setWestType(BCType::FixedValue);
    bc_director.setEastType(BCType::FixedValue);

    bc_director.setNorthValue({ 1,0 });
    bc_director.setSouthValue({ 1,0 });
    bc_director.setWestValue({ 0,1 });
    bc_director.setEastValue({ 0,1 });

    std::map<std::string, const BoundaryCondition*> vector_bcs;
    vector_bcs["director"] = &bc_director;

    MyBoundaryCondition bc_temperature;
    bc_temperature.setNorthType(BCType::FixedValue);
    bc_temperature.setSouthType(BCType::FixedValue);
    bc_temperature.setWestType(BCType::FixedValue);
    bc_temperature.setEastType(BCType::FixedValue);
    bc_temperature.setNorthValue(1);
    bc_temperature.setSouthValue(1);
    bc_temperature.setWestValue(0);
    bc_temperature.setEastValue(0);

    std::map<std::string, const BoundaryCondition*> scalar_bcs;
    scalar_bcs["temperature"] = &bc_temperature;

    Solver solver(eq, grid);
    solver.solve(1000, 0.001, scalar_bcs, vector_bcs); // arguments: steps, dt, BCs

    CSVExporter::saveToCSVWithCoordinates(grid, eq.getVectorField("director"), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/directorField.csv");
    CSVExporter::saveToCSVWithCoordinates(grid, eq.getVectorFieldMagnitude("director"), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/director_mag.csv");
    CSVExporter::saveToCSVWithCoordinates(grid, eq.getScalarField("temperature"), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/temp_field.csv");

    
    /*
     CSVExporter::saveToCSVWithCoordinates(grid, eq.getVectorField("director"), "C:/Users/kfedorowicz/source/repos/my_solver2/director.csv");
     CSVExporter::saveToCSVWithCoordinates(grid, eq.getVectorFieldMagnitude("director"), "C:/Users/kfedorowicz/source/repos/my_solver2/director_mag.csv");
     CSVExporter::saveToCSVWithCoordinates(grid, eq.getScalarField("temperature"), "C:/Users/kfedorowicz/source/repos/my_solver2/temp_field.csv");
     */
     
     
}
