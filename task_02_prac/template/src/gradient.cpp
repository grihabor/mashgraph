#include "gradient.h"

#include <iostream>
#include <xmmintrin.h>

/** @file gradient.cpp
 * Implementation for gradient module.
 *
 * This file contatins implementation of functions for gradient calculation.
 */

using std::vector;
using std::ostream;

Matrix<float> BMPToGrayscale(BMP* bmp)
{
    BMP& image = *bmp;
    Matrix<float> result(bmp->TellHeight(), bmp->TellWidth());

    for(uint y = 0, x; y < result.n_rows; ++y){
        for(x = 0; x < result.n_cols; ++x){
            result(y, x) = 0.299 * image(x, y)->Red
                           + 0.587 * image(x, y)->Green
                           + 0.114 * image(x, y)->Blue;
        }
    }

    return result;
}

/**
 * Performs itemwise square computation.
 *
 * Takes matrix IMG and returns it's square.
 */
template<typename T>
Matrix<T> Square(const Matrix<T> &IMG)
{
    Matrix<T> result(IMG.n_rows, IMG.n_cols);
    T t;
    for(uint i = 0; i < IMG.n_rows; ++i){
        for(uint j = 0; j < IMG.n_cols; ++j){
            t = IMG(i, j)*IMG(i, j);
            result(i, j) = t;
        }
    }
    return result;
}

/**
 * Performs itemwise addition.
 *
 * Takes matrices m1 and m2 and returns their sum.
 */
template<typename T>
Matrix<T> operator +(const Matrix<T> &m1, const Matrix<T> &m2)
{
    Matrix<T> result(m1.n_rows, m1.n_cols);
    T t;
    for(uint i = 0; i < m2.n_rows; ++i){
        for(uint j = 0; j < m2.n_cols; ++j){
            t = m1(i, j) + m2(i, j);
            result(i, j) = t;
        }
    }
    return result;
}

/**
 * Performs itemwise square root computation.
 *
 * Takes matrix IMG and returns it's square root.
 */
template<typename T>
Matrix<T> sqrt(const Matrix<T> &IMG)
{
    Matrix<T> result(IMG.n_rows, IMG.n_cols);
    T t;
    for(uint i = 0; i < IMG.n_rows; ++i){
        for(uint j = 0; j < IMG.n_cols; ++j){
            t = sqrt(IMG(i, j));
            result(i, j) = t;
        }
    }
    return result;
}

/**
 * Class to use with Matrix<T>::unary_map(const UnaryMatrixOperator &)
 */
template<typename T>
class CustomOp
{
    Matrix<T> kernel;
public:
    int vert_radius;
    int hor_radius;

    CustomOp(const Matrix<T>& kern)
            : kernel(kern), vert_radius(kern.n_rows/2), hor_radius(kern.n_cols/2)
    {}

    T operator()(const Matrix<T>& neighbourhood) const
    {
        T value = 0.;
        for(uint i = 0, j; i < neighbourhood.n_rows; ++i){
            for(j = 0; j < neighbourhood.n_cols; ++j){
                value += neighbourhood(i, j) * kernel(i, j);
            }
        }
        return value;
    }
};

/**
 * Applies filter to an image.
 *
 * Takes image src_image, applies filter with kernel and returns the result.
 */
template<typename T>
Matrix<T> custom(const Matrix<T>& src_image, const Matrix<T>& kernel) {

    if((kernel.n_cols % 2) * (kernel.n_rows % 2) == 0){
        throw "custom: Invalid kernel";
    }
    return src_image.unary_map(CustomOp<float>(kernel));
}

/**
 * Calculates horizontal sobel filter.
 */
Matrix<float> sobel_x(const Matrix<float>& src_image) {
    Matrix<float> kernel = {{-1, 0, 1},
                            {-2, 0, 2},
                            {-1, 0, 1}};
    return custom(src_image, kernel);
}

/**
 * Calculates vertical sobel filter.
 */
Matrix<float> sobel_y(const Matrix<float>& src_image) {
    Matrix<float> kernel = {{-1, -2, -1},
                            {0, 0, 0},
                            {1, 2, 1}};
    return custom(src_image, kernel);
}

/**
 * Gradient function simple implementation.
 */
