#pragma once
#include "BoundaryCondition.h"
#include "Grid.h"
#include <vector>

class MyBoundaryCondition : public BoundaryCondition {
public:
    MyBoundaryCondition() = default;

    void apply(std::vector<std::vector<double>>& field, const Grid& grid) const override {
        applyWallBCs(field, grid);
        applyObstacles(field, grid);
    }

    // === Wall BC setters ===
    void setEastValue(double val) { east_value = val; }
    void setWestValue(double val) { west_value = val; }
    void setNorthValue(double val) { north_value = val; }
    void setSouthValue(double val) { south_value = val; }

    void setEastType(BCType type) { east_type = type; }
    void setWestType(BCType type) { west_type = type; }
    void setNorthType(BCType type) { north_type = type; }
    void setSouthType(BCType type) { south_type = type; }



private:
    // === Wall BC parameters ===
    double east_value = 0.0, west_value = 0.0, north_value = 0.0, south_value = 0.0;
    BCType east_type = BCType::FixedValue;
    BCType west_type = BCType::FixedValue;
    BCType north_type = BCType::FixedValue;
    BCType south_type = BCType::FixedValue;

    // === Obstacle info ===
    std::vector<double> x_e, x_w, y_s, y_n;
    std::vector<double> obstacle_value;

    void applyWallBCs(std::vector<std::vector<double>>& field, const Grid& grid) const {
        int nx = grid.nx;
        int ny = grid.ny;

        // WEST
        for (int i = 0; i < ny; ++i)
            field[i][0] = (west_type == BCType::FixedValue) ? west_value : field[i][1];

        // EAST
        for (int i = 0; i < ny; ++i)
            field[i][nx - 1] = (east_type == BCType::FixedValue) ? east_value : field[i][nx - 2];

        // SOUTH
        for (int j = 0; j < nx; ++j)
            field[0][j] = (south_type == BCType::FixedValue) ? south_value : field[1][j];

        // NORTH
        for (int j = 0; j < nx; ++j)
            field[ny - 1][j] = (north_type == BCType::FixedValue) ? north_value : field[ny - 2][j];
    }

    void applyObstacles(std::vector<std::vector<double>>& field, const Grid& grid) const {
        for (size_t obs = 0; obs < obstacle_value.size(); ++obs) {
            for (int i = 0; i < grid.ny; ++i) {
                for (int j = 0; j < grid.nx; ++j) {
                    if (grid.x[j] >= x_w[obs] && grid.x[j] <= x_e[obs] &&
                        grid.y[i] >= y_s[obs] && grid.y[i] <= y_n[obs]) {
                        field[i][j] = obstacle_value[obs];
                    }
                }
            }
        }
    }
};
