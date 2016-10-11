#include "medianfilter.h"

using std::tuple;
using std::make_tuple;
using std::get;
using std::vector;


MedianFilterSingleton singleton;

PluginFilter*
MedianFilterSingleton::Get()
{
    if(!plugin){
        plugin = new MedianFilter;
    }
    return plugin;
}

MedianFilterSingleton::~MedianFilterSingleton()
{
    delete plugin;
}

void RegisterPlugins(Manager* manager)
{
    if(manager){
        manager->RegisterPlugin(&singleton);
    }
}

Image median_const(Image src_image, int radius);

MedianFilter::MedianFilter()
{}

MedianFilter::~MedianFilter()
{}

Image MedianFilter::Apply(const Image *img, double param1)
{
    return median_const(*img, param1);
}

std::string MedianFilter::Name()
{
    return "Median filter";
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
    Histogram hist(radius);
    return src_image.unary_map(hist);
}

Image median_linear(Image src_image, int radius) {
    Histogram hist(radius);
    return hist.filter(src_image);
}

Image median_const(Image src_image, int radius) {
    Histogram_For_Const hist(radius, src_image);
    return hist.filter(src_image);
}
