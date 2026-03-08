/********************************************************************************
** Form generated from reading UI file 'listadoventas.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTADOVENTAS_H
#define UI_LISTADOVENTAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ListadoVentas
{
public:
    QGridLayout *gridLayout;
    QDateEdit *dateEditHasta;
    QLabel *labelTotal;
    QLabel *label_2;
    QPushButton *pushButtonImprimir;
    QPushButton *pushButtonVer;
    QDateEdit *dateEditDesde;
    QLabel *labelTotalB;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBoxFPago;
    QGridLayout *gridLayout_2;
    QTableView *tableViewFPago;
    QGroupBox *groupBoxVendedores;
    QGridLayout *gridLayout_3;
    QTableView *tableViewVendedores;
    QDialogButtonBox *buttonBox;
    QLabel *labelTotal2;
    QTableView *tableView;

    void setupUi(QDialog *ListadoVentas)
    {
        if (ListadoVentas->objectName().isEmpty())
            ListadoVentas->setObjectName("ListadoVentas");
        ListadoVentas->resize(904, 581);
        gridLayout = new QGridLayout(ListadoVentas);
        gridLayout->setObjectName("gridLayout");
        dateEditHasta = new QDateEdit(ListadoVentas);
        dateEditHasta->setObjectName("dateEditHasta");
        dateEditHasta->setCalendarPopup(true);

        gridLayout->addWidget(dateEditHasta, 0, 3, 1, 1);

        labelTotal = new QLabel(ListadoVentas);
        labelTotal->setObjectName("labelTotal");
        QFont font;
        font.setPointSize(12);
        labelTotal->setFont(font);

        gridLayout->addWidget(labelTotal, 2, 1, 1, 2);

        label_2 = new QLabel(ListadoVentas);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        pushButtonImprimir = new QPushButton(ListadoVentas);
        pushButtonImprimir->setObjectName("pushButtonImprimir");

        gridLayout->addWidget(pushButtonImprimir, 2, 4, 1, 1);

        pushButtonVer = new QPushButton(ListadoVentas);
        pushButtonVer->setObjectName("pushButtonVer");

        gridLayout->addWidget(pushButtonVer, 0, 4, 1, 1);

        dateEditDesde = new QDateEdit(ListadoVentas);
        dateEditDesde->setObjectName("dateEditDesde");
        dateEditDesde->setCalendarPopup(true);

        gridLayout->addWidget(dateEditDesde, 0, 1, 1, 1);

        labelTotalB = new QLabel(ListadoVentas);
        labelTotalB->setObjectName("labelTotalB");
        labelTotalB->setFont(font);

        gridLayout->addWidget(labelTotalB, 2, 3, 1, 1);

        label = new QLabel(ListadoVentas);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        groupBoxFPago = new QGroupBox(ListadoVentas);
        groupBoxFPago->setObjectName("groupBoxFPago");
        gridLayout_2 = new QGridLayout(groupBoxFPago);
        gridLayout_2->setObjectName("gridLayout_2");
        tableViewFPago = new QTableView(groupBoxFPago);
        tableViewFPago->setObjectName("tableViewFPago");
        tableViewFPago->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableViewFPago->setAlternatingRowColors(true);
        tableViewFPago->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableViewFPago->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableViewFPago->verticalHeader()->setVisible(false);

        gridLayout_2->addWidget(tableViewFPago, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBoxFPago);

        groupBoxVendedores = new QGroupBox(ListadoVentas);
        groupBoxVendedores->setObjectName("groupBoxVendedores");
        gridLayout_3 = new QGridLayout(groupBoxVendedores);
        gridLayout_3->setObjectName("gridLayout_3");
        tableViewVendedores = new QTableView(groupBoxVendedores);
        tableViewVendedores->setObjectName("tableViewVendedores");
        tableViewVendedores->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableViewVendedores->setAlternatingRowColors(true);
        tableViewVendedores->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableViewVendedores->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableViewVendedores->verticalHeader()->setVisible(false);

        gridLayout_3->addWidget(tableViewVendedores, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBoxVendedores);


        gridLayout->addLayout(verticalLayout, 1, 4, 1, 2);

        buttonBox = new QDialogButtonBox(ListadoVentas);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 2, 5, 1, 1);

        labelTotal2 = new QLabel(ListadoVentas);
        labelTotal2->setObjectName("labelTotal2");
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        labelTotal2->setFont(font1);
        labelTotal2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(labelTotal2, 3, 1, 1, 3);

        tableView = new QTableView(ListadoVentas);
        tableView->setObjectName("tableView");
        tableView->setMinimumSize(QSize(350, 0));
        tableView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableView->verticalHeader()->setVisible(false);

        gridLayout->addWidget(tableView, 1, 0, 1, 4);


        retranslateUi(ListadoVentas);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, ListadoVentas, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, ListadoVentas, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(ListadoVentas);
    } // setupUi

    void retranslateUi(QDialog *ListadoVentas)
    {
        ListadoVentas->setWindowTitle(QCoreApplication::translate("ListadoVentas", "Listado de ventas", nullptr));
        dateEditHasta->setDisplayFormat(QCoreApplication::translate("ListadoVentas", "yyyy-MM-dd", nullptr));
        labelTotal->setText(QString());
        label_2->setText(QCoreApplication::translate("ListadoVentas", "Hasta", nullptr));
        pushButtonImprimir->setText(QCoreApplication::translate("ListadoVentas", "Imprimir", nullptr));
        pushButtonVer->setText(QCoreApplication::translate("ListadoVentas", "Ver", nullptr));
        dateEditDesde->setDisplayFormat(QCoreApplication::translate("ListadoVentas", "yyyy-MM-dd", nullptr));
        labelTotalB->setText(QString());
        label->setText(QCoreApplication::translate("ListadoVentas", "Desde", nullptr));
        groupBoxFPago->setTitle(QCoreApplication::translate("ListadoVentas", "Forma de pago", nullptr));
        groupBoxVendedores->setTitle(QCoreApplication::translate("ListadoVentas", "Vendedores", nullptr));
        labelTotal2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ListadoVentas: public Ui_ListadoVentas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTADOVENTAS_H
