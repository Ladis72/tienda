/********************************************************************************
** Form generated from reading UI file 'formaspago.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMASPAGO_H
#define UI_FORMASPAGO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_FormasPago
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonAnadir;
    QPushButton *pushButtonModificar;
    QPushButton *pushButtonUnificar;
    QPushButton *pushButtonSaneador;
    QTableView *tableView;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *FormasPago)
    {
        if (FormasPago->objectName().isEmpty())
            FormasPago->setObjectName("FormasPago");
        FormasPago->resize(396, 572);
        gridLayout_2 = new QGridLayout(FormasPago);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        lineEdit = new QLineEdit(FormasPago);
        lineEdit->setObjectName("lineEdit");

        gridLayout->addWidget(lineEdit, 1, 0, 1, 1);

        pushButtonBorrar = new QPushButton(FormasPago);
        pushButtonBorrar->setObjectName("pushButtonBorrar");

        gridLayout->addWidget(pushButtonBorrar, 3, 0, 1, 1);

        pushButtonAnadir = new QPushButton(FormasPago);
        pushButtonAnadir->setObjectName("pushButtonAnadir");

        gridLayout->addWidget(pushButtonAnadir, 2, 0, 1, 1);

        pushButtonModificar = new QPushButton(FormasPago);
        pushButtonModificar->setObjectName("pushButtonModificar");

        gridLayout->addWidget(pushButtonModificar, 4, 0, 1, 1);

        pushButtonUnificar = new QPushButton(FormasPago);
        pushButtonUnificar->setObjectName("pushButtonUnificar");

        gridLayout->addWidget(pushButtonUnificar, 5, 0, 1, 1);

        pushButtonSaneador = new QPushButton(FormasPago);
        pushButtonSaneador->setObjectName("pushButtonSaneador");

        gridLayout->addWidget(pushButtonSaneador, 6, 0, 1, 1);

        tableView = new QTableView(FormasPago);
        tableView->setObjectName("tableView");
        tableView->verticalHeader()->setVisible(false);

        gridLayout->addWidget(tableView, 0, 0, 1, 2);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(FormasPago);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout_2->addWidget(buttonBox, 1, 0, 1, 1);

        QWidget::setTabOrder(lineEdit, pushButtonAnadir);
        QWidget::setTabOrder(pushButtonAnadir, pushButtonBorrar);
        QWidget::setTabOrder(pushButtonBorrar, pushButtonModificar);
        QWidget::setTabOrder(pushButtonModificar, tableView);

        retranslateUi(FormasPago);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, FormasPago, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, FormasPago, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(FormasPago);
    } // setupUi

    void retranslateUi(QDialog *FormasPago)
    {
        FormasPago->setWindowTitle(QCoreApplication::translate("FormasPago", "Formas de Pago", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("FormasPago", "Borrar", nullptr));
        pushButtonAnadir->setText(QCoreApplication::translate("FormasPago", "A\303\261adir", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("FormasPago", "Modificar", nullptr));
        pushButtonUnificar->setText(QCoreApplication::translate("FormasPago", "Unificar y Limpiar", nullptr));
        pushButtonSaneador->setText(QCoreApplication::translate("FormasPago", "Saneador Global", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormasPago: public Ui_FormasPago {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMASPAGO_H
