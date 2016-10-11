#include "align.h"
#include <string>
#include <tuple>
#include <cmath>
#include <fstream>
#include <vector>
#include <iomanip>

using std::vector;
using std::begin;
using std::end;
using std::ofstream;
using std::string;
using std::cout;
using std::endl;

using std::get;
using std::tie;
using std::make_tuple;
using std::tuple;
using std::cin;

#define PI 3.14159265358979323846
/*
template<typename T>
void PrintPixel(T t)
{
    uint r, g, b;
    tie(r, g, b) = t;
    cout << "r = " << r << ", g = " << g << ", b = " << b << endl;
}
*/
const double range_rel = 17./400.;
//init this as range_rel * img_width
int range = -1;
//init this as range
int bbox_limit = -1;

bool debug_custom = false;
bool debug_save_masks = false;
bool debug_crop = false;
bool debug_shift = false;
bool debug_median = false;
bool debug_range = false;
bool debug_autocontrast = false;

uint maxof3(int, int, int);
uint minof3(int, int, int);


struct Crop
{    
private:
    static double margin;

public:
    uint left;
    uint top;
    uint right;
    uint bottom;
    
    Crop(const Image& img_canny)
    :   left(LeftCropIndex(img_canny)),
        top(TopCropIndex(img_canny)),
        right(RightCropIndex(img_canny)),
        bottom(BottomCropIndex(img_canny))
    {}

    Crop()
    : left(0), top(0), right(0), bottom(0)
    {}
    
    uint LeftCropIndex(const Image& img_canny) const
    {
        uint r_max1 = 0;
        uint r_max2 = 0;
        uint i1 = 0, i2 = 0;
        for(uint x = 0; x < img_canny.n_cols * margin; ++x){
            uint count = 0;
            for(uint y = 0; y < img_canny.n_rows; ++y){
                if(get<0>(img_canny(y, x)) > 0)
                    ++count;
            }
            if(count > r_max1){
                r_max2 = r_max1;
                r_max1 = count;
                i2 = i1;
                i1 = x;
            } else if (count > r_max2){
                r_max2 = count;
                i2 = x;
            }
        }
        return (i1 > i2) ? i1 : i2;
    }

    uint RightCropIndex(const Image& img_canny) const
    {
        uint r_max1 = 0;
        uint r_max2 = 0;
        uint i1 = 0, i2 = 0;

        for(uint x = img_canny.n_cols-1; x >= img_canny.n_cols * (1-margin); --x){
            uint count = 0;
            for(uint y = 0; y < img_canny.n_rows; ++y){
                if(get<0>(img_canny(y, x)) > 0)
                    ++count;
            }
            if(count > r_max1){
                r_max2 = r_max1;
                r_max1 = count;
                i2 = i1;
                i1 = x;
            } else if (count > r_max2){
                r_max2 = count;
                i2 = x;
            }
        }
        return (i1 < i2) ? i1 : i2;
    }
        
    uint BottomCropIndex(const Image& img_canny) const
    {
        uint r_max1 = 0;
        uint r_max2 = 0;
        uint i1 = 0, i2 = 0;
        
        for(uint y = img_canny.n_rows-1; y >= img_canny.n_rows * (1-margin); --y){
            uint count = 0;
            for(uint x = 0; x < img_canny.n_cols; ++x){
                if(get<0>(img_canny(y, x)) > 0)
                    ++count;
            }
            if(count > r_max1){
                r_max2 = r_max1;
                r_max1 = count;
                i2 = i1;
                i1 = y;
            } else if (count > r_max2){
                r_max2 = count;
                i2 = y;
            }
        }
        return (i1 < i2) ? i1 : i2;
    }
    
    uint TopCropIndex(const Image& img_canny) const
    {
        uint r_max1 = 0;
        uint r_max2 = 0;
        uint i1 = 0, i2 = 0;

        for(uint y = 0; y < img_canny.n_rows * margin; ++y){
            uint count = 0;
            for(uint x = 0; x < img_canny.n_cols; ++x){
                if(get<0>(img_canny(y, x)) > 0)
                    ++count;
            }
            if(count > r_max1){
                r_max2 = r_max1;
                r_max1 = count;
                i2 = i1;
                i1 = y;
            } else if (count > r_max2){
                r_max2 = count;
                i2 = y;
            }
        }
        return (i1 > i2) ? i1 : i2;
    }
};

