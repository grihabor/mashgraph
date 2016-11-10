#ifndef TEMPLATE_GRADIENT_H
#define TEMPLATE_GRADIENT_H

#include "matrix.h"
#include "EasyBMP.h"

/** @file gradient.h
 * Header for gradient module.
 *
 * This header contains function declarations which are needed for gradient calculation.
 */

/*! @addtogroup Gradient
 * @{
 */

/**
 * Converts RGB image to Grayscale.
 *
 * Takes a BMP image pointed by bmp and returns the image in grayscale.
 */
Matrix<float> BMPToGrayscale(BMP* bmp);

//! namespace with simple implementation
namespace simple{
    /**
     * Computes gradient using naive algorithm.
     *
     * Takes grayscale image and returns it's gradient.
     */
    Matrix<float> gradient(const Matrix<float>& bmp);
}

//! namespace with sse implementation
namespace sse{
    /**
     * Computes gradient using sse.
     *
     * Takes grayscale image and returns it's gradient.
     */
    Matrix<float> gradient(const Matrix<float>& bmp);
}

/*
 * @}
 */

#endif //TEMPLATE_GRADIENT_H
