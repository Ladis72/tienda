/********************************************************************************
** Form generated from reading UI file 'verfacturas.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VERFACTURAS_H
#define UI_VERFACTURAS_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
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
            VerFacturas->setObjectName(QString::fromUtf8("VerFacturas"));
        VerFacturas->resize(1018, 599);
        gridLayout = new QGridLayout(VerFacturas);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(VerFacturas);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        dateEditDesde = new QDateEdit(VerFacturas);
        dateEditDesde->setObjectName(QString::fromUtf8("dateEditDesde"));
        dateEditDesde->setMinimumDate(QDate(2014, 1, 1));
        dateEditDesde->setCalendarPopup(true);

        horizontalLayout->addWidget(dateEditDesde);

        label_2 = new QLabel(VerFacturas);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        dateEditHasta = new QDateEdit(VerFacturas);
        dateEditHasta->setObjectName(QString::fromUtf8("dateEditHasta"));
        dateEditHasta->setMinimumDate(QDate(2014, 1, 1));
        dateEditHasta->setCalendarPopup(true);

        horizontalLayout->addWidget(dateEditHasta);

        label_3 = new QLabel(VerFacturas);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        comboBoxProceedores = new QComboBox(VerFacturas);
        comboBoxProceedores->setObjectName(QString::fromUtf8("comboBoxProceedores"));
        comboBoxProceedores->setEnabled(false);

        horizontalLayout->addWidget(comboBoxProceedores);

        checkBoxTodosProveedores = new QCheckBox(VerFacturas);
        checkBoxTodosProveedores->setObjectName(QString::fromUtf8("checkBoxTodosProveedores"));
        checkBoxTodosProveedores->setChecked(true);

        horizontalLayout->addWidget(checkBoxTodosProveedores);

        pushButtonVer = new QPushButton(VerFacturas);
        pushButtonVer->setObjectName(QString::fromUtf8("pushButtonVer"));

        horizontalLayout->addWidget(pushButtonVer);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        tableView = new QTableView(VerFacturas);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        gridLayout->addWidget(tableView, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButtonVerFactura = new QPushButton(VerFacturas);
        pushButtonVerFactura->setObjectName(QString::fromUtf8("pushButtonVerFactura"));
        pushButtonVerFactura->setAutoDefault(false);

        horizontalLayout_2->addWidget(pushButtonVerFactura);

        pushButtonPagar = new QPushButton(VerFacturas);
        pushButtonPagar->setObjectName(QString::fromUtf8("pushButtonPagar"));

        horizontalLayout_2->addWidget(pushButtonPagar);


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        buttonBox = new QDialogButtonBox(VerFacturas);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 3, 0, 1, 1);


        retranslateUi(VerFacturas);
        QObject::connect(buttonBox, SIGNAL(accepted()), VerFacturas, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), VerFacturas, SLOT(reject()));

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
