/********************************************************************************
** Form generated from reading UI file 'DarkenUI.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DARKENUI_H
#define UI_DARKENUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DarkenUIClass
{
public:
    QWidget *CentralWidget;
    QGridLayout *gridLayout_2;

    void setupUi(QMainWindow *DarkenUIClass)
    {
        if (DarkenUIClass->objectName().isEmpty())
            DarkenUIClass->setObjectName(QString::fromUtf8("DarkenUIClass"));
        DarkenUIClass->resize(800, 600);
        CentralWidget = new QWidget(DarkenUIClass);
        CentralWidget->setObjectName(QString::fromUtf8("CentralWidget"));
        gridLayout_2 = new QGridLayout(CentralWidget);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        DarkenUIClass->setCentralWidget(CentralWidget);

        retranslateUi(DarkenUIClass);

        QMetaObject::connectSlotsByName(DarkenUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *DarkenUIClass)
    {
        DarkenUIClass->setWindowTitle(QApplication::translate("DarkenUIClass", "Darken", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DarkenUIClass: public Ui_DarkenUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DARKENUI_H
