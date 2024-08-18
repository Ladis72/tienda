/********************************************************************************
** Form generated from reading UI file 'stock.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STOCK_H
#define UI_STOCK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_Stock
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonActualizar;
    QPushButton *pushButton;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *Stock)
    {
        if (Stock->objectName().isEmpty())
            Stock->setObjectName(QString::fromUtf8("Stock"));
        Stock->resize(400, 300);
        gridLayout = new QGridLayout(Stock);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(Stock);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        tableView = new QTableView(Stock);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout->addWidget(tableView, 1, 0, 1, 2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonActualizar = new QPushButton(Stock);
        pushButtonActualizar->setObjectName(QString::fromUtf8("pushButtonActualizar"));
        pushButtonActualizar->setEnabled(false);

        horizontalLayout->addWidget(pushButtonActualizar);

        pushButton = new QPushButton(Stock);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setEnabled(false);

        horizontalLayout->addWidget(pushButton);

        pushButtonBorrar = new QPushButton(Stock);
        pushButtonBorrar->setObjectName(QString::fromUtf8("pushButtonBorrar"));
        pushButtonBorrar->setEnabled(false);

        horizontalLayout->addWidget(pushButtonBorrar);

        pushButtonCerrar = new QPushButton(Stock);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));
        pushButtonCerrar->setEnabled(true);

        horizontalLayout->addWidget(pushButtonCerrar);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 2);


        retranslateUi(Stock);
        QObject::connect(pushButtonCerrar, SIGNAL(clicked()), Stock, SLOT(accept()));

        QMetaObject::connectSlotsByName(Stock);
    } // setupUi

    void retranslateUi(QDialog *Stock)
    {
        Stock->setWindowTitle(QCoreApplication::translate("Stock", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("Stock", "TextLabel", nullptr));
        pushButtonActualizar->setText(QCoreApplication::translate("Stock", "Actualizar", nullptr));
        pushButton->setText(QCoreApplication::translate("Stock", "A\303\261adir", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("Stock", "Borrar", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("Stock", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Stock: public Ui_Stock {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STOCK_H
