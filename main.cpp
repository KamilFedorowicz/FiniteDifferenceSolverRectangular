#include <iostream>
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

// Optional: helper to print 2D field
void print(const std::vector<std::vector<double>>& field) {
    for (const auto& row : field) {
        for (double val : row)
            std::cout << val << " ";
        std::cout << "\n";
    }
}

int main() {

    int nx = 20;
    int ny = 40;
    Grid grid(0, 0, 1, 1, nx, ny);

    

    SourceTerm source(1.0);
    Equation01 eq(grid, 0.1, 0.002, source); // arguments: grid, D, dt, source
    //std::vector<std::vector<double>> initialField(ny, std::vector<double>(nx, 0.0));
    // eq.initializeField(initialField); // no need to initialise the field because this is done in the constructor

    MyBoundaryCondition bc_temp;
    bc_temp.setNorthType(BCType::FixedValue);
    bc_temp.setNorthValue(0.0);
    bc_temp.setSouthType(BCType::FixedValue);
    bc_temp.setSouthValue(0.0);
    bc_temp.setEastType(BCType::ZeroGradient);
    bc_temp.setEastValue(0.0);
    bc_temp.setWestType(BCType::ZeroGradient);
    
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
    VariableMonitor monitor1(grid, 0.4, 0.4, "temperature");
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


    return 0;
}
