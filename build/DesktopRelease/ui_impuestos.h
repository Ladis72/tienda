/********************************************************************************
** Form generated from reading UI file 'impuestos.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPUESTOS_H
#define UI_IMPUESTOS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_impuestos
{
public:
    QGridLayout *gridLayout;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonAgregar;
    QPushButton *pushButtonElimiar;
    QPushButton *pushButtonGuardar;
    QPushButton *pushButtonSalir;

    void setupUi(QDialog *impuestos)
    {
        if (impuestos->objectName().isEmpty())
            impuestos->setObjectName("impuestos");
        impuestos->resize(640, 480);
        gridLayout = new QGridLayout(impuestos);
        gridLayout->setObjectName("gridLayout");
        tableView = new QTableView(impuestos);
        tableView->setObjectName("tableView");
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        gridLayout->addWidget(tableView, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pushButtonAgregar = new QPushButton(impuestos);
        pushButtonAgregar->setObjectName("pushButtonAgregar");

        horizontalLayout->addWidget(pushButtonAgregar);

        pushButtonElimiar = new QPushButton(impuestos);
        pushButtonElimiar->setObjectName("pushButtonElimiar");

        horizontalLayout->addWidget(pushButtonElimiar);

        pushButtonGuardar = new QPushButton(impuestos);
        pushButtonGuardar->setObjectName("pushButtonGuardar");

        horizontalLayout->addWidget(pushButtonGuardar);

        pushButtonSalir = new QPushButton(impuestos);
        pushButtonSalir->setObjectName("pushButtonSalir");

        horizontalLayout->addWidget(pushButtonSalir);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(impuestos);
        QObject::connect(pushButtonSalir, &QPushButton::clicked, impuestos, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(impuestos);
    } // setupUi

    void retranslateUi(QDialog *impuestos)
    {
        impuestos->setWindowTitle(QCoreApplication::translate("impuestos", "Dialog", nullptr));
        pushButtonAgregar->setText(QCoreApplication::translate("impuestos", "Agregar", nullptr));
        pushButtonElimiar->setText(QCoreApplication::translate("impuestos", "Eliminar", nullptr));
        pushButtonGuardar->setText(QCoreApplication::translate("impuestos", "Guardar", nullptr));
        pushButtonSalir->setText(QCoreApplication::translate("impuestos", "Salir", nullptr));
    } // retranslateUi

};

namespace Ui {
    class impuestos: public Ui_impuestos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPUESTOS_H
