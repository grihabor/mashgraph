#ifndef MODEL_H
#define MODEL_H

#include <QImage>
#include <QFileInfo>
#include <QThread>

#include "matrix.h"
#include "pluginmanager.h"
#include "logger.h"

struct Crop;

class Model : public QObject
{
    Q_OBJECT

    QThread thread;

    QImage qoriginal_image;
    QImage qaligned_image;
    Image original_image;
    Image aligned_image;

    QVector<QImage*> images;
    PluginManager pluginManager;
    Logger& Log;

    const double range_rel = 17./400.;
    //init this as range_rel * img_width
    int range = -1;
    //init this as range
    int bbox_limit = -1;

public slots:
    void StartAlignImage();
    void StartLoadImage(const QString& filename);
    void StartLoadPlugins(const QString& dirpath);
    void StartApplyFilter(int index, double* params);
    void StartSaveLastImage(QString path);

public:
    Model(Logger& logger);
    ~Model();

    void LoadImage(const QString &fileinfo);
    void AlignImage();
    void LoadPlugins(const QString& dirpath);
    void ApplyFilter(int index, double* params);
    void RemoveLastFilter();
    void SaveLastImage(QString path);

signals:
    void ImageLoaded(QImage* img);
    void ImageAligned(QImage* img);
    void FilterApplied(PluginFilter*, QImage* img);
    void PluginsLoaded(QVector<PluginFilter*> plugins);
    void ClearFilters();
    void LastFilterRemoved();
    void ImageSaved(QString path);

private:
    void RemoveFilterImages();
    Image custom(Image src_image, Matrix<double> kernel);
    Image gaussian(Image src_image, double sigma, int radius);
    Image gaussian_separable(Image src_image, double sigma, int radius);
    Image canny(Image src_image, int threshold1, int threshold2);
    std::tuple<Image, Image, Image, Crop, Crop, Crop>
        FindAndRemoveEdges(const Image& red_channel,
                           const Image& green_channel,
                           const Image& blue_channel);
    std::tuple<int, int> GetShift(const Image &img1, const Image &img2);
    Image MergeRGB(
        const Image &red_channel,
        const Image &green_channel,
        const Image &blue_channel,
        std::tuple<int, int> green_shift,
        std::tuple<int, int> blue_shift
    );
    double MSE(const Image &img1, const Image &img2, std::tuple<int, int> shift);
    Image sobel_x(Image src_image);
    Image sobel_y(Image src_image);
    Image custom(Image src_image);
};

#endif // MODEL_H
