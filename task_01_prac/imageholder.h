#ifndef IMAGEHOLDER_H
#define IMAGEHOLDER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

#include "observer.h"

class ImageHolder : public QWidget, public Observer
{
public:
    Event event;
    std::string title;
    QVBoxLayout *layout;
    QLabel *title_label;
    QLabel *image_label;

public:
    ImageHolder(Event event, std::string title, QWidget *parent=nullptr);
    void HandleEvent(Event msg, QImage *obj) override;
};

#endif // IMAGEHOLDER_H
