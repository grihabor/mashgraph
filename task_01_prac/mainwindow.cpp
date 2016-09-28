#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include "imageholder.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fileinfo("/home/")
{
    ui->setupUi(this);
    ui->filename_label->setText(fileinfo.absolutePath());
    ui->actions_widget->hide();

    imageHolders.push_back(new ImageHolder(Load, "Original"));
    imageHolders.push_back(new ImageHolder(Align, "Aligned"));

    for(auto imageHolder : imageHolders){
        ui->views_layout->addWidget(imageHolder);
        ui->views_layout->setAlignment(imageHolder, Qt::AlignLeft | Qt::AlignTop);
        model.AddObserver(imageHolder);
    }
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
    model.LoadImage(filename);
    ui->filename_label->setText(fileinfo.absoluteFilePath());
    ui->actions_widget->show();
}

int system (QString command)
{
    system(command.toStdString().c_str());
}

void MainWindow::on_align_button_clicked()
{
    model.AlignImage();
    /*
    system("pwd");
    QString exe_path = "../task_01/align_project/build/bin/align";
    QString input_img = fileinfo.absoluteFilePath();
    QString output_dir = fileinfo.absolutePath() + "/output";
    system("mkdir " + output_dir);
    QString output_img = output_dir + "/" + fileinfo.fileName();
    QString flag = "--align";
    QString cmd = exe_path + ' ' + input_img + ' ' + output_img + ' ' + flag;
    qInfo() << cmd;
    system(cmd);

    QPixmap pixmap(output_img);
    ui->aligned_image_label->setGeometry(pixmap.rect());
    ui->aligned_image_label->setPixmap(pixmap);
    //ui->align_widget->show();
    */
}
