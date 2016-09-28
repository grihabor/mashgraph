#ifndef OBSERVER_H
#define OBSERVER_H

#include <QObject>

enum Event{
    Load,
    Align
};

class Observer
{
public:
    virtual void HandleEvent(Event, QImage* obj) = 0;
};

#endif // OBSERVER_H
