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

void runCase1_Equation01(){
    int nx = 40;
    int ny = 50;
    Grid grid(0, 0, 1, 1, nx, ny);
    
    
    
    SourceTerm source(1.0);
    Equation01 eq(grid, 0.1, 0.001, source); // arguments: grid, D, dt, source
    //std::vector<std::vector<double>> initialField(ny, std::vector<double>(nx, 0.0));
    // eq.initializeField(initialField); // no need to initialise the field because this is done in the constructor
    
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
    
    std::vector<const BoundaryCondition*> bcs;
    bcs.push_back(&bc_temp);
    bcs.push_back(&bc_pres);
    
    
    // std::cout << "Working here" << std::endl;
    
    // something is wrong with VariableMonitor
    VariableMonitor monitor1(grid, 0.2, 0.2, "temperature");
    VariableMonitor monitor2(grid, 0.6, 0.6, "pressure");
    
    Solver solver(eq, grid);
    
    solver.addVariableMonitor(monitor1);
    solver.addVariableMonitor(monitor2);
    
    
    solver.solve(5000, bcs);
    
    
    // === Export result ===
    CSVExporter::saveToCSVWithCoordinates(grid, solver.getResult("temperature"), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/result.csv");
    CSVExporter::saveMonitoredVariable(monitor1.returnMonitoredVariable(), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/time_plot1.csv");
    CSVExporter::saveMonitoredVariable(monitor2.returnMonitoredVariable(), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/time_plot2.csv");
    
    std::cout << "Simulation completed. Output saved to result.csv\n";
    
}
