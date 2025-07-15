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
    
    void setObstacle(double x_west, double y_south, double x_east, double y_north, double val){
        x_obs_west.push_back(x_west);
        y_obs_south.push_back(y_south);
        x_obs_east.push_back(x_east);
        y_obs_north.push_back(y_north);
        value_obs.push_back(val);
    };



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

    // === Obstacle info ===
    std::vector<double> x_obs_east, y_obs_south, x_obs_west, y_obs_north, value_obs;
    
    void applyObstacles(std::vector<std::vector<double>>& field, const Grid& grid) const {
        for(int obs_count=0; obs_count<value_obs.size(); obs_count++){
            double x_o_east = x_obs_east[obs_count];
            double x_o_west = x_obs_west[obs_count];
            double y_o_south = y_obs_south[obs_count];
            double y_o_north = y_obs_north[obs_count];
            double value_o = value_obs[obs_count];
            
            
            for(int i=0; i<grid.ny; i++){
                for(int j=0; j<grid.nx; j++){
                    double x_field = grid.start_x + (grid.end_x - grid.start_x)*j/grid.nx;
                    double y_field = grid.start_y + (grid.end_y - grid.start_y)*i/grid.ny;
                    
                    
                    if(x_o_east>=x_field && x_o_west<=x_field && y_o_south<=y_field && y_o_north>=y_field){
                        field[i][j] = value_o;
                    }
                    
                }
            }
            
        }
        
        
    }
};
