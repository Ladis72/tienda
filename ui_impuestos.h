/********************************************************************************
** Form generated from reading UI file 'impuestos.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
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
            impuestos->setObjectName(QString::fromUtf8("impuestos"));
        impuestos->resize(640, 480);
        gridLayout = new QGridLayout(impuestos);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableView = new QTableView(impuestos);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        gridLayout->addWidget(tableView, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonAgregar = new QPushButton(impuestos);
        pushButtonAgregar->setObjectName(QString::fromUtf8("pushButtonAgregar"));

        horizontalLayout->addWidget(pushButtonAgregar);

        pushButtonElimiar = new QPushButton(impuestos);
        pushButtonElimiar->setObjectName(QString::fromUtf8("pushButtonElimiar"));

        horizontalLayout->addWidget(pushButtonElimiar);

        pushButtonGuardar = new QPushButton(impuestos);
        pushButtonGuardar->setObjectName(QString::fromUtf8("pushButtonGuardar"));

        horizontalLayout->addWidget(pushButtonGuardar);

        pushButtonSalir = new QPushButton(impuestos);
        pushButtonSalir->setObjectName(QString::fromUtf8("pushButtonSalir"));

        horizontalLayout->addWidget(pushButtonSalir);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(impuestos);
        QObject::connect(pushButtonSalir, SIGNAL(clicked()), impuestos, SLOT(reject()));

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
