#pragma once
#include "Grid.h"



class VariableMonitor{
public:
    VariableMonitor(Grid& grid, double x, double y, std::string name): grid(grid), monitor_x(x), monitor_y(y), varName(name){
        ix = static_cast<int>(grid.get_nx() * monitor_x / grid.get_x_lim(1));
        iy = static_cast<int>(grid.get_ny() * monitor_y / grid.get_y_lim(1));
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
    
    std::string getName(){
        return varName;
    }
    
    
private:
    double monitor_x = 0;
    double monitor_y = 0;
    Grid& grid;
    int ix, iy; // integers that store the number of monitored cell
    std::vector<double> monitoredVariable{};
    std::string varName;
    
};
