#pragma once
#include <vector>
#include <map>
#include <string>
#include <array>

using scalarField = std::vector<std::vector<double>>;
using vectorField = std::vector<std::vector<std::vector<double>>>;
using tensorField = std::vector<std::vector<std::vector<std::vector<double>>>>;

using scalarFieldMap = std::map<std::string, scalarField*>;
using vectorFieldMap = std::map<std::string, vectorField*>;

using tensor = std::array<std::array<double, 2>, 2>;
