// this code runs all the routines to generate some result with equation01
#pragma once
#include "Grid.h"
#include "Laplacian.h"
#include "Gradient.h"
#include "PressureEquation02.h"
#include "SourceTerm.h"
#include "CSVExporter.h"
#include "MyBoundaryCondition.h"
#include "Solver.h"
#include "MathOperators.h"
#include "VariableMonitor.h"
#include <map>
#include "FieldTypes.h"

void runCase1_PressureEquation02(){
    int nx = 30;
    int ny = 1;
    Grid grid(0, 0, 1, 1, nx, ny);
    
    std::map<std::string, scalarSourceTerm> scalarSourceTerms;
    std::map<std::string, vectorSourceTerm> vectorSourceTerms;
    
    scalarSourceTerms["Po"] = scalarSourceTerm(0);

    double compressibility = 1;
    double mu = 1;
    double rho = 1;
    tensor k = { { {1.0, 0.0}, {0.0, 1.0} } };

    PressureEquation02 eq(grid, compressibility, k, scalarSourceTerms, vectorSourceTerms); // arguments: grid, D, source

    scalarField initialFieldPo(ny, std::vector<double>(nx, 0.0));
    eq.initialiseField("Po", initialFieldPo);

    MyBoundaryCondition bc_pres;
    bc_pres.setNorthType(BCType::ZeroGradient);
    //bc_pres.setNorthValue(1.0);
    bc_pres.setSouthType(BCType::ZeroGradient);
    //bc_pres.setSouthValue(0.0);
    bc_pres.setEastType(BCType::FixedValue);
    bc_pres.setEastValue(1.0);
    //bc_pres.setEastGradient(1.0);
    bc_pres.setWestType(BCType::FixedValue);
    //bc_pres.setWestGradient(2.0);
    bc_pres.setWestValue(0.0);

    scalarField initialFieldSo(ny, std::vector<double>(nx, 0.7));
    eq.initialiseField("So", initialFieldSo);

    MyBoundaryCondition bc_So;
    bc_So.setNorthType(BCType::ZeroGradient);
    bc_So.setSouthType(BCType::ZeroGradient);
    bc_So.setEastType(BCType::ZeroGradient);
    bc_So.setWestType(BCType::ZeroGradient);
    //bc_So.setWestValue(0.3);


    std::map<std::string, const BoundaryCondition*> scalar_bcs;
    scalar_bcs["Po"] = &bc_pres;
    scalar_bcs["So"] = &bc_So;

    std::map<std::string, const BoundaryCondition*> vector_bcs; // this is for vector boundary conditions. not relevant for this case

    
    Solver solver(eq, grid);
    solver.solve(100, 0.001, scalar_bcs, vector_bcs);
    
    
    // === Export result ===
    // on mac
    //CSVExporter::saveToCSVWithCoordinates(grid, eq.getScalarField("temperature"), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/result.csv");
    //CSVExporter::saveMonitoredVariable(monitor1.returnMonitoredVariable(), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/time_plot1.csv");
    //CSVExporter::saveMonitoredVariable(monitor2.returnMonitoredVariable(), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/time_plot2.csv");
    
    
    // on windows
    CSVExporter::saveToCSVWithCoordinates(grid, eq.getScalarField("Po"), "C:/Users/kfedorowicz/source/repos/project_reveal/csv_files/pressure.csv");
    CSVExporter::saveToCSVWithCoordinates(grid, eq.getScalarField("So"), "C:/Users/kfedorowicz/source/repos/project_reveal/csv_files/saturation.csv");

     
    std::cout << "Simulation completed. Output saved to result.csv\n";
    
}
