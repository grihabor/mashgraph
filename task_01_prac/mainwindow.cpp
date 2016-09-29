#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include "imageholder.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fileinfo(QDir::currentPath())
{
    ui->setupUi(this);
    ui->filename_label->setText(fileinfo.absolutePath());
    ui->actions_widget->hide();

    imageHolders.push_back(new ImageHolder(Load, "Original", ui->progressBar));
    imageHolders.push_back(new ImageHolder(Align, "Aligned", ui->progressBar));

    for(auto imageHolder : imageHolders){
        ui->views_layout->addWidget(imageHolder);
        ui->views_layout->setAlignment(imageHolder, Qt::AlignLeft | Qt::AlignTop);
        //model.AddObserver(imageHolder);
    }
    QObject::connect(&model, SIGNAL(ImageLoaded(QImage*)), imageHolders[0], SLOT(HandleSignal(QImage*)));
    QObject::connect(&model, SIGNAL(ImageAligned(QImage*)), imageHolders[1], SLOT(HandleSignal(QImage*)));

    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    ui->progressBar->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_choose_file_button_clicked()
{
    QString filename, start_dir;
    start_dir = fileinfo.absolutePath();
    filename = QFileDialog::getOpenFileName(this,
        tr("Open Image"), start_dir, tr("Image Files (*.bmp)"));

    if(filename == ""){
        return;
    }
    fileinfo.setFile(filename);
    ui->progressBar->show();
    model.LoadImage(filename);
    ui->filename_label->setText(fileinfo.absoluteFilePath());
    ui->actions_widget->show();
}

void MainWindow::on_align_button_clicked()
{
    ui->progressBar->show();
    model.AlignImage();
}
