#include "../include/lbp.h"
#include "../include/histogram.h"
#include "../externals/EasyBMP/include/EasyBMP.h"
#include "../include/bmp_management.h"

using std::vector;

LBP::LBP(int img_size_in, int n_cells_in)
    : img_size(img_size_in), n_cells(n_cells_in), cell_size(img_size / n_cells)
{}

BYTE LBP::Value(Matrix<double>& image, uint y, uint x)
{
    BYTE r = 0;
    if(image(y, x) <= image(y, x+1))
        r |= 0x01;
    if(image(y, x) <= image(y-1, x+1))
        r |= 0x02;
    if(image(y, x) <= image(y-1, x))
        r |= 0x04;
    if(image(y, x) <= image(y-1, x-1))
        r |= 0x08;
    if(image(y, x) <= image(y, x-1))
        r |= 0x10;
    if(image(y, x) <= image(y+1, x-1))
        r |= 0x20;
    if(image(y, x) <= image(y+1, x))
        r |= 0x40;
    if(image(y, x) <= image(y+1, x+1))
        r |= 0x80;
    return r;
}

vector<float>& LBP::Extract(BMP* bmp_in, vector<float>& features)
{
    BMP bmp;
    Resize(bmp_in, img_size, img_size, bmp);
    Matrix<double> img = BMPToGrayscale(&bmp);

    Matrix<double> image = img.extra_borders(1, 1);

    for(int y = 0, x, i, j; y < n_cells; ++y){
        for(x = 0; x < n_cells; ++x){

            vector<BYTE> cell;
            for(i = 0; i < cell_size; ++i){
                for(j = 0; j < cell_size; ++j){
                    BYTE v = Value(image, y * cell_size + i + 1,
                                              x * cell_size + j + 1);
                    cell.push_back(v);
                }
            }
            Histogram hist(cell);

            Append(features, hist.Normalize());
        }
    }

    return features;
}
