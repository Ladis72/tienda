/********************************************************************************
** Form generated from reading UI file 'gestionpedidos.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GESTIONPEDIDOS_H
#define UI_GESTIONPEDIDOS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_GestionPedidos
{
public:
    QGridLayout *gridLayout;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonNuevo;
    QPushButton *pushButtonGestionar;
    QPushButton *pushButtonAceptar;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonImprimir;
    QPushButton *pushButton;

    void setupUi(QDialog *GestionPedidos)
    {
        if (GestionPedidos->objectName().isEmpty())
            GestionPedidos->setObjectName("GestionPedidos");
        GestionPedidos->resize(960, 617);
        GestionPedidos->setMinimumSize(QSize(0, 0));
        gridLayout = new QGridLayout(GestionPedidos);
        gridLayout->setObjectName("gridLayout");
        tableView = new QTableView(GestionPedidos);
        tableView->setObjectName("tableView");
        tableView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        gridLayout->addWidget(tableView, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pushButtonNuevo = new QPushButton(GestionPedidos);
        pushButtonNuevo->setObjectName("pushButtonNuevo");
        pushButtonNuevo->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonNuevo);

        pushButtonGestionar = new QPushButton(GestionPedidos);
        pushButtonGestionar->setObjectName("pushButtonGestionar");
        pushButtonGestionar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonGestionar);

        pushButtonAceptar = new QPushButton(GestionPedidos);
        pushButtonAceptar->setObjectName("pushButtonAceptar");

        horizontalLayout->addWidget(pushButtonAceptar);

        pushButtonBorrar = new QPushButton(GestionPedidos);
        pushButtonBorrar->setObjectName("pushButtonBorrar");
        pushButtonBorrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonBorrar);

        pushButtonImprimir = new QPushButton(GestionPedidos);
        pushButtonImprimir->setObjectName("pushButtonImprimir");
        pushButtonImprimir->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonImprimir);

        pushButton = new QPushButton(GestionPedidos);
        pushButton->setObjectName("pushButton");
        pushButton->setAutoDefault(false);

        horizontalLayout->addWidget(pushButton);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(GestionPedidos);
        QObject::connect(pushButton, &QPushButton::clicked, GestionPedidos, qOverload<>(&QDialog::close));

        QMetaObject::connectSlotsByName(GestionPedidos);
    } // setupUi

    void retranslateUi(QDialog *GestionPedidos)
    {
        GestionPedidos->setWindowTitle(QCoreApplication::translate("GestionPedidos", "Gesti\303\263n de pedidos", nullptr));
        pushButtonNuevo->setText(QCoreApplication::translate("GestionPedidos", "Nuevo", nullptr));
        pushButtonGestionar->setText(QCoreApplication::translate("GestionPedidos", "Gestionar", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("GestionPedidos", "Aceptar", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("GestionPedidos", "Borrar", nullptr));
        pushButtonImprimir->setText(QCoreApplication::translate("GestionPedidos", "Imprimir", nullptr));
        pushButton->setText(QCoreApplication::translate("GestionPedidos", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GestionPedidos: public Ui_GestionPedidos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GESTIONPEDIDOS_H
