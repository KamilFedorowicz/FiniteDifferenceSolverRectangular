#pragma once
#include <vector>
#include <map>
#include <string>
#include "Grid.h"
#include "BoundaryCondition.h"
#include "FieldTypes.h"

class EquationBase {
public:
    EquationBase(Grid& grid_);
    virtual ~EquationBase() = default;

    virtual void step(std::map<std::string, const BoundaryCondition*>& scalar_bcs,
                      std::map<std::string, const BoundaryCondition*>& vector_bcs,
                      double dt) = 0;

    virtual void solve(int steps,
                       std::map<std::string, const BoundaryCondition*>& scalar_bcs,
                       std::map<std::string, const BoundaryCondition*>& vector_bcs,
                       double dt);

    void initialiseField(std::string name, const scalarField field);
    void initialiseField(std::string name, const vectorField field);

    void checkIfAllVariablesAreInitialised();
    void checkIfScalarFieldsAreInitialised();
    void checkIfVectorFieldsAreInitialsed();

    // Accessors for fields
    const scalarField& getScalarField(std::string name) const;
    const vectorField& getVectorField(std::string name) const;

    const scalarField getVectorFieldMagnitude(std::string name) const;

    scalarFieldMap scalarFields;
    vectorFieldMap vectorFields;

    scalarFieldMap dScalarFields_dt;
    vectorFieldMap dVectorFields_dt;

    scalarFieldMap scalarSourceTerms;
    vectorFieldMap vectorSourceTerms;

private:
    Grid& grid;
};
