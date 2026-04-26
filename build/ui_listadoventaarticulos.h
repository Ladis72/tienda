/********************************************************************************
** Form generated from reading UI file 'listadoventaarticulos.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTADOVENTAARTICULOS_H
#define UI_LISTADOVENTAARTICULOS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_ListadoVentaArticulos
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDateTimeEdit *dateTimeEditDesde;
    QLabel *label_2;
    QDateTimeEdit *dateTimeEditHasta;
    QPushButton *pushButtonVer;
    QTableView *tableView;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ListadoVentaArticulos)
    {
        if (ListadoVentaArticulos->objectName().isEmpty())
            ListadoVentaArticulos->setObjectName("ListadoVentaArticulos");
        ListadoVentaArticulos->resize(867, 524);
        gridLayout = new QGridLayout(ListadoVentaArticulos);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(ListadoVentaArticulos);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label);

        dateTimeEditDesde = new QDateTimeEdit(ListadoVentaArticulos);
        dateTimeEditDesde->setObjectName("dateTimeEditDesde");
        dateTimeEditDesde->setCalendarPopup(true);

        horizontalLayout->addWidget(dateTimeEditDesde);

        label_2 = new QLabel(ListadoVentaArticulos);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label_2);

        dateTimeEditHasta = new QDateTimeEdit(ListadoVentaArticulos);
        dateTimeEditHasta->setObjectName("dateTimeEditHasta");
        dateTimeEditHasta->setCalendarPopup(true);

        horizontalLayout->addWidget(dateTimeEditHasta);

        pushButtonVer = new QPushButton(ListadoVentaArticulos);
        pushButtonVer->setObjectName("pushButtonVer");

        horizontalLayout->addWidget(pushButtonVer);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        tableView = new QTableView(ListadoVentaArticulos);
        tableView->setObjectName("tableView");
        tableView->setSortingEnabled(true);

        gridLayout->addWidget(tableView, 1, 0, 1, 1);

        buttonBox = new QDialogButtonBox(ListadoVentaArticulos);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 2, 0, 1, 1);


        retranslateUi(ListadoVentaArticulos);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, ListadoVentaArticulos, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, ListadoVentaArticulos, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(ListadoVentaArticulos);
    } // setupUi

    void retranslateUi(QDialog *ListadoVentaArticulos)
    {
        ListadoVentaArticulos->setWindowTitle(QCoreApplication::translate("ListadoVentaArticulos", "Venta art\303\255culos entre fechas", nullptr));
        label->setText(QCoreApplication::translate("ListadoVentaArticulos", "Desde", nullptr));
        dateTimeEditDesde->setDisplayFormat(QCoreApplication::translate("ListadoVentaArticulos", "yyyy-MM-dd H:mm", nullptr));
        label_2->setText(QCoreApplication::translate("ListadoVentaArticulos", "Hasta", nullptr));
        dateTimeEditHasta->setDisplayFormat(QCoreApplication::translate("ListadoVentaArticulos", "yyyy-MM-dd H:mm", nullptr));
        pushButtonVer->setText(QCoreApplication::translate("ListadoVentaArticulos", "Ver", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ListadoVentaArticulos: public Ui_ListadoVentaArticulos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTADOVENTAARTICULOS_H
