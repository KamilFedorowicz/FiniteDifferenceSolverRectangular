#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>   // For std::ofstream
#include <string>    // For std::string

class Grid {
public:
    const int nx, ny;
    std::vector<double> x, y;
    double start_x = 0, start_y = 0;
    double end_x = 1, end_y = 1;




    Grid(double start_x, double start_y, double end_x, double end_y, int nx, int ny)
        :start_x(start_x), start_y(start_y), end_x(end_x), end_y(end_y), nx(nx), ny(ny) {
        double step_x = (end_x - start_x) / (nx - 1);
        double step_y = (end_y - start_y) / (ny - 1);

        for (int i = 0; i < nx; ++i)
            x.push_back(start_x + i * step_x);
        for (int i = 0; i < ny; ++i)
            y.push_back(start_y + i * step_y);
    }

    ~Grid() = default;


};