double Crop::margin = 0.05;


/*
//map of mse values
Matrix<double> mse_map(2*range + 1, 2*range + 1);
*/

double MSE(const Image &img1, const Image &img2, tuple<int, int> shift)
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

Image Normalize(const Image &src_image)
{
    double maximum = 0;
    uint x, y;
    for(y = 0; y < src_image.n_rows; ++y){
        for(x = range; x < src_image.n_cols; ++x){
            if(get<0>(src_image(y, x)) > maximum){
                maximum = get<0>(src_image(y, x));
            }
        }
    }
    Image img(src_image.n_rows, src_image.n_cols);
    double t;
    for(y = 0; y < src_image.n_rows; ++y){
        for(x = 0; x < src_image.n_cols; ++x){
            t = get<0>(src_image(y, x)) * 255. / maximum;
            img(y, x) = make_tuple(t, t, t);
        }
    }
    return img;
}

Matrix<double> Normalize(const Matrix<double> &src_image)
{
    double maximum = 0;
    uint x, y;
    for(y = 0; y < src_image.n_rows; ++y){
        for(x = range; x < src_image.n_cols; ++x){
            if(src_image(y, x) > maximum){
                maximum = src_image(y, x);
            }
        }
    }
    Matrix<double> img(src_image.n_rows, src_image.n_cols);
    for(y = 0; y < src_image.n_rows; ++y){
        for(x = 0; x < src_image.n_cols; ++x){
            img(y, x) = src_image(y, x) * 255. / maximum;
        }
    }
    return img;
}

tuple<int, int> GetShift(const Image &img1, const Image &img2)
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
    /*
    //save as txt    
    ofstream ofile("output/log.txt");
    uint x, y;
    for(y = 0; y < mse_map.n_rows; ++y){
        for(x = 0; x < mse_map.n_cols; ++x){
            ofile << mse_map(y, x) << ' ';
        }
        ofile << endl;
    }
    //save as bmp
    save_image(MakeImage(Normalize(mse_map)), "output/mse_map.bmp");    
    */
    return result;
}

uint LeftMargin(int shift1, int shift2)
{
    if(shift1 < shift2)
        return (shift1 < 0) ? uint(-shift1) : 0;
    else
        return (shift2 < 0) ? uint(-shift2) : 0;
}

uint RightMargin(int shift1, int shift2, int size)
{
    if(shift1 > shift2)
        return (shift1 > 0) ? uint(size - shift1) : size;
    else
        return (shift2 > 0) ? uint(size - shift2) : size;
}

void DrawCrop(Image &img, Crop crop, tuple<int, int> shift, tuple<int, int, int> color)
{
    for(uint x = crop.left; x < crop.right; ++x){
        img(crop.top    + get<0>(shift), x + get<1>(shift)) = color;
        img(crop.bottom + get<0>(shift), x + get<1>(shift)) = color;
    }
    for(uint y = crop.top; y < crop.bottom; ++y){
        img(y + get<0>(shift), crop.left  + get<1>(shift)) = color;
        img(y + get<0>(shift), crop.right + get<1>(shift)) = color;
    }
}

Image MergeRGB(
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
    /*
    cout << height-get<0>(green_shift) << " - " << height-get<0>(blue_shift) << " - " << height << endl;
    cout << y_from << ' ' << y_to << endl;
    cout << x_from << ' ' << x_to << endl;
    */
    return aligned_image;
}


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


