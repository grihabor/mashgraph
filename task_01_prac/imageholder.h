#ifndef IMAGEHOLDER_H
#define IMAGEHOLDER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>

#include "observer.h"

class ImageHolder : public QWidget, public Observer
{
    Q_OBJECT

public:
    Event event;
    std::string title;
    QVBoxLayout *layout;
    QLabel *title_label;
    QLabel *image_label;
    QProgressBar* progressBar;

public:
    ImageHolder(Event event, std::string title, QProgressBar* progressBar, QWidget *parent=nullptr);

    void HandleEvent(Event msg, QImage *obj) override;
public slots:
    void HandleSignal(QImage *obj) override;
};

#endif // IMAGEHOLDER_H
