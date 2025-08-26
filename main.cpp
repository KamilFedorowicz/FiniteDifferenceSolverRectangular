#include <iostream>
#include "runSimulationFile.h"

// use this to get info about errors
// xcodebuild -scheme my_solver2 -configuration Debug -destination 'platform=macOS'

int main() {

    // source files in txt files like in openfoam - that would substitute Case1_Equation01 etc
    /*
     next: save as a csv file
     */
    

    


    runSimulation();

    
    
    std::cout << "Simulation ended!" << std::endl;

    // clang++ -std=c++17 *.cpp -o mySolver
    // above compiles all source files and when when running ./mySolver it runs the solution
    

    return 0;
}
