#ifndef BASE_H
#define BASE_H

#include <QString>
#include <QObject>

class Logger : public QObject
{
    Q_OBJECT

public:
    Logger()
    {}
    ~Logger()
    {}
    void operator ()(QString s)
    {
        emit Log(s);
    }

signals:
    void Log(QString);
};

#endif // BASE_H
