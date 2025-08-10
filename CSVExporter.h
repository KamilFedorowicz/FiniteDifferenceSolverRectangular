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

        for (int i = 0; i < grid.get_ny(); ++i) {
            for (int j = 0; j < grid.get_nx(); ++j) {
                file << grid.get_x(j) << "," << grid.get_y(i) << "," << data[i][j] << "\n";
            }
        }

        file.close();
    }

    static void saveToCSVWithCoordinates(const Grid& grid,
                                                const std::vector<std::vector<std::vector<double>>>& data,
                                                const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    file << "x,y,u,v\n";

    for (int i = 0; i < grid.get_ny(); ++i) {
        for (int j = 0; j < grid.get_nx(); ++j) {
            const auto& vec = data[i][j];
            double u = vec.size() > 0 ? vec[0] : 0.0;
            double v = vec.size() > 1 ? vec[1] : 0.0;
            file << grid.get_x(j) << "," << grid.get_y(i) << "," << u << "," << v << "\n";
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
    
    static void saveMonitoredVariable(const std::vector<std::vector<double>>& field,
                          const std::string& filename)
    {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        // Optional header
        file << "row,col,value\n";

        for (size_t i = 0; i < field.size(); ++i) {
            file << i << ", " << field[i][0] << ", " << field[i][1] << "\n";
        }

        file.close();
    }
    
};