tuple</*Image, Image, Image, */Image, Image, Image, Crop, Crop, Crop> 
FindAndRemoveEdges(const Image& red_channel, const Image& green_channel, const Image& blue_channel)
{
    int threshold1 = 1;
    int threshold2 = 2;
    Image r_canny = canny(red_channel, threshold1, threshold2);
    Image g_canny = canny(green_channel, threshold1, threshold2);
    Image b_canny = canny(blue_channel, threshold1, threshold2);
    
    Crop r_crop(r_canny);
    Crop g_crop(g_canny);
    Crop b_crop(b_canny);
    
    if(debug_crop){    
        cout << "Crop bboxes for r, g, b" << endl;
        cout << r_crop.left << ' ' << r_crop.top << ' ' << r_crop.right << ' ' << r_crop.bottom << endl;
        cout << g_crop.left << ' ' << g_crop.top << ' ' << g_crop.right << ' ' << g_crop.bottom << endl;
        cout << b_crop.left << ' ' << b_crop.top << ' ' << b_crop.right << ' ' << b_crop.bottom << endl;
    }
    if(debug_save_masks){
        save_image(b_canny, "output/mask_channel_blue0.bmp");
        save_image(g_canny, "output/mask_channel_green0.bmp");
        save_image(r_canny, "output/mask_channel_red0.bmp");
    }
    uint width  = -2 + minof3(r_crop.right-r_crop.left, g_crop.right-g_crop.left, b_crop.right-b_crop.left);
    uint height = -2 + minof3(r_crop.bottom-r_crop.top, g_crop.bottom-g_crop.top, b_crop.bottom-b_crop.top);
    /*
    Image r_cropped = red_channel.submatrix(r_crop.top + 1, r_crop.left + 1, height, width);
    Image g_cropped = green_channel.submatrix(g_crop.top + 1, g_crop.left + 1, height, width);
    Image b_cropped = blue_channel.submatrix(b_crop.top + 1, b_crop.left + 1, height, width);
    */
    Image r_mask = r_canny.submatrix(r_crop.top + 1, r_crop.left + 1, height, width);
    Image g_mask = g_canny.submatrix(g_crop.top + 1, g_crop.left + 1, height, width);
    Image b_mask = b_canny.submatrix(b_crop.top + 1, b_crop.left + 1, height, width);
    
    return make_tuple(/*r_cropped, g_cropped, b_cropped, */
                    r_mask, g_mask, b_mask,
                    r_crop, g_crop, b_crop);
}

Image align(Image srcImage, bool isPostprocessing, std::string postprocessingType, double fraction, bool isMirror, 
            bool isInterp, bool isSubpixel, double subScale)
{
    uint width = srcImage.n_cols;
    uint height = srcImage.n_rows / 3;
    
    range = range_rel * width;
    bbox_limit = range;
    if(debug_range){
        cout << "range = " << range << endl;
        cout << "bbox_limit = " << bbox_limit << endl;
    }
    
    Image blue_channel  = srcImage.submatrix(         0, 0, height, width);
    Image green_channel = srcImage.submatrix(    height, 0, height, width);
    Image red_channel   = srcImage.submatrix(2 * height, 0, height, width);
    
    if(debug_save_masks){
        save_image(blue_channel, "output/channel_blue.bmp");
        save_image(green_channel, "output/channel_green.bmp");
        save_image(red_channel, "output/channel_red.bmp");
    }
    /*
    Image red_channel_edges;
    Image green_channel_edges;
    Image blue_channel_edges;
    */
    Image blue_mask, green_mask, red_mask;
    
    Crop r_crop, g_crop, b_crop;

    tie(/*red_channel_edges, green_channel_edges, blue_channel_edges,*/
        red_mask, green_mask, blue_mask, 
        r_crop, g_crop, b_crop) =
            FindAndRemoveEdges(red_channel, green_channel, blue_channel);
    
    if(debug_save_masks){
        /*
        save_image(blue_channel_edges, "output/mask_channel_blue.bmp");
        save_image(green_channel_edges, "output/mask_channel_green.bmp");
        save_image(red_channel_edges, "output/mask_channel_red.bmp");
        * */
    }
    
    auto green_shift = GetShift(red_mask, green_mask);
    auto blue_shift = GetShift(red_mask, blue_mask);
    
    if(debug_shift){
        cout << "with crop:" << endl;
        cout << "\tgreen_shift = " << get<0>(green_shift) << ", " << get<1>(green_shift) << endl;
        cout << "\tblue_shift = " << get<0>(blue_shift) << ", " << get<1>(blue_shift) << endl;
    }
    
    get<0>(green_shift) += g_crop.top - r_crop.top;
    get<1>(green_shift) += g_crop.left - r_crop.left;
    get<0>(blue_shift) += b_crop.top - r_crop.top;
    get<1>(blue_shift) += b_crop.left - r_crop.left;
    
    if(debug_shift){
        cout << "final:" << endl;
        cout << "\tgreen_shift = " << get<0>(green_shift) << ", " << get<1>(green_shift) << endl;
        cout << "\tblue_shift = " << get<0>(blue_shift) << ", " << get<1>(blue_shift) << endl;
    }
       
    Image aligned_image = MergeRGB(red_channel, green_channel, blue_channel, green_shift, blue_shift); 
    
    uint res_left   = maxof3(r_crop.left,   g_crop.left   - get<1>(green_shift), b_crop.left   - get<1>(blue_shift));
    uint res_top    = maxof3(r_crop.top,    g_crop.top    - get<0>(green_shift), b_crop.top    - get<0>(blue_shift));        
    uint res_right  = minof3(r_crop.right,  g_crop.right  - get<1>(green_shift), b_crop.right  - get<1>(blue_shift));
    uint res_bottom = minof3(r_crop.bottom, g_crop.bottom - get<0>(green_shift), b_crop.bottom - get<0>(blue_shift));
        
    uint res_width = res_right - res_left;
    uint res_height = res_bottom - res_top;
    /*
    cout << res_left << ' ' << res_top << ' ' << res_width << ' ' << res_height << endl;
    cout << aligned_image.n_cols << ' ' << aligned_image.n_rows << endl;
    */
    
    aligned_image = aligned_image.submatrix(res_top, res_left, res_height, res_width);
    
    //DrawCrop(aligned_image, b_crop, blue_shift, make_tuple(255, 0, 0));
    
    if(isPostprocessing){
        if (postprocessingType == "--gray-world") {
            aligned_image = gray_world(aligned_image);
        } else if (postprocessingType == "--unsharp") {
            aligned_image = unsharp(aligned_image);
        } else if (postprocessingType == "--autocontrast") {
            aligned_image = autocontrast(aligned_image, fraction);
        }
    }    
    
    return aligned_image;
}

