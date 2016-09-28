#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfo>

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
};

#endif // MAINWINDOW_H
