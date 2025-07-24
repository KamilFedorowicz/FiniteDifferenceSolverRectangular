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
    Equation02 eq(grid, 0.1, 0.001, source);


}