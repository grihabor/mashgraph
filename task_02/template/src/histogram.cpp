#include "../include/histogram.h"

using std::vector;

Histogram::Histogram()
{}

Histogram& Histogram::operator +=(const Histogram& h)
{
    for(int x : h.hist){
        hist.push_back(x);
    }
    return *this;
}

Histogram::Histogram(vector<BYTE> cell)
{
    hist.resize(256, 0.);
    for(BYTE v : cell){
        hist[v] += 1;
    }
}

void Histogram::Clear()
{
    hist.clear();
}

Histogram::Histogram(const Matrix<double>& grad, const Matrix<double>& angle,
                     double from, double to, double n, double threshold)
    : magnitude_threshold(threshold)
{
    double step = (to - from) / (n - 1);
    vector<double> values;
    for(int i = 0; i < n-2; ++i){
        from += step;
        values.push_back(from);
    }

    bool next = false;

    hist.resize(n, 0);
    for(uint y = 0, x; y < angle.n_rows; ++y){
        for(x = 0; x < angle.n_cols; ++x){

            if(grad(y, x) < magnitude_threshold){
                hist[n-1] += 1.;
                continue;
            }

            next = false;
            uint i = 0;
            for(i = 0; i < values.size(); ++i){
                if(angle(y, x) < values[i]){
                    hist[i] += 1.;
                    next = true;
                    break;
                }
            }
            if(!next)
                hist[i] += 1;
        }
    }
}

Histogram& Histogram::Normalize()
{
    double n = 0.;
    for(double x : hist){
        n += x;
    }
    for(double& x : hist){
        x /= n;
    }
    return *this;
}

const vector<double>& Histogram::Get() const
{
    return hist;
}

vector<float>& Append(vector<float>& v, const Histogram& h)
{
    auto& hist = h.Get();
    v.reserve(v.size() + hist.size());
    for(double x : hist){
        v.push_back(x);
    }
    return v;
}

std::ostream &operator << (std::ostream &out, const Histogram &h)
{
    for(int x : h.Get()){
        out << x << ' ';
    }
    out << std::endl;
    return out;
}
