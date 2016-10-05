#include "autocontrast.h"

using std::get;
using std::make_tuple;


PluginFilter* pFilter = nullptr;

PluginFilter* GetFilter()
{
    if(!pFilter)
        pFilter = new Autocontrast;
    return pFilter;
}
void CleanUp()
{
    //if(pFilter) delete pFilter;
}

Autocontrast::Autocontrast()
{}

Autocontrast::~Autocontrast()
{}

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

Image Autocontrast::Apply(const Image * src, double param1)
{
    return autocontrast(*src, param1);
}

std::string Autocontrast::Name()
{
    return "Autocontrast";
}
