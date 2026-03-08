/********************************************************************************
** Form generated from reading UI file 'verfacturas.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VERFACTURAS_H
#define UI_VERFACTURAS_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_VerFacturas
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDateEdit *dateEditDesde;
    QLabel *label_2;
    QDateEdit *dateEditHasta;
    QLabel *label_3;
    QComboBox *comboBoxProceedores;
    QCheckBox *checkBoxTodosProveedores;
    QPushButton *pushButtonVer;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButtonVerFactura;
    QPushButton *pushButtonPagar;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *VerFacturas)
    {
        if (VerFacturas->objectName().isEmpty())
            VerFacturas->setObjectName("VerFacturas");
        VerFacturas->resize(1018, 599);
        gridLayout = new QGridLayout(VerFacturas);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(VerFacturas);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        dateEditDesde = new QDateEdit(VerFacturas);
        dateEditDesde->setObjectName("dateEditDesde");
        dateEditDesde->setMinimumDate(QDate(2014, 1, 1));
        dateEditDesde->setCalendarPopup(true);

        horizontalLayout->addWidget(dateEditDesde);

        label_2 = new QLabel(VerFacturas);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        dateEditHasta = new QDateEdit(VerFacturas);
        dateEditHasta->setObjectName("dateEditHasta");
        dateEditHasta->setMinimumDate(QDate(2014, 1, 1));
        dateEditHasta->setCalendarPopup(true);

        horizontalLayout->addWidget(dateEditHasta);

        label_3 = new QLabel(VerFacturas);
        label_3->setObjectName("label_3");

        horizontalLayout->addWidget(label_3);

        comboBoxProceedores = new QComboBox(VerFacturas);
        comboBoxProceedores->setObjectName("comboBoxProceedores");
        comboBoxProceedores->setEnabled(false);

        horizontalLayout->addWidget(comboBoxProceedores);

        checkBoxTodosProveedores = new QCheckBox(VerFacturas);
        checkBoxTodosProveedores->setObjectName("checkBoxTodosProveedores");
        checkBoxTodosProveedores->setChecked(true);

        horizontalLayout->addWidget(checkBoxTodosProveedores);

        pushButtonVer = new QPushButton(VerFacturas);
        pushButtonVer->setObjectName("pushButtonVer");

        horizontalLayout->addWidget(pushButtonVer);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        tableView = new QTableView(VerFacturas);
        tableView->setObjectName("tableView");
        tableView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        gridLayout->addWidget(tableView, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        pushButtonVerFactura = new QPushButton(VerFacturas);
        pushButtonVerFactura->setObjectName("pushButtonVerFactura");
        pushButtonVerFactura->setAutoDefault(false);

        horizontalLayout_2->addWidget(pushButtonVerFactura);

        pushButtonPagar = new QPushButton(VerFacturas);
        pushButtonPagar->setObjectName("pushButtonPagar");

        horizontalLayout_2->addWidget(pushButtonPagar);


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        buttonBox = new QDialogButtonBox(VerFacturas);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 3, 0, 1, 1);


        retranslateUi(VerFacturas);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, VerFacturas, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, VerFacturas, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(VerFacturas);
    } // setupUi

    void retranslateUi(QDialog *VerFacturas)
    {
        VerFacturas->setWindowTitle(QCoreApplication::translate("VerFacturas", "Listado de facturas", nullptr));
        label->setText(QCoreApplication::translate("VerFacturas", "Desde", nullptr));
        dateEditDesde->setDisplayFormat(QCoreApplication::translate("VerFacturas", "yyyy-MM-dd", nullptr));
        label_2->setText(QCoreApplication::translate("VerFacturas", "Hasta", nullptr));
        dateEditHasta->setDisplayFormat(QCoreApplication::translate("VerFacturas", "yyyy-MM-dd", nullptr));
        label_3->setText(QCoreApplication::translate("VerFacturas", "Proveedor", nullptr));
        checkBoxTodosProveedores->setText(QCoreApplication::translate("VerFacturas", "Todos", nullptr));
        pushButtonVer->setText(QCoreApplication::translate("VerFacturas", "Ver", nullptr));
        pushButtonVerFactura->setText(QCoreApplication::translate("VerFacturas", "Ver factura", nullptr));
        pushButtonPagar->setText(QCoreApplication::translate("VerFacturas", "Marcar como pagada", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VerFacturas: public Ui_VerFacturas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VERFACTURAS_H