Image sobel_x(Image src_image) {
    Matrix<double> kernel = {{-1, 0, 1},
                             {-2, 0, 2},
                             {-1, 0, 1}};
    return custom(src_image, kernel);
}

Image sobel_y(Image src_image) {
    Matrix<double> kernel = {{ 1,  2,  1},
                             { 0,  0,  0},
                             {-1, -2, -1}};
    return custom(src_image, kernel);
}

Image unsharp(Image src_image) {    
    Matrix<double> kernel = {{ -1/6.,  -2/3.,  -1/6.},
                             { -2/3.,  13/3.,  -2/3.},
                             { -1/6.,  -2/3.,  -1/6.}};
    src_image = custom(src_image, kernel);
    uint x, y;
    for(y = 0; y < src_image.n_rows; y++){
        for(x = 0; x < src_image.n_cols; x++){
            if(get<0>(src_image(y, x)) > 255)
                get<0>(src_image(y, x)) = 255;
            if(get<1>(src_image(y, x)) > 255)
                get<1>(src_image(y, x)) = 255;
            if(get<2>(src_image(y, x)) > 255)
                get<2>(src_image(y, x)) = 255;

            if(get<0>(src_image(y, x)) < 0)
                get<0>(src_image(y, x)) = 0;
            if(get<1>(src_image(y, x)) < 0)
                get<1>(src_image(y, x)) = 0;
            if(get<2>(src_image(y, x)) < 0)
                get<2>(src_image(y, x)) = 0;
        }
    }
    return src_image;
}

Image gray_world(Image src_image) {

    tuple<double, double, double> mean;
    uint x, y;
    for(y = 0; y < src_image.n_rows; y++){
        for(x = 0; x < src_image.n_cols; x++){
            get<0>(mean) += get<0>(src_image(y, x));
            get<1>(mean) += get<1>(src_image(y, x));
            get<2>(mean) += get<2>(src_image(y, x));
        }
    }
    uint n = src_image.n_rows * src_image.n_cols;
    get<0>(mean) /= n;
    get<1>(mean) /= n;
    get<2>(mean) /= n;

    double S = (get<0>(mean) + get<1>(mean) + get<2>(mean)) / 3;
    
    tuple<double, double, double> coef = make_tuple(S / get<0>(mean), 
                                                    S / get<1>(mean), 
                                                    S / get<2>(mean));    

    for(y = 0; y < src_image.n_rows; y++){
        for(x = 0; x < src_image.n_cols; x++){
            get<0>(src_image(y, x)) *= get<0>(coef);
            get<1>(src_image(y, x)) *= get<1>(coef);
            get<2>(src_image(y, x)) *= get<2>(coef);

            if(get<0>(src_image(y, x)) > 255)
                get<0>(src_image(y, x)) = 255;
            if(get<1>(src_image(y, x)) > 255)
                get<1>(src_image(y, x)) = 255;
            if(get<2>(src_image(y, x)) > 255)
                get<2>(src_image(y, x)) = 255;
        }
    }

    return src_image;
}

