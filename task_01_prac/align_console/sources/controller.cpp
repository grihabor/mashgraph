#include "../headers/controller.h"
#include <QMetaType>

Controller::Controller(Logger& logger)
    : Log(logger), model(logger), printer(logger)
{
    qRegisterMetaType<QVector<PluginFilter*> >("QVector<PluginFilter*>");
    qRegisterMetaType<double*>("double*");
    QObject::connect(&model, SIGNAL(ImageLoaded(QImage*)), &printer, SLOT(ImageLoaded()));
    QObject::connect(&model, SIGNAL(ImageAligned(QImage*)), &printer, SLOT(ImageAligned()));
    QObject::connect(&model, SIGNAL(ImageSaved(QString)), &printer, SLOT(ImageSaved(QString)));
    QObject::connect(&model, SIGNAL(FilterApplied(PluginFilter*,QImage*)), &printer, SLOT(FilterApplied(PluginFilter*)));
    QObject::connect(&model, SIGNAL(PluginsLoaded(QVector<PluginFilter*>)), &printer, SLOT(FiltersLoaded(QVector<PluginFilter *>)));
    QObject::connect(&model, SIGNAL(PluginsLoaded(QVector<PluginFilter*>)), this, SLOT(FiltersLoaded(QVector<PluginFilter*>)));
    QObject::connect(this, SIGNAL(ShowFilters(QVector<PluginFilter*>)), &printer, SLOT(FiltersLoaded(QVector<PluginFilter*>)));
}

void Controller::FiltersLoaded(QVector<PluginFilter *> plugins)
{
    filters.clear();
    filters = plugins;
}

double params[1];

void Controller::HandleInput(QString input)
{
    QString command = input.section(' ', 0, 0).toLower();
    QString arg1 = input.section(' ', 1, 1);
    QString cmd_args = input.section(' ', 1);

    if(command == "help"){
        Log("List of supported commands:");
        Log("  help             - show list of commands");
        Log("  exit             - exit the program");
        Log("  open  <filename> - load image");
        Log("  align <filename> - align image");
        Log("  save  <filename> - align image");
        Log("  filters          - list of available filters");
        Log("  filter <filter>  - apply filter");
        Log("  load <dirname>   - load filters");
    } else if(command == "open") {
        model.StartLoadImage(arg1);
    } else if(command == "align") {
        model.StartAlignImage();
    } else if(command == "save") {
        model.StartSaveLastImage(arg1);
    } else if(command == "filters") {
        emit ShowFilters(filters);
    } else if(command == "back"){
        model.RemoveLastFilter();
    } else if(command == "load") {
        if(arg1 == ""){
            Log("Usage: load <dirname>");
        } else {
            model.StartLoadPlugins(arg1);
        }
    } else if(command == "filter") {
        if(cmd_args == ""){
            Log("No filter specified");
        } else {
            int index = 0;
            for(PluginFilter *filter : filters){
                if(cmd_args == filter->Name().c_str()){
                    std::cout << "  params: ";
                    std::string t;
                    getline(std::cin, t);
                    params[0] = QString(t.c_str()).toDouble();
                    model.StartApplyFilter(index, params);
                    return;
                }
                ++index;
            }
            Log("No filter named \'" + cmd_args + "\'");
        }
    } else {
        Log("Unknown command");
        Log("Type 'help' for list of commands");
    }
}
