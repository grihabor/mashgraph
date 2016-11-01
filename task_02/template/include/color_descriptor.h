#ifndef COLOR_DESCRIPTOR_H
#define COLOR_DESCRIPTOR_H

#include <vector>

class BMP;

class ColorDescriptor
{
    int img_size;
    int n_cells;
    int cell_size;

public:
    ColorDescriptor(int img_size, int n_cells);

    std::vector<float>&
    Extract(BMP* image, std::vector<float>& features);
};

#endif
