#pragma once
#include <string>
#include <map>
#include <vector>
#include "Grid.h"
#include "EquationBase.h"
#include "MyBoundaryCondition.h"

// Function declarations

// Reads mesh info from a file into a map
std::map<std::string, double> readMeshInfo(const std::string& filename);

// Reads constants from a file and checks that all required constants exist
std::map<std::string, double> readConstantsFromFile(
    const std::vector<std::string>& requiredConstants,
    const std::string& filename
);

// Reads the equation name from a file
EquationNames readEquationName(const std::string& filename);

std::string getValueFromFile(const std::string &filename, const std::string &key);

BCType parseBCType(const std::string &s);

std::map<std::string, std::string> parseFile(const std::string &filename);

void setScalarBC(MyBoundaryCondition &bc, const std::string &dir,
           const std::map<std::string, std::string> &config);

void setVectorBC(MyBoundaryCondition &bc, const std::string &dir,
                 const std::map<std::string, std::string> &config);

std::vector<double> convertStringToVector(std::string str);

std::vector<std::tuple<std::string, double, double>> readMonitors(std::string filename);
