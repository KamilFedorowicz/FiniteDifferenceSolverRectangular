#pragma once
#include <memory>
#include <vector>
#include "Grid.h"
#include "FieldTypes.h"

class Divergence{
public:
    
    static scalarField compute(
        const Grid& grid,
        const vectorField& vectField)
    {
        scalarField result(grid.get_ny(), std::vector<double>(grid.get_nx(), 0));

        double dx = (grid.get_nx() > 1) ? (grid.get_x(1) - grid.get_x(0)) : 1.0;
        double dy = (grid.get_ny() > 1) ? (grid.get_y(1) - grid.get_y(0)) : 1.0;

        for (int i = 0; i < grid.get_ny(); ++i) {
            for (int j = 0; j < grid.get_nx(); ++j) {

                double d_dx = 0.0;
                double d_dy = 0.0;

                // --- df/dx ---
                if (grid.get_nx() > 1) {
                    if (j == 0) {
                        // forward difference
                        d_dx = (vectField[i][j + 1][0] - vectField[i][j][0]) / dx;
                    }
                    else if (j == grid.get_nx() - 1) {
                        // backward difference
                        d_dx = (vectField[i][j][0] - vectField[i][j - 1][0]) / dx;
                    }
                    else {
                        // central difference
                        d_dx = (vectField[i][j + 1][0] - vectField[i][j - 1][0]) / (2.0 * dx);
                    }
                }

                // --- df/dy ---
                if (grid.get_ny() > 1) {
                    if (i == 0) {
                        // forward difference
                        d_dy = (vectField[i + 1][j][1] - vectField[i][j][1]) / dy;
                    }
                    else if (i == grid.get_ny() - 1) {
                        // backward difference
                        d_dy = (vectField[i][j][1] - vectField[i - 1][j][1]) / dy;
                    }
                    else {
                        // central difference
                        d_dy = (vectField[i + 1][j][1] - vectField[i - 1][j][1]) / (2.0 * dy);
                    }
                }

                result[i][j] = d_dx + d_dy;
            }
        }

        return result;
    }

    
    
private:
    
    
    
};