#include "../include/bmp_management.h"
#include "../externals/EasyBMP/include/EasyBMP.h"
#include "../include/matrix.h"

#define height TellHeight
#define width  TellWidth

Matrix<double> BMPToGrayscale(BMP* bmp)
{
    BMP& image = *bmp;
    Matrix<double> result(bmp->height(), bmp->width());

    for(uint y = 0, x; y < result.n_rows; ++y){
        for(x = 0; x < result.n_cols; ++x){
            result(y, x) = 0.299 * image(y, x)->Red
                         + 0.587 * image(y, x)->Green
                         + 0.114 * image(y, x)->Blue;
        }
    }

    return result;
}


inline RGBApixel GetPixel(BMP& img, float x, float y)
{
    int px = static_cast<int>(x);
    int py = static_cast<int>(y);

    RGBApixel* p[4];
    p[0] = img(py, px);
    p[1] = img(py, px+1);
    p[2] = img(py+1, px);
    p[3] = img(py+1, px+1);

    float fx = x - px;
    float fy = y - py;
    float fx1 = 1.0f - fx;
    float fy1 = 1.0f - fy;

    int w1 = static_cast<int>(fx1 * fy1 * 256.0f);
    int w2 = static_cast<int>(fx  * fy1 * 256.0f);
    int w3 = static_cast<int>(fx1 * fy  * 256.0f);
    int w4 = static_cast<int>(fx  * fy  * 256.0f);

    int outr = p[0]->Red * w1   + p[1]->Red * w2   + p[2]->Red * w3   + p[3]->Red * w4;
    int outg = p[0]->Green * w1 + p[1]->Green * w2 + p[2]->Green * w3 + p[3]->Green * w4;
    int outb = p[0]->Blue * w1  + p[1]->Blue * w2  + p[2]->Blue * w3  + p[3]->Blue * w4;
    int outa = p[0]->Alpha * w1 + p[1]->Alpha * w2 + p[2]->Alpha * w3 + p[3]->Alpha * w4;

    return {static_cast<ebmpBYTE>(outb >> 8),
            static_cast<ebmpBYTE>(outg >> 8),
            static_cast<ebmpBYTE>(outr >> 8),
            static_cast<ebmpBYTE>(outa >> 8)};
}

BMP Resize(BMP* image, uint width, uint height, BMP& bmp)
{

    bmp.SetSize(width, height);
    float scale_y = image->height() / float(height);
    float scale_x = image->width() / float(width);
    //cout << scale_x << ' ' << scale_y << endl;

    for(uint y = 0, x; y < height; ++y){
        for(x = 0; x < width; ++x){
            float xf = x * scale_y;
            float yf = y * scale_x;
            *bmp(y, x) = GetPixel(*image, xf, yf);
        }
    }

    return bmp;
}

