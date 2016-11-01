#ifndef HOG_H
#define HOG_H

#include <vector>
class BMP;

class HOG
{
    int img_size;
    int n_cells;
    int n_cells_in_block;
    int n_bins;
    int n_blocks;
    int cell_size;

public:
    HOG(int img_size, int n_cells, int n_cells_in_block, int n_bins);

    std::vector<float>&
    Extract(BMP* image, std::vector<float>& features);
};


#endif
