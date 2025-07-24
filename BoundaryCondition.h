#pragma once
#include <vector>
#include "Grid.h"

enum class BCType {
    FixedValue,
    ZeroGradient
};

enum class BoundarySide {
    North,
    South,
    East,
    West
};

class BoundaryCondition {
public:
    // Apply all BCs (entire field/grid)
    virtual void apply(std::vector<std::vector<double>>& field, const Grid& grid) const =0;
    //virtual void apply(std::vector<std::vector<std::vector<double>>>& field, const Grid& grid) const ;
    virtual ~BoundaryCondition() {}
};
