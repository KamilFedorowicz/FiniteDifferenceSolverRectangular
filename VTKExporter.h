#pragma once
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "Grid.h"

class VTKExporter {
public:
    // Save scalar field (2D data)
    static void saveScalarField(const Grid& grid,
                                const std::vector<std::vector<double>>& data,
                                const std::string& filename,
                                const std::string& fieldName = "scalarField")
    {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        int nx = grid.get_nx();
        int ny = grid.get_ny();

        // VTK legacy header
        file << "# vtk DataFile Version 3.0\n";
        file << fieldName << "\n";
        file << "ASCII\n";
        file << "DATASET STRUCTURED_POINTS\n";
        file << "DIMENSIONS " << nx << " " << ny << " 1\n";
        file << "ORIGIN " << grid.get_x(0) << " " << grid.get_y(0) << " 0\n";
        file << "SPACING " << (grid.get_x(1) - grid.get_x(0)) << " "
             << (grid.get_y(1) - grid.get_y(0)) << " 1\n";
        file << "POINT_DATA " << nx * ny << "\n";
        file << "SCALARS " << fieldName << " double\n";
        file << "LOOKUP_TABLE default\n";

        // Write scalar values (VTK expects row-major ordering: y outer, x inner)
        for (int i = 0; i < ny; ++i) {
            for (int j = 0; j < nx; ++j) {
                file << data[i][j] << "\n";
            }
        }

        file.close();
    }

    // Save vector field (2D, but VTK vectors are 3D, so we set z=0)
    static void saveVectorField(const Grid& grid,
                                const std::vector<std::vector<std::vector<double>>>& data,
                                const std::string& filename,
                                const std::string& fieldName = "vectorField")
    {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        int nx = grid.get_nx();
        int ny = grid.get_ny();

        // VTK legacy header
        file << "# vtk DataFile Version 3.0\n";
        file << fieldName << "\n";
        file << "ASCII\n";
        file << "DATASET STRUCTURED_POINTS\n";
        file << "DIMENSIONS " << nx << " " << ny << " 1\n";
        file << "ORIGIN " << grid.get_x(0) << " " << grid.get_y(0) << " 0\n";
        file << "SPACING " << (grid.get_x(1) - grid.get_x(0)) << " "
             << (grid.get_y(1) - grid.get_y(0)) << " 1\n";
        file << "POINT_DATA " << nx * ny << "\n";
        file << "VECTORS " << fieldName << " double\n";

        // Write vectors (u,v,0)
        for (int i = 0; i < ny; ++i) {
            for (int j = 0; j < nx; ++j) {
                const auto& vec = data[i][j];
                double u = vec.size() > 0 ? vec[0] : 0.0;
                double v = vec.size() > 1 ? vec[1] : 0.0;
                file << u << " " << v << " 0\n";
            }
        }

        file.close();
    }
};
