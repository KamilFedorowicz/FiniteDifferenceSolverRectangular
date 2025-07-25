#pragma once
#include <vector>
#include "Grid.h"
#include "BoundaryCondition.h"
#include <variant>
#include <map>
#include <cassert>



// base for the equation class. only has the solve function which is the same for each equation

class EquationBase {
public:
    EquationBase(Grid& grid_) : grid(grid_) {};
    virtual ~EquationBase() = default;

    virtual void step(const std::vector<const BoundaryCondition*>& scalar_bcs, const std::vector<const BoundaryCondition*>& vector_bcs) =0;
    virtual void solve(int steps, const std::vector<const BoundaryCondition*>& scalar_bcs, const std::vector<const BoundaryCondition*>& vector_bcs) {

        for (int i = 0; i < steps; ++i) {
            step(scalar_bcs, vector_bcs);
        }
    }
    
    // INITIALISATION FUNCTIONS

    // we provide two arguments: name of the field and the values assigned to the vector field
    void initialiseField(std::string name, const std::vector<std::vector<double>> field) {
        auto it = scalarFields.find(name);

        if (it != scalarFields.end()) {
            if (field.size() != grid.get_ny() || field[0].size() != grid.get_nx()) 
            {
                throw std::runtime_error("Scalar field size does not match grid dimensions.");
            }
            *(it->second) = field;
        }
        else 
        {
            std::cout << "The name "<< name << " is not the right name of a scalar field!" << std::endl;
        }
    }

    void initialiseField(std::string name, const std::vector<std::vector<std::vector<double>>> field) {
        auto it = vectorFields.find(name);
        if (it!=vectorFields.end()) 
        {
            if (field.size() != grid.get_ny() || field[0].size() != grid.get_nx()) 
            {
                throw std::runtime_error("Vector field size does not match grid dimensions.");
            }
            *(it->second) = field;
        }
        else 
        {
            std::cout << "The name "<< name << " is not the right name of a vector field!" << std::endl;
        }
    }

    // CHECKS IF VARIABLES ARE INITIALISED
    void checkIfAllVariablesAreInitialised() 
    {
        checkIfScalarFieldsAreInitialised();
        checkIfVectorFieldsAreInitialsed();
    }

    void checkIfScalarFieldsAreInitialised() 
    {
        for (const auto& it : scalarFields) 
        {
            const std::string& name = it.first;
            const auto* fieldPtr = it.second;

            std::cout << "Name: " << name << ", ptr: " << fieldPtr << std::endl;

            if (!fieldPtr) {
                std::cout << "Field " << name << " is not initialised!" << std::endl;
                throw std::runtime_error("Field not initialised!.");
            } 
            else if (fieldPtr->empty() || (*fieldPtr)[0].empty()) {
                std::cout << "Field " << name << " is empty!" << std::endl;
               throw std::runtime_error("Field is empty.");
            }
        }
    }

    void checkIfVectorFieldsAreInitialsed() 
    {
        for (const auto& it : vectorFields) 
        {
            const std::string& name = it.first;
            const auto* fieldPtr = it.second;

            std::cout << "Name: " << name << ", ptr: " << fieldPtr << std::endl;

            if (!fieldPtr) {
                std::cout << "Field " << name << " is not initialised!" << std::endl;
                throw std::runtime_error("Field not initialised!.");
            } 
            else if (fieldPtr->empty() || (*fieldPtr)[0].empty()) {
                std::cout << "Field " << name << " is empty!" << std::endl;
               throw std::runtime_error("Field is empty.");
            }
        }
    }



    // FUNCTIONS USED TO GET SCALAR AND VECTOR FIELDS
    const std::vector<std::vector<double>>& getScalarField(std::string name) const 
    {  
        auto it = scalarFields.find(name);
        if (it!=scalarFields.end()) {
            return *(it->second); // dereference the pointer
        }else{
            std::cout << "Incorrect name, returning zero field!" << std::endl;
            std::vector<std::vector<double>> zeroField(grid.get_ny(), std::vector<double>(grid.get_nx(), 0.0));
                return zeroField;
        }
    }

    const std::vector<std::vector<std::vector<double>>>& getVectorField(std::string name) const
    {  
        auto it = vectorFields.find(name);
        if (it!=vectorFields.end()) {
            return *(it->second); // dereference the pointer
        }else{
            std::cout << "Incorrect name, returning zero field!" << std::endl;
            std::vector<std::vector<std::vector<double>>> zeroField(grid.get_ny(), std::vector<std::vector<double>>(grid.get_nx(), std::vector<double>(0)));
                return zeroField;
        }
    }

    const std::vector<std::vector<double>> getVectorFieldMagnitude(std::string name) const
    {
        std::vector<std::vector<double>> result(grid.get_ny(), std::vector<double>(grid.get_nx(), 0));

        auto it = vectorFields.find(name);
        if (it != vectorFields.end()) 
        {
            for (int i = 0; i < grid.get_ny(); i++) 
            {
                for (int j = 0; j < grid.get_nx(); j++)
                {
                    double value_x = (*(it->second))[i][j][0];
                    double value_y = (*(it->second))[i][j][1];
                    result[i][j] = std::sqrt( std::pow(value_x,2) + std::pow(value_y,2) ); // if a vector field is found, update the result
                }
            }

        } else
        {
            std::cout << "Incorrect name, returning zero field!" << std::endl;
        }

        return result;
    }

    std::map< std::string, std::vector<std::vector<double>>* > scalarFields; // the second variable is a reference to the field
    std::map< std::string, std::vector<std::vector<std::vector<double>>>* > vectorFields; // the second variable is a reference to the field

    std::map< std::string, std::vector<std::vector<double>>* > dScalarFields_dt; // the second variable is a reference to the field
    std::map< std::string, std::vector<std::vector<std::vector<double>>>* > dVectorFields_dt; // the second variable is a reference to the field

private:
    Grid& grid;
};
