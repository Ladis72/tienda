/********************************************************************************
** Form generated from reading UI file 'historicotickets.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISTORICOTICKETS_H
#define UI_HISTORICOTICKETS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_HistoricoTickets
{
public:
    QVBoxLayout *mainVerticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *labelTitle;
    QSpacerItem *horizontalSpacerHeader;
    QFrame *filterFrame;
    QHBoxLayout *horizontalLayoutFilters;
    QLabel *label;
    QDateTimeEdit *dateTimeEditDesde;
    QLabel *label_2;
    QDateTimeEdit *dateTimeEditHasta;
    QSpacerItem *horizontalSpacerFilters;
    QPushButton *pushButtonConsultar;
    QSplitter *splitter;
    QTableView *tableViewTickets;
    QTableView *tableViewLineasTicket;
    QHBoxLayout *horizontalLayoutActions;
    QPushButton *pushButtonImprimir;
    QPushButton *pushButtonImprimirFactura;
    QPushButton *pushButtonFormaPago;
    QSpacerItem *horizontalSpacerActions;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *HistoricoTickets)
    {
        if (HistoricoTickets->objectName().isEmpty())
            HistoricoTickets->setObjectName(QString::fromUtf8("HistoricoTickets"));
        HistoricoTickets->resize(1100, 750);
        HistoricoTickets->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        mainVerticalLayout = new QVBoxLayout(HistoricoTickets);
        mainVerticalLayout->setSpacing(15);
        mainVerticalLayout->setObjectName(QString::fromUtf8("mainVerticalLayout"));
        mainVerticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(HistoricoTickets);
        headerFrame->setObjectName(QString::fromUtf8("headerFrame"));
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame {\n"
"    background-color: #e8f5e9;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #c8e6c9;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName(QString::fromUtf8("horizontalLayoutHeader"));
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName(QString::fromUtf8("labelIcono"));
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/tickets.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelTitle = new QLabel(headerFrame);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelTitle->setFont(font);
        labelTitle->setStyleSheet(QString::fromUtf8("color: #1b5e20;"));

        horizontalLayoutHeader->addWidget(labelTitle);

        horizontalSpacerHeader = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);


        mainVerticalLayout->addWidget(headerFrame);

        filterFrame = new QFrame(HistoricoTickets);
        filterFrame->setObjectName(QString::fromUtf8("filterFrame"));
        filterFrame->setStyleSheet(QString::fromUtf8("QFrame#filterFrame { background-color: white; border-radius: 5px; border: 1px solid #e0e0e0; }"));
        horizontalLayoutFilters = new QHBoxLayout(filterFrame);
        horizontalLayoutFilters->setObjectName(QString::fromUtf8("horizontalLayoutFilters"));
        label = new QLabel(filterFrame);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayoutFilters->addWidget(label);

        dateTimeEditDesde = new QDateTimeEdit(filterFrame);
        dateTimeEditDesde->setObjectName(QString::fromUtf8("dateTimeEditDesde"));
        dateTimeEditDesde->setMinimumSize(QSize(160, 0));
        dateTimeEditDesde->setCalendarPopup(true);

        horizontalLayoutFilters->addWidget(dateTimeEditDesde);

        label_2 = new QLabel(filterFrame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayoutFilters->addWidget(label_2);

        dateTimeEditHasta = new QDateTimeEdit(filterFrame);
        dateTimeEditHasta->setObjectName(QString::fromUtf8("dateTimeEditHasta"));
        dateTimeEditHasta->setMinimumSize(QSize(160, 0));
        dateTimeEditHasta->setCalendarPopup(true);

        horizontalLayoutFilters->addWidget(dateTimeEditHasta);

        horizontalSpacerFilters = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutFilters->addItem(horizontalSpacerFilters);

        pushButtonConsultar = new QPushButton(filterFrame);
        pushButtonConsultar->setObjectName(QString::fromUtf8("pushButtonConsultar"));
        pushButtonConsultar->setMinimumSize(QSize(120, 35));
        pushButtonConsultar->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #2e7d32; color: white; font-weight: bold; border-radius: 4px; }\n"
"QPushButton:hover { background-color: #1b5e20; }"));

        horizontalLayoutFilters->addWidget(pushButtonConsultar);


        mainVerticalLayout->addWidget(filterFrame);

        splitter = new QSplitter(HistoricoTickets);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Orientation::Vertical);
        tableViewTickets = new QTableView(splitter);
        tableViewTickets->setObjectName(QString::fromUtf8("tableViewTickets"));
        tableViewTickets->setAlternatingRowColors(true);
        splitter->addWidget(tableViewTickets);
        tableViewTickets->verticalHeader()->setVisible(false);
        tableViewLineasTicket = new QTableView(splitter);
        tableViewLineasTicket->setObjectName(QString::fromUtf8("tableViewLineasTicket"));
        tableViewLineasTicket->setAlternatingRowColors(true);
        splitter->addWidget(tableViewLineasTicket);

        mainVerticalLayout->addWidget(splitter);

        horizontalLayoutActions = new QHBoxLayout();
        horizontalLayoutActions->setObjectName(QString::fromUtf8("horizontalLayoutActions"));
        pushButtonImprimir = new QPushButton(HistoricoTickets);
        pushButtonImprimir->setObjectName(QString::fromUtf8("pushButtonImprimir"));
        pushButtonImprimir->setMinimumSize(QSize(150, 40));

        horizontalLayoutActions->addWidget(pushButtonImprimir);

        pushButtonImprimirFactura = new QPushButton(HistoricoTickets);
        pushButtonImprimirFactura->setObjectName(QString::fromUtf8("pushButtonImprimirFactura"));
        pushButtonImprimirFactura->setMinimumSize(QSize(150, 40));

        horizontalLayoutActions->addWidget(pushButtonImprimirFactura);

        pushButtonFormaPago = new QPushButton(HistoricoTickets);
        pushButtonFormaPago->setObjectName(QString::fromUtf8("pushButtonFormaPago"));
        pushButtonFormaPago->setMinimumSize(QSize(150, 40));

        horizontalLayoutActions->addWidget(pushButtonFormaPago);

        horizontalSpacerActions = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutActions->addItem(horizontalSpacerActions);

        pushButtonCerrar = new QPushButton(HistoricoTickets);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));
        pushButtonCerrar->setMinimumSize(QSize(100, 40));

        horizontalLayoutActions->addWidget(pushButtonCerrar);


        mainVerticalLayout->addLayout(horizontalLayoutActions);


        retranslateUi(HistoricoTickets);

        QMetaObject::connectSlotsByName(HistoricoTickets);
    } // setupUi

    void retranslateUi(QDialog *HistoricoTickets)
    {
        HistoricoTickets->setWindowTitle(QCoreApplication::translate("HistoricoTickets", "Hist\303\263rico de Tickets", nullptr));
        labelTitle->setText(QCoreApplication::translate("HistoricoTickets", "Hist\303\263rico de Ventas", nullptr));
        label->setText(QCoreApplication::translate("HistoricoTickets", "Desde:", nullptr));
        dateTimeEditDesde->setDisplayFormat(QCoreApplication::translate("HistoricoTickets", "yyyy/MM/dd HH:mm", nullptr));
        label_2->setText(QCoreApplication::translate("HistoricoTickets", "Hasta:", nullptr));
        dateTimeEditHasta->setDisplayFormat(QCoreApplication::translate("HistoricoTickets", "yyyy/MM/dd HH:mm", nullptr));
        pushButtonConsultar->setText(QCoreApplication::translate("HistoricoTickets", "Consultar", nullptr));
        pushButtonImprimir->setText(QCoreApplication::translate("HistoricoTickets", "Imprimir Ticket", nullptr));
        pushButtonImprimirFactura->setText(QCoreApplication::translate("HistoricoTickets", "Imprimir Factura", nullptr));
        pushButtonFormaPago->setText(QCoreApplication::translate("HistoricoTickets", "Cambiar F. Pago", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("HistoricoTickets", "Cerrar", nullptr));
#if QT_CONFIG(shortcut)
        pushButtonCerrar->setShortcut(QCoreApplication::translate("HistoricoTickets", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
    } // retranslateUi

};

namespace Ui {
    class HistoricoTickets: public Ui_HistoricoTickets {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTORICOTICKETS_H
