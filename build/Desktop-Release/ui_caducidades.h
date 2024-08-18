/********************************************************************************
** Form generated from reading UI file 'caducidades.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CADUCIDADES_H
#define UI_CADUCIDADES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_Caducidades
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QTableView *tableView;

    void setupUi(QDialog *Caducidades)
    {
        if (Caducidades->objectName().isEmpty())
            Caducidades->setObjectName(QString::fromUtf8("Caducidades"));
        Caducidades->resize(1003, 523);
        gridLayout = new QGridLayout(Caducidades);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButton = new QPushButton(Caducidades);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 1, 0, 1, 1);

        tableView = new QTableView(Caducidades);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout->addWidget(tableView, 0, 0, 1, 1);


        retranslateUi(Caducidades);
        QObject::connect(pushButton, SIGNAL(clicked()), Caducidades, SLOT(close()));

        QMetaObject::connectSlotsByName(Caducidades);
    } // setupUi

    void retranslateUi(QDialog *Caducidades)
    {
        Caducidades->setWindowTitle(QCoreApplication::translate("Caducidades", "Caducidades", nullptr));
        pushButton->setText(QCoreApplication::translate("Caducidades", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Caducidades: public Ui_Caducidades {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CADUCIDADES_H
