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
    VariableMonitor monitorScalar1(grid, 0.5, 0.4, "temperature");
    solver.addScalarVariableMonitor(monitorScalar1);
    
    VariableMonitor monitorVector1(grid, 0.5, 0.4, "director");
    solver.addVectorVariableMonitor(monitorVector1);
    
    VariableMonitor monitorVector2(grid, 0.5, 0.5, "director");
    solver.addVectorVariableMonitor(monitorVector2);
    
    solver.solve(500, 0.001, scalar_bcs, vector_bcs); // arguments: steps, dt, BCs
    
    std::vector<double> monitorScalarResult1 = monitorScalar1.returnMonitoredScalarVariable();
    std::vector<std::vector<double>> monitorVectorResult1 = monitorVector1.returnMonitoredVectorVariable();
    std::vector<std::vector<double>> monitorVectorResult2 = monitorVector2.returnMonitoredVectorVariable();

    /*
    CSVExporter::saveToCSVWithCoordinates(grid, eq.getVectorField("director"), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/directorField.csv");
    CSVExporter::saveToCSVWithCoordinates(grid, eq.getVectorFieldMagnitude("director"), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/director_mag.csv");
    CSVExporter::saveToCSVWithCoordinates(grid, eq.getScalarField("temperature"), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/temp_field.csv");
    CSVExporter::saveMonitoredVariable(monitorScalarResult1, "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/temp_monitor.csv");
    CSVExporter::saveMonitoredVariable(getVectorComponent(monitorVectorResult1,0), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/dir_monitor1.csv");
    CSVExporter::saveMonitoredVariable(getVectorComponent(monitorVectorResult2,0), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/dir_monitor2.csv");
    */


    
    
     CSVExporter::saveToCSVWithCoordinates(grid, eq.getVectorField("director"), "C:/Users/kfedorowicz/source/repos/project_reveal/csv_files/dir_field.csv");
     CSVExporter::saveToCSVWithCoordinates(grid, eq.getVectorFieldMagnitude("director"), "C:/Users/kfedorowicz/source/repos/project_reveal/csv_files/dir_mag.csv");
     CSVExporter::saveToCSVWithCoordinates(grid, eq.getScalarField("temperature"), "C:/Users/kfedorowicz/source/repos/project_reveal/csv_files/temp.csv");
     
     
     
}
