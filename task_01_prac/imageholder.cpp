#include "imageholder.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

ImageHolder::ImageHolder(Event event, string title, QProgressBar* progressBar, QWidget *parent)
    : QWidget(parent), event(event), title(title), progressBar(progressBar)
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

void ImageHolder::HandleEvent(Event e, QImage *img)
{
    cout << "ImageHolder handle event: " << e << endl;
    if(e != event){
        cout << "Skipping..." << endl;
        return;
    }

    image_label->setPixmap(QPixmap::fromImage(*img));
    this->show();
    cout << "Handled" << endl;
}

void ImageHolder::HandleSignal(QImage *img)
{
    cout << "ImageHolder handle signal" << endl;
    image_label->setPixmap(QPixmap::fromImage(*img));
    this->show();
    progressBar->hide();
}
