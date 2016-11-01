#ifndef LBP_H
#define LBP_H

#include <vector>
#include "matrix.h"
#include "histogram.h"
#include "../bridge/include/EasyBMP.h"

class LBP
{
    int img_size;
    int n_cells;
    int cell_size;

public:
    LBP(int img_size, int n_cells);
    std::vector<float> &Extract(BMP *bmp, std::vector<float> &features);

private:
    BYTE Value(Matrix<double>& image, uint y, uint x);

};

#endif
