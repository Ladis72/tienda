/********************************************************************************
** Form generated from reading UI file 'prestamistas.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRESTAMISTAS_H
#define UI_PRESTAMISTAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_Prestamistas
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButtonAnadir;
    QPushButton *pushButtonModificar;
    QPushButton *pushButtonAceptar;
    QTableView *tableView;

    void setupUi(QDialog *Prestamistas)
    {
        if (Prestamistas->objectName().isEmpty())
            Prestamistas->setObjectName(QString::fromUtf8("Prestamistas"));
        Prestamistas->resize(400, 300);
        gridLayout = new QGridLayout(Prestamistas);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButtonAnadir = new QPushButton(Prestamistas);
        pushButtonAnadir->setObjectName(QString::fromUtf8("pushButtonAnadir"));
        pushButtonAnadir->setAutoDefault(false);

        gridLayout->addWidget(pushButtonAnadir, 1, 0, 1, 1);

        pushButtonModificar = new QPushButton(Prestamistas);
        pushButtonModificar->setObjectName(QString::fromUtf8("pushButtonModificar"));
        pushButtonModificar->setAutoDefault(false);

        gridLayout->addWidget(pushButtonModificar, 1, 1, 1, 1);

        pushButtonAceptar = new QPushButton(Prestamistas);
        pushButtonAceptar->setObjectName(QString::fromUtf8("pushButtonAceptar"));
        pushButtonAceptar->setAutoDefault(false);

        gridLayout->addWidget(pushButtonAceptar, 1, 2, 1, 1);

        tableView = new QTableView(Prestamistas);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout->addWidget(tableView, 0, 0, 1, 3);

        QWidget::setTabOrder(pushButtonAnadir, pushButtonModificar);
        QWidget::setTabOrder(pushButtonModificar, pushButtonAceptar);
        QWidget::setTabOrder(pushButtonAceptar, tableView);

        retranslateUi(Prestamistas);
        QObject::connect(pushButtonAceptar, SIGNAL(clicked()), Prestamistas, SLOT(accept()));

        QMetaObject::connectSlotsByName(Prestamistas);
    } // setupUi

    void retranslateUi(QDialog *Prestamistas)
    {
        Prestamistas->setWindowTitle(QCoreApplication::translate("Prestamistas", "Dialog", nullptr));
        pushButtonAnadir->setText(QCoreApplication::translate("Prestamistas", "A\303\261adir", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("Prestamistas", "Modificar", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("Prestamistas", "Aceptar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Prestamistas: public Ui_Prestamistas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRESTAMISTAS_H
