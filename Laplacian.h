#pragma once
#include <memory>
#include <vector>
#include "Grid.h"

class Laplacian{
public:
    
    // LAPLACIAN OF A SCALAR FIELD
    static const std::vector<std::vector<double>> compute(const Grid& grid,
                                                          const std::vector<std::vector<double>>& field){
        std::vector<std::vector<double>> result(grid.get_ny(), std::vector<double>(grid.get_nx(), 0));

        double dx = grid.get_x(1) - grid.get_x(0);
        double dy = grid.get_y(1) - grid.get_y(0);
        
        double d2_dx2;
        double d2_dy2;
        
        for (int i = 0; i < grid.get_ny(); ++i) {
            for (int j = 0; j < grid.get_nx(); ++j) {
                
                // Second derivative in x
                if (j == 0) {
                    // Forward one-sided
                    d2_dx2 = (2 * field[i][j] - 5 * field[i][j + 1] + 4 * field[i][j + 2] - field[i][j + 3]) / (dx * dx);
                } else if (j == grid.get_nx() - 1) {
                    // Backward one-sided
                    d2_dx2 = (2 * field[i][j] - 5 * field[i][j - 1] + 4 * field[i][j - 2] - field[i][j - 3]) / (dx * dx);
                } else {
                    // Central difference
                    d2_dx2 = (field[i][j + 1] - 2 * field[i][j] + field[i][j - 1]) / (dx * dx);
                }

                // Second derivative in y
                if (i == 0) {
                    d2_dy2 = (2 * field[i][j] - 5 * field[i + 1][j] + 4 * field[i + 2][j] - field[i + 3][j]) / (dy * dy);
                } else if (i == grid.get_ny() - 1) {
                    d2_dy2 = (2 * field[i][j] - 5 * field[i - 1][j] + 4 * field[i - 2][j] - field[i - 3][j]) / (dy * dy);
                } else {
                    d2_dy2 = (field[i + 1][j] - 2 * field[i][j] + field[i - 1][j]) / (dy * dy);
                }
                
                result[i][j] = d2_dx2 + d2_dy2;
                
            }
        }
        return result;
    }
    
    // LAPLACIAN OF A VECTOR FIELD
    static std::vector<std::vector<std::vector<double>>> compute(
        const Grid& grid,
        const std::vector<std::vector<std::vector<double>>>& field)
    {
        // Determine number of components in the vector field
        if (field.empty() || field[0].empty() || field[0][0].empty()) {
            throw std::runtime_error("Invalid input field dimensions.");
        }

        size_t nComponents = field[0][0].size();

        // Initialize result field
        std::vector<std::vector<std::vector<double>>> result(
            grid.get_ny(),
            std::vector<std::vector<double>>(grid.get_nx(), std::vector<double>(nComponents, 0.0))
        );

        // Loop over each component
        for (size_t k = 0; k < nComponents; ++k) {
            // Extract k-th component as a scalar field
            std::vector<std::vector<double>> componentField(grid.get_ny(), std::vector<double>(grid.get_nx(), 0.0));
            for (int i = 0; i < grid.get_ny(); ++i) {
                for (int j = 0; j < grid.get_nx(); ++j) {
                    componentField[i][j] = field[i][j][k];
                }
            }

            // Compute Laplacian of this component
            std::vector<std::vector<double>> laplacianComponent = compute(grid, componentField);

            // Insert result back into the k-th component of result field
            for (int i = 0; i < grid.get_ny(); ++i) {
                for (int j = 0; j < grid.get_nx(); ++j) {
                    result[i][j][k] = laplacianComponent[i][j];
                }
            }
        }

        return result;
    }
    
};
