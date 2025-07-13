#pragma once
#include <vector>
#include <fstream>
#include <string>
#include "Grid.h"

class CSVExporter {
public:
    static void saveToCSVWithCoordinates(const Grid& grid,
                                         const std::vector<std::vector<double>>& data,
                                         const std::string& filename)
    {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        file << "x,y,value\n";

        for (int i = 0; i < grid.ny; ++i) {
            for (int j = 0; j < grid.nx; ++j) {
                file << grid.x[j] << "," << grid.y[i] << "," << data[i][j] << "\n";
            }
        }

        file.close();
    }
    
    static void saveMonitoredVariable(const std::vector<double>& monitoredData,
                                      const std::string& filename)
    {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        file << "step,value\n";

        for (size_t step = 0; step < monitoredData.size(); ++step) {
            file << step << "," << monitoredData[step] << "\n";
        }

        file.close();
    }
    
    
};
