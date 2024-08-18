/********************************************************************************
** Form generated from reading UI file 'visorfacturas.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VISORFACTURAS_H
#define UI_VISORFACTURAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_VisorFacturas
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QTableView *tableView;

    void setupUi(QDialog *VisorFacturas)
    {
        if (VisorFacturas->objectName().isEmpty())
            VisorFacturas->setObjectName(QString::fromUtf8("VisorFacturas"));
        VisorFacturas->resize(1024, 627);
        gridLayout = new QGridLayout(VisorFacturas);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        buttonBox = new QDialogButtonBox(VisorFacturas);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 1, 1, 1, 1);

        tableView = new QTableView(VisorFacturas);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout->addWidget(tableView, 0, 0, 1, 2);


        retranslateUi(VisorFacturas);
        QObject::connect(buttonBox, SIGNAL(accepted()), VisorFacturas, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), VisorFacturas, SLOT(reject()));

        QMetaObject::connectSlotsByName(VisorFacturas);
    } // setupUi

    void retranslateUi(QDialog *VisorFacturas)
    {
        VisorFacturas->setWindowTitle(QCoreApplication::translate("VisorFacturas", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VisorFacturas: public Ui_VisorFacturas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISORFACTURAS_H
