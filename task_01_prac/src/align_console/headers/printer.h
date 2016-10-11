#ifndef PRINTER_H
#define PRINTER_H

#include <QObject>
#include <QVector>
#include <iostream>
#include "../../shared/headers/logger.h"
#include "../../shared/headers/pluginfilter.h"

using std::cout;
using std::endl;

class Printer : public QObject
{
    Q_OBJECT

public:
    Printer()
        : QObject()
    {}
    Printer(Logger& logger)
        : QObject()
    {
        QObject::connect(&logger, SIGNAL(Log(QString)), this, SLOT(Log(QString)));
    }
    ~Printer(){}

public slots:
    void Log(QString s)
    {
        cout << "# " << s.toStdString() << endl;
    }
    void ImageLoaded()
    {
        cout << "- Image loaded successfully" << endl;
    }
    void ImageAligned()
    {
        cout << "- Image aligned successfully" << endl;
    }
    void ImageSaved(QString filename)
    {
        cout << ("- " + filename + ":").toStdString() << endl;
        cout << "    Image saved successfully" << endl;
    }
    void FilterApplied(PluginFilter *filter)
    {
        cout << (QString("- Filter \'") + filter->Name().c_str() + "\' applied").toStdString() << endl;
    }
    void FiltersLoaded(QVector<PluginFilter*> filters)
    {
        if(filters.size() == 0){
            cout << "- No filters loaded" << endl;
        } else {
            cout << "- Loaded " << filters.size() << " filters:" << endl;
            for(PluginFilter* filter : filters)
                cout << "-   " << filter->Name() << endl;
        }
    }
};

#endif // PRINTER_H