Image resize(Image src_image, double scale) {    
    return src_image;
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

Image custom(Image src_image, Matrix<double> kernel) {
    // Function custom is useful for making concrete linear filtrations
    // like gaussian or sobel. So, we assume that you implement custom
    // and then implement other filtrations using this function.
    // sobel_x and sobel_y are given as an example.
    if(debug_custom){
        cout << "custom_kernel" << endl;
        cout << kernel;
    }
    return src_image.unary_map(CustomFilter(kernel));
}

int ColorCrop(int c)
{
    return (c < 0) ? 0 : (c > 255) ? 255 : c;
}

Image autocontrast(Image src_image, double fraction) {
    int Y;
    uint hist[256] = {};
    uint x, y;
    for(y = 0; y < src_image.n_rows; ++y){
        for(x = 0; x < src_image.n_cols; ++x){
            Y = 0.2125 * get<0>(src_image(y, x)) +
                0.7154 * get<1>(src_image(y, x)) +
                0.0721 * get<2>(src_image(y, x));            
            if(Y > 255){
                Y = 255;
            }
            hist[Y] += 1;
        }
    }
    uint i, count = 0;
    double left_margin = fraction * src_image.n_cols * src_image.n_rows;
    double right_margin = (1 - fraction) * src_image.n_cols * src_image.n_rows;    
    uint Y_min = 0, Y_max = 255;
    for(i = 0; i < 256; ++i){
        if(count <= left_margin && count + hist[i] > left_margin){
            Y_min = i;
        }     
        if(count <= right_margin && count + hist[i] > right_margin){
            Y_max = i;
            break;
        }
        count += hist[i];
    }
    if(debug_autocontrast){
        cout << "autocontrast crop: " << Y_min << ' ' << Y_max << endl;
    }
    
    Image result(src_image.n_rows, src_image.n_cols);
    double k = 255. / (Y_max - Y_min);
    
    for(y = 0; y < src_image.n_rows; ++y){
        for(x = 0; x < src_image.n_cols; ++x){
            Y = 0.2125 * get<0>(src_image(y, x)) +
                0.7154 * get<1>(src_image(y, x)) +
                0.0721 * get<2>(src_image(y, x));
            result(y, x) = make_tuple(
                ColorCrop((get<0>(src_image(y, x)) - Y_min) * k),
                ColorCrop((get<1>(src_image(y, x)) - Y_min) * k),
                ColorCrop((get<2>(src_image(y, x)) - Y_min) * k)
            );
        }
    }
    
    return result;
}

Image gaussian(Image src_image, double sigma, int radius)  {
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

Image gaussian_separable(Image src_image, double sigma, int radius) {
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

#define addTuples(t1, t2) (make_tuple(get<0>(t1) + get<0>(t2), \
                                      get<1>(t1) + get<1>(t2), \
                                      get<2>(t1) + get<2>(t2)))
#define subTuples(t1, t2) (make_tuple(get<0>(t1) - get<0>(t2), \
                                      get<1>(t1) - get<1>(t2), \
                                      get<2>(t1) - get<2>(t2)))

class Histogram
{
protected:
    tuple<uint, uint, uint> hist[256];
    int radius;
    uint middle;

public:
    int radius_x;
    int radius_y;
    
    Histogram(int r)
    : radius(r), middle((2*radius + 1)*(2*radius + 1) / 2),
        radius_x(radius), radius_y(radius)
    {}
    
