/********************************************************************************
** Form generated from reading UI file 'fabricantes.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FABRICANTES_H
#define UI_FABRICANTES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_Fabricantes
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QPushButton *pushButtonDeshacer;
    QPushButton *pushButtonBorrar;
    QTableView *tableView;
    QPushButton *pushButtonAnadir;
    QPushButton *pushButtonModificar;
    QLineEdit *lineEdit;
    QLabel *label;

    void setupUi(QDialog *Fabricantes)
    {
        if (Fabricantes->objectName().isEmpty())
            Fabricantes->setObjectName("Fabricantes");
        Fabricantes->resize(562, 540);
        gridLayout = new QGridLayout(Fabricantes);
        gridLayout->setObjectName("gridLayout");
        buttonBox = new QDialogButtonBox(Fabricantes);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 2, 5, 1, 1);

        pushButtonDeshacer = new QPushButton(Fabricantes);
        pushButtonDeshacer->setObjectName("pushButtonDeshacer");

        gridLayout->addWidget(pushButtonDeshacer, 2, 2, 1, 1);

        pushButtonBorrar = new QPushButton(Fabricantes);
        pushButtonBorrar->setObjectName("pushButtonBorrar");

        gridLayout->addWidget(pushButtonBorrar, 2, 4, 1, 1);

        tableView = new QTableView(Fabricantes);
        tableView->setObjectName("tableView");
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableView->setTextElideMode(Qt::TextElideMode::ElideRight);
        tableView->setSortingEnabled(true);
        tableView->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
        tableView->verticalHeader()->setVisible(false);

        gridLayout->addWidget(tableView, 0, 0, 1, 6);

        pushButtonAnadir = new QPushButton(Fabricantes);
        pushButtonAnadir->setObjectName("pushButtonAnadir");

        gridLayout->addWidget(pushButtonAnadir, 2, 0, 1, 1);

        pushButtonModificar = new QPushButton(Fabricantes);
        pushButtonModificar->setObjectName("pushButtonModificar");

        gridLayout->addWidget(pushButtonModificar, 2, 1, 1, 1);

        lineEdit = new QLineEdit(Fabricantes);
        lineEdit->setObjectName("lineEdit");

        gridLayout->addWidget(lineEdit, 1, 0, 1, 5);

        label = new QLabel(Fabricantes);
        label->setObjectName("label");

        gridLayout->addWidget(label, 1, 5, 1, 1);

        QWidget::setTabOrder(lineEdit, pushButtonAnadir);
        QWidget::setTabOrder(pushButtonAnadir, pushButtonModificar);
        QWidget::setTabOrder(pushButtonModificar, pushButtonDeshacer);
        QWidget::setTabOrder(pushButtonDeshacer, pushButtonBorrar);
        QWidget::setTabOrder(pushButtonBorrar, tableView);

        retranslateUi(Fabricantes);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, Fabricantes, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, Fabricantes, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(Fabricantes);
    } // setupUi

    void retranslateUi(QDialog *Fabricantes)
    {
        Fabricantes->setWindowTitle(QCoreApplication::translate("Fabricantes", "Familias", nullptr));
        pushButtonDeshacer->setText(QCoreApplication::translate("Fabricantes", "Deshacer", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("Fabricantes", "Borrar", nullptr));
        pushButtonAnadir->setText(QCoreApplication::translate("Fabricantes", "A\303\261adir", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("Fabricantes", "Modificar", nullptr));
        label->setText(QCoreApplication::translate("Fabricantes", "<-- Filtro", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Fabricantes: public Ui_Fabricantes {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FABRICANTES_H
