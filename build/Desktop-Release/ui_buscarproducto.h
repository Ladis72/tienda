/********************************************************************************
** Form generated from reading UI file 'buscarproducto.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUSCARPRODUCTO_H
#define UI_BUSCARPRODUCTO_H

#include <QtCore/QVariant>
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
            BuscarProducto->setObjectName(QString::fromUtf8("BuscarProducto"));
        BuscarProducto->resize(1024, 600);
        verticalLayout = new QVBoxLayout(BuscarProducto);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableView = new QTableView(BuscarProducto);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayout->addWidget(tableView);

        buttonBox = new QDialogButtonBox(BuscarProducto);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(BuscarProducto);
        QObject::connect(buttonBox, SIGNAL(accepted()), BuscarProducto, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), BuscarProducto, SLOT(reject()));

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
