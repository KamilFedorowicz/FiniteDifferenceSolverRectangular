#pragma once
#include "Grid.h"
#include "Laplacian.h"
#include "Gradient.h"
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

void runCase1_Equation02() {
        
    // READING GRID INFO
    auto meshInfo = readMeshInfo("/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/simulationFiles/meshInfo.txt");
    
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
    std::string equationFile = "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/simulationFiles/equationInfo.txt";
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
                  << " was not found in the equation database. Aborting!" << std::endl;
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
        std::string readFile = "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/simulationFiles/fields/" + scalField + ".txt";
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
        std::string readFile = "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/simulationFiles/fields/" + vectField + ".txt";
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
    
    
    /*
    vectorField initialDirectorField(ny, std::vector<std::vector<double>>(nx, std::vector<double>{1,0}));
    eq->initialiseField("director", initialDirectorField);
    
    MyBoundaryCondition bc_director;
    bc_director.setNorthType(BCType::FixedValue);
    bc_director.setSouthType(BCType::FixedValue);
    bc_director.setWestType(BCType::FixedValue);
    bc_director.setEastType(BCType::FixedValue);
    bc_director.setNorthValue({ 1,0 });
    bc_director.setSouthValue({ 1,0 });
    bc_director.setWestValue({ 0,1 });
    bc_director.setEastValue({ 0,1 });
    
    vector_bcs["director"] = &bc_director;
    */
    

    // defining monitors
    Solver solver(*eq, grid);
    VariableMonitor monitorScalar1(grid, 0.5, 0.4, "temperature");
    solver.addScalarVariableMonitor(monitorScalar1);
    
    VariableMonitor monitorVector1(grid, 0.5, 0.4, "director");
    solver.addVectorVariableMonitor(monitorVector1);
    
    VariableMonitor monitorVector2(grid, 0.5, 0.5, "director");
    solver.addVectorVariableMonitor(monitorVector2);
    
    solver.solve(50, 0.001, scalar_bcs, vector_bcs); // arguments: steps, dt, BCs
    
    std::vector<double> monitorScalarResult1 = monitorScalar1.returnMonitoredScalarVariable();
    std::vector<std::vector<double>> monitorVectorResult1 = monitorVector1.returnMonitoredVectorVariable();
    std::vector<std::vector<double>> monitorVectorResult2 = monitorVector2.returnMonitoredVectorVariable();

    
    CSVExporter::saveToCSVWithCoordinates(grid, eq->getVectorField("director"), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/directorField.csv");
    CSVExporter::saveToCSVWithCoordinates(grid, eq->getVectorFieldMagnitude("director"), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/director_mag.csv");
    CSVExporter::saveToCSVWithCoordinates(grid, eq->getScalarField("temperature"), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/temp_field.csv");
    CSVExporter::saveMonitoredVariable(monitorScalarResult1, "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/temp_monitor.csv");
    CSVExporter::saveMonitoredVariable(getVectorComponent(monitorVectorResult1,0), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/dir_monitor1.csv");
    CSVExporter::saveMonitoredVariable(getVectorComponent(monitorVectorResult2,0), "/Users/Kamil/Desktop/cpp/work_udemy/my_solver2/my_solver2/CSV_files/dir_monitor2.csv");
    


    
    /*
     CSVExporter::saveToCSVWithCoordinates(grid, eq.getVectorField("director"), "C:/Users/kfedorowicz/source/repos/project_reveal/csv_files/dir_field.csv");
     CSVExporter::saveToCSVWithCoordinates(grid, eq.getVectorFieldMagnitude("director"), "C:/Users/kfedorowicz/source/repos/project_reveal/csv_files/dir_mag.csv");
     CSVExporter::saveToCSVWithCoordinates(grid, eq.getScalarField("temperature"), "C:/Users/kfedorowicz/source/repos/project_reveal/csv_files/temp.csv");
     */
     
     
}
