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

    MyBoundaryCondition bc;
    bc.setNorthType(BCType::FixedValue);
    bc.setNorthValue(0.0);
    bc.setSouthType(BCType::FixedValue);
    bc.setSouthValue(0.0);
    bc.setEastType(BCType::ZeroGradient);
    bc.setEastValue(0.0);
    bc.setWestType(BCType::ZeroGradient);
    //bc.setWestValue(0.0);

    // std::cout << "Working here" << std::endl;
    
    VariableMonitor monitor1(grid, 0.2, 0.2);
    VariableMonitor monitor2(grid, 0.3, 0.3);

    Solver solver(eq, grid);
    
    solver.addVariableMonitor(monitor1);
    solver.addVariableMonitor(monitor2);
    
    
    solver.solve(5000, bc);
    

    // === Export result ===
    CSVExporter::saveToCSVWithCoordinates(grid, solver.getResult(), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/result.csv");
    CSVExporter::saveMonitoredVariable(monitor1.returnMonitoredVariable(), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/time_plot1.csv");
    CSVExporter::saveMonitoredVariable(monitor2.returnMonitoredVariable(), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/time_plot2.csv");
    
    std::cout << "Simulation completed. Output saved to result.csv\n";


    return 0;
}
