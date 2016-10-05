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

extern "C"
PluginFilter* GetFilter();
extern "C"
void CleanUp();

#endif // AUTOCONTRAST_H
