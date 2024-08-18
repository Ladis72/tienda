/********************************************************************************
** Form generated from reading UI file 'familias.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FAMILIAS_H
#define UI_FAMILIAS_H

#include <QtCore/QVariant>
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

class Ui_Familias
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QPushButton *pushButtonAnadir;
    QPushButton *pushButtonModificar;
    QPushButton *pushButtonDeshacer;
    QTableView *tableView;
    QPushButton *pushButtonBorrar;
    QLineEdit *lineEdit;
    QLabel *label;

    void setupUi(QDialog *Familias)
    {
        if (Familias->objectName().isEmpty())
            Familias->setObjectName(QString::fromUtf8("Familias"));
        Familias->resize(560, 540);
        gridLayout = new QGridLayout(Familias);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        buttonBox = new QDialogButtonBox(Familias);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 2, 5, 1, 1);

        pushButtonAnadir = new QPushButton(Familias);
        pushButtonAnadir->setObjectName(QString::fromUtf8("pushButtonAnadir"));

        gridLayout->addWidget(pushButtonAnadir, 2, 0, 1, 1);

        pushButtonModificar = new QPushButton(Familias);
        pushButtonModificar->setObjectName(QString::fromUtf8("pushButtonModificar"));

        gridLayout->addWidget(pushButtonModificar, 2, 1, 1, 1);

        pushButtonDeshacer = new QPushButton(Familias);
        pushButtonDeshacer->setObjectName(QString::fromUtf8("pushButtonDeshacer"));

        gridLayout->addWidget(pushButtonDeshacer, 2, 2, 1, 1);

        tableView = new QTableView(Familias);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableView->setTextElideMode(Qt::TextElideMode::ElideRight);
        tableView->setSortingEnabled(true);
        tableView->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
        tableView->verticalHeader()->setVisible(false);

        gridLayout->addWidget(tableView, 0, 0, 1, 6);

        pushButtonBorrar = new QPushButton(Familias);
        pushButtonBorrar->setObjectName(QString::fromUtf8("pushButtonBorrar"));

        gridLayout->addWidget(pushButtonBorrar, 2, 4, 1, 1);

        lineEdit = new QLineEdit(Familias);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 1, 0, 1, 5);

        label = new QLabel(Familias);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 5, 1, 1);

        QWidget::setTabOrder(lineEdit, pushButtonAnadir);
        QWidget::setTabOrder(pushButtonAnadir, pushButtonModificar);
        QWidget::setTabOrder(pushButtonModificar, pushButtonDeshacer);
        QWidget::setTabOrder(pushButtonDeshacer, pushButtonBorrar);
        QWidget::setTabOrder(pushButtonBorrar, tableView);

        retranslateUi(Familias);
        QObject::connect(buttonBox, SIGNAL(accepted()), Familias, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Familias, SLOT(reject()));

        QMetaObject::connectSlotsByName(Familias);
    } // setupUi

    void retranslateUi(QDialog *Familias)
    {
        Familias->setWindowTitle(QCoreApplication::translate("Familias", "Familias", nullptr));
        pushButtonAnadir->setText(QCoreApplication::translate("Familias", "A\303\261adir", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("Familias", "Modificar", nullptr));
        pushButtonDeshacer->setText(QCoreApplication::translate("Familias", "Deshacer", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("Familias", "Borrar", nullptr));
        label->setText(QCoreApplication::translate("Familias", "<-- Filtro", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Familias: public Ui_Familias {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FAMILIAS_H
