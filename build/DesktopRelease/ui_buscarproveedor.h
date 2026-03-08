/********************************************************************************
** Form generated from reading UI file 'buscarproveedor.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUSCARPROVEEDOR_H
#define UI_BUSCARPROVEEDOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_BuscarProveedor
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButtonCancelar;
    QPushButton *pushButtonAceptar;
    QTableView *tableView;

    void setupUi(QDialog *BuscarProveedor)
    {
        if (BuscarProveedor->objectName().isEmpty())
            BuscarProveedor->setObjectName("BuscarProveedor");
        BuscarProveedor->resize(578, 348);
        gridLayout = new QGridLayout(BuscarProveedor);
        gridLayout->setObjectName("gridLayout");
        pushButtonCancelar = new QPushButton(BuscarProveedor);
        pushButtonCancelar->setObjectName("pushButtonCancelar");

        gridLayout->addWidget(pushButtonCancelar, 1, 1, 1, 1);

        pushButtonAceptar = new QPushButton(BuscarProveedor);
        pushButtonAceptar->setObjectName("pushButtonAceptar");

        gridLayout->addWidget(pushButtonAceptar, 1, 0, 1, 1);

        tableView = new QTableView(BuscarProveedor);
        tableView->setObjectName("tableView");

        gridLayout->addWidget(tableView, 0, 0, 1, 2);

        QWidget::setTabOrder(tableView, pushButtonAceptar);
        QWidget::setTabOrder(pushButtonAceptar, pushButtonCancelar);

        retranslateUi(BuscarProveedor);
        QObject::connect(pushButtonAceptar, &QPushButton::clicked, BuscarProveedor, qOverload<>(&QDialog::accept));
        QObject::connect(pushButtonCancelar, &QPushButton::clicked, BuscarProveedor, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(BuscarProveedor);
    } // setupUi

    void retranslateUi(QDialog *BuscarProveedor)
    {
        BuscarProveedor->setWindowTitle(QCoreApplication::translate("BuscarProveedor", "Dialog", nullptr));
        pushButtonCancelar->setText(QCoreApplication::translate("BuscarProveedor", "Cancelar", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("BuscarProveedor", "Aceptar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BuscarProveedor: public Ui_BuscarProveedor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUSCARPROVEEDOR_H
