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

class MedianFilterSingleton : public Singleton
{
    PluginFilter* plugin;
public:
    PluginFilter* Get() override;
    virtual ~MedianFilterSingleton();
};

extern "C"
void RegisterPlugins(Manager* manager);


#endif // MEDIANFILTER_H
