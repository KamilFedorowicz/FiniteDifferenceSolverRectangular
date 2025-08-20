#pragma once
#include <string>
#include <map>
#include <vector>
#include "Grid.h"
#include "EquationBase.h"

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
