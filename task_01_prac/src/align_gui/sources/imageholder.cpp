#include "../headers/imageholder.h"

using std::string;

ImageHolder::ImageHolder(string title, QProgressBar* progressBar, Logger& logger, QWidget *parent)
    : QWidget(parent), title(title), progressBar(progressBar), Log(logger)
{
    image_label = new QLabel;
    image_label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    title_label = new QLabel;
    title_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    title_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    title_label->setText(title.c_str());

    layout = new QVBoxLayout;
    layout->setAlignment(image_label, Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(title_label);
    layout->addWidget(image_label);

    setLayout(layout);
    hide();
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void ImageHolder::HandleSignal(QImage *img)
{
    //Log("ImageHolder handle signal");
    image_label->setPixmap(QPixmap::fromImage(*img));
    this->show();
    progressBar->hide();
}
