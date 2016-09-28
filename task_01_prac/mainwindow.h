#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfo>
#include "model.h"
#include "imageholder.h"

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

private:
    Ui::MainWindow *ui;
    QFileInfo fileinfo;
    Model model;
    std::vector<ImageHolder*> imageHolders;
};

#endif // MAINWINDOW_H
