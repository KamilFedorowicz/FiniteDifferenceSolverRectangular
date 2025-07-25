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
	int nx = 40;
    int ny = 50;
    Grid grid(0, 0, 1, 1, nx, ny);

    SourceTermVector source({ 0,0 });
    Equation02 eq(grid, 0.1, 0.1, 0.001, source);

    MyBoundaryCondition bc_director;
    bc_director.setNorthType(BCType::FixedValue);
    bc_director.setSouthType(BCType::FixedValue);
    bc_director.setWestType(BCType::FixedValue);
    bc_director.setEastType(BCType::FixedValue);

    bc_director.setNorthValue({ 1,0 });
    bc_director.setSouthValue({ 1,0 });
    bc_director.setWestValue({ 0,1 });
    bc_director.setEastValue({ 0,1 });

    std::vector<const BoundaryCondition*> vector_bcs;
    vector_bcs.push_back(&bc_director);

    MyBoundaryCondition bc_temperature;
    bc_temperature.setNorthType(BCType::FixedValue);
    bc_temperature.setSouthType(BCType::FixedValue);
    bc_temperature.setWestType(BCType::FixedValue);
    bc_temperature.setEastType(BCType::FixedValue);
    bc_temperature.setNorthValue(1);
    bc_temperature.setSouthValue(1);
    bc_temperature.setWestValue(0);
    bc_temperature.setEastValue(0);

    std::vector<const BoundaryCondition*> scalar_bcs;
    scalar_bcs.push_back(&bc_temperature);

    Solver solver(eq, grid);
    solver.solve(100, scalar_bcs, vector_bcs);

     CSVExporter::saveToCSVWithCoordinates(grid, eq.getVectorField("director"), "C:/Users/kfedorowicz/source/repos/my_solver2/director.csv");
     CSVExporter::saveToCSVWithCoordinates(grid, eq.getVectorFieldMagnitude("director"), "C:/Users/kfedorowicz/source/repos/my_solver2/director_mag.csv");
     CSVExporter::saveToCSVWithCoordinates(grid, eq.getScalarField("temperature"), "C:/Users/kfedorowicz/source/repos/my_solver2/temp_field.csv");

}