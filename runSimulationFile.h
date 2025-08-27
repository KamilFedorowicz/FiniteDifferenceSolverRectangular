#pragma once
#include "Grid.h"
#include "Laplacian.h"
#include "Gradient.h"
#include "Equation01.h"
#include "Equation02.h"
#include "SourceTerm.h"
#include "CSVExporter.h"
#include "MyBoundaryCondition.h"
#include "Solver.h"
#include "MathOperators.h"
#include "VariableMonitor.h"
#include "readingFiles.h"
#include "EquationFactory.h"
#include <iostream>
#include <string>
#include <filesystem>
#include "VTKExporter.h"

namespace fs = std::filesystem;

void runSimulation() {
        
    // get the current working directory
    fs::path cwd = fs::current_path();
    std::string cwdStr = cwd.string();
    
    // READING GRID INFO
    auto meshInfo = readMeshInfo(cwdStr+"/meshInfo.txt"); // note that this will not work from Xcode
    
    // this is checking if all the keys have been read
    const std::vector<std::string> requiredKeys = {"nx", "ny", "min_x", "min_y", "max_x", "max_y"};
    for (const auto& key : requiredKeys) {
        if (meshInfo.find(key) == meshInfo.end()) {
            std::cout << "Missing key in meshInfo.txt: " + key << std::endl;
            return;
        }
    }
    
    int nx = static_cast<int>(meshInfo.at("nx"));
    int ny = static_cast<int>(meshInfo.at("ny"));
    double min_x = meshInfo.at("min_x");
    double min_y = meshInfo.at("min_y");
    double max_x = meshInfo.at("max_x");
    double max_y = meshInfo.at("max_y");
    Grid grid(min_x, min_y, max_x, max_y, nx, ny);
    
    // READING EQUATION INFO
    std::string equationFile = cwdStr+"/equationInfo.txt";
    EquationNames equationName = readEquationName(equationFile); // read which equation we are using
    std::cout << "the equation name is: " << EquationUtils::to_string(equationName) << std::endl;
    
    std::map<std::string, double> equationConstantsMap; // map that has a constant name and value
    
    auto requiredConstants = EquationFactoryRegistry::instance().getRequiredConstants(EquationUtils::to_string(equationName));

    for(std::string str: requiredConstants){
        std::cout << str << std::endl;
    }
    
    if (!requiredConstants.empty()) {
        equationConstantsMap = readConstantsFromFile(requiredConstants, equationFile);
    } else {
        std::cerr << "Equation " << EquationUtils::to_string(equationName)
                  << " was not found in the equation database. Aborting! Check the file EquationBase.h to see if the equation is there!" << std::endl;
        return;
    }
    std::unique_ptr<EquationBase> eq = EquationFactoryRegistry::instance().create(EquationUtils::to_string(equationName), grid, equationFile);

    
    // ----------- initial and boundary conditions for the scalar fields
    std::vector<std::string> scalarFieldsList = eq->getScalarVariableNames();
    std::map<std::string, const BoundaryCondition*> scalar_bcs;
    std::map<std::string, MyBoundaryCondition> scalar_bcs_storage;
    
    // iterate over scalar fields and set up boundary conditions according to the defining file
    for(std::string scalField: scalarFieldsList)
    {
        std::string readFile = cwdStr + "/fields/" + scalField + ".txt";
        double initialValue = std::stod(getValueFromFile(readFile, "initialValue")); // read initial value
        
        scalarField initialField(ny, std::vector<double>(nx, initialValue)); // create an initial field
        eq->initialiseField(scalField, initialField); // use the field from line above to initialise
        
        MyBoundaryCondition bc_scalar;
        auto BC_config = parseFile(readFile);
        
        for (auto &dir : {"north", "south", "west", "east"})
        {
            setScalarBC(bc_scalar, dir, BC_config);
        }
        // store the object by value in the storage map
        scalar_bcs_storage[scalField] = bc_scalar;

        // and keep a stable pointer to it in your original map
        scalar_bcs[scalField] = &scalar_bcs_storage.at(scalField);
    }
    

    // defining director field
    std::vector<std::string> vectorFieldsList = eq->getVectorVariableNames();
    std::map<std::string, const BoundaryCondition*> vector_bcs;
    std::map<std::string, MyBoundaryCondition> vector_bcs_storage; // owns the BC by value. it guarantees that the objects live as long as the map, not just for the loop duration
    
    
    for(std::string vectField: vectorFieldsList)
    {
        std::string readFile = cwdStr + "/fields/" + vectField + ".txt";
        std::string initialValueString = getValueFromFile(readFile, "initialValue");
        std::vector<double> initialValue = convertStringToVector(initialValueString);
        
        vectorField initialVectorField(ny, std::vector<std::vector<double>>(nx, initialValue));
        eq->initialiseField(vectField, initialVectorField);
        
        MyBoundaryCondition bc_vector;
        auto BC_config = parseFile(readFile);
        
        for (auto &dir : {"north", "south", "west", "east"})
        {
            setVectorBC(bc_vector, dir, BC_config);
        }
        // store the object by value in the storage map
        vector_bcs_storage[vectField] = bc_vector;

        // and keep a stable pointer to it in your original map
        vector_bcs[vectField] = &vector_bcs_storage.at(vectField);
    }
    
    Solver solver(*eq, grid);
    
    std::map<std::string, double> simulationSetup = readSimulationInfo(cwdStr + "/simulationSetup.txt");
    solver.solve(simulationSetup.at("iterations"), simulationSetup.at("dt"), scalar_bcs, vector_bcs); // arguments: steps, dt, BCs
    
    // saving fields to csv format
    std::vector<std::tuple<std::string, std::string>> savedFields = fieldsToSave(cwdStr + "/savingFieldsCSV.txt");

    for(std::tuple<std::string, std::string> fieldToSave: savedFields)
    {
        std::string fieldName = std::get<0>(fieldToSave);
        std::string pathName = std::get<1>(fieldToSave);
        
        if(std::find(scalarFieldsList.begin(), scalarFieldsList.end(), fieldName) != scalarFieldsList.end())
        {
            CSVExporter::saveToCSVWithCoordinates(grid, eq->getScalarField(fieldName), pathName);
        }
        if(std::find(vectorFieldsList.begin(), vectorFieldsList.end(), fieldName) != vectorFieldsList.end())
        {
            CSVExporter::saveToCSVWithCoordinates(grid, eq->getVectorField(fieldName), pathName);
        }
        
    }
    
    // saving fields to VTK format
    
    std::vector<std::tuple<std::string, std::string>> savedFieldsVTK = fieldsToSave(cwdStr + "/savingFieldsVTK.txt");

    for(std::tuple<std::string, std::string> fieldToSave: savedFieldsVTK)
    {
        std::string fieldName = std::get<0>(fieldToSave);
        std::string pathName = std::get<1>(fieldToSave);
        
        if(std::find(scalarFieldsList.begin(), scalarFieldsList.end(), fieldName) != scalarFieldsList.end())
        {
            VTKExporter::saveScalarField(grid, eq->getScalarField(fieldName), pathName, fieldName);
        }
        if(std::find(vectorFieldsList.begin(), vectorFieldsList.end(), fieldName) != vectorFieldsList.end())
        {
            VTKExporter::saveVectorField(grid, eq->getVectorField(fieldName), pathName, fieldName);
        }

        
    }
    
}
