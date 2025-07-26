#pragma once
#include <vector>
#include "Grid.h"

class SourceTermScalar {
public:
    // Default: constant source of 0
    SourceTermScalar() : constantSourceScalar(0.0) {}

    // Custom constant value
    SourceTermScalar(double value) : constantSourceScalar(value) {}


    // Override this method to define a spatially varying source
    virtual std::vector<std::vector<double>> compute(const Grid& grid) const {
        std::vector<std::vector<double>> source(grid.get_ny(), std::vector<double>(grid.get_nx(), constantSourceScalar));
        return source;
    }

protected:
    double constantSourceScalar;
};

// applies to 2D vectors
class SourceTermVector {
public:
    // Default: zero vector source
    SourceTermVector() : constantSourceVector({0.0, 0.0}) {}

    // Custom constant value (e.g., {1.0, 0.0})
    SourceTermVector(const std::vector<double>& value) : constantSourceVector(value) {}

    // Returns a vector-valued source field (3D structure)
    virtual std::vector<std::vector<std::vector<double>>> compute(const Grid& grid) const {
        return std::vector<std::vector<std::vector<double>>>(
            grid.get_ny(),
            std::vector<std::vector<double>>(grid.get_nx(), constantSourceVector)
        );
    }

protected:
    std::vector<double> constantSourceVector;  // e.g., size 2: [Fx, Fy]
};
