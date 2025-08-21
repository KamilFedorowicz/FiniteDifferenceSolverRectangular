#include "readingFiles.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

// Reads mesh info from a file into a map
std::map<std::string, double> readMeshInfo(const std::string& filename) {
    std::ifstream file(filename);
    std::map<std::string, double> values;

    if (!file.is_open()) {
        throw std::runtime_error("Could not open mesh info file: " + filename);
    }

    std::string key;
    double value;
    char semicolon;

    while (file >> key >> value >> semicolon) {
        if (!key.empty() && key.back() == ';') {
            key.pop_back();
        }
        values[key] = value;
    }

    return values;
}

// Reads constants from a file
std::map<std::string, double> readConstantsFromFile(
    const std::vector<std::string>& requiredConstants,
    const std::string& filename
) {
    std::map<std::string, double> constantsMap;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name, valueStr;
        if (!(iss >> name >> valueStr)) {
            continue; // skip invalid lines
        }

        if (!valueStr.empty() && valueStr.back() == ';') {
            valueStr.pop_back();
        }

        if (name != "EquationName") {
            double value = std::stod(valueStr);
            constantsMap[name] = value;
        }
    }

    // Check that all required constants are present
    for (const auto& key : requiredConstants) {
        if (constantsMap.find(key) == constantsMap.end()) {
            throw std::runtime_error("Missing constant in file: " + key);
        }
    }

    return constantsMap;
}

// Reads the equation name from a file
EquationNames readEquationName(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name, valueStr;
        if (!(iss >> name >> valueStr)) {
            continue; // skip invalid lines
        }

        if (!valueStr.empty() && valueStr.back() == ';') {
            valueStr.pop_back();
        }

        if (name == "EquationName") {
            if (valueStr == "Equation02") {
                return EquationNames::Equation02;
            }
        }
    }

    return EquationNames::Empty;
}

std::string getValueFromFile(const std::string &filename, const std::string &key) {
    std::ifstream infile(filename);
    if (!infile) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string foundKey, value;
        if (!(iss >> foundKey >> value)) continue;

        // remove trailing ';'
        if (!value.empty() && value.back() == ';') {
            value.pop_back();
        }

        if (foundKey == key) {
            return value;
        }
    }

    throw std::runtime_error("Key not found: " + key);
}


// string -> BCType
BCType parseBCType(const std::string &s) {
    if (s == "FixedValue") return BCType::FixedValue;
    if (s == "ZeroGradient") return BCType::ZeroGradient;
    throw std::runtime_error("Unknown BCType: " + s);
}

std::map<std::string, std::string> parseFile(const std::string &filename) {
    std::ifstream infile(filename);
    if (!infile) throw std::runtime_error("Could not open " + filename);

    std::map<std::string, std::string> config;
    std::string key, value;
    while (infile >> key >> value) {
        if (!value.empty() && value.back() == ';') value.pop_back();
        config[key] = value;
    }
    return config;
}

void setBC(MyBoundaryCondition &bc, const std::string &dir,
           const std::map<std::string, std::string> &config)
{
    std::string typeKey  = dir + "Type";   // e.g. "northType"
    std::string valueKey = dir + "Value";  // e.g. "northValue"

    auto typeIt = config.find(typeKey);
    if (typeIt == config.end()) {
        std::cout << "Missing key: " + typeKey << std::endl;
        return;
    }

    BCType type = parseBCType(typeIt->second);

    if (dir == "north") bc.setNorthType(type);
    else if (dir == "south") bc.setSouthType(type);
    else if (dir == "west") bc.setWestType(type);
    else if (dir == "east") bc.setEastType(type);

    if (type == BCType::FixedValue) {
        auto valIt = config.find(valueKey);
        if (valIt == config.end()) {
            throw std::runtime_error("Missing key: " + valueKey);
        }
        double val = std::stod(valIt->second);
        if (dir == "north") bc.setNorthValue(val);
        else if (dir == "south") bc.setSouthValue(val);
        else if (dir == "west") bc.setWestValue(val);
        else if (dir == "east") bc.setEastValue(val);
    }
}
