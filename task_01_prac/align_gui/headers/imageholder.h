#ifndef IMAGEHOLDER_H
#define IMAGEHOLDER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>

#include "../../shared/headers/logger.h"

class ImageHolder : public QWidget
{
    Q_OBJECT

public:
    std::string title;
    QVBoxLayout *layout;
    QLabel *title_label;
    QLabel *image_label;
    QProgressBar* progressBar;
    Logger& Log;

public:
    ImageHolder(std::string title, QProgressBar* progressBar, Logger& logger, QWidget *parent=nullptr);

public slots:
    void HandleSignal(QImage *obj);
};

#endif // IMAGEHOLDER_H
