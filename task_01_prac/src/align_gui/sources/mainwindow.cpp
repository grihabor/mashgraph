#include "../headers/mainwindow.h"
#include "../headers/imageholder.h"
#include "ui_mainwindow.h"
#include "../headers/pluginscontainer.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fileinfo(QDir::currentPath()),
    pluginDirInfo("./plugins"),
    Log(),
    model(Log)
{
    ui->setupUi(this);
    ui->filename_label->setText(fileinfo.absoluteFilePath());
    ui->plugins_dir_label->setText(pluginDirInfo.absoluteFilePath());
    ui->actions_widget->hide();

    imageHolders.push_back(new ImageHolder("Aligned", ui->progressBar, Log));
    imageHolders.push_back(new ImageHolder("Original", ui->progressBar, Log));

    for(auto imageHolder : imageHolders){
        ui->views_layout->addWidget(imageHolder);
        ui->views_layout->setAlignment(imageHolder, Qt::AlignLeft | Qt::AlignTop);
        //model.AddObserver(imageHolder);
    }
    QObject::connect(&model, SIGNAL(ImageAligned(QImage*)), imageHolders[0], SLOT(HandleSignal(QImage*)));
    QObject::connect(&model, SIGNAL(ImageLoaded(QImage*)), imageHolders[1], SLOT(HandleSignal(QImage*)));
    QObject::connect(&model, SIGNAL(ImageLoaded(QImage*)), imageHolders[0], SLOT(hide()));

    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    ui->progressBar->hide();

    pluginsContainer = new PluginsContainer(&model, ui->views_layout, ui->progressBar, Log, this);
    ui->plugins_container_layout->addLayout(pluginsContainer);
    QObject::connect(&model, SIGNAL(ClearFilters()), pluginsContainer, SLOT(ClearFilters()));

    QObject::connect(&Log, SIGNAL(Log(QString)), this, SLOT(LogTextEdit(QString)));

    qRegisterMetaType<QVector<PluginFilter*> >("QVector<PluginFilter*>");
    qRegisterMetaType<double*>("double*");
    QObject::connect(&model, SIGNAL(PluginsLoaded(QVector<PluginFilter*>)), pluginsContainer, SLOT(ShowPlugins(QVector<PluginFilter*>)));
    QObject::connect(&model, SIGNAL(LastFilterRemoved()), pluginsContainer, SLOT(RemoveLastFilter()));
    model.LoadPlugins(pluginDirInfo.absoluteFilePath());

    QObject::connect(&model, SIGNAL(ImageAligned(QImage*)), this, SLOT(UnlockButtons()));
    QObject::connect(&model, SIGNAL(ImageLoaded(QImage*)), this, SLOT(UnlockButtons()));
    QObject::connect(&model, SIGNAL(FilterApplied(PluginFilter*,QImage*)), this, SLOT(UnlockButtons()));
}

void MainWindow::LogTextEdit(QString log)
{
    QString text = ui->log_textedit->toPlainText();
    QString newText = "- " + QString(log) + "\n" + text;
    ui->log_textedit->setPlainText(newText);
    std::cout << log.toStdString() << std::endl;
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

    SetButtonState(true);
    ui->progressBar->show();
    model.LoadImage(filename);
    ui->filename_label->setText(fileinfo.absoluteFilePath());
    ui->actions_widget->show();
}

void MainWindow::on_align_button_clicked()
{
    SetButtonState(true);
    ui->progressBar->show();
    model.AlignImage();    
}

void MainWindow::SetButtonState(bool b)
{
    ui->align_button->blockSignals(b);
    ui->back_button->blockSignals(b);
    ui->save_button->blockSignals(b);
    ui->choose_dir_button->blockSignals(b);
    ui->choose_file_button->blockSignals(b);
    pluginsContainer->BlockButtons(b);
}

void MainWindow::UnlockButtons()
{
    SetButtonState(false);
}
void MainWindow::BlockButtons()
{
    SetButtonState(true);
}

void MainWindow::on_choose_dir_button_clicked()
{
    QString directory, start_dir;
    start_dir = pluginDirInfo.absolutePath();

    directory = QFileDialog::getExistingDirectory(this,
        tr("Open plugin directory"), start_dir, QFileDialog::ShowDirsOnly);
    if(directory == ""){
        Log("User didn't choose any plugin directory");
        return;
    }
    pluginDirInfo.setFile(directory);
    ui->plugins_dir_label->setText(directory);
    model.LoadPlugins(pluginDirInfo.absoluteFilePath());
}

void MainWindow::on_back_button_clicked()
{
    model.RemoveLastFilter();
}

void MainWindow::on_save_button_clicked()
{
    model.SaveLastImage(fileinfo.absolutePath() + "/output/" + fileinfo.fileName());
}