    Histogram(int r_y, int r_x)
    : radius(r_y), middle((2*r_x + 1)*(2*r_y + 1) / 2),
        radius_x(r_x), radius_y(r_y)
    {}
    
    Image filter(const Image& src_image)
    {
        Image result(src_image.n_rows, src_image.n_cols);
        uint x, y;
        for(y = 0; y < src_image.n_rows - 2*radius; ++y){
            Init(src_image, y, 0, 2*radius + 1, 2*radius + 1);
            result(y + radius, radius) = get_median();
            for(x = 1; x < src_image.n_cols - 2*radius; ++x){
                AddRemoveColumn(src_image, y, x + 2*radius, x - 1);
                result(y + radius, x + radius) = get_median();
            }
        }
        return result;    
    }
    
    tuple<int, int, int> operator ()(const Image &neighbourhood)
    {        
        for(int i = 0; i < 256; ++i){
            hist[i] = make_tuple(0u, 0u, 0u);
        }
        uint x, y;
        for(y = 0; y < neighbourhood.n_rows; ++y){
            for(x = 0; x < neighbourhood.n_cols; ++x){
                get<0>(hist[get<0>(neighbourhood(y, x))]) += 1;
                get<1>(hist[get<1>(neighbourhood(y, x))]) += 1;
                get<2>(hist[get<2>(neighbourhood(y, x))]) += 1;
            }
        }
        tuple<int, int, int> m = get_median();
        return m;
    }    
    
    Histogram& Init(const Image& src_image, uint y_start, uint x_start, uint height, uint width)
    {
        for(int i = 0; i < 256; ++i){
            hist[i] = make_tuple(0u, 0u, 0u);
        }
        uint x, y;
        for(y = y_start; y < y_start + height; ++y){
            for(x = x_start; x < x_start + width; ++x){
                const auto& rgb = src_image(y, x);
                get<0>(hist[get<0>(rgb)]) += 1;
                get<1>(hist[get<1>(rgb)]) += 1;
                get<2>(hist[get<2>(rgb)]) += 1;
            }
        }
        return *this;
    }
    
    void AddRemoveColumn(const Image& src_image, uint y_start, uint x_add, uint x_remove)
    {
        uint y;
        for(y = y_start; y < y_start + radius*2 + 1; ++y){
            {
                const auto& rgb = src_image(y, x_add);
                get<0>(hist[get<0>(rgb)]) += 1;
                get<1>(hist[get<1>(rgb)]) += 1;
                get<2>(hist[get<2>(rgb)]) += 1;            
            }
            {
                const auto& rgb = src_image(y, x_remove);
                get<0>(hist[get<0>(rgb)]) -= 1;
                get<1>(hist[get<1>(rgb)]) -= 1;
                get<2>(hist[get<2>(rgb)]) -= 1;            
            }
        }
    }
    
    Histogram& addHist(const Histogram &h)
    {
        for(int i = 0; i < 256; ++i){
            hist[i] = addTuples(hist[i], h.hist[i]);
        }
        return *this;
    }
    Histogram& subHist(const Histogram &h)
    {
        for(int i = 0; i < 256; ++i){
            hist[i] = subTuples(hist[i], h.hist[i]);
        }
        return *this;
    }
    Histogram& addPixel(tuple<int, int, int> px)
    {
        get<0>(hist[get<0>(px)]) += 1;
        get<1>(hist[get<1>(px)]) += 1;
        get<2>(hist[get<2>(px)]) += 1;
        return *this;
    }
    Histogram& subPixel(tuple<int, int, int> px)
    {
        get<0>(hist[get<0>(px)]) -= 1;
        get<1>(hist[get<1>(px)]) -= 1;
        get<2>(hist[get<2>(px)]) -= 1;
        return *this;
    }
    
