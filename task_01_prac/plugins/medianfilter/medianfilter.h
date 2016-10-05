#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include "../../shared/headers/pluginfilter.h"

class MedianFilter : public PluginFilter
{
public:
    MedianFilter();
    Image Apply(const Image *, double param1) override;
    std::string Name() override;
    ~MedianFilter();
};

extern "C"
PluginFilter* GetFilter();
extern "C"
void CleanUp();

#endif // MEDIANFILTER_H
