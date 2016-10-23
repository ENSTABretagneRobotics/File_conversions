/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu 20. Oct 18:30:51 2016
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "widgetVaimos.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    widgetVaimos *widgetVaimos1;
    QScrollBar *k0_horizontalScrollBar;
    QScrollBar *Echelle_verticalScrollBar;
    QPushButton *pushButtonLoadFile;
    QScrollBar *Step_verticalScrollBar;
    QCheckBox *Wind_checkBox;
    QCheckBox *Sail1_checkBox;
    QCheckBox *Sail2_checkBox;
    QCheckBox *Text_checkBox;
    QCheckBox *Go_checkBox;
    QScrollBar *ks0_verticalScrollBar;
    QScrollBar *ks1_verticalScrollBar;
    QLabel *ks0_label;
    QLabel *ks1_label;
    QLabel *Echelle_label;
    QLabel *Step_label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(1000, 650);
        MainWindow->setMinimumSize(QSize(1000, 650));
        MainWindow->setMaximumSize(QSize(1000, 650));
        MainWindow->setCursor(QCursor(Qt::PointingHandCursor));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        widgetVaimos1 = new widgetVaimos(centralWidget);
        widgetVaimos1->setObjectName(QString::fromUtf8("widgetVaimos1"));
        widgetVaimos1->setGeometry(QRect(120, 30, 600, 600));
        widgetVaimos1->setMinimumSize(QSize(600, 600));
        k0_horizontalScrollBar = new QScrollBar(centralWidget);
        k0_horizontalScrollBar->setObjectName(QString::fromUtf8("k0_horizontalScrollBar"));
        k0_horizontalScrollBar->setGeometry(QRect(20, 0, 701, 20));
        k0_horizontalScrollBar->setMinimum(0);
        k0_horizontalScrollBar->setMaximum(50000);
        k0_horizontalScrollBar->setSingleStep(1);
        k0_horizontalScrollBar->setValue(0);
        k0_horizontalScrollBar->setOrientation(Qt::Horizontal);
        Echelle_verticalScrollBar = new QScrollBar(centralWidget);
        Echelle_verticalScrollBar->setObjectName(QString::fromUtf8("Echelle_verticalScrollBar"));
        Echelle_verticalScrollBar->setGeometry(QRect(20, 60, 21, 311));
        Echelle_verticalScrollBar->setMinimum(1);
        Echelle_verticalScrollBar->setMaximum(100);
        Echelle_verticalScrollBar->setSingleStep(1);
        Echelle_verticalScrollBar->setValue(25);
        Echelle_verticalScrollBar->setOrientation(Qt::Vertical);
        pushButtonLoadFile = new QPushButton(centralWidget);
        pushButtonLoadFile->setObjectName(QString::fromUtf8("pushButtonLoadFile"));
        pushButtonLoadFile->setGeometry(QRect(760, 0, 75, 23));
        Step_verticalScrollBar = new QScrollBar(centralWidget);
        Step_verticalScrollBar->setObjectName(QString::fromUtf8("Step_verticalScrollBar"));
        Step_verticalScrollBar->setGeometry(QRect(70, 60, 16, 311));
        Step_verticalScrollBar->setMinimum(1);
        Step_verticalScrollBar->setValue(10);
        Step_verticalScrollBar->setOrientation(Qt::Vertical);
        Wind_checkBox = new QCheckBox(centralWidget);
        Wind_checkBox->setObjectName(QString::fromUtf8("Wind_checkBox"));
        Wind_checkBox->setGeometry(QRect(760, 70, 70, 17));
        Wind_checkBox->setChecked(true);
        Sail1_checkBox = new QCheckBox(centralWidget);
        Sail1_checkBox->setObjectName(QString::fromUtf8("Sail1_checkBox"));
        Sail1_checkBox->setEnabled(true);
        Sail1_checkBox->setGeometry(QRect(760, 100, 151, 17));
        Sail1_checkBox->setAcceptDrops(false);
        Sail1_checkBox->setChecked(false);
        Sail2_checkBox = new QCheckBox(centralWidget);
        Sail2_checkBox->setObjectName(QString::fromUtf8("Sail2_checkBox"));
        Sail2_checkBox->setGeometry(QRect(760, 130, 101, 17));
        Sail2_checkBox->setChecked(true);
        Text_checkBox = new QCheckBox(centralWidget);
        Text_checkBox->setObjectName(QString::fromUtf8("Text_checkBox"));
        Text_checkBox->setGeometry(QRect(760, 40, 70, 17));
        Text_checkBox->setChecked(true);
        Go_checkBox = new QCheckBox(centralWidget);
        Go_checkBox->setObjectName(QString::fromUtf8("Go_checkBox"));
        Go_checkBox->setGeometry(QRect(760, 160, 81, 17));
        Go_checkBox->setChecked(true);
        ks0_verticalScrollBar = new QScrollBar(centralWidget);
        ks0_verticalScrollBar->setObjectName(QString::fromUtf8("ks0_verticalScrollBar"));
        ks0_verticalScrollBar->setGeometry(QRect(760, 240, 16, 331));
        ks0_verticalScrollBar->setOrientation(Qt::Vertical);
        ks1_verticalScrollBar = new QScrollBar(centralWidget);
        ks1_verticalScrollBar->setObjectName(QString::fromUtf8("ks1_verticalScrollBar"));
        ks1_verticalScrollBar->setGeometry(QRect(810, 240, 16, 331));
        ks1_verticalScrollBar->setOrientation(Qt::Vertical);
        ks0_label = new QLabel(centralWidget);
        ks0_label->setObjectName(QString::fromUtf8("ks0_label"));
        ks0_label->setGeometry(QRect(760, 220, 46, 13));
        ks1_label = new QLabel(centralWidget);
        ks1_label->setObjectName(QString::fromUtf8("ks1_label"));
        ks1_label->setGeometry(QRect(810, 220, 46, 13));
        Echelle_label = new QLabel(centralWidget);
        Echelle_label->setObjectName(QString::fromUtf8("Echelle_label"));
        Echelle_label->setGeometry(QRect(20, 40, 46, 13));
        Step_label = new QLabel(centralWidget);
        Step_label->setObjectName(QString::fromUtf8("Step_label"));
        Step_label->setGeometry(QRect(70, 40, 46, 13));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1000, 20));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Interface pour VAIMOS (luc.jaulin@ensta-bretagne.fr)", 0, QApplication::UnicodeUTF8));
        pushButtonLoadFile->setText(QApplication::translate("MainWindow", "Load file", 0, QApplication::UnicodeUTF8));
        Wind_checkBox->setText(QApplication::translate("MainWindow", "Wind", 0, QApplication::UnicodeUTF8));
        Sail1_checkBox->setText(QApplication::translate("MainWindow", "Sail with compass", 0, QApplication::UnicodeUTF8));
        Sail2_checkBox->setText(QApplication::translate("MainWindow", "Sail with motor", 0, QApplication::UnicodeUTF8));
        Text_checkBox->setText(QApplication::translate("MainWindow", "Text", 0, QApplication::UnicodeUTF8));
        Go_checkBox->setText(QApplication::translate("MainWindow", "Go direction", 0, QApplication::UnicodeUTF8));
        ks0_label->setText(QApplication::translate("MainWindow", "ks0", 0, QApplication::UnicodeUTF8));
        ks1_label->setText(QApplication::translate("MainWindow", "ks1", 0, QApplication::UnicodeUTF8));
        Echelle_label->setText(QApplication::translate("MainWindow", "Echelle", 0, QApplication::UnicodeUTF8));
        Step_label->setText(QApplication::translate("MainWindow", "Step", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
