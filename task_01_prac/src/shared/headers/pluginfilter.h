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

class Singleton
{
public:
    virtual PluginFilter* Get() = 0;
    virtual ~Singleton(){}
};

class Manager
{
public:
    virtual void RegisterPlugin(Singleton* singleton) = 0;
    virtual ~Manager(){}
};

#endif // PLUGINFILTER_H
