#include "model.h"
#include "crop.h"

#define PI 3.14159265358979323846

using std::make_tuple;
using std::tie;
using std::get;
using std::vector;
using std::tuple;
using std::string;

/*
 * Non-Model functions
 */

uint minof3(int a, int b, int c)
{
    int result = a;
    if(b < result)
        result = b;
    if(c < result)
        result = c;
    return result;
}

uint maxof3(int a, int b, int c)
{
    int result = a;
    if(b > result)
        result = b;
    if(c > result)
        result = c;
    return result;
}

Image MakeImage(Matrix<double> m)
{
    Image image(m.n_rows, m.n_cols);
    uint x, y;
    for(y = 0; y < m.n_rows; ++y){
        for(x = 0; x < m.n_cols; ++x){
            image(y, x) = make_tuple(int(m(y, x)), int(m(y, x)), int(m(y, x)));
        }
    }
    return image;
}

Image QImageToImage(QImage img)
{
    QRgb rgb;
    int red, green, blue;
    Image result(img.height(), img.width());

    for(int y = 0; y < img.height(); ++y){
        for(int x = 0; x < img.width(); ++x){
            rgb = img.pixel(x, y);
            red = qRed(rgb);
            green = qGreen(rgb);
            blue = qBlue(rgb);
            result(y, x) = make_tuple(red, green, blue);
        }
    }
    return result;
}

QImage ImageToQImage(Image img, QImage::Format f)
{
    QImage result(img.n_cols, img.n_rows, f);
    int red, green, blue;

    for(uint y = 0; y < img.n_rows; ++y){
        for(uint x = 0; x < img.n_cols; ++x){
            tie(red, green, blue) = img(y, x);
            result.setPixel(x, y, qRgb(red, green, blue));
        }
    }
    return result;
}


