/********************************************************************************
** Form generated from reading UI file 'formatos.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMATOS_H
#define UI_FORMATOS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_Formatos
{
public:
    QGridLayout *gridLayout;
    QTableView *tableView;
    QLineEdit *lineEditFiltro;
    QLabel *label;
    QPushButton *pushButtonModificar;
    QPushButton *pushButtonnadir;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *Formatos)
    {
        if (Formatos->objectName().isEmpty())
            Formatos->setObjectName(QString::fromUtf8("Formatos"));
        Formatos->resize(359, 381);
        gridLayout = new QGridLayout(Formatos);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableView = new QTableView(Formatos);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout->addWidget(tableView, 0, 0, 1, 4);

        lineEditFiltro = new QLineEdit(Formatos);
        lineEditFiltro->setObjectName(QString::fromUtf8("lineEditFiltro"));

        gridLayout->addWidget(lineEditFiltro, 1, 0, 1, 2);

        label = new QLabel(Formatos);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 2, 1, 1);

        pushButtonModificar = new QPushButton(Formatos);
        pushButtonModificar->setObjectName(QString::fromUtf8("pushButtonModificar"));
        pushButtonModificar->setEnabled(false);

        gridLayout->addWidget(pushButtonModificar, 2, 0, 1, 1);

        pushButtonnadir = new QPushButton(Formatos);
        pushButtonnadir->setObjectName(QString::fromUtf8("pushButtonnadir"));
        pushButtonnadir->setEnabled(false);

        gridLayout->addWidget(pushButtonnadir, 2, 1, 1, 2);

        pushButtonCerrar = new QPushButton(Formatos);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));

        gridLayout->addWidget(pushButtonCerrar, 2, 3, 1, 1);


        retranslateUi(Formatos);
        QObject::connect(pushButtonCerrar, SIGNAL(clicked()), Formatos, SLOT(reject()));

        QMetaObject::connectSlotsByName(Formatos);
    } // setupUi

    void retranslateUi(QDialog *Formatos)
    {
        Formatos->setWindowTitle(QCoreApplication::translate("Formatos", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("Formatos", "<---Filtro", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("Formatos", "Modificar", nullptr));
        pushButtonnadir->setText(QCoreApplication::translate("Formatos", "A\303\261adir", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("Formatos", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Formatos: public Ui_Formatos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMATOS_H
