#ifndef AUTOCONTRAST_H
#define AUTOCONTRAST_H

#include "../../shared/headers/pluginfilter.h"

class Autocontrast : public PluginFilter
{

public:
    Autocontrast();
    Image Apply(const Image *, double param1) override;
    std::string Name() override;
    ~Autocontrast();
};

class AutocontrastSingleton : public Singleton
{
    PluginFilter* plugin;
public:
    PluginFilter* Get() override;
    virtual ~AutocontrastSingleton();
};

extern "C"
void RegisterPlugins(Manager* manager);


#endif // AUTOCONTRAST_H
