#pragma once
#include "Grid.h"



class VariableMonitor{
public:
    VariableMonitor(Grid& grid, double x, double y): grid(grid), monitor_x(x), monitor_y(y){
        ix = static_cast<int>(grid.nx * monitor_x / grid.end_x);
        iy = static_cast<int>(grid.ny * monitor_y / grid.end_y);
    }
    ~VariableMonitor() = default;
    
    void update(double value){
        monitoredVariable.push_back(value);
    }
    
    std::vector<double> returnMonitoredVariable(){
        return monitoredVariable;
    }
    
    void print(){
        std::cout  << "Monitored variable: " << std::endl;
        for(double x: monitoredVariable){
            std::cout << x << " " ;
        }
        std::cout << std::endl;
    }
    
    int getIx(){
        return ix;
    }
    int getIy(){
        return iy;
    }
    
    
private:
    double monitor_x = 0;
    double monitor_y = 0;
    Grid& grid;
    int ix, iy; // integers that store the number of monitored cell
    std::vector<double> monitoredVariable{};
    
};
