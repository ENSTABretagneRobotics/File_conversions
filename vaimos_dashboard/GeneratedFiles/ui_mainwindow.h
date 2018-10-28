/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun 28. Oct 21:51:57 2018
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
    QCheckBox *FilteredWindDirMode_checkBox;
    QCheckBox *ResetOrigin_checkBox;
    QCheckBox *ThickLine_checkBox;
    QCheckBox *MotorboatMode_checkBox;
    QCheckBox *Recalce_checkBox;
    QCheckBox *SwitchAvailable_checkBox;
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
        Echelle_verticalScrollBar->setGeometry(QRect(20, 60, 21, 531));
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
        Step_verticalScrollBar->setGeometry(QRect(70, 60, 16, 531));
        Step_verticalScrollBar->setMinimum(1);
        Step_verticalScrollBar->setValue(10);
        Step_verticalScrollBar->setOrientation(Qt::Vertical);
        Wind_checkBox = new QCheckBox(centralWidget);
        Wind_checkBox->setObjectName(QString::fromUtf8("Wind_checkBox"));
        Wind_checkBox->setGeometry(QRect(760, 50, 131, 17));
        Wind_checkBox->setChecked(true);
        Sail1_checkBox = new QCheckBox(centralWidget);
        Sail1_checkBox->setObjectName(QString::fromUtf8("Sail1_checkBox"));
        Sail1_checkBox->setEnabled(true);
        Sail1_checkBox->setGeometry(QRect(760, 70, 131, 17));
        Sail1_checkBox->setAcceptDrops(false);
        Sail1_checkBox->setChecked(false);
        Sail2_checkBox = new QCheckBox(centralWidget);
        Sail2_checkBox->setObjectName(QString::fromUtf8("Sail2_checkBox"));
        Sail2_checkBox->setGeometry(QRect(760, 90, 131, 17));
        Sail2_checkBox->setChecked(true);
        Text_checkBox = new QCheckBox(centralWidget);
        Text_checkBox->setObjectName(QString::fromUtf8("Text_checkBox"));
        Text_checkBox->setGeometry(QRect(760, 30, 131, 17));
        Text_checkBox->setChecked(true);
        Go_checkBox = new QCheckBox(centralWidget);
        Go_checkBox->setObjectName(QString::fromUtf8("Go_checkBox"));
        Go_checkBox->setGeometry(QRect(760, 110, 131, 17));
        Go_checkBox->setChecked(true);
        ks0_verticalScrollBar = new QScrollBar(centralWidget);
        ks0_verticalScrollBar->setObjectName(QString::fromUtf8("ks0_verticalScrollBar"));
        ks0_verticalScrollBar->setGeometry(QRect(760, 280, 20, 311));
        ks0_verticalScrollBar->setOrientation(Qt::Vertical);
        ks1_verticalScrollBar = new QScrollBar(centralWidget);
        ks1_verticalScrollBar->setObjectName(QString::fromUtf8("ks1_verticalScrollBar"));
        ks1_verticalScrollBar->setGeometry(QRect(810, 280, 20, 311));
        ks1_verticalScrollBar->setOrientation(Qt::Vertical);
        ks0_label = new QLabel(centralWidget);
        ks0_label->setObjectName(QString::fromUtf8("ks0_label"));
        ks0_label->setGeometry(QRect(760, 260, 46, 13));
        ks1_label = new QLabel(centralWidget);
        ks1_label->setObjectName(QString::fromUtf8("ks1_label"));
        ks1_label->setGeometry(QRect(810, 260, 46, 13));
        Echelle_label = new QLabel(centralWidget);
        Echelle_label->setObjectName(QString::fromUtf8("Echelle_label"));
        Echelle_label->setGeometry(QRect(20, 40, 46, 13));
        Step_label = new QLabel(centralWidget);
        Step_label->setObjectName(QString::fromUtf8("Step_label"));
        Step_label->setGeometry(QRect(70, 40, 46, 13));
        FilteredWindDirMode_checkBox = new QCheckBox(centralWidget);
        FilteredWindDirMode_checkBox->setObjectName(QString::fromUtf8("FilteredWindDirMode_checkBox"));
        FilteredWindDirMode_checkBox->setGeometry(QRect(760, 150, 131, 18));
        FilteredWindDirMode_checkBox->setChecked(false);
        ResetOrigin_checkBox = new QCheckBox(centralWidget);
        ResetOrigin_checkBox->setObjectName(QString::fromUtf8("ResetOrigin_checkBox"));
        ResetOrigin_checkBox->setGeometry(QRect(760, 210, 131, 18));
        ThickLine_checkBox = new QCheckBox(centralWidget);
        ThickLine_checkBox->setObjectName(QString::fromUtf8("ThickLine_checkBox"));
        ThickLine_checkBox->setGeometry(QRect(760, 130, 131, 18));
        ThickLine_checkBox->setChecked(true);
        MotorboatMode_checkBox = new QCheckBox(centralWidget);
        MotorboatMode_checkBox->setObjectName(QString::fromUtf8("MotorboatMode_checkBox"));
        MotorboatMode_checkBox->setGeometry(QRect(760, 170, 131, 18));
        Recalce_checkBox = new QCheckBox(centralWidget);
        Recalce_checkBox->setObjectName(QString::fromUtf8("Recalce_checkBox"));
        Recalce_checkBox->setGeometry(QRect(760, 190, 131, 18));
        Recalce_checkBox->setChecked(true);
        SwitchAvailable_checkBox = new QCheckBox(centralWidget);
        SwitchAvailable_checkBox->setObjectName(QString::fromUtf8("SwitchAvailable_checkBox"));
        SwitchAvailable_checkBox->setGeometry(QRect(760, 230, 131, 18));
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
        Echelle_label->setText(QApplication::translate("MainWindow", "Scale", 0, QApplication::UnicodeUTF8));
        Step_label->setText(QApplication::translate("MainWindow", "Step", 0, QApplication::UnicodeUTF8));
        FilteredWindDirMode_checkBox->setText(QApplication::translate("MainWindow", "Filtered wind mode", 0, QApplication::UnicodeUTF8));
        ResetOrigin_checkBox->setText(QApplication::translate("MainWindow", "Reset origin", 0, QApplication::UnicodeUTF8));
        ThickLine_checkBox->setText(QApplication::translate("MainWindow", "Thick line", 0, QApplication::UnicodeUTF8));
        MotorboatMode_checkBox->setText(QApplication::translate("MainWindow", "Motorboat mode", 0, QApplication::UnicodeUTF8));
        Recalce_checkBox->setText(QApplication::translate("MainWindow", "Recalc dist to the line", 0, QApplication::UnicodeUTF8));
        SwitchAvailable_checkBox->setText(QApplication::translate("MainWindow", "Switch available", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
