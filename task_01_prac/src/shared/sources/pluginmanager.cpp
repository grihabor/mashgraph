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

void PluginManager::RegisterPlugin(Singleton *singleton)
{
    plugins.append(singleton->Get());
}

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

    const char *symbol = "RegisterPlugins";
    typedef void (*RegisterPlugins)(Manager*);
    RegisterPlugins register_plugins;
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

        register_plugins = (RegisterPlugins) dlsym(handle, symbol);
        if(register_plugins == nullptr){
            Log(path + ": Failed to get symbol \'" + symbol + "\'");

            dlclose(handle);
            Log(path + ": Close lib");
            continue;
        }

        register_plugins(this);
        Log(it.filePath() + ": Plugin \'" + plugins[plugins.size()-1]->Name().c_str() + "\' loaded successfully");

        libHandles[nLibHandles++] = handle;
    }
    Log(QString("PluginManager: ") + QString::number(plugins.size()) + " plugins loaded");
}

void PluginManager::CleanUp()
{
    for(int i = 0; i < nLibHandles; ++i){
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
