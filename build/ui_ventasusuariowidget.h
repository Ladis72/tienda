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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ventasUsuarioWidget
{
public:
    QGridLayout *gridLayout_2;
    QHBoxLayout *topLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioDia;
    QRadioButton *radioMes;
    QRadioButton *radioAnio;
    QSpacerItem *horizontalSpacer;
    QFrame *frameStats;
    QVBoxLayout *statsLayout;
    QLabel *lblStats;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QDateEdit *dateDesde;
    QLabel *label_2;
    QDateEdit *dateHasta;
    QCheckBox *chkGlobal;
    QSpacerItem *horizontalSpacer_2;
    QTabWidget *tabWidge;
    QWidget *tabFechas;
    QHBoxLayout *horizontalLayout_Fechas;
    QTableView *tableResumen;
    QVBoxLayout *fechasLayout;
    QWidget *tabHoras;
    QHBoxLayout *horizontalLayout_Horas;
    QTableView *tableHoras;
    QVBoxLayout *chartLayoutHoras;
    QWidget *tabSemana;
    QHBoxLayout *horizontalLayout_Semana;
    QTableView *tableSemana;
    QVBoxLayout *chartLayoutSemana;
    QWidget *tabTickets;
    QVBoxLayout *verticalLayout_Tickets;
    QTableView *tableTickets;
    QWidget *tabProductos;
    QVBoxLayout *verticalLayout_Productos;
    QTableView *tableProductos;

    void setupUi(QWidget *ventasUsuarioWidget)
    {
        if (ventasUsuarioWidget->objectName().isEmpty())
            ventasUsuarioWidget->setObjectName("ventasUsuarioWidget");
        ventasUsuarioWidget->resize(1100, 800);
        ventasUsuarioWidget->setStyleSheet(QString::fromUtf8("QWidget { background-color: #f0f2f5; color: #1c1e21; font-family: 'Segoe UI', Roboto, Helvetica, Arial, sans-serif; }\n"
"QTableView { background-color: white; border: 1px solid #dddfe2; border-radius: 8px; selection-background-color: #e7f3ff; selection-color: #1877f2; gridline-color: #f0f2f5; }\n"
"QHeaderView::section { background-color: #f0f2f5; padding: 10px; border: none; border-bottom: 2px solid #dddfe2; font-weight: bold; color: #65676b; }\n"
"QTabWidget::pane { border: 1px solid #dddfe2; top: -1px; background: white; border-bottom-left-radius: 8px; border-bottom-right-radius: 8px; }\n"
"QTabBar::tab { background: #f0f2f5; border: 1px solid #dddfe2; padding: 12px 24px; border-top-left-radius: 8px; border-top-right-radius: 8px; margin-right: 4px; font-weight: 600; color: #65676b; }\n"
"QTabBar::tab:selected { background: white; border-bottom-color: white; color: #1877f2; }\n"
"QGroupBox { font-weight: bold; border: 1px solid #dddfe2; border-radius: 8px; margin-top: 15px; padding-top: 15px; background: wh"
                        "ite; }\n"
"QGroupBox::title { subcontrol-origin: margin; left: 15px; padding: 0 5px; color: #1877f2; }\n"
"QDateEdit { padding: 5px; border: 1px solid #dddfe2; border-radius: 4px; background: white; }\n"
"QCheckBox { font-weight: bold; color: #1c1e21; }"));
        gridLayout_2 = new QGridLayout(ventasUsuarioWidget);
        gridLayout_2->setSpacing(15);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(20, 20, 20, 20);
        topLayout = new QHBoxLayout();
        topLayout->setSpacing(15);
        topLayout->setObjectName("topLayout");
        groupBox = new QGroupBox(ventasUsuarioWidget);
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


        topLayout->addWidget(groupBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topLayout->addItem(horizontalSpacer);

        frameStats = new QFrame(ventasUsuarioWidget);
        frameStats->setObjectName("frameStats");
        frameStats->setMinimumSize(QSize(500, 0));
        frameStats->setStyleSheet(QString::fromUtf8("QFrame#frameStats { background-color: #ffffff; border: 1px solid #1877f2; border-radius: 12px; padding: 15px; }"));
        statsLayout = new QVBoxLayout(frameStats);
        statsLayout->setObjectName("statsLayout");
        lblStats = new QLabel(frameStats);
        lblStats->setObjectName("lblStats");
        QFont font;
        font.setPointSize(11);
        font.setBold(false);
        lblStats->setFont(font);
        lblStats->setAlignment(Qt::AlignCenter);

        statsLayout->addWidget(lblStats);


        topLayout->addWidget(frameStats);


        gridLayout_2->addLayout(topLayout, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(15);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label = new QLabel(ventasUsuarioWidget);
        label->setObjectName("label");
        QFont font1;
        font1.setBold(true);
        label->setFont(font1);

        horizontalLayout_3->addWidget(label);

        dateDesde = new QDateEdit(ventasUsuarioWidget);
        dateDesde->setObjectName("dateDesde");
        dateDesde->setMinimumSize(QSize(120, 0));

        horizontalLayout_3->addWidget(dateDesde);

        label_2 = new QLabel(ventasUsuarioWidget);
        label_2->setObjectName("label_2");
        label_2->setFont(font1);

        horizontalLayout_3->addWidget(label_2);

        dateHasta = new QDateEdit(ventasUsuarioWidget);
        dateHasta->setObjectName("dateHasta");
        dateHasta->setMinimumSize(QSize(120, 0));

        horizontalLayout_3->addWidget(dateHasta);

        chkGlobal = new QCheckBox(ventasUsuarioWidget);
        chkGlobal->setObjectName("chkGlobal");
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(true);
        chkGlobal->setFont(font2);

        horizontalLayout_3->addWidget(chkGlobal);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        gridLayout_2->addLayout(horizontalLayout_3, 1, 0, 1, 1);

        tabWidge = new QTabWidget(ventasUsuarioWidget);
        tabWidge->setObjectName("tabWidge");
        tabFechas = new QWidget();
        tabFechas->setObjectName("tabFechas");
        horizontalLayout_Fechas = new QHBoxLayout(tabFechas);
        horizontalLayout_Fechas->setSpacing(20);
        horizontalLayout_Fechas->setObjectName("horizontalLayout_Fechas");
        horizontalLayout_Fechas->setContentsMargins(15, 15, 15, 15);
        tableResumen = new QTableView(tabFechas);
        tableResumen->setObjectName("tableResumen");
        tableResumen->setAlternatingRowColors(true);
        tableResumen->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableResumen->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

        horizontalLayout_Fechas->addWidget(tableResumen);

        fechasLayout = new QVBoxLayout();
        fechasLayout->setObjectName("fechasLayout");

        horizontalLayout_Fechas->addLayout(fechasLayout);

        horizontalLayout_Fechas->setStretch(0, 2);
        horizontalLayout_Fechas->setStretch(1, 3);
        tabWidge->addTab(tabFechas, QString());
        tabHoras = new QWidget();
        tabHoras->setObjectName("tabHoras");
        horizontalLayout_Horas = new QHBoxLayout(tabHoras);
        horizontalLayout_Horas->setSpacing(20);
        horizontalLayout_Horas->setObjectName("horizontalLayout_Horas");
        horizontalLayout_Horas->setContentsMargins(15, 15, 15, 15);
        tableHoras = new QTableView(tabHoras);
        tableHoras->setObjectName("tableHoras");
        tableHoras->setAlternatingRowColors(true);

        horizontalLayout_Horas->addWidget(tableHoras);

        chartLayoutHoras = new QVBoxLayout();
        chartLayoutHoras->setObjectName("chartLayoutHoras");

        horizontalLayout_Horas->addLayout(chartLayoutHoras);

        horizontalLayout_Horas->setStretch(0, 1);
        horizontalLayout_Horas->setStretch(1, 2);
        tabWidge->addTab(tabHoras, QString());
        tabSemana = new QWidget();
        tabSemana->setObjectName("tabSemana");
        horizontalLayout_Semana = new QHBoxLayout(tabSemana);
        horizontalLayout_Semana->setSpacing(20);
        horizontalLayout_Semana->setObjectName("horizontalLayout_Semana");
        horizontalLayout_Semana->setContentsMargins(15, 15, 15, 15);
        tableSemana = new QTableView(tabSemana);
        tableSemana->setObjectName("tableSemana");
        tableSemana->setAlternatingRowColors(true);

        horizontalLayout_Semana->addWidget(tableSemana);

        chartLayoutSemana = new QVBoxLayout();
        chartLayoutSemana->setObjectName("chartLayoutSemana");

        horizontalLayout_Semana->addLayout(chartLayoutSemana);

        horizontalLayout_Semana->setStretch(0, 1);
        horizontalLayout_Semana->setStretch(1, 2);
        tabWidge->addTab(tabSemana, QString());
        tabTickets = new QWidget();
        tabTickets->setObjectName("tabTickets");
        verticalLayout_Tickets = new QVBoxLayout(tabTickets);
        verticalLayout_Tickets->setObjectName("verticalLayout_Tickets");
        verticalLayout_Tickets->setContentsMargins(15, 15, 15, 15);
        tableTickets = new QTableView(tabTickets);
        tableTickets->setObjectName("tableTickets");
        tableTickets->setAlternatingRowColors(true);

        verticalLayout_Tickets->addWidget(tableTickets);

        tabWidge->addTab(tabTickets, QString());
        tabProductos = new QWidget();
        tabProductos->setObjectName("tabProductos");
        verticalLayout_Productos = new QVBoxLayout(tabProductos);
        verticalLayout_Productos->setObjectName("verticalLayout_Productos");
        verticalLayout_Productos->setContentsMargins(15, 15, 15, 15);
        tableProductos = new QTableView(tabProductos);
        tableProductos->setObjectName("tableProductos");
        tableProductos->setAlternatingRowColors(true);

        verticalLayout_Productos->addWidget(tableProductos);

        tabWidge->addTab(tabProductos, QString());

        gridLayout_2->addWidget(tabWidge, 2, 0, 1, 1);


        retranslateUi(ventasUsuarioWidget);

        tabWidge->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ventasUsuarioWidget);
    } // setupUi

    void retranslateUi(QWidget *ventasUsuarioWidget)
    {
        ventasUsuarioWidget->setWindowTitle(QCoreApplication::translate("ventasUsuarioWidget", "Form", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ventasUsuarioWidget", "Agrupar por:", nullptr));
        radioDia->setText(QCoreApplication::translate("ventasUsuarioWidget", "D\303\255a", nullptr));
        radioMes->setText(QCoreApplication::translate("ventasUsuarioWidget", "Mes", nullptr));
        radioAnio->setText(QCoreApplication::translate("ventasUsuarioWidget", "A\303\261o", nullptr));
        lblStats->setText(QCoreApplication::translate("ventasUsuarioWidget", "Estad\303\255sticas:", nullptr));
        label->setText(QCoreApplication::translate("ventasUsuarioWidget", "Desde:", nullptr));
        dateDesde->setDisplayFormat(QCoreApplication::translate("ventasUsuarioWidget", "yyyy-MM-dd", nullptr));
        label_2->setText(QCoreApplication::translate("ventasUsuarioWidget", "Hasta:", nullptr));
        dateHasta->setDisplayFormat(QCoreApplication::translate("ventasUsuarioWidget", "yyyy-MM-dd", nullptr));
        chkGlobal->setText(QCoreApplication::translate("ventasUsuarioWidget", "Cargar Global (Todas las tiendas)", nullptr));
        tabWidge->setTabText(tabWidge->indexOf(tabFechas), QCoreApplication::translate("ventasUsuarioWidget", "Fechas", nullptr));
        tabWidge->setTabText(tabWidge->indexOf(tabHoras), QCoreApplication::translate("ventasUsuarioWidget", "Horas", nullptr));
        tabWidge->setTabText(tabWidge->indexOf(tabSemana), QCoreApplication::translate("ventasUsuarioWidget", "D\303\255as Semana", nullptr));
        tabWidge->setTabText(tabWidge->indexOf(tabTickets), QCoreApplication::translate("ventasUsuarioWidget", "Tickets", nullptr));
        tabWidge->setTabText(tabWidge->indexOf(tabProductos), QCoreApplication::translate("ventasUsuarioWidget", "Productos", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ventasUsuarioWidget: public Ui_ventasUsuarioWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VENTASUSUARIOWIDGET_H
