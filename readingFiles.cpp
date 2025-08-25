#include "readingFiles.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <cctype>

// trim from start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(),
                         [](unsigned char ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); }).base(),
            s.end());
}

// trim from both ends (in place)
inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

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
        std::string foundKey;
        if (!(iss >> foundKey)) continue;  // no key found, skip

        if (foundKey == key) {
            std::string value;
            std::getline(iss, value);  // read the rest of the line after the key

            // trim whitespace
            auto trim = [](std::string &s) {
                s.erase(0, s.find_first_not_of(" \t\r\n"));
                s.erase(s.find_last_not_of(" \t\r\n") + 1);
            };
            trim(value);

            // remove trailing ';'
            if (!value.empty() && value.back() == ';') {
                value.pop_back();
            }
            trim(value);

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
    std::string line;

    auto trim = [](std::string &s) {
        s.erase(0, s.find_first_not_of(" \t\r\n"));
        s.erase(s.find_last_not_of(" \t\r\n") + 1);
    };

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string key;
        if (!(iss >> key)) continue;  // skip empty lines

        std::string value;
        std::getline(iss, value);     // get the rest of the line
        trim(value);

        // remove trailing ';'
        if (!value.empty() && value.back() == ';') {
            value.pop_back();
        }
        trim(value);

        if (!key.empty() && !value.empty()) {
            config[key] = value;
        }
    }
    return config;
}

void setScalarBC(MyBoundaryCondition &bc, const std::string &dir,
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

    // set BC - either fixed value of zero gradient
    if (dir == "north") bc.setNorthType(type);
    else if (dir == "south") bc.setSouthType(type);
    else if (dir == "west") bc.setWestType(type);
    else if (dir == "east") bc.setEastType(type);

    // if it is a fixed value, then set a value
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

void setVectorBC(MyBoundaryCondition &bc, const std::string &dir,
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
        
        std::string vectorString = valIt->second;
        std::vector<double> val = convertStringToVector(vectorString);
        if (dir == "north") bc.setNorthValue(val);
        else if (dir == "south") bc.setSouthValue(val);
        else if (dir == "west") bc.setWestValue(val);
        else if (dir == "east") bc.setEastValue(val);
    }
}

std::vector<double> convertStringToVector(std::string s)
{
    std::vector<double> result;
    // Remove braces { }
    s.erase(remove(s.begin(), s.end(), '{'), s.end());
    s.erase(remove(s.begin(), s.end(), '}'), s.end());
    
    // Use stringstream to split by comma
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, ','))
    {
        // Trim whitespace
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        
        // Convert to double
        result.push_back(std::stod(token));
    }
    
    return result;
}

std::vector<std::tuple<std::string, double, double>> readMonitors(std::string filename)
{
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Could not open monitor info file: " + filename);
    }
    
    std::vector<std::tuple<std::string, double, double>> result{};
    
    std::string line;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        if (line.empty()) continue;

        std::string variable;
        double x, y;

        std::getline(iss, variable, ',');  // read up to first comma
        iss >> x;
        iss.ignore(std::numeric_limits<std::streamsize>::max(), ',');               // discard until it finds a delimiter ','
        iss >> y;
        iss.ignore(std::numeric_limits<std::streamsize>::max(), ';');               // skip semicolon (and maybe a space)

        result.emplace_back(variable, x, y);
        
    }
    return result;
}

// the first string is the field name, the second is the file path to save
std::vector<std::tuple<std::string, std::string>> fieldsToSave(std::string filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open saving fields info file: " + filename);
    }
    
    std::vector<std::tuple<std::string, std::string>> result{};
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string variable, filePath;
        
        // variable up to comma
        if (!std::getline(iss, variable, ',')) continue;

        // filePath up to semicolon
        if (!std::getline(iss, filePath, ';')) continue;

        // trim spaces
        trim(variable);
        trim(filePath);

        result.emplace_back(variable, filePath);
        
    }
    return result;
    
}


std::map<std::string, double> readSimulationInfo(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::map<std::string, double> simulationInfoMap;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string name, valueStr;

        // read name up to comma
        if (!std::getline(iss, name, ',')) continue;

        // read value up to semicolon
        if (!std::getline(iss, valueStr, ';')) continue;

        // trim spaces from both ends
        trim(name);
        trim(valueStr);

        // convert value to double
        double value = std::stod(valueStr);

        // insert into map
        simulationInfoMap[name] = value;
    }
    
    // check required keys
    if (simulationInfoMap.find("iterations") == simulationInfoMap.end() ||
        simulationInfoMap.find("dt") == simulationInfoMap.end())
    {
        throw std::runtime_error("Simulation info incomplete: 'iterations' or 'dt' missing");
    }

    return simulationInfoMap;
}