    tuple<int, int, int> get_median()
    {
        tuple<uint, uint, uint> cur(0, 0, 0);
        tuple<bool, bool, bool> done(false, false, false);
        tuple<int, int, int> result;
        uint t;
        for(int i = 0; i < 256; ++i){
            if(!get<0>(done)) {
                t = get<0>(hist[i]) + get<0>(cur);
                if(t <= middle){
                    get<0>(cur) = t;
                } else {
                    get<0>(result) = i;
                    get<0>(done) = true;
                }
            }
            if(!get<1>(done)) {
                t = get<1>(hist[i]) + get<1>(cur);
                if(t <= middle){
                    get<1>(cur) = t;
                } else {
                    get<1>(result) = i;
                    get<1>(done) = true;
                }
            }
            if(!get<2>(done)) {
                t = get<2>(hist[i]) + get<2>(cur);
                if(t <= middle){
                    get<2>(cur) = t;
                } else {
                    get<2>(result) = i;
                    get<2>(done) = true;
                }
            }            
            if(get<0>(done) && get<1>(done) && get<2>(done)){
                break;
            }
        }
        if(debug_median){
            cout << "get_median\t" << get<0>(result) << ' ' << get<1>(result) << ' ' << get<2>(result) << endl;
        
            for(int i = 0; i < 256; ++i){
                cout << i << '\t';
                cout << get<0>(hist[i]) << ' ';
                cout << get<1>(hist[i]) << ' ';
                cout << get<2>(hist[i]) << endl;
            }
            if(cin.get() == 't')
                throw;
        }
        return result;
    }
};

class Histogram_For_Const : public Histogram
{
    vector<Histogram> hist_cols;
public:
    Histogram_For_Const(int r, const Image &src_image)
    : Histogram(r), hist_cols()
    {
        hist_cols.reserve(src_image.n_cols);
        for(uint x = 0; x < src_image.n_cols; ++x){
            hist_cols.push_back(Histogram(r, 0).Init(src_image, 0, x, 2*r + 1, 1));
        }
    }
    
    Image filter(const Image &src_image)
    {
        Image result(src_image.n_rows, src_image.n_cols);
        int x, y;        
        for(y = radius; uint(y) < src_image.n_rows - radius; ++y){
            
            Histogram window(radius);
            
            for(x = 0; x < 2*radius+1; ++x){
                //shift down each column
                if(y > radius){   
                    hist_cols[x].addPixel(src_image(y + radius, x))
                                .subPixel(src_image(y - radius - 1, x));
                }                
                window.addHist(hist_cols[x]);
            }
            result(y, radius) = window.get_median();
            for(; uint(x) < src_image.n_cols; ++x){
                //shift down each column
                if(y > radius){   
                    hist_cols[x].addPixel(src_image(y + radius, x))
                                .subPixel(src_image(y - radius - 1, x));
                }    
                window.addHist(hist_cols[x])
                      .subHist(hist_cols[x - 2*radius - 1]);
                result(y, x - radius) = window.get_median();
            }
        }
        return result;
    }
};


Image median(Image src_image, int radius) {
    cout << "median: radius = " << radius << endl;
    Histogram hist(radius);
    return src_image.unary_map(hist);
}

Image median_linear(Image src_image, int radius) {
    cout << "median_linear: radius = " << radius << endl;
    Histogram hist(radius);
    return hist.filter(src_image);
}

Image median_const(Image src_image, int radius) {
    cout << "median_const: radius = " << radius << endl;
    Histogram_For_Const hist(radius, src_image);
    return hist.filter(src_image);
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
            /*
            for(auto l : indices){
                cout << l << ' ';
            }
            cout << endl << labels;
            cout << val;
            cin.get();
            */
        }
    }
    /*
    for(bool t : strong){
        cout << t << ' ';
    }
    cin.get();
    */
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
#include <ctime>
*/

Image canny(Image src_image, int threshold1, int threshold2) {
    Image image = gaussian(src_image, 1.4, 2);
    Image grad_x = sobel_x(image);
    Image grad_y = sobel_y(image);

    Matrix<double> grad = sqrt(Square(grad_x) + Square(grad_y));
    Matrix<double> angle = atan2(grad_y, grad_x);

    //save_image(Normalize(MakeImage(grad)), "output/grad.bmp");

    Matrix<double> grads = binary_map(NonMaxSuppression(), grad, angle);
    /*
    Matrix<double> test(10, 10);
    srand(time(NULL));
    for(uint y = 0; y < 10; ++y)
        for(uint x = 0; x < 10; ++x)
            test(y, x) = (rand() % 10000) / 100.;
    cout << test;
    FinalGrads(test, 40, 60);
    */

    FinalGrads(grads, threshold1, threshold2);
    

    return MakeImage(grads);
}
