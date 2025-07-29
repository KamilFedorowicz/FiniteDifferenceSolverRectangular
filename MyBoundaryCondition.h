#pragma once
#include "BoundaryCondition.h"
#include "Grid.h"
#include <vector>
#include "FieldTypes.h"

class MyBoundaryCondition : public BoundaryCondition {
public:
    MyBoundaryCondition() = default;

    // === Apply wall and obstacle BCs for a scalar field
    void apply(scalarField& field, const Grid& grid) const {
        applyWallBCs(field, grid);
        applyObstacles(field, grid);
    }
    
    // === Apply wall and obstacle BCs for a vector field
    void apply(vectorField& field, const Grid& grid) const {
        applyWallBCs(field, grid);
    }

    // === Wall BC type setters - independent of scalar/vector ===
    void setEastType(BCType type) { eastType = type; }
    void setWestType(BCType type) { westType = type; }
    void setNorthType(BCType type) { northType = type; }
    void setSouthType(BCType type) { southType = type; }
    
    // === Wall BC setters for scalars===
    void setEastValue(double val) { east_value_scalar = val; }
    void setWestValue(double val) { west_value_scalar = val; }
    void setNorthValue(double val) { north_value_scalar = val; }
    void setSouthValue(double val) { south_value_scalar = val; }
    
    // === Obstacle BC setters for scalars===
    void setObstacle(double x_west, double y_south, double x_east, double y_north, double val){
        xObsWest.push_back(x_west);
        yObsSouth.push_back(y_south);
        xObsEast.push_back(x_east);
        yObsNorth.push_back(y_north);
        valueObsScalar.push_back(val);
    };
    
    // === Wall BC setters for vectors===
    void setEastValue(std::vector<double> val) { east_value_vector = val; }
    void setWestValue(std::vector<double> val) { west_value_vector = val; }
    void setNorthValue(std::vector<double> val) { north_value_vector = val; }
    void setSouthValue(std::vector<double> val) { south_value_vector = val; }
    
    // === Obstacle BC setters for vectors=== the only difference is the value imposed
    void setObstacle(double x_west, double y_south, double x_east, double y_north, std::vector<double> val){
        xObsWest.push_back(x_west);
        yObsSouth.push_back(y_south);
        xObsEast.push_back(x_east);
        yObsNorth.push_back(y_north);
        valueObsVector.push_back(val);
    };

    

private:
    
    // === Wall BC parameters ===
    double east_value_scalar = 0.0, west_value_scalar = 0.0, north_value_scalar = 0.0, south_value_scalar = 0.0;
    std::vector<double> east_value_vector = {}, west_value_vector = {}, north_value_vector = {}, south_value_vector = {};

    BCType eastType = BCType::FixedValue;
    BCType westType = BCType::FixedValue;
    BCType northType = BCType::FixedValue;
    BCType southType = BCType::FixedValue;


    // === Apply wall BCs for a scalar field
    void applyWallBCs(scalarField& field, const Grid& grid) const {
        int nx = grid.get_nx();
        int ny = grid.get_ny();

        // WEST
        for (int i = 0; i < ny; ++i)
            field[i][0] = (westType == BCType::FixedValue) ? west_value_scalar : field[i][1];
        // EAST
        for (int i = 0; i < ny; ++i)
            field[i][nx - 1] = (eastType == BCType::FixedValue) ? east_value_scalar : field[i][nx - 2];
        // SOUTH
        for (int j = 0; j < nx; ++j)
            field[0][j] = (southType == BCType::FixedValue) ? south_value_scalar : field[1][j];
        // NORTH
        for (int j = 0; j < nx; ++j)
            field[ny - 1][j] = (northType == BCType::FixedValue) ? north_value_scalar : field[ny - 2][j];
    }
    
    // === Apply wall BCs for a vector field
    void applyWallBCs(vectorField& field, const Grid& grid) const {
        int nx = grid.get_nx();
        int ny = grid.get_ny();

        // WEST
        for (int i = 0; i < ny; ++i)
            field[i][0] = (westType == BCType::FixedValue) ? west_value_vector : field[i][1];
        // EAST
        for (int i = 0; i < ny; ++i)
            field[i][nx - 1] = (eastType == BCType::FixedValue) ? east_value_vector : field[i][nx - 2];
        // SOUTH
        for (int j = 0; j < nx; ++j)
            field[0][j] = (southType == BCType::FixedValue) ? south_value_vector : field[1][j];
        // NORTH
        for (int j = 0; j < nx; ++j)
            field[ny - 1][j] = (northType == BCType::FixedValue) ? north_value_vector : field[ny - 2][j];
    }

    // === Obstacle info ===
    std::vector<double> xObsEast, yObsSouth, xObsWest, yObsNorth; // just coords so work for both vector and scalar
    std::vector<double> valueObsScalar; // value imposed by obstacle just for scalar
    std::vector<std::vector<double>> valueObsVector; // value imposed by obstacle just for vector

    
    // === Apply obstacle BCs for a scalar field
    void applyObstacles(scalarField& field, const Grid& grid) const {
        for(int obs_count=0; obs_count<valueObsScalar.size(); obs_count++){
            double x_o_east = xObsEast[obs_count];
            double x_o_west = xObsWest[obs_count];
            double y_o_south = yObsSouth[obs_count];
            double y_o_north = yObsNorth[obs_count];
            double value_o = valueObsScalar[obs_count];
            
            for(int i=0; i<grid.get_ny(); i++){
                for(int j=0; j<grid.get_nx(); j++){
                    double x_field = grid.get_x_lim(0) + (grid.get_x_lim(1) - grid.get_x_lim(0))*j/grid.get_nx();
                    double y_field = grid.get_y_lim(0) + (grid.get_y_lim(1) - grid.get_y_lim(0))*i/grid.get_ny();
                    
                    if(x_o_east>=x_field && x_o_west<=x_field && y_o_south<=y_field && y_o_north>=y_field){
                        field[i][j] = value_o;
                    }
                }
            }
        }
    }
    
    // === Apply obstacle BCs for a vector field
    void applyObstacles(vectorField& field, const Grid& grid) const {
        for(int obs_count=0; obs_count<valueObsScalar.size(); obs_count++){
            double x_o_east = xObsEast[obs_count];
            double x_o_west = xObsWest[obs_count];
            double y_o_south = yObsSouth[obs_count];
            double y_o_north = yObsNorth[obs_count];
            std::vector<double> value_o = valueObsVector[obs_count];
            
            for(int i=0; i<grid.get_ny(); i++){
                for(int j=0; j<grid.get_nx(); j++){
                    double x_field = grid.get_x_lim(0) + (grid.get_x_lim(1) - grid.get_x_lim(0))*j/grid.get_nx();
                    double y_field = grid.get_y_lim(0) + (grid.get_y_lim(1) - grid.get_y_lim(0))*i/grid.get_ny();
                    
                    if(x_o_east>=x_field && x_o_west<=x_field && y_o_south<=y_field && y_o_north>=y_field){
                        field[i][j] = value_o;
                    }
                }
            }
        }
    }
    

    
    
    
    
};
