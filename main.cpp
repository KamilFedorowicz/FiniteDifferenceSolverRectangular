#include <iostream>
#include "Grid.h"
#include "Laplacian.h"

void print(std::vector<std::vector<double>> test){
    for(int i=0; i<test.size(); i++){
        for(int j=0; j<test[0].size(); j++){
            std::cout << test[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, const char * argv[]) {


    double start_x = 0;
    double start_y = 0;
    double end_x = 1;
    double end_y = 1;
    double nx = 20;
    double ny = 20;
    
    Grid grid(start_x,start_y,end_x,end_y,nx,ny);
    
    std::vector<std::vector<double>> test(grid.ny, std::vector<double>(grid.nx, 0));
    std::vector<std::vector<double>> result = Laplacian::compute(grid, test);
    std::cout << result.size() << " " << result[0].size() << std::endl;
    print(result);
    
    return 0;
}
