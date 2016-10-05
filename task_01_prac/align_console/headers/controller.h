#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QString>
#include "../../shared/headers/logger.h"
#include "../../shared/headers/model.h"
#include "printer.h"

class Controller : public QObject
{
    Q_OBJECT

private:
    Logger& Log;
    Model model;
    Printer printer;
    QVector<PluginFilter*> filters;

signals:
    void ShowFilters(QVector<PluginFilter*> plugins);

public slots:
    void FiltersLoaded(QVector<PluginFilter*> plugins);

public:
    Controller(Logger& logger);
    void HandleInput(QString input);
};

#endif
