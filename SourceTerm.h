#pragma once
#include <vector>
#include "Grid.h"

class SourceTerm {
public:
    // Default: constant source of 1
    SourceTerm() : constantSource(1.0) {}

    // Custom constant value
    SourceTerm(double value) : constantSource(value) {}

    // Override this method to define a spatially varying source
    virtual std::vector<std::vector<double>> compute(const Grid& grid) const {
        std::vector<std::vector<double>> source(grid.get_ny(), std::vector<double>(grid.get_nx(), constantSource));
        return source;
    }

protected:
    double constantSource;
};
