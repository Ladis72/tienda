/********************************************************************************
** Form generated from reading UI file 'listadosalidas.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTADOSALIDAS_H
#define UI_LISTADOSALIDAS_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_ListadoSalidas
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDateEdit *dateEditDesde;
    QLabel *label_2;
    QDateEdit *dateEditHasta;
    QPushButton *pushButtonVer;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLabel *labelTotal;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QDialog *ListadoSalidas)
    {
        if (ListadoSalidas->objectName().isEmpty())
            ListadoSalidas->setObjectName(QString::fromUtf8("ListadoSalidas"));
        ListadoSalidas->resize(922, 642);
        gridLayout = new QGridLayout(ListadoSalidas);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(ListadoSalidas);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(label);

        dateEditDesde = new QDateEdit(ListadoSalidas);
        dateEditDesde->setObjectName(QString::fromUtf8("dateEditDesde"));
        dateEditDesde->setCalendarPopup(true);
        dateEditDesde->setDate(QDate(2018, 1, 1));

        horizontalLayout->addWidget(dateEditDesde);

        label_2 = new QLabel(ListadoSalidas);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(label_2);

        dateEditHasta = new QDateEdit(ListadoSalidas);
        dateEditHasta->setObjectName(QString::fromUtf8("dateEditHasta"));

        horizontalLayout->addWidget(dateEditHasta);

        pushButtonVer = new QPushButton(ListadoSalidas);
        pushButtonVer->setObjectName(QString::fromUtf8("pushButtonVer"));

        horizontalLayout->addWidget(pushButtonVer);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        tableView = new QTableView(ListadoSalidas);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout->addWidget(tableView, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(ListadoSalidas);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_2->addWidget(label_3);

        labelTotal = new QLabel(ListadoSalidas);
        labelTotal->setObjectName(QString::fromUtf8("labelTotal"));

        horizontalLayout_2->addWidget(labelTotal);

        pushButton_2 = new QPushButton(ListadoSalidas);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout_2->addWidget(pushButton_2);

        pushButton = new QPushButton(ListadoSalidas);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_2->addWidget(pushButton);


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 1);


        retranslateUi(ListadoSalidas);
        QObject::connect(pushButton, SIGNAL(clicked()), ListadoSalidas, SLOT(close()));

        QMetaObject::connectSlotsByName(ListadoSalidas);
    } // setupUi

    void retranslateUi(QDialog *ListadoSalidas)
    {
        ListadoSalidas->setWindowTitle(QCoreApplication::translate("ListadoSalidas", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("ListadoSalidas", "Desde", nullptr));
        dateEditDesde->setDisplayFormat(QCoreApplication::translate("ListadoSalidas", "yyyy/MM/dd", nullptr));
        label_2->setText(QCoreApplication::translate("ListadoSalidas", "Hasta", nullptr));
        dateEditHasta->setDisplayFormat(QCoreApplication::translate("ListadoSalidas", "yyyy/MM/dd", nullptr));
        pushButtonVer->setText(QCoreApplication::translate("ListadoSalidas", "Ver", nullptr));
        label_3->setText(QCoreApplication::translate("ListadoSalidas", "Total", nullptr));
        labelTotal->setText(QString());
        pushButton_2->setText(QCoreApplication::translate("ListadoSalidas", "Imprimir", nullptr));
        pushButton->setText(QCoreApplication::translate("ListadoSalidas", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ListadoSalidas: public Ui_ListadoSalidas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTADOSALIDAS_H
