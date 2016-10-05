#include "../headers/crop.h"

using std::get;

double Crop::margin = 0.05;

Crop::Crop(const Image& img_canny)
:   left(LeftCropIndex(img_canny)),
    top(TopCropIndex(img_canny)),
    right(RightCropIndex(img_canny)),
    bottom(BottomCropIndex(img_canny))
{}

Crop::Crop()
: left(0), top(0), right(0), bottom(0)
{}

uint Crop::LeftCropIndex(const Image& img_canny) const
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

uint Crop::RightCropIndex(const Image& img_canny) const
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

uint Crop::BottomCropIndex(const Image& img_canny) const
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

uint Crop::TopCropIndex(const Image& img_canny) const
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
