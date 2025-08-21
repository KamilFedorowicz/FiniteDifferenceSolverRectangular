// this code runs all the routines to generate some result with equation01
#pragma once
#include "Grid.h"
#include "Laplacian.h"
#include "Gradient.h"
#include "Equation01.h"
#include "SourceTerm.h"
#include "CSVExporter.h"
#include "MyBoundaryCondition.h"
#include "Solver.h"
#include "MathOperators.h"
#include "VariableMonitor.h"
#include <map>
#include "FieldTypes.h"

void runCase1_Equation01(){
    int nx = 40;
    int ny = 50;
    Grid grid(0, 0, 1, 1, nx, ny);
    
    std::map<std::string, scalarSourceTerm> scalarSourceTerms;
    std::map<std::string, vectorSourceTerm> vectorSourceTerms;
    
    scalarSourceTerms["temperature"] = scalarSourceTerm(0);
    Equation01 eq(grid, 0.1, scalarSourceTerms, vectorSourceTerms); // arguments: grid, D, source
    scalarField initialField(ny, std::vector<double>(nx, 0.0));
    
    eq.initialiseField("temperature", initialField);
    eq.initialiseField("pressure", initialField);

    MyBoundaryCondition bc_temp;
    bc_temp.setNorthType(BCType::FixedValue);
    bc_temp.setNorthValue(0.0);
    bc_temp.setSouthType(BCType::FixedValue);
    bc_temp.setSouthValue(0.0);
    bc_temp.setEastType(BCType::FixedValue);
    bc_temp.setEastValue(0.0);
    bc_temp.setWestType(BCType::FixedValue);
    bc_temp.setWestValue(0.0);
    
    bc_temp.setObstacle(0.0, 0.0, 0.3, 0.3, 1.0);
    bc_temp.setObstacle(0.8, 0.8, 1.0, 1.0, 1.0);
    
    MyBoundaryCondition bc_pres;
    bc_pres.setNorthType(BCType::FixedValue);
    bc_pres.setNorthValue(1.0);
    bc_pres.setSouthType(BCType::FixedValue);
    bc_pres.setSouthValue(1.0);
    bc_pres.setEastType(BCType::FixedValue);
    bc_pres.setEastValue(1.0);
    bc_pres.setWestType(BCType::FixedValue);
    bc_pres.setWestValue(1.0);
    


    std::map<std::string, const BoundaryCondition*> scalar_bcs;
    scalar_bcs["temperature"] = &bc_temp;
    scalar_bcs["pressure"] = &bc_pres;


    std::map<std::string, const BoundaryCondition*> vector_bcs; // this is for vector boundary conditions. not relevant for this case
    
    // std::cout << "Working here" << std::endl;
    
    // something is wrong with VariableMonitor
    VariableMonitor monitor1(grid, 0.2, 0.2, "temperature");
    VariableMonitor monitor2(grid, 0.6, 0.6, "pressure");
    
    Solver solver(eq, grid);
    
    solver.addScalarVariableMonitor(monitor1);
    solver.addScalarVariableMonitor(monitor2);
    
    
    solver.solve(50, 0.0001, scalar_bcs, vector_bcs);
    
    
    // === Export result ===
    // on mac
    
    //CSVExporter::saveToCSVWithCoordinates(grid, eq.getScalarField("temperature"), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/result.csv");
    //CSVExporter::saveMonitoredVariable(monitor1.returnMonitoredVariable(), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/time_plot1.csv");
    //CSVExporter::saveMonitoredVariable(monitor2.returnMonitoredVariable(), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/time_plot2.csv");
    
    /*
    // on windows
    CSVExporter::saveToCSVWithCoordinates(grid, eq.getScalarField("temperature"), "C:/Users/kfedorowicz/source/repos/my_solver2/result.csv");
    CSVExporter::saveMonitoredVariable(monitor1.returnMonitoredVariable(), "C:/Users/kfedorowicz/source/repos/my_solver2/time_plot1.csv");
    CSVExporter::saveMonitoredVariable(monitor2.returnMonitoredVariable(), "C:/Users/kfedorowicz/source/repos/my_solver2/time_plot2.csv");
    */
     
    std::cout << "Simulation completed. Output saved to result.csv\n";
    
}
