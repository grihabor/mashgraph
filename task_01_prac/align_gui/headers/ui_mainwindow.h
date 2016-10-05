/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QWidget *filename_widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *choose_file_button;
    QLabel *filename_label;
    QWidget *actions_widget;
    QHBoxLayout *actions_layout;
    QPushButton *align_button;
    QProgressBar *progressBar;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *views_layout;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(372, 331);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(1);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        filename_widget = new QWidget(centralWidget);
        filename_widget->setObjectName(QString::fromUtf8("filename_widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(filename_widget->sizePolicy().hasHeightForWidth());
        filename_widget->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(filename_widget);
        horizontalLayout->setSpacing(10);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        choose_file_button = new QPushButton(filename_widget);
        choose_file_button->setObjectName(QString::fromUtf8("choose_file_button"));
        sizePolicy1.setHeightForWidth(choose_file_button->sizePolicy().hasHeightForWidth());
        choose_file_button->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(choose_file_button);

        filename_label = new QLabel(filename_widget);
        filename_label->setObjectName(QString::fromUtf8("filename_label"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(filename_label->sizePolicy().hasHeightForWidth());
        filename_label->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(filename_label);


        verticalLayout->addWidget(filename_widget);

        actions_widget = new QWidget(centralWidget);
        actions_widget->setObjectName(QString::fromUtf8("actions_widget"));
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(actions_widget->sizePolicy().hasHeightForWidth());
        actions_widget->setSizePolicy(sizePolicy3);
        actions_layout = new QHBoxLayout(actions_widget);
        actions_layout->setSpacing(6);
        actions_layout->setContentsMargins(11, 11, 11, 11);
        actions_layout->setObjectName(QString::fromUtf8("actions_layout"));
        actions_layout->setContentsMargins(0, 0, 0, 0);
        align_button = new QPushButton(actions_widget);
        align_button->setObjectName(QString::fromUtf8("align_button"));
        QSizePolicy sizePolicy4(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(align_button->sizePolicy().hasHeightForWidth());
        align_button->setSizePolicy(sizePolicy4);

        actions_layout->addWidget(align_button);

        progressBar = new QProgressBar(actions_widget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        actions_layout->addWidget(progressBar);


        verticalLayout->addWidget(actions_widget);

        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 20, 20));
        sizePolicy1.setHeightForWidth(scrollAreaWidgetContents_2->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents_2->setSizePolicy(sizePolicy1);
        verticalLayout_2 = new QVBoxLayout(scrollAreaWidgetContents_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        views_layout = new QHBoxLayout();
        views_layout->setSpacing(6);
        views_layout->setObjectName(QString::fromUtf8("views_layout"));

        verticalLayout_2->addLayout(views_layout);

        scrollArea->setWidget(scrollAreaWidgetContents_2);

        verticalLayout->addWidget(scrollArea);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 372, 19));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Image aligner", 0, QApplication::UnicodeUTF8));
        choose_file_button->setText(QApplication::translate("MainWindow", "Choose file", 0, QApplication::UnicodeUTF8));
        filename_label->setText(QApplication::translate("MainWindow", "/home", 0, QApplication::UnicodeUTF8));
        align_button->setText(QApplication::translate("MainWindow", "Align", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
