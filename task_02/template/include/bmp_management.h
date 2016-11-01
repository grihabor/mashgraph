#ifndef BMP_MANAGEMENT_H
#define BMP_MANAGEMENT_H

#include "matrix.h"

class BMP;
struct RGBApixel;

BMP Resize(BMP* image, uint width, uint height, BMP& bmp);
Matrix<double> BMPToGrayscale(BMP* bmp);

#endif
