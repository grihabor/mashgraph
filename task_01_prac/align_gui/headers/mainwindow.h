#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfo>
#include "../../shared/headers/model.h"
#include "imageholder.h"
#include "pluginscontainer.h"
#include "../../shared/headers/logger.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_choose_file_button_clicked();

    void on_align_button_clicked();

    void on_choose_dir_button_clicked();

    void LogTextEdit(QString);

    void on_back_button_clicked();

    void on_save_button_clicked();

private:
    Ui::MainWindow *ui;
    QFileInfo fileinfo;
    QFileInfo pluginDirInfo;
    std::vector<ImageHolder*> imageHolders;
    PluginsContainer* pluginsContainer;
    Logger Log;
    Model model;
};

#endif // MAINWINDOW_H
