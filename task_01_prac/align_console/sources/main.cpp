#include <QCoreApplication>
#include <QTimer>

#include "../headers/maintask.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MainTask *task = new MainTask(&a);
    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));
    QTimer::singleShot(0, task, SLOT(run()));

    return a.exec();
}
