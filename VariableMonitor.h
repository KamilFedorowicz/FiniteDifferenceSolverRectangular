#pragma once
#include "Grid.h"



class VariableMonitor{
public:
    VariableMonitor(Grid& grid, double x, double y, std::string name): grid(grid), monitor_x(x), monitor_y(y), varName(name){
        ix = static_cast<int>(grid.get_nx() * monitor_x / grid.get_x_lim(1));
        iy = static_cast<int>(grid.get_ny() * monitor_y / grid.get_y_lim(1));
    }
    ~VariableMonitor() = default;
    
    void updateMonitoredScalarVariable(double value)
    {
        monitoredScalarVariable.push_back(value);
    }
    
    void updateMonitoredVectorVariable(std::vector<double> value)
    {
        monitoredVectorVariable.push_back(value);
    }
    
    std::vector<double> returnMonitoredScalarVariable()
    {
        return monitoredScalarVariable;
    }
    
    std::vector<std::vector<double>> returnMonitoredVectorVariable()
    {
        return monitoredVectorVariable;
    }
    
    void print()
    {
        std::cout  << "Monitored variable: " << std::endl;
        for(double x: monitoredScalarVariable){
            std::cout << x << " " ;
        }
        std::cout << std::endl;
    }
    
    int getIx()
    {
        return ix;
    }
    int getIy()
    {
        return iy;
    }
    
    std::string getName()
    {
        return varName;
    }
    
    
private:
    double monitor_x = 0;
    double monitor_y = 0;
    Grid& grid;
    int ix, iy; // integers that store the number of monitored cell
    std::vector<double> monitoredScalarVariable{};
    std::vector<std::vector<double>> monitoredVectorVariable{};
    std::string varName;
    
};
