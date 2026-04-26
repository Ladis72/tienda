/********************************************************************************
** Form generated from reading UI file 'gestorencargosdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GESTORENCARGOSDIALOG_H
#define UI_GESTORENCARGOSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GestorEncargosDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *labelFiltroEstado;
    QComboBox *comboBoxEstado;
    QLabel *labelFiltroCliente;
    QLineEdit *lineEditFiltroCliente;
    QSpacerItem *horizontalSpacer;
    QTableView *tableViewEncargos;
    QHBoxLayout *horizontalLayoutBtn;
    QPushButton *btnNuevoEncargo;
    QPushButton *btnMarcarRecibido;
    QPushButton *btnMarcarEntregado;
    QPushButton *btnCobrarTPV;
    QPushButton *btnBorrar;
    QSpacerItem *horizontalSpacerBtn;
    QPushButton *btnCerrar;

    void setupUi(QDialog *GestorEncargosDialog)
    {
        if (GestorEncargosDialog->objectName().isEmpty())
            GestorEncargosDialog->setObjectName(QString::fromUtf8("GestorEncargosDialog"));
        GestorEncargosDialog->resize(1262, 600);
        verticalLayout = new QVBoxLayout(GestorEncargosDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        labelFiltroEstado = new QLabel(GestorEncargosDialog);
        labelFiltroEstado->setObjectName(QString::fromUtf8("labelFiltroEstado"));

        horizontalLayout->addWidget(labelFiltroEstado);

        comboBoxEstado = new QComboBox(GestorEncargosDialog);
        comboBoxEstado->addItem(QString());
        comboBoxEstado->addItem(QString());
        comboBoxEstado->addItem(QString());
        comboBoxEstado->addItem(QString());
        comboBoxEstado->setObjectName(QString::fromUtf8("comboBoxEstado"));

        horizontalLayout->addWidget(comboBoxEstado);

        labelFiltroCliente = new QLabel(GestorEncargosDialog);
        labelFiltroCliente->setObjectName(QString::fromUtf8("labelFiltroCliente"));

        horizontalLayout->addWidget(labelFiltroCliente);

        lineEditFiltroCliente = new QLineEdit(GestorEncargosDialog);
        lineEditFiltroCliente->setObjectName(QString::fromUtf8("lineEditFiltroCliente"));

        horizontalLayout->addWidget(lineEditFiltroCliente);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        tableViewEncargos = new QTableView(GestorEncargosDialog);
        tableViewEncargos->setObjectName(QString::fromUtf8("tableViewEncargos"));
        tableViewEncargos->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableViewEncargos->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableViewEncargos->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayout->addWidget(tableViewEncargos);

        horizontalLayoutBtn = new QHBoxLayout();
        horizontalLayoutBtn->setObjectName(QString::fromUtf8("horizontalLayoutBtn"));
        btnNuevoEncargo = new QPushButton(GestorEncargosDialog);
        btnNuevoEncargo->setObjectName(QString::fromUtf8("btnNuevoEncargo"));

        horizontalLayoutBtn->addWidget(btnNuevoEncargo);

        btnMarcarRecibido = new QPushButton(GestorEncargosDialog);
        btnMarcarRecibido->setObjectName(QString::fromUtf8("btnMarcarRecibido"));

        horizontalLayoutBtn->addWidget(btnMarcarRecibido);

        btnMarcarEntregado = new QPushButton(GestorEncargosDialog);
        btnMarcarEntregado->setObjectName(QString::fromUtf8("btnMarcarEntregado"));

        horizontalLayoutBtn->addWidget(btnMarcarEntregado);

        btnCobrarTPV = new QPushButton(GestorEncargosDialog);
        btnCobrarTPV->setObjectName(QString::fromUtf8("btnCobrarTPV"));
        btnCobrarTPV->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 170, 0);\n"
"color: rgb(255, 255, 255);\n"
"font: 75 10pt \"MS Shell Dlg 2\";"));

        horizontalLayoutBtn->addWidget(btnCobrarTPV);

        btnBorrar = new QPushButton(GestorEncargosDialog);
        btnBorrar->setObjectName(QString::fromUtf8("btnBorrar"));

        horizontalLayoutBtn->addWidget(btnBorrar);

        horizontalSpacerBtn = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutBtn->addItem(horizontalSpacerBtn);

        btnCerrar = new QPushButton(GestorEncargosDialog);
        btnCerrar->setObjectName(QString::fromUtf8("btnCerrar"));

        horizontalLayoutBtn->addWidget(btnCerrar);


        verticalLayout->addLayout(horizontalLayoutBtn);


        retranslateUi(GestorEncargosDialog);
        QObject::connect(btnCerrar, SIGNAL(clicked()), GestorEncargosDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(GestorEncargosDialog);
    } // setupUi

    void retranslateUi(QDialog *GestorEncargosDialog)
    {
        GestorEncargosDialog->setWindowTitle(QCoreApplication::translate("GestorEncargosDialog", "Gestor de Encargos", nullptr));
        labelFiltroEstado->setText(QCoreApplication::translate("GestorEncargosDialog", "Estado:", nullptr));
        comboBoxEstado->setItemText(0, QCoreApplication::translate("GestorEncargosDialog", "Todos", nullptr));
        comboBoxEstado->setItemText(1, QCoreApplication::translate("GestorEncargosDialog", "Pendiente", nullptr));
        comboBoxEstado->setItemText(2, QCoreApplication::translate("GestorEncargosDialog", "Recibido", nullptr));
        comboBoxEstado->setItemText(3, QCoreApplication::translate("GestorEncargosDialog", "Entregado", nullptr));

        labelFiltroCliente->setText(QCoreApplication::translate("GestorEncargosDialog", "Cliente (ID/Nombre):", nullptr));
        btnNuevoEncargo->setText(QCoreApplication::translate("GestorEncargosDialog", "Nuevo Encargo", nullptr));
        btnMarcarRecibido->setText(QCoreApplication::translate("GestorEncargosDialog", "Marcar como Recibido", nullptr));
        btnMarcarEntregado->setText(QCoreApplication::translate("GestorEncargosDialog", "Marcar como Entregado", nullptr));
        btnCobrarTPV->setText(QCoreApplication::translate("GestorEncargosDialog", "Cobrar en TPV", nullptr));
        btnBorrar->setText(QCoreApplication::translate("GestorEncargosDialog", "Borrar Encargo", nullptr));
        btnCerrar->setText(QCoreApplication::translate("GestorEncargosDialog", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GestorEncargosDialog: public Ui_GestorEncargosDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GESTORENCARGOSDIALOG_H
