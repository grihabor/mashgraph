#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QString>
#include <QStringBuilder>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fileinfo("/home/")
{
    ui->setupUi(this);
    ui->filename_label->setText(fileinfo.absolutePath());
    ui->actions_widget->hide();
    ui->original_widget->hide();
    ui->align_widget->hide();
    ui->aligned_image_layout->setAlignment(ui->aligned_image_label, Qt::AlignLeft | Qt::AlignTop);
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
    ui->filename_label->setText(fileinfo.absoluteFilePath());

    QPixmap pixmap(filename);
    ui->image_label->setGeometry(pixmap.rect());
    ui->image_label->setPixmap(pixmap);
    ui->original_widget->show();
    ui->actions_widget->show();
}

int system (QString command)
{
    system(command.toStdString().c_str());
}

void MainWindow::on_align_button_clicked()
{
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
    ui->align_widget->show();
}
