/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

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
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout_2;
    QWidget *original_widget;
    QVBoxLayout *original_layout;
    QLabel *label;
    QLabel *image_label;
    QWidget *align_widget;
    QVBoxLayout *align_layout;
    QVBoxLayout *aligned_image_layout;
    QLabel *label_2;
    QLabel *aligned_image_label;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(1);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        filename_widget = new QWidget(centralWidget);
        filename_widget->setObjectName(QStringLiteral("filename_widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(filename_widget->sizePolicy().hasHeightForWidth());
        filename_widget->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(filename_widget);
        horizontalLayout->setSpacing(10);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        choose_file_button = new QPushButton(filename_widget);
        choose_file_button->setObjectName(QStringLiteral("choose_file_button"));
        sizePolicy1.setHeightForWidth(choose_file_button->sizePolicy().hasHeightForWidth());
        choose_file_button->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(choose_file_button);

        filename_label = new QLabel(filename_widget);
        filename_label->setObjectName(QStringLiteral("filename_label"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(filename_label->sizePolicy().hasHeightForWidth());
        filename_label->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(filename_label);


        verticalLayout->addWidget(filename_widget);

        actions_widget = new QWidget(centralWidget);
        actions_widget->setObjectName(QStringLiteral("actions_widget"));
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(actions_widget->sizePolicy().hasHeightForWidth());
        actions_widget->setSizePolicy(sizePolicy3);
        actions_layout = new QHBoxLayout(actions_widget);
        actions_layout->setSpacing(6);
        actions_layout->setContentsMargins(11, 11, 11, 11);
        actions_layout->setObjectName(QStringLiteral("actions_layout"));
        actions_layout->setContentsMargins(0, 0, 0, 0);
        align_button = new QPushButton(actions_widget);
        align_button->setObjectName(QStringLiteral("align_button"));
        QSizePolicy sizePolicy4(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(align_button->sizePolicy().hasHeightForWidth());
        align_button->setSizePolicy(sizePolicy4);

        actions_layout->addWidget(align_button);


        verticalLayout->addWidget(actions_widget);

        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 202, 156));
        sizePolicy1.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy1);
        horizontalLayout_2 = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        original_widget = new QWidget(scrollAreaWidgetContents);
        original_widget->setObjectName(QStringLiteral("original_widget"));
        original_layout = new QVBoxLayout(original_widget);
        original_layout->setSpacing(6);
        original_layout->setContentsMargins(11, 11, 11, 11);
        original_layout->setObjectName(QStringLiteral("original_layout"));
        label = new QLabel(original_widget);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        label->setAlignment(Qt::AlignCenter);

        original_layout->addWidget(label);

        image_label = new QLabel(original_widget);
        image_label->setObjectName(QStringLiteral("image_label"));
        sizePolicy1.setHeightForWidth(image_label->sizePolicy().hasHeightForWidth());
        image_label->setSizePolicy(sizePolicy1);
        image_label->setMinimumSize(QSize(0, 0));
        image_label->setBaseSize(QSize(0, 0));
        image_label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        original_layout->addWidget(image_label);


        horizontalLayout_2->addWidget(original_widget);

        align_widget = new QWidget(scrollAreaWidgetContents);
        align_widget->setObjectName(QStringLiteral("align_widget"));
        align_layout = new QVBoxLayout(align_widget);
        align_layout->setSpacing(6);
        align_layout->setContentsMargins(11, 11, 11, 11);
        align_layout->setObjectName(QStringLiteral("align_layout"));
        aligned_image_layout = new QVBoxLayout();
        aligned_image_layout->setSpacing(6);
        aligned_image_layout->setObjectName(QStringLiteral("aligned_image_layout"));
        label_2 = new QLabel(align_widget);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);
        label_2->setAlignment(Qt::AlignCenter);

        aligned_image_layout->addWidget(label_2);

        aligned_image_label = new QLabel(align_widget);
        aligned_image_label->setObjectName(QStringLiteral("aligned_image_label"));
        sizePolicy1.setHeightForWidth(aligned_image_label->sizePolicy().hasHeightForWidth());
        aligned_image_label->setSizePolicy(sizePolicy1);
        aligned_image_label->setMinimumSize(QSize(0, 0));
        aligned_image_label->setBaseSize(QSize(0, 0));
        aligned_image_label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        aligned_image_layout->addWidget(aligned_image_label);


        align_layout->addLayout(aligned_image_layout);


        horizontalLayout_2->addWidget(align_widget);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1015, 19));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Image aligner", 0));
        choose_file_button->setText(QApplication::translate("MainWindow", "Choose file", 0));
        filename_label->setText(QApplication::translate("MainWindow", "/home", 0));
        align_button->setText(QApplication::translate("MainWindow", "Align", 0));
        label->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">Original</span></p></body></html>", 0));
        image_label->setText(QApplication::translate("MainWindow", "<html><head/><body><p>original image</p><p>will be </p><p>placed here</p><p><br/></p></body></html>", 0));
        label_2->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">Aligned</span></p></body></html>", 0));
        aligned_image_label->setText(QApplication::translate("MainWindow", "<html><head/><body><p>aligned image</p><p>will be </p><p>placed here</p><p><br/></p></body></html>", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
