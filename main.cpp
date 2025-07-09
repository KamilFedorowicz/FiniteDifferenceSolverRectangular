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

// Optional: helper to print 2D field
void print(const std::vector<std::vector<double>>& field) {
    for (const auto& row : field) {
        for (double val : row)
            std::cout << val << " ";
        std::cout << "\n";
    }
}

int main() {

    Grid grid(0, 0, 1, 1, 20, 20);

    std::vector<std::vector<double>> initialField(20, std::vector<double>(20, 0.0));

    SourceTerm source(1.0);
    Equation01 eq(grid, 0.1, 0.001, source);
    eq.initializeField(initialField);

    MyBoundaryCondition bc;
    bc.setNorthType(BCType::FixedValue);
    bc.setNorthValue(0.0);
    bc.setSouthType(BCType::FixedValue);
    bc.setSouthValue(0.0);
    bc.setEastType(BCType::ZeroGradient);
    //bc.setEastValue(0.0);
    bc.setWestType(BCType::ZeroGradient);
    //bc.setWestValue(0.0);

    Solver solver(eq);
    solver.solve(1000, bc);

    // === Export result ===
    CSVExporter::saveToCSVWithCoordinates(grid, solver.getResult(), "result.csv");

    std::cout << "Simulation completed. Output saved to result.csv\n";


    return 0;
}
