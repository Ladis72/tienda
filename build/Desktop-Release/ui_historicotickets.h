/********************************************************************************
** Form generated from reading UI file 'historicotickets.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
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
            HistoricoTickets->setObjectName(QString::fromUtf8("HistoricoTickets"));
        HistoricoTickets->resize(953, 536);
        gridLayout = new QGridLayout(HistoricoTickets);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(HistoricoTickets);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label);

        dateTimeEditDesde = new QDateTimeEdit(HistoricoTickets);
        dateTimeEditDesde->setObjectName(QString::fromUtf8("dateTimeEditDesde"));
        dateTimeEditDesde->setCalendarPopup(true);

        horizontalLayout->addWidget(dateTimeEditDesde);

        label_2 = new QLabel(HistoricoTickets);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label_2);

        dateTimeEditHasta = new QDateTimeEdit(HistoricoTickets);
        dateTimeEditHasta->setObjectName(QString::fromUtf8("dateTimeEditHasta"));
        dateTimeEditHasta->setCalendarPopup(true);

        horizontalLayout->addWidget(dateTimeEditHasta);

        pushButtonConsultar = new QPushButton(HistoricoTickets);
        pushButtonConsultar->setObjectName(QString::fromUtf8("pushButtonConsultar"));

        horizontalLayout->addWidget(pushButtonConsultar);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        tableViewTickets = new QTableView(HistoricoTickets);
        tableViewTickets->setObjectName(QString::fromUtf8("tableViewTickets"));
        tableViewTickets->setMaximumSize(QSize(16777215, 16777215));
        tableViewTickets->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustIgnored);
        tableViewTickets->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableViewTickets->setAlternatingRowColors(true);
        tableViewTickets->setSortingEnabled(false);
        tableViewTickets->horizontalHeader()->setMinimumSectionSize(20);
        tableViewTickets->verticalHeader()->setVisible(false);

        gridLayout->addWidget(tableViewTickets, 1, 0, 1, 3);

        tableViewLineasTicket = new QTableView(HistoricoTickets);
        tableViewLineasTicket->setObjectName(QString::fromUtf8("tableViewLineasTicket"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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
        pushButtonImprimir->setObjectName(QString::fromUtf8("pushButtonImprimir"));

        gridLayout->addWidget(pushButtonImprimir, 4, 0, 1, 1);

        pushButtonFormaPago = new QPushButton(HistoricoTickets);
        pushButtonFormaPago->setObjectName(QString::fromUtf8("pushButtonFormaPago"));

        gridLayout->addWidget(pushButtonFormaPago, 4, 1, 1, 2);

        pushButtonImprimirFactura = new QPushButton(HistoricoTickets);
        pushButtonImprimirFactura->setObjectName(QString::fromUtf8("pushButtonImprimirFactura"));

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
