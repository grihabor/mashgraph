#include "../include/hog.h"
#include "../include/histogram.h"
#include "../externals/EasyBMP/include/EasyBMP.h"
#include "../include/bmp_management.h"

using std::vector;


Matrix<double> Square(const Matrix<double> &m)
{
    Matrix<double> result(m.n_rows, m.n_cols);
    double t;
    for(uint i = 0; i < m.n_rows; ++i){
        for(uint j = 0; j < m.n_cols; ++j){
            t = m(i, j)*m(i, j);
            result(i, j) = t;
        }
    }
    return result;
}

Matrix<double> operator +(const Matrix<double> &m1, const Matrix<double> &m2)
{
    Matrix<double> result(m1.n_rows, m1.n_cols);
    int t;
    for(uint i = 0; i < m2.n_rows; ++i){
        for(uint j = 0; j < m2.n_cols; ++j){
            t = m1(i, j) + m2(i, j);
            result(i, j) = t;
        }
    }
    return result;
}

Matrix<double> sqrt(const Matrix<double> &m)
{
    Matrix<double> result(m.n_rows, m.n_cols);
    double t;
    for(uint i = 0; i < m.n_rows; ++i){
        for(uint j = 0; j < m.n_cols; ++j){
            t = sqrt(m(i, j));
            result(i, j) = t;
        }
    }
    return result;
}

Matrix<double> atan2(const Matrix<double> &y, const Matrix<double> &x)
{
    Matrix<double> result(y.n_rows, y.n_cols);
    double t;
    for(uint i = 0; i < y.n_rows; ++i){
        for(uint j = 0; j < y.n_cols; ++j){
            t = atan2(y(i, j), x(i, j));
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
    return src_image.unary_map(CustomOp<double>(kernel));
}

Matrix<double> sobel_x(const Matrix<double>& src_image) {
    Matrix<double> kernel(1, 3);
    kernel(0, 0) = -1;
    kernel(0, 1) =  0;
    kernel(0, 2) =  1;
    return custom(src_image, kernel);
}

Matrix<double> sobel_y(const Matrix<double>& src_image) {
    Matrix<double> kernel(3, 1);
    kernel(0, 0) =  1;
    kernel(1, 0) =  0;
    kernel(2, 0) = -1;
    return custom(src_image, kernel);
}

HOG::HOG(int img_size_in, int n_cells_in, int n_cells_in_block_in, int n_bins_in)
    : img_size(img_size_in), n_cells(n_cells_in),
      n_cells_in_block(n_cells_in_block_in), n_bins(n_bins_in),
      n_blocks(n_cells - n_cells_in_block + 1), cell_size(img_size / n_cells)
{}

vector<float> & HOG::Extract(BMP* img, vector<float>& features)
{
    BMP bmp;
    Resize(img, img_size, img_size, bmp);
    Matrix<double> image = BMPToGrayscale(&bmp);

    Matrix<double> grad_x = sobel_x(image);
    Matrix<double> grad_y = sobel_y(image);

    Matrix<double> grad = sqrt(Square(grad_x) + Square(grad_y));
    Matrix<double> angle = atan2(grad_y, grad_x);

    Matrix<Histogram> cells(n_cells, n_cells);

    for(int y = 0; y < n_cells; ++y){
        for(int x = 0; x < n_cells; ++x){
            Matrix<double> cell_angle = angle.submatrix(y*cell_size, x*cell_size,
                                                  cell_size, cell_size);
            Matrix<double> cell_grad = grad.submatrix(y*cell_size, x*cell_size,
                                                  cell_size, cell_size);
            Histogram hist(cell_grad, cell_angle, -PI, PI, n_cells);
            cells(y, x) += hist;
        }
    }

    for(int i = 0, j, a, b; i < n_blocks; ++i){
        for(j = 0; j < n_blocks; ++j){

            Histogram block;
            for(a = 0; a < n_cells_in_block; ++a){
                for(b = 0; b < n_cells_in_block; ++b){
                    block += cells(i+a, j+b);
                }
            }
            Append(features, block.Normalize());
        }
    }
    return features;
}
