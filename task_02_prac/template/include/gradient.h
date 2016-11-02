#ifndef TEMPLATE_GRADIENT_H
#define TEMPLATE_GRADIENT_H

#include "matrix.h"
#include "../externals/EasyBMP/include/EasyBMP.h"

Matrix<float> BMPToGrayscale(BMP* bmp);
namespace simple{
    Matrix<float> gradient(const Matrix<float>& bmp);
}

namespace sse{
    Matrix<float> gradient(const Matrix<float>& bmp);
}

#endif //TEMPLATE_GRADIENT_H
