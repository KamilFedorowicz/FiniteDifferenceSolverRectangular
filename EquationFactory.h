#pragma once
#include <map>
#include <memory>
#include <functional>
#include <string>
#include "EquationBase.h"
#include "Grid.h"
#include "readingFiles.h"

// allows to register equation automatically without editing the factory
class EquationFactoryRegistry {
public:
    // factoryFn is an alias for the lambda function that knows how to construct a pointer to EquationBase.
    using FactoryFn = std::function<std::unique_ptr<EquationBase>(Grid&, const std::string&)>; // creates a new equation base
    // similarly ConstantsFn is a pointer to a function that knows how to construct a vector of strings
    using ConstantsFn = std::function<std::vector<std::string>()>; // return list of constants required by the equation
    
    static EquationFactoryRegistry& instance() // only one instants exists globally
    {
        static EquationFactoryRegistry registry;
        return registry;
    }

    // each equation registers equation itself and the required constants
    void registerFactory(const std::string& name, FactoryFn fn, ConstantsFn constFn) {
        factories[name] = fn;
        constants[name] = constFn;
    }

    // lookup the equation by name and then use grid and constants file to initialise it
    std::unique_ptr<EquationBase> create(const std::string& name, Grid& grid, const std::string& constantsFile) const {
        auto it = factories.find(name);
        if (it == factories.end()) {
            throw std::runtime_error("Equation '" + name + "' is not registered.");
        }
        // it->second is the lambda that knows how to create equation that was registered before
        return (it->second)(grid, constantsFile);
    }
    
    // lookup name in the constants map, call the constant function and return the constants needed
    std::vector<std::string> getRequiredConstants(const std::string& name) const {
        auto it = constants.find(name); // constants is a map that maps from equation name to the vector of strings that contains constants
        if (it == constants.end()) return {};
        return it->second(); // return the vector of strings
    }

private:
    std::map<std::string, FactoryFn> factories;
    std::map<std::string, ConstantsFn> constants;
};

// for each equation this template tells how to build an instance and the constants eqauired
template <typename EqClass> // EqClass is specific equation class
struct EquationRegistrar {
    EquationRegistrar(const std::string& name) {
        // calls the registerFactory function
        EquationFactoryRegistry::instance().registerFactory( // we are calling registerFactory function
            name,
            // the signature matches the factory function, calls this function to build an object
            // input grid and a filename, output std::unique_ptr<EquationBase>
            // this lambda matches the signature of FactoryFn - it returns std::unique_ptr<EquationBase> so it can be converted into FactoryFn
            // all in all, this returns a pointer to the equation base, and this is what the FactoryFn does
            // note that EquationRegistrar runs it calls static functions on EqClass
            // the lambda function is not run when the equation is registered - we do not need the filename yet
            // then when the create function is called, the filename is provided
            // create() looks up the "Equation02" key in the factories map. It finds the lambda we stored earlier. Then it calls the lambda, passing: grid and filename
            [](Grid& grid, const std::string& filename) -> std::unique_ptr<EquationBase> {
                // readConstantsFromFile is defined elsewhere. we just need to include the header
                // filename is caputred by the lambda parameter. filename is passed when the factory function create is called (it has the filename there)
                auto constants = readConstantsFromFile(EqClass::getConstantsNames(), filename); // read values from the file
                return std::make_unique<EqClass>(grid, constants); // construct an object of the correct equation and wrap it in the pointer to EquationBase
            },
            []() -> std::vector<std::string> {
                return EqClass::getConstantsNames(); // call the function getConstantNames
            }
        );
    }
};

// when a new class is created, the EquationRegistrar is run automatically through this command
#define REGISTER_EQUATION(EqClass) \
    static EquationRegistrar<EqClass> reg_##EqClass{#EqClass};

// running the command above we run equationRegistrar
// when we run equationRegistrar, we are registering a new equation

/*
 When you add an equation to the registry (via REGISTER_EQUATION), you are:
 Creating an EquationRegistrar<EqClass> object (a static one).
 That registrar immediately calls registerFactory.
 registerFactory stores two lambdas inside the registry:
 one for constructing an equation object (FactoryFn)
 one for asking which constants are needed (ConstantsFn).
 At this stage, no equation object is created yet. It’s just “knowledge” about how to build one later.
 in summary:
 Registration stage: store a “recipe” (the lambda).
 Creation stage: supply grid and filename, run the recipe, get a live equation object.
 */
