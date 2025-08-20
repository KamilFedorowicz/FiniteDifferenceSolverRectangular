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
