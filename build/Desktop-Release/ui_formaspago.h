/********************************************************************************
** Form generated from reading UI file 'formaspago.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMASPAGO_H
#define UI_FORMASPAGO_H

#include <QtCore/QVariant>
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
    QTableView *tableView;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *FormasPago)
    {
        if (FormasPago->objectName().isEmpty())
            FormasPago->setObjectName(QString::fromUtf8("FormasPago"));
        FormasPago->resize(396, 572);
        gridLayout_2 = new QGridLayout(FormasPago);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineEdit = new QLineEdit(FormasPago);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 1, 0, 1, 1);

        pushButtonBorrar = new QPushButton(FormasPago);
        pushButtonBorrar->setObjectName(QString::fromUtf8("pushButtonBorrar"));

        gridLayout->addWidget(pushButtonBorrar, 3, 0, 1, 1);

        pushButtonAnadir = new QPushButton(FormasPago);
        pushButtonAnadir->setObjectName(QString::fromUtf8("pushButtonAnadir"));

        gridLayout->addWidget(pushButtonAnadir, 2, 0, 1, 1);

        pushButtonModificar = new QPushButton(FormasPago);
        pushButtonModificar->setObjectName(QString::fromUtf8("pushButtonModificar"));

        gridLayout->addWidget(pushButtonModificar, 4, 0, 1, 1);

        tableView = new QTableView(FormasPago);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->verticalHeader()->setVisible(false);

        gridLayout->addWidget(tableView, 0, 0, 1, 2);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(FormasPago);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout_2->addWidget(buttonBox, 1, 0, 1, 1);

        QWidget::setTabOrder(lineEdit, pushButtonAnadir);
        QWidget::setTabOrder(pushButtonAnadir, pushButtonBorrar);
        QWidget::setTabOrder(pushButtonBorrar, pushButtonModificar);
        QWidget::setTabOrder(pushButtonModificar, tableView);

        retranslateUi(FormasPago);
        QObject::connect(buttonBox, SIGNAL(accepted()), FormasPago, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), FormasPago, SLOT(reject()));

        QMetaObject::connectSlotsByName(FormasPago);
    } // setupUi

    void retranslateUi(QDialog *FormasPago)
    {
        FormasPago->setWindowTitle(QCoreApplication::translate("FormasPago", "Dialog", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("FormasPago", "Borrar", nullptr));
        pushButtonAnadir->setText(QCoreApplication::translate("FormasPago", "A\303\261adir", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("FormasPago", "Modificar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormasPago: public Ui_FormasPago {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMASPAGO_H
