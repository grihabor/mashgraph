#ifndef MAINTASK_H
#define MAINTASK_H

#include "printer.h"
#include "controller.h"
#include "../../shared/headers/logger.h"
#include "../../shared/headers/model.h"

using std::cin;

class MainTask : public QObject
{
    Q_OBJECT

public:
    MainTask(QObject *parent = 0)
        : QObject(parent)
    {}
    ~MainTask(){}

public slots:
    void run()
    {
        Logger logger;
        Controller c(logger);
        std::string cmd;
        while(getline(cin, cmd)){
            QString command(cmd.c_str());
            command = command.simplified();
            if(command == "exit"){
                break;
            }
            c.HandleInput(command);
        }
        emit finished();
    }

signals:
    void finished();
};

#endif // MAINTASK_H
