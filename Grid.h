#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>   // For std::ofstream
#include <string>    // For std::string
#include <cassert> // for assert

class Grid {
public:
    
    double get_x(int i) const {
        assert(i>=0 && i<=nx);
        if (nx > 1) {
            return x[i];
        }
        else {
            return (start_x+end_x)/2;
        }
    }
    
    double get_y(int i) const {
        assert(i>=0 && i<=ny);
        if (ny > 1) {
            return y[i];
        }
        else {
            return (start_y+end_y)/2;
        }
        
    }
    
    const double get_x_lim(int i) const {
        assert(i==0 || i==1);
        if(i==0){
            return start_x;
        } else if(i==1){
            return end_x;
        }
        
        return 0; // this should never happen
    }
    
    const double get_y_lim(int i) const {
        assert(i==0 || i==1);
        if(i==0){
            return start_y;
        } else if(i==1){
            return end_y;
        }
        
        return 0; // this should never happen
    }

    const int get_nx() const {
        return nx;
    }
    const int get_ny() const {
        return ny;
    }



    Grid(double start_x, double start_y, double end_x, double end_y, int nx, int ny)
        :start_x(start_x), start_y(start_y), end_x(end_x), end_y(end_y), nx(nx), ny(ny) {
        double step_x = (end_x - start_x) / (nx - 1);
        double step_y = (end_y - start_y) / (ny - 1);

            for (int i = 0; i < nx; ++i){
                x.push_back(start_x + i * step_x);
            }
            
            for (int i = 0; i < ny; ++i){
                y.push_back(start_y + i * step_y);
            }
    }

    ~Grid() = default;
    
private:
    const int nx, ny;
    std::vector<double> x, y;
    double start_x = 0, start_y = 0;
    double end_x = 1, end_y = 1;


};