template<typename ValueT>
Matrix<double> Square(const Matrix<ValueT> &m)
{
    Matrix<double> result(m.n_rows, m.n_cols);
    int t;
    for(uint i = 0; i < m.n_rows; ++i){
        for(uint j = 0; j < m.n_cols; ++j){
            t = get<0>(m(i, j))*get<0>(m(i, j));
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


template<typename ValueT>
Matrix<double> atan2(const Matrix<ValueT> &y, const Matrix<ValueT> &x)
{
    Matrix<double> result(y.n_rows, y.n_cols);
    double t;
    for(uint i = 0; i < y.n_rows; ++i){
        for(uint j = 0; j < y.n_cols; ++j){
            t = atan2(double(get<0>(y(i, j))), double(get<0>(x(i, j))));
            result(i, j) = t;
        }
    }
    return result;
}

class NonMaxSuppression
{
public:
    int radius_x = 1;
    int radius_y = 1;

    double operator () (const Matrix<double> &grad, const Matrix<double> &angle) const
    {
        double aligned_pixels[2];
        double a = angle(1, 1);
        if (a < 0) {
            a += PI;
        }
        if (a >= 7*PI/8 || a <= PI/8) {
            aligned_pixels[0] = grad(1, 0);
            aligned_pixels[1] = grad(1, 2);
        } else if (a >= PI/8 && a <= 3*PI/8) {
            aligned_pixels[0] = grad(0, 2);
            aligned_pixels[1] = grad(2, 0);
        } else if (a >= 3*PI/8 && a <= 5*PI/8) {
            aligned_pixels[0] = grad(0, 1);
            aligned_pixels[1] = grad(2, 1);
        } else {
            aligned_pixels[0] = grad(0, 0);
            aligned_pixels[1] = grad(2, 2);
        }

        if(grad(1, 1) > aligned_pixels[0] && grad(1, 1) > aligned_pixels[1]){
            return grad(1, 1);
        } else {
            return 0.;
        }
    }
};

bool is_weak(int label, const vector<bool>& strong, const vector<int>& indices)
{
    while(label != indices[label]){
        label = indices[label];
        if(strong[label]){
            return false;
        }
    }
    if(strong[label]){
        return false;
    }
    return true;
}

void FinalGrads(Matrix<double>& val, double threshold1, double threshold2)
{
    //go through all the values and label them
    uint x, y;
    vector<int> indices;
    vector<bool> strong;
    indices.push_back(0);
    Matrix<int> labels(val.n_rows, val.n_cols);
    int final_label;


    for(y = 0; y < val.n_rows; ++y){
        for(x = 0; x < val.n_cols; ++x){
            if(val(y, x) < threshold1){
                val(y, x) = 0.;
                final_label = 0;
                if(y == 0 || x == 0){
                } else if(labels(y-1, x) * labels(y, x-1) == 0){
                } else if(labels(y-1, x) != labels(y, x-1)){
                    int label1 = indices[labels(y-1, x)];
                    int label2 = indices[labels(y, x-1)];

                    while(label1 != indices[label1]){
                        label1 = indices[label1];
                    }
                    while(label2 != indices[label2]){
                        label2 = indices[label2];
                    }
                    if(label1 > label2){
                        int t = label1;
                        label1 = label2;
                        label2 = t;
                    }
                    indices[label2] = label1;
                }
            } else {
                if(x == 0 && y == 0){
                    indices.push_back(1);
                    final_label = 1;
                } else if(y == 0) {
                    if(labels(0, x-1) > 0){
                        final_label = indices[labels(0, x-1)];
                    } else {
                        final_label = indices.size();
                        indices.push_back(final_label);
                    }
                } else if(x == 0) {
                    if(labels(y-1, 0) > 0){
                        final_label = indices[labels(y-1, 0)];
                    } else {
                        final_label = indices.size();
                        indices.push_back(final_label);
                    }
                } else {
                    if(labels(y-1, x) == 0 && labels(y, x-1) == 0){
                        if(labels(y-1, x-1) == 0){
                            final_label = indices.size();
                            indices.push_back(final_label);
                        } else {
                            final_label = indices[labels(y-1, x-1)];
                        }
                    } else if(labels(y-1, x) == 0){
                        final_label = indices[labels(y, x-1)];
                    } else if(labels(y, x-1) == 0){
                        final_label = indices[labels(y-1, x)];
                    } else {
                        if(labels(y-1, x) == labels(y, x-1)){
                            final_label = indices[labels(y, x-1)];
                        } else {
                            int label1 = indices[labels(y-1, x)];
                            int label2 = indices[labels(y, x-1)];

                            while(label1 != indices[label1]){
                                label1 = indices[label1];
                            }
                            while(label2 != indices[label2]){
                                label2 = indices[label2];
                            }
                            if(label1 > label2){
                                int t = label1;
                                label1 = label2;
                                label2 = t;
                            }
                            final_label = label1;
                            indices[label2] = label1;
                        }
                    }
                }
            }
            labels(y, x) = final_label;
            strong.resize(indices.size(), false);
            if(val(y, x) > threshold2){
                strong[final_label] = true;
            }
        }
    }
    //normalize links between labels and strong flags
    for(int i = strong.size() - 1; i >= 1; --i){
        if(strong[i]){
            while(i != indices[i]){
                i = indices[i];
                strong[i] = true;
            }
        }
    }

    //nullify all weak values
    for(y = 0; y < val.n_rows; ++y){
        for(x = 0; x < val.n_cols; ++x){
            if(is_weak(labels(y, x), strong, indices)){
                val(y, x) = 0;
            }
        }
    }
}






/*
 * Model public functions
 */

Model::Model()
{}

void Model::LoadImage(const QString& filename)
{
    qoriginal_image = QImage(filename);
    original_image = QImageToImage(qoriginal_image);
    NotifyAll(Load, &qoriginal_image);
}

void Model::AlignImage()
{
    Image& srcImage = this->original_image;

    uint width = srcImage.n_cols;
    uint height = srcImage.n_rows / 3;

    range = range_rel * width;
    bbox_limit = range;

    Image blue_channel  = srcImage.submatrix(         0, 0, height, width);
    Image green_channel = srcImage.submatrix(    height, 0, height, width);
    Image red_channel   = srcImage.submatrix(2 * height, 0, height, width);

    Image blue_mask, green_mask, red_mask;

    Crop r_crop, g_crop, b_crop;

    tie(red_mask, green_mask, blue_mask,
        r_crop, g_crop, b_crop) =
            FindAndRemoveEdges(red_channel, green_channel, blue_channel);

    auto green_shift = GetShift(red_mask, green_mask);
    auto blue_shift = GetShift(red_mask, blue_mask);

    get<0>(green_shift) += g_crop.top - r_crop.top;
    get<1>(green_shift) += g_crop.left - r_crop.left;
    get<0>(blue_shift) += b_crop.top - r_crop.top;
    get<1>(blue_shift) += b_crop.left - r_crop.left;

    Image aligned_img = MergeRGB(red_channel, green_channel, blue_channel, green_shift, blue_shift);

    uint res_left   = maxof3(r_crop.left,   g_crop.left   - get<1>(green_shift), b_crop.left   - get<1>(blue_shift));
    uint res_top    = maxof3(r_crop.top,    g_crop.top    - get<0>(green_shift), b_crop.top    - get<0>(blue_shift));
    uint res_right  = minof3(r_crop.right,  g_crop.right  - get<1>(green_shift), b_crop.right  - get<1>(blue_shift));
    uint res_bottom = minof3(r_crop.bottom, g_crop.bottom - get<0>(green_shift), b_crop.bottom - get<0>(blue_shift));

    uint res_width = res_right - res_left;
    uint res_height = res_bottom - res_top;

    aligned_img = aligned_img.submatrix(res_top, res_left, res_height, res_width);

    aligned_image = aligned_img;
    qaligned_image = ImageToQImage(aligned_img, qoriginal_image.format());
    NotifyAll(Align, &qaligned_image);
}


void Model::NotifyAll(Event event, QImage* img)
{
    for(auto observer : observers){
        observer->HandleEvent(event, img);
    }
}

void Model::AddObserver(Observer *obs)
{
    observers.push_back(obs);
}









/*
 * Model private functions
 */


Image Model::MergeRGB(
    const Image &red_channel,
    const Image &green_channel,
    const Image &blue_channel,
    tuple<int, int> green_shift,
    tuple<int, int> blue_shift
)
{
    Image aligned_image(red_channel.n_rows, red_channel.n_cols);
    uint x, y;
    uint width = aligned_image.n_cols;
    uint height = aligned_image.n_rows;

    uint y_from = maxof3(-get<0>(green_shift), -get<0>(blue_shift), 0);
    uint x_from = maxof3(-get<1>(green_shift), -get<1>(blue_shift), 0);
    uint y_to = minof3(height-get<0>(green_shift), height-get<0>(blue_shift), height);
    uint x_to = minof3(width-get<1>(green_shift), width-get<1>(blue_shift), width);

    for(y = y_from; y < y_to; ++y){
        for(x = x_from; x < x_to; ++x){
            aligned_image(y, x) = make_tuple(
                get<0>(red_channel(y, x)),
                get<0>(green_channel(y + get<0>(green_shift), x + get<1>(green_shift))),
                get<0>(blue_channel(y + get<0>(blue_shift), x + get<1>(blue_shift)))
            );
        }
    }
    return aligned_image;
}

tuple<Image, Image, Image, Crop, Crop, Crop>
Model::FindAndRemoveEdges(const Image& red_channel, const Image& green_channel, const Image& blue_channel)
{
    int threshold1 = 1;
    int threshold2 = 2;
    Image r_canny = canny(red_channel, threshold1, threshold2);
    Image g_canny = canny(green_channel, threshold1, threshold2);
    Image b_canny = canny(blue_channel, threshold1, threshold2);

    Crop r_crop(r_canny);
    Crop g_crop(g_canny);
    Crop b_crop(b_canny);

    uint width  = -2 + minof3(r_crop.right-r_crop.left, g_crop.right-g_crop.left, b_crop.right-b_crop.left);
    uint height = -2 + minof3(r_crop.bottom-r_crop.top, g_crop.bottom-g_crop.top, b_crop.bottom-b_crop.top);

    Image r_mask = r_canny.submatrix(r_crop.top + 1, r_crop.left + 1, height, width);
    Image g_mask = g_canny.submatrix(g_crop.top + 1, g_crop.left + 1, height, width);
    Image b_mask = b_canny.submatrix(b_crop.top + 1, b_crop.left + 1, height, width);

    return make_tuple(r_mask, g_mask, b_mask,
                      r_crop, g_crop, b_crop);
}

tuple<int, int> Model::GetShift(const Image &img1, const Image &img2)
{
    int x_shift = 0;
    int y_shift = 0;
    auto result = make_tuple(y_shift, x_shift);
    double mse;
    double min_mse = -1;

    for(y_shift = -range; y_shift <= range; ++y_shift){
        for(x_shift = -range; x_shift <= range; ++x_shift){
            mse = MSE(img1, img2, make_tuple(y_shift, x_shift));
            if(mse < min_mse || min_mse < 0){
                min_mse = mse;
                result = make_tuple(y_shift, x_shift);
            }
        }
    }
    return result;
}

Image Model::canny(Image src_image, int threshold1, int threshold2)
{
    Image image = gaussian(src_image, 1.4, 2);
    Image grad_x = sobel_x(image);
    Image grad_y = sobel_y(image);

    Matrix<double> grad = sqrt(Square(grad_x) + Square(grad_y));
    Matrix<double> angle = atan2(grad_y, grad_x);
    Matrix<double> grads = binary_map(NonMaxSuppression(), grad, angle);

    FinalGrads(grads, threshold1, threshold2);

    return MakeImage(grads);
}

double Model::MSE(const Image &img1, const Image &img2, tuple<int, int> shift)
{
    double mse = 0.0;
    uint y, x;
    double t;
    uint width = img1.n_cols;
    uint height = img1.n_rows;

    uint x_shift, y_shift;
    tie(y_shift, x_shift) = shift;

    for(y = bbox_limit; y < height - bbox_limit; ++y){
        for(x = bbox_limit; x < width - bbox_limit; ++x){
            t = (get<0>(img1(y, x)) - get<0>(img2(y + y_shift, x + x_shift))) / 255.;
            mse += t * t;
        }
    }
    mse /= (width - 2*bbox_limit) * (height - 2*bbox_limit);
    /*
    //fill the mse map
    mse_map(y_shift + range, x_shift + range) = mse;
    */
    return mse;
}

class CustomFilter
{
public:
    Matrix<double> kernel;
    int radius_x;
    int radius_y;

    CustomFilter(Matrix<double> kern)
    :   kernel(kern),
        radius_x(kern.n_cols / 2),
        radius_y(kern.n_rows / 2)
    {}

    tuple<int, int, int> operator () (const Image &m) const
    {
        double r, g, b;
        uint sum_r = 0, sum_g = 0, sum_b = 0;
        for (uint y = 0; y < m.n_rows; ++y) {
            for (uint x = 0; x < m.n_cols; ++x) {
                tie(r, g, b) = m(y, x);
                //cout << y << ' ' << x << endl;
                sum_r += r * kernel(y, x);
                sum_g += g * kernel(y, x);
                sum_b += b * kernel(y, x);
            }
        }
        return make_tuple(sum_r, sum_g, sum_b);
    }
};

Image Model::custom(Image src_image, Matrix<double> kernel) {
    return src_image.unary_map(CustomFilter(kernel));
}

Image Model::sobel_x(Image src_image) {
    Matrix<double> kernel = {{-1, 0, 1},
                             {-2, 0, 2},
                             {-1, 0, 1}};
    return custom(src_image, kernel);
}

Image Model::sobel_y(Image src_image) {
    Matrix<double> kernel = {{ 1,  2,  1},
                             { 0,  0,  0},
                             {-1, -2, -1}};
    return custom(src_image, kernel);
}


Image Model::gaussian(Image src_image, double sigma, int radius)  {
    int size = 2 * radius + 1;
    Matrix<double> kernel(size, size);
    //cout << "gaussian kernel: sigma = " << sigma << ", radius = " << radius << endl;
    int x, y;
    double t, norm_coef = 0.;
    double temp = 2*sigma*sigma;
    for(y = 0; y < size; ++y){
        for(x = 0; x < size; ++x){
            t = (x - radius)*(x - radius) + (y - radius)*(y - radius);
            kernel(y, x) = exp(-t / temp) / (temp * PI);
            norm_coef += kernel(y, x);
        }
    }
    //normalize the kernel
    for(y = 0; y < size; ++y){
        for(x = 0; x < size; ++x){
            kernel(y, x) /= norm_coef;
        }
    }
    return custom(src_image, kernel);
}

Image Model::gaussian_separable(Image src_image, double sigma, int radius) {
    int size = 2 * radius + 1;
    Matrix<double> kernel_x(1, size);
    Matrix<double> kernel_y(size, 1);
    int i;
    double t, norm_coef = 0.;
    double temp = sigma * sqrt(2*PI);
    for(i = 0; i < size; ++i){
        t = (i-radius)*(i-radius)/(2*sigma*sigma);
        kernel_x(0, i) = exp(-t) / temp;
        norm_coef += kernel_x(0, i);
    }
    //normalize the kernel
    for(i = 0; i < size; ++i){
        kernel_x(0, i) /= norm_coef;
        kernel_y(i, 0) = kernel_x(0, i);
    }
    return custom(custom(src_image, kernel_x), kernel_y);
}
