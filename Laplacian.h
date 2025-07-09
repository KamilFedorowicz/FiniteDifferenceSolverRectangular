#pragma once
#include <memory>
#include <vector>
#include "Grid.h"

class Laplacian{
public:
    
    static const std::vector<std::vector<double>> compute(const Grid& grid,
                                                          const std::vector<std::vector<double>>& scalarField){
        std::vector<std::vector<double>> result(grid.ny, std::vector<double>(grid.nx, 0));
        
        double dx = grid.x[1] - grid.x[0];
        double dy = grid.y[1] - grid.y[0];
        
        double d2_dx2;
        double d2_dy2;
        
        for (int i = 0; i < grid.ny; ++i) {
            for (int j = 0; j < grid.nx; ++j) {
                
                // Second derivative in x
                if (j == 0) {
                    // Forward one-sided
                    d2_dx2 = (2 * scalarField[i][j] - 5 * scalarField[i][j + 1] + 4 * scalarField[i][j + 2] - scalarField[i][j + 3]) / (dx * dx);
                } else if (j == grid.nx - 1) {
                    // Backward one-sided
                    d2_dx2 = (2 * scalarField[i][j] - 5 * scalarField[i][j - 1] + 4 * scalarField[i][j - 2] - scalarField[i][j - 3]) / (dx * dx);
                } else {
                    // Central difference
                    d2_dx2 = (scalarField[i][j + 1] - 2 * scalarField[i][j] + scalarField[i][j - 1]) / (dx * dx);
                }

                // Second derivative in y
                if (i == 0) {
                    d2_dy2 = (2 * scalarField[i][j] - 5 * scalarField[i + 1][j] + 4 * scalarField[i + 2][j] - scalarField[i + 3][j]) / (dy * dy);
                } else if (i == grid.ny - 1) {
                    d2_dy2 = (2 * scalarField[i][j] - 5 * scalarField[i - 1][j] + 4 * scalarField[i - 2][j] - scalarField[i - 3][j]) / (dy * dy);
                } else {
                    d2_dy2 = (scalarField[i + 1][j] - 2 * scalarField[i][j] + scalarField[i - 1][j]) / (dy * dy);
                }
                
                result[i][j] = d2_dx2 + d2_dy2;
                
            }
        }
        return result;
    }
    
    
};
