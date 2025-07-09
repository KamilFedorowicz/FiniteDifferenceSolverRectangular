#pragma once
#include <memory>
#include <vector>
#include "grid.h"

class Gradient{
public:
    
    static std::vector<std::vector<std::vector<double>>> compute(const Grid& grid,
                                                                 const std::vector<std::vector<double>>& scalarField)
    {
        std::vector<std::vector<std::vector<double>>> result(grid.ny, std::vector<std::vector<double>>(grid.nx, std::vector<double>(2,0.0)));
        
        //result[i][j][0]; // df/dx at (i, j)
        //result[i][j][1]; // df/dy
        double dx = grid.x[1] - grid.x[0];
        double dy = grid.y[1] - grid.y[0];
        
        for (int i = 0; i < grid.ny; ++i) {
            for (int j = 0; j < grid.nx; ++j) {
                
                double d_dx = 0.0, d_dy = 0.0;

                // df/dx
                if (j == 0) {
                    d_dx = (scalarField[i][j + 1] - scalarField[i][j]) / dx; // forward
                } else if (j == grid.nx - 1) {
                    d_dx = (scalarField[i][j] - scalarField[i][j - 1]) / dx; // backward
                } else {
                    d_dx = (scalarField[i][j + 1] - scalarField[i][j - 1]) / (2 * dx); // central
                }

                // df/dy
                if (i == 0) {
                    d_dy = (scalarField[i + 1][j] - scalarField[i][j]) / dy; // forward
                } else if (i == grid.ny - 1) {
                    d_dy = (scalarField[i][j] - scalarField[i - 1][j]) / dy; // backward
                } else {
                    d_dy = (scalarField[i + 1][j] - scalarField[i - 1][j]) / (2 * dy); // central
                }
                
                result[i][j][0] = d_dx;
                result[i][j][1] = d_dy;
            }
        }
        return result;
    }
    
    static std::vector<std::vector<double>> return_nth_elem(const std::shared_ptr<Grid>& grid,
                                                            const std::vector<std::vector<std::vector<double>>>& field,
                                                            int n) {
        std::vector<std::vector<double>> result(grid->ny, std::vector<double>(grid->nx, 0.0));
        for (int i = 0; i < field.size(); ++i) {
            for (int j = 0; j < field[0].size(); ++j) {
                result[i][j] = field[i][j][n];
            }
        }
        return result;
    }
    
    
private:
    
//    std::shared_ptr<Grid> grid;
    
    
};