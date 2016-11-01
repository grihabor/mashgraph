#include "../include/color_descriptor.h"
#include "../externals/EasyBMP/include/EasyBMP.h"
#include "../include/bmp_management.h"

#include <iostream>
using namespace std;

ColorDescriptor::ColorDescriptor(int img_size_in, int n_cells_in)
    : img_size(img_size_in), n_cells(n_cells_in), cell_size(img_size / n_cells)
{}

std::vector<float>&
ColorDescriptor::Extract(BMP* image, std::vector<float>& features)
{
    BMP bmp;
    Resize(image, img_size, img_size, bmp);

    features.reserve(features.size() + n_cells * n_cells * 3);

    double k = 1. / (cell_size * cell_size * 256);

    for(int y = 0, x, i, j; y < n_cells; ++y){
        for(x = 0; x < n_cells; ++x){

            float r = 0;
            float g = 0;
            float b = 0;

            for(i = 0; i < cell_size; ++i){
                for(j = 0; j < cell_size; ++j){
                    RGBApixel *pixel = bmp(y * cell_size + i, x * cell_size + j);
                    r += pixel->Red;
                    g += pixel->Green;
                    b += pixel->Blue;
                }
            }
            r *= k;
            g *= k;
            b *= k;

            features.push_back(r);
            features.push_back(g);
            features.push_back(b);
        }
    }
    return features;
}
