#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>
#include <ostream>
#include "matrix.h"

typedef unsigned char BYTE;

class Histogram
{
    std::vector<double> hist;
    double magnitude_threshold;

public:
    Histogram();
    Histogram& operator +=(const Histogram& h);
    Histogram(std::vector<BYTE> cell);
    Histogram(const Matrix<double>& grad, const Matrix<double>& angle,
              double from, double to, double n, double magnitude_threshold=20.);

    void Clear();
    Histogram& Normalize();
    const std::vector<double>& Get() const;
};

std::ostream &
operator << (std::ostream &out, const Histogram &h);

std::vector<float>& Append(std::vector<float>& v, const Histogram& h);

#endif
