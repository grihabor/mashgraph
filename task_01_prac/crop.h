#ifndef CROP_H
#define CROP_H

#include <model.h>

/*
 * This class provides structure, which
 * stores information about the image
 * crop, which is calculated from canny
 * filter data
 */

struct Crop
{
private:
    static double margin;

public:
    uint left;
    uint top;
    uint right;
    uint bottom;

    Crop(const Image& img_canny);
    Crop();

private:
    uint LeftCropIndex(const Image& img_canny) const;
    uint RightCropIndex(const Image& img_canny) const;
    uint BottomCropIndex(const Image& img_canny) const;
    uint TopCropIndex(const Image& img_canny) const;
};


#endif // CROP_H
