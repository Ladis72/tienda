/********************************************************************************
** Form generated from reading UI file 'buscarproducto.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUSCARPRODUCTO_H
#define UI_BUSCARPRODUCTO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_BuscarProducto
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *tableView;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *BuscarProducto)
    {
        if (BuscarProducto->objectName().isEmpty())
            BuscarProducto->setObjectName("BuscarProducto");
        BuscarProducto->resize(1024, 600);
        verticalLayout = new QVBoxLayout(BuscarProducto);
        verticalLayout->setObjectName("verticalLayout");
        tableView = new QTableView(BuscarProducto);
        tableView->setObjectName("tableView");
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayout->addWidget(tableView);

        buttonBox = new QDialogButtonBox(BuscarProducto);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(BuscarProducto);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, BuscarProducto, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, BuscarProducto, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(BuscarProducto);
    } // setupUi

    void retranslateUi(QDialog *BuscarProducto)
    {
        BuscarProducto->setWindowTitle(QCoreApplication::translate("BuscarProducto", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BuscarProducto: public Ui_BuscarProducto {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUSCARPRODUCTO_H
