#pragma once
#include <vector>
#include "Grid.h"
#include "FieldTypes.h"

enum class BCType {
    FixedValue,
    ZeroGradient,
    FixedGradient
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
    virtual void apply(scalarField& field, const Grid& grid) const = 0;
    virtual void apply(vectorField& field, const Grid& grid) const = 0;
    virtual ~BoundaryCondition() {}
};
