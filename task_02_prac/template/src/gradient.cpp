#include "../include/gradient.h"

#include <iostream>
#include <stdio.h>
#include <xmmintrin.h>

using std::vector;
using std::cout;
using std::cin;
using std::endl;
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

template<typename T>
Matrix<T> Square(const Matrix<T> &m)
{
    Matrix<T> result(m.n_rows, m.n_cols);
    T t;
    for(uint i = 0; i < m.n_rows; ++i){
        for(uint j = 0; j < m.n_cols; ++j){
            t = m(i, j)*m(i, j);
            result(i, j) = t;
        }
    }
    return result;
}

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

template<typename T>
Matrix<T> sqrt(const Matrix<T> &m)
{
    Matrix<T> result(m.n_rows, m.n_cols);
    T t;
    for(uint i = 0; i < m.n_rows; ++i){
        for(uint j = 0; j < m.n_cols; ++j){
            t = sqrt(m(i, j));
            result(i, j) = t;
        }
    }
    return result;
}


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

template<typename T>
Matrix<T> custom(const Matrix<T>& src_image, const Matrix<T>& kernel) {

    if((kernel.n_cols % 2) * (kernel.n_rows % 2) == 0){
        throw "custom: Invalid kernel";
    }
    return src_image.unary_map(CustomOp<float>(kernel));
}

Matrix<float> sobel_x(const Matrix<float>& src_image) {
    Matrix<float> kernel = {{-1, 0, 1},
                            {-2, 0, 2},
                            {-1, 0, 1}};
    return custom(src_image, kernel);
}

Matrix<float> sobel_y(const Matrix<float>& src_image) {
    Matrix<float> kernel = {{-1, -2, -1},
                            {0, 0, 0},
                            {1, 2, 1}};
    return custom(src_image, kernel);
}

Matrix<float> simple::gradient(const Matrix<float>& image) {

    Matrix<float> grad_x = sobel_x(image);
    cout << endl << "gradient_x" << endl;
    cout << grad_x << endl;
    Matrix<float> grad_y = sobel_y(image);
    cout << "gradient_y" << endl;
    cout << grad_y << endl;

    return sqrt(Square(grad_x) + Square(grad_y));
}

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

__m128 gradient_vec(float *raw1, float *raw2, float *raw3)
{
    //calc 1st line
    float * raw = raw1;
    __m128 a11 = _mm_loadu_ps(raw);
    __m128 a12 = _mm_loadu_ps(raw + 1);
    a12 = _mm_add_ps(a12, a12);
    __m128 a13 = _mm_loadu_ps(raw + 2);

    // calc 2nd line
    raw = raw2;
    __m128 a21 = _mm_loadu_ps(raw);
    a21 = _mm_add_ps(a21, a21);
    __m128 a23 = _mm_loadu_ps(raw + 2);
    a23 = _mm_add_ps(a23, a23);

    // calc 3rd line
    raw = raw3;
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

void log_coords(uint y, uint x, const Matrix<float>& img)
{
    //return;
    cout << '(' << y << ", " << x << ") out of (" <<
         img.n_rows << ", " << img.n_cols << ")" << endl;
}

Matrix<float> sse::gradient(const Matrix<float>& image)
{
    Matrix<float> img(image);
    img = img.extra_borders(1, 1);
    Matrix<float> r(img.n_rows - 2, img.n_cols - 2);

    uint x, y;
    float *raw;

    // TODO: check borders
    for(y = 0; y < img.n_rows - 2; ++y) {
        for(x = 0; x < img.n_cols - 5; x += 4) {

                log_coords(y, x, img);

            raw = img.raw_ptr() + (y*img.n_cols + x);

            __m128 grad = gradient_vec(raw, raw + img.n_cols, raw + 2 * img.n_cols);

            cout << grad;

            cout << r.raw_ptr() + (y*r.n_cols + x);

            // store result
            _mm_storeu_ps(r.raw_ptr() + (y*r.n_cols + x), grad);

            cout << "here";
        }
        cout << "endfor" << endl;



        log_coords(y, x, img);

        // number of pixels left
        int n = (img.n_cols - 2) - x;
        cout << "n = " << n << endl;

        // all pixels calculated
        if(n == 0){
            continue;
        }
        // 1            [          ]
        // 2               [          ]
        // 3                  [          ]
        float data1[] = {0, 0, 0, 0, 0, 0};
        float data2[] = {0, 0, 0, 0, 0, 0};
        float data3[] = {0, 0, 0, 0, 0, 0};
        for(int i = 0; i < n + 2; ++i) {
            data1[i] = img(y, x + i);
            data2[i] = img(y + 1, x + i);
            data3[i] = img(y + 2, x + i);
        }
        cout << data1 << data2 << data3;

        __m128 grad = gradient_vec(data1, data2, data3);

        cout << '!' << grad;

        _mm_storeu_ps(data1, grad);

        cout << data1;
        for(int i = 0; i < n; ++i) {
            r(y, x + i) = data1[i];
        }
        cout << endl;
        cout << r;
    }


    return r;

}
