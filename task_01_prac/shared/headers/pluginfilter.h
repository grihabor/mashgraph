#ifndef PLUGINFILTER_H
#define PLUGINFILTER_H

#include "matrix.h"

class PluginFilter
{
public:
    virtual Image Apply(const Image*, double param) = 0;
    virtual std::string Name() = 0;
    virtual ~PluginFilter(){}
};

#endif // PLUGINFILTER_H
