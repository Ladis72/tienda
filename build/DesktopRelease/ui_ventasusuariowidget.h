/********************************************************************************
** Form generated from reading UI file 'ventasusuariowidget.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VENTASUSUARIOWIDGET_H
#define UI_VENTASUSUARIOWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ventasUsuarioWidget
{
public:
    QGridLayout *gridLayout_2;
    QTabWidget *tabWidge;
    QWidget *tabFechas;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QDateEdit *dateDesde;
    QLabel *label_2;
    QDateEdit *dateHasta;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioDia;
    QRadioButton *radioMes;
    QRadioButton *radioAnio;
    QGridLayout *fechasLayout;
    QTableView *tableResumen;
    QWidget *tabHoras;
    QWidget *tabSemana;
    QWidget *tabTickets;
    QWidget *tabProductos;

    void setupUi(QWidget *ventasUsuarioWidget)
    {
        if (ventasUsuarioWidget->objectName().isEmpty())
            ventasUsuarioWidget->setObjectName("ventasUsuarioWidget");
        ventasUsuarioWidget->resize(1054, 760);
        gridLayout_2 = new QGridLayout(ventasUsuarioWidget);
        gridLayout_2->setObjectName("gridLayout_2");
        tabWidge = new QTabWidget(ventasUsuarioWidget);
        tabWidge->setObjectName("tabWidge");
        tabFechas = new QWidget();
        tabFechas->setObjectName("tabFechas");
        gridLayout_3 = new QGridLayout(tabFechas);
        gridLayout_3->setObjectName("gridLayout_3");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label = new QLabel(tabFechas);
        label->setObjectName("label");

        horizontalLayout_3->addWidget(label);

        dateDesde = new QDateEdit(tabFechas);
        dateDesde->setObjectName("dateDesde");

        horizontalLayout_3->addWidget(dateDesde);

        label_2 = new QLabel(tabFechas);
        label_2->setObjectName("label_2");

        horizontalLayout_3->addWidget(label_2);

        dateHasta = new QDateEdit(tabFechas);
        dateHasta->setObjectName("dateHasta");

        horizontalLayout_3->addWidget(dateHasta);


        gridLayout_3->addLayout(horizontalLayout_3, 3, 1, 1, 1);

        groupBox = new QGroupBox(tabFechas);
        groupBox->setObjectName("groupBox");
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        radioDia = new QRadioButton(groupBox);
        radioDia->setObjectName("radioDia");

        horizontalLayout_2->addWidget(radioDia);

        radioMes = new QRadioButton(groupBox);
        radioMes->setObjectName("radioMes");
        radioMes->setChecked(true);

        horizontalLayout_2->addWidget(radioMes);

        radioAnio = new QRadioButton(groupBox);
        radioAnio->setObjectName("radioAnio");

        horizontalLayout_2->addWidget(radioAnio);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 3);

        fechasLayout = new QGridLayout();
        fechasLayout->setObjectName("fechasLayout");

        gridLayout_3->addLayout(fechasLayout, 4, 5, 1, 1);

        tableResumen = new QTableView(tabFechas);
        tableResumen->setObjectName("tableResumen");

        gridLayout_3->addWidget(tableResumen, 4, 0, 1, 5);

        tabWidge->addTab(tabFechas, QString());
        tabHoras = new QWidget();
        tabHoras->setObjectName("tabHoras");
        tabWidge->addTab(tabHoras, QString());
        tabSemana = new QWidget();
        tabSemana->setObjectName("tabSemana");
        tabWidge->addTab(tabSemana, QString());
        tabTickets = new QWidget();
        tabTickets->setObjectName("tabTickets");
        tabWidge->addTab(tabTickets, QString());
        tabProductos = new QWidget();
        tabProductos->setObjectName("tabProductos");
        tabWidge->addTab(tabProductos, QString());

        gridLayout_2->addWidget(tabWidge, 0, 0, 1, 1);


        retranslateUi(ventasUsuarioWidget);

        tabWidge->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ventasUsuarioWidget);
    } // setupUi

    void retranslateUi(QWidget *ventasUsuarioWidget)
    {
        ventasUsuarioWidget->setWindowTitle(QCoreApplication::translate("ventasUsuarioWidget", "Form", nullptr));
        label->setText(QCoreApplication::translate("ventasUsuarioWidget", "Desde;", nullptr));
        dateDesde->setDisplayFormat(QCoreApplication::translate("ventasUsuarioWidget", "yyyy/MM/dd", nullptr));
        label_2->setText(QCoreApplication::translate("ventasUsuarioWidget", "Hasta", nullptr));
        dateHasta->setDisplayFormat(QCoreApplication::translate("ventasUsuarioWidget", "yyyy/MM/dd", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ventasUsuarioWidget", "Ordenar por;", nullptr));
        radioDia->setText(QCoreApplication::translate("ventasUsuarioWidget", "D\303\255a", nullptr));
        radioMes->setText(QCoreApplication::translate("ventasUsuarioWidget", "Mes", nullptr));
        radioAnio->setText(QCoreApplication::translate("ventasUsuarioWidget", "A\303\261o", nullptr));
        tabWidge->setTabText(tabWidge->indexOf(tabFechas), QCoreApplication::translate("ventasUsuarioWidget", "Fechas", nullptr));
        tabWidge->setTabText(tabWidge->indexOf(tabHoras), QCoreApplication::translate("ventasUsuarioWidget", "Horas", nullptr));
        tabWidge->setTabText(tabWidge->indexOf(tabSemana), QCoreApplication::translate("ventasUsuarioWidget", "Dias semana", nullptr));
        tabWidge->setTabText(tabWidge->indexOf(tabTickets), QCoreApplication::translate("ventasUsuarioWidget", "Tickets", nullptr));
        tabWidge->setTabText(tabWidge->indexOf(tabProductos), QCoreApplication::translate("ventasUsuarioWidget", "Productos", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ventasUsuarioWidget: public Ui_ventasUsuarioWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VENTASUSUARIOWIDGET_H