Matrix<float> simple::gradient(const Matrix<float>& image) {

    Matrix<float> grad_x = sobel_x(image);
    Matrix<float> grad_y = sobel_y(image);
    return sqrt(Square(grad_x) + Square(grad_y));
}
/*
ostream& operator<<(ostream& os, __m128 x)
{
    float data[] = {0, 0, 0, 0};
    _mm_storeu_ps(data, x);
    cout << data[0] << ' ' << data[1] << ' ' << data[2] << ' ' << data[3] << endl;
    return os;
}

ostream& operator<<(ostream& os, const float *data)
{
    cout << data[0] << ' ' << data[1] << ' ' <<
            data[2] << ' ' << data[3] << ' ' <<
            data[4] << ' ' << data[5] << endl;
    return os;
}

*/

/**
 * Calculates 4 gradients and returns them.
 *
 * Takes image IMG with size of 3x6 and calculates gradients for elements IMG[1][1], IMG[1][2], IMG[1][3] and IMG[1][4]
 *
 * @param row1 float[6] aka IMG+0
 *
 * This is first row of the image IMG
 * @param row2 float[6] aka IMG+1
 *
 * This is second row of the image IMG
 * @param row3 float[6] aka IMG+2
 *
 * This is third row of the image IMG
 * @return 4 gradients for elements IMG[1][1], IMG[1][2], IMG[1][3] and IMG[1][4]
 */
inline __m128 gradient_vec(float *row1, float *row2, float *row3)
{
    //calc 1st line
    float * raw = row1;
    __m128 a11 = _mm_loadu_ps(raw);
    __m128 a12 = _mm_loadu_ps(raw + 1);
    a12 = _mm_add_ps(a12, a12);
    __m128 a13 = _mm_loadu_ps(raw + 2);

    // calc 2nd line
    raw = row2;
    __m128 a21 = _mm_loadu_ps(raw);
    a21 = _mm_add_ps(a21, a21);
    __m128 a23 = _mm_loadu_ps(raw + 2);
    a23 = _mm_add_ps(a23, a23);

    // calc 3rd line
    raw = row3;
    __m128 a31 = _mm_loadu_ps(raw);
    __m128 a32 = _mm_loadu_ps(raw + 1);
    a32 = _mm_add_ps(a32, a32);
    __m128 a33 = _mm_loadu_ps(raw + 2);

    //calc sobel x
    __m128 res_x = _mm_add_ps(a31, a32);
    res_x = _mm_add_ps(res_x, a33);
    res_x = _mm_sub_ps(res_x, a11);
    res_x = _mm_sub_ps(res_x, a12);
    res_x = _mm_sub_ps(res_x, a13);

    //calc sobel y
    __m128 res_y = _mm_add_ps(a13, a23);
    res_y = _mm_add_ps(res_y, a33);
    res_y = _mm_sub_ps(res_y, a11);
    res_y = _mm_sub_ps(res_y, a21);
    res_y = _mm_sub_ps(res_y, a31);

    //calc gradient
    res_x = _mm_mul_ps(res_x, res_x);
    res_y = _mm_mul_ps(res_y, res_y);

    __m128 grad = _mm_sqrt_ps(_mm_add_ps(res_x, res_y));

    return grad;
}

/**
 * Gradient function SSE implementation.
 */
Matrix<float> sse::gradient(const Matrix<float>& image)
{
    Matrix<float> img(image);
    img = img.extra_borders(1, 1);
    Matrix<float> r(img.n_rows - 2, img.n_cols - 2);


    int x, y;
    float *raw;

    // TODO: check borders
    for(y = 0; y < int(img.n_rows - 2); ++y) {
        for(x = 0; x < int(img.n_cols - 5); x += 4) {

            raw = img.raw_ptr() + (y*img.n_cols + x);
            __m128 grad_vec = gradient_vec(raw, raw + img.n_cols, raw + 2 * img.n_cols);
            // store result
            _mm_storeu_ps(r.raw_ptr() + (y*r.n_cols + x), grad_vec);
        }

        // number of pixels left
        int n = (img.n_cols - 2) - x;

        // all pixels calculated
        if(n == 0){
            continue;
        }

        // if not: create temp buffers
        // 1            [          ]
        // 2               [          ]
        // 3                  [          ]
        float data1[] = {0, 0, 0, 0, 0, 0};
        float data2[] = {0, 0, 0, 0, 0, 0};
        float data3[] = {0, 0, 0, 0, 0, 0};

        // fill buffers from image
        for(int i = 0; i < n + 2; ++i) {
            data1[i] = img(y, x + i);
            data2[i] = img(y + 1, x + i);
            data3[i] = img(y + 2, x + i);
        }

        // calc gradient and store it in resulting image
        __m128 grad_vec = gradient_vec(data1, data2, data3);
        _mm_storeu_ps(data1, grad_vec);
        for(int i = 0; i < n; ++i) {
            r(y, x + i) = data1[i];
        }
    }


    return r;

}
