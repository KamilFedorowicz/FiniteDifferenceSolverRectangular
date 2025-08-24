#include <iostream>
#include "Case1_Equation01.h"
#include "Case1_Equation02.h"

// use this to get info about errors
// xcodebuild -scheme my_solver2 -configuration Debug -destination 'platform=macOS'

int main() {

    // source files in txt files like in openfoam - that would substitute Case1_Equation01 etc
    /*
     how to structure the files?
     1. file for equation parameters, constants etc, dt
     2. file for boundary conditions for all fields, initial fields
     3. file for saving things
     4. file for the mesh
     
     next: understand recent changes - EquationFactory.h file
     keep working on reading simulation info from the file
     next in the future: save files as csv
     tomorrow: reading vector files
     */
    

    
    // how to make it compile etc?
    
    // runCase1_Equation01();

    runCase1_Equation02();

    
    
    std::cout << "Simulation ended!" << std::endl;


    return 0;
}
