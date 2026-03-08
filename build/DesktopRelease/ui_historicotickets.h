/********************************************************************************
** Form generated from reading UI file 'historicotickets.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISTORICOTICKETS_H
#define UI_HISTORICOTICKETS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_HistoricoTickets
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDateTimeEdit *dateTimeEditDesde;
    QLabel *label_2;
    QDateTimeEdit *dateTimeEditHasta;
    QPushButton *pushButtonConsultar;
    QTableView *tableViewTickets;
    QTableView *tableViewLineasTicket;
    QPushButton *pushButtonImprimir;
    QPushButton *pushButtonFormaPago;
    QPushButton *pushButtonImprimirFactura;

    void setupUi(QDialog *HistoricoTickets)
    {
        if (HistoricoTickets->objectName().isEmpty())
            HistoricoTickets->setObjectName("HistoricoTickets");
        HistoricoTickets->resize(953, 536);
        gridLayout = new QGridLayout(HistoricoTickets);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(HistoricoTickets);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label);

        dateTimeEditDesde = new QDateTimeEdit(HistoricoTickets);
        dateTimeEditDesde->setObjectName("dateTimeEditDesde");
        dateTimeEditDesde->setCalendarPopup(true);

        horizontalLayout->addWidget(dateTimeEditDesde);

        label_2 = new QLabel(HistoricoTickets);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label_2);

        dateTimeEditHasta = new QDateTimeEdit(HistoricoTickets);
        dateTimeEditHasta->setObjectName("dateTimeEditHasta");
        dateTimeEditHasta->setCalendarPopup(true);

        horizontalLayout->addWidget(dateTimeEditHasta);

        pushButtonConsultar = new QPushButton(HistoricoTickets);
        pushButtonConsultar->setObjectName("pushButtonConsultar");

        horizontalLayout->addWidget(pushButtonConsultar);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        tableViewTickets = new QTableView(HistoricoTickets);
        tableViewTickets->setObjectName("tableViewTickets");
        tableViewTickets->setMaximumSize(QSize(16777215, 16777215));
        tableViewTickets->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        tableViewTickets->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableViewTickets->setAlternatingRowColors(true);
        tableViewTickets->setSortingEnabled(false);
        tableViewTickets->horizontalHeader()->setMinimumSectionSize(20);
        tableViewTickets->verticalHeader()->setVisible(false);

        gridLayout->addWidget(tableViewTickets, 1, 0, 1, 3);

        tableViewLineasTicket = new QTableView(HistoricoTickets);
        tableViewLineasTicket->setObjectName("tableViewLineasTicket");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableViewLineasTicket->sizePolicy().hasHeightForWidth());
        tableViewLineasTicket->setSizePolicy(sizePolicy);
        tableViewLineasTicket->setMinimumSize(QSize(0, 0));
        tableViewLineasTicket->setMaximumSize(QSize(16777215, 16777215));
        tableViewLineasTicket->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        tableViewLineasTicket->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableViewLineasTicket->setTabKeyNavigation(true);
        tableViewLineasTicket->setProperty("showDropIndicator", QVariant(true));
        tableViewLineasTicket->setDragEnabled(false);
        tableViewLineasTicket->setDragDropOverwriteMode(true);
        tableViewLineasTicket->setAlternatingRowColors(true);
        tableViewLineasTicket->horizontalHeader()->setCascadingSectionResizes(false);
        tableViewLineasTicket->horizontalHeader()->setMinimumSectionSize(20);

        gridLayout->addWidget(tableViewLineasTicket, 2, 0, 1, 3);

        pushButtonImprimir = new QPushButton(HistoricoTickets);
        pushButtonImprimir->setObjectName("pushButtonImprimir");

        gridLayout->addWidget(pushButtonImprimir, 4, 0, 1, 1);

        pushButtonFormaPago = new QPushButton(HistoricoTickets);
        pushButtonFormaPago->setObjectName("pushButtonFormaPago");

        gridLayout->addWidget(pushButtonFormaPago, 4, 1, 1, 2);

        pushButtonImprimirFactura = new QPushButton(HistoricoTickets);
        pushButtonImprimirFactura->setObjectName("pushButtonImprimirFactura");

        gridLayout->addWidget(pushButtonImprimirFactura, 3, 0, 1, 1);


        retranslateUi(HistoricoTickets);

        QMetaObject::connectSlotsByName(HistoricoTickets);
    } // setupUi

    void retranslateUi(QDialog *HistoricoTickets)
    {
        HistoricoTickets->setWindowTitle(QCoreApplication::translate("HistoricoTickets", "Historico de tickets", nullptr));
        label->setText(QCoreApplication::translate("HistoricoTickets", "Desde", nullptr));
        dateTimeEditDesde->setDisplayFormat(QCoreApplication::translate("HistoricoTickets", "yyyy/MM/dd HH:mm", nullptr));
        label_2->setText(QCoreApplication::translate("HistoricoTickets", "Hasta", nullptr));
        dateTimeEditHasta->setDisplayFormat(QCoreApplication::translate("HistoricoTickets", "yyyy/MM/dd HH:mm", nullptr));
        pushButtonConsultar->setText(QCoreApplication::translate("HistoricoTickets", "Consultar", nullptr));
        pushButtonImprimir->setText(QCoreApplication::translate("HistoricoTickets", "Imprimir ticket", nullptr));
        pushButtonFormaPago->setText(QCoreApplication::translate("HistoricoTickets", "Forma de pago", nullptr));
        pushButtonImprimirFactura->setText(QCoreApplication::translate("HistoricoTickets", "Imprimir Factura", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HistoricoTickets: public Ui_HistoricoTickets {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTORICOTICKETS_H
