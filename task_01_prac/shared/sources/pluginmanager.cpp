#include "../headers/pluginmanager.h"
#include <dlfcn.h>
#include <QDirIterator>

QVector<PluginFilter*> PluginManager::GetPlugins()
{
    return plugins;
}

PluginManager::PluginManager(Logger& logger)
    : Log(logger)
{}

void PluginManager::LoadPlugins(const QString& dirpath)
{
    CleanUp();
    QDir dir = QDir(dirpath);
    if(!dir.exists()){
        Log(dirpath + ": folder doesn't exist");
        return;
    }
    Log(dirpath + ": Start loading plugins");
    QDirIterator it(dir, QDirIterator::NoIteratorFlags);
    QString path;

    const char *symbol = "GetFilter";
    typedef PluginFilter* (*GetFilter)(void);
    GetFilter get_filter;
    while(it.hasNext()){
        path = it.next();
        if(it.fileName() == "." || it.fileName() == ".."){
            continue;
        }
        if(!it.fileName().endsWith(".so")){
            continue;
        }

        void * handle = dlopen(path.toStdString().c_str(), RTLD_NOW);

        if(handle == nullptr){
            Log(dlerror());
            Log(path + ": Failed to open lib");
            continue;
        }

        get_filter = (GetFilter) dlsym(handle, symbol);
        if(get_filter == nullptr){
            Log(path + ": Failed to get symbol \'" + symbol + "\'");

            dlclose(handle);
            Log(path + ": Close lib");
            continue;
        }

        Log(it.filePath() + ": Plugin \'" + get_filter()->Name().c_str() + "\' loaded successfully");
        plugins.push_back(get_filter());
        libHandles[nLibHandles++] = handle;
    }
    Log(QString("PluginManager: ") + QString::number(plugins.size()) + " plugins loaded");
}

void PluginManager::CleanUp()
{
    const char* symbol = "CleanUp";
    for(int i = 0; i < nLibHandles; ++i){
        void (*clean)() = (void (*)())dlsym(libHandles[i], symbol);
        if(!clean){
            Log(dlerror());
        } else {
            clean();
        }
        dlclose(libHandles[i]);
    }
    plugins.clear();
    nLibHandles = 0;
    Log("PluginManager: cleanup");
}

PluginManager::~PluginManager()
{
    CleanUp();
}
