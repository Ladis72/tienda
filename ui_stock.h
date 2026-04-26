/********************************************************************************
** Form generated from reading UI file 'stock.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STOCK_H
#define UI_STOCK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Stock
{
public:
    QVBoxLayout *mainVerticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *labelProductoHeader;
    QSpacerItem *horizontalSpacerHeader;
    QGridLayout *gridLayout;
    QTableView *tableView;
    QGroupBox *groupBoxAjuste;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QLineEdit *lineEditLoteSel;
    QLabel *label_3;
    QDateEdit *dateEditNewCad;
    QLabel *label_4;
    QLineEdit *lineEditCantAct;
    QLabel *label_5;
    QDoubleSpinBox *doubleSpinBoxNewCant;
    QLabel *label_6;
    QComboBox *comboBoxMotivo;
    QLabel *label_notas;
    QLineEdit *lineEditNotas;
    QPushButton *pushButtonValidar;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonHistory;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonNuevoLote;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *Stock)
    {
        if (Stock->objectName().isEmpty())
            Stock->setObjectName(QString::fromUtf8("Stock"));
        Stock->resize(750, 500);
        Stock->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        mainVerticalLayout = new QVBoxLayout(Stock);
        mainVerticalLayout->setSpacing(15);
        mainVerticalLayout->setObjectName(QString::fromUtf8("mainVerticalLayout"));
        mainVerticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(Stock);
        headerFrame->setObjectName(QString::fromUtf8("headerFrame"));
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame {\n"
"    background-color: #eceff1;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #cfd8dc;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName(QString::fromUtf8("horizontalLayoutHeader"));
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName(QString::fromUtf8("labelIcono"));
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/stock.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelProductoHeader = new QLabel(headerFrame);
        labelProductoHeader->setObjectName(QString::fromUtf8("labelProductoHeader"));
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelProductoHeader->setFont(font);
        labelProductoHeader->setStyleSheet(QString::fromUtf8("color: #455a64;"));

        horizontalLayoutHeader->addWidget(labelProductoHeader);

        horizontalSpacerHeader = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);


        mainVerticalLayout->addWidget(headerFrame);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableView = new QTableView(Stock);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        gridLayout->addWidget(tableView, 1, 0, 1, 1);

        groupBoxAjuste = new QGroupBox(Stock);
        groupBoxAjuste->setObjectName(QString::fromUtf8("groupBoxAjuste"));
        gridLayout_2 = new QGridLayout(groupBoxAjuste);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_2 = new QLabel(groupBoxAjuste);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        lineEditLoteSel = new QLineEdit(groupBoxAjuste);
        lineEditLoteSel->setObjectName(QString::fromUtf8("lineEditLoteSel"));
        lineEditLoteSel->setReadOnly(true);

        gridLayout_2->addWidget(lineEditLoteSel, 0, 1, 1, 1);

        label_3 = new QLabel(groupBoxAjuste);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 1, 0, 1, 1);

        dateEditNewCad = new QDateEdit(groupBoxAjuste);
        dateEditNewCad->setObjectName(QString::fromUtf8("dateEditNewCad"));
        dateEditNewCad->setCalendarPopup(true);

        gridLayout_2->addWidget(dateEditNewCad, 1, 1, 1, 1);

        label_4 = new QLabel(groupBoxAjuste);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 2, 0, 1, 1);

        lineEditCantAct = new QLineEdit(groupBoxAjuste);
        lineEditCantAct->setObjectName(QString::fromUtf8("lineEditCantAct"));
        lineEditCantAct->setReadOnly(true);

        gridLayout_2->addWidget(lineEditCantAct, 2, 1, 1, 1);

        label_5 = new QLabel(groupBoxAjuste);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 3, 0, 1, 1);

        doubleSpinBoxNewCant = new QDoubleSpinBox(groupBoxAjuste);
        doubleSpinBoxNewCant->setObjectName(QString::fromUtf8("doubleSpinBoxNewCant"));
        doubleSpinBoxNewCant->setMaximum(999999.989999999990687);

        gridLayout_2->addWidget(doubleSpinBoxNewCant, 3, 1, 1, 1);

        label_6 = new QLabel(groupBoxAjuste);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 4, 0, 1, 1);

        comboBoxMotivo = new QComboBox(groupBoxAjuste);
        comboBoxMotivo->addItem(QString());
        comboBoxMotivo->addItem(QString());
        comboBoxMotivo->addItem(QString());
        comboBoxMotivo->addItem(QString());
        comboBoxMotivo->setObjectName(QString::fromUtf8("comboBoxMotivo"));
        comboBoxMotivo->setEditable(true);

        gridLayout_2->addWidget(comboBoxMotivo, 4, 1, 1, 1);

        label_notas = new QLabel(groupBoxAjuste);
        label_notas->setObjectName(QString::fromUtf8("label_notas"));

        gridLayout_2->addWidget(label_notas, 5, 0, 1, 1);

        lineEditNotas = new QLineEdit(groupBoxAjuste);
        lineEditNotas->setObjectName(QString::fromUtf8("lineEditNotas"));

        gridLayout_2->addWidget(lineEditNotas, 5, 1, 1, 1);

        pushButtonValidar = new QPushButton(groupBoxAjuste);
        pushButtonValidar->setObjectName(QString::fromUtf8("pushButtonValidar"));
        pushButtonValidar->setMinimumSize(QSize(0, 40));
        QFont font1;
        font1.setBold(true);
        pushButtonValidar->setFont(font1);
        pushButtonValidar->setStyleSheet(QString::fromUtf8("background-color: #1976D2; color: white;"));

        gridLayout_2->addWidget(pushButtonValidar, 6, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 7, 0, 1, 2);


        gridLayout->addWidget(groupBoxAjuste, 1, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonHistory = new QPushButton(Stock);
        pushButtonHistory->setObjectName(QString::fromUtf8("pushButtonHistory"));

        horizontalLayout->addWidget(pushButtonHistory);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonNuevoLote = new QPushButton(Stock);
        pushButtonNuevoLote->setObjectName(QString::fromUtf8("pushButtonNuevoLote"));

        horizontalLayout->addWidget(pushButtonNuevoLote);

        pushButtonCerrar = new QPushButton(Stock);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));

        horizontalLayout->addWidget(pushButtonCerrar);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 2);


        mainVerticalLayout->addLayout(gridLayout);


        retranslateUi(Stock);
        QObject::connect(pushButtonCerrar, SIGNAL(clicked()), Stock, SLOT(accept()));

        QMetaObject::connectSlotsByName(Stock);
    } // setupUi

    void retranslateUi(QDialog *Stock)
    {
        Stock->setWindowTitle(QCoreApplication::translate("Stock", "Gesti\303\263n de Existencias y Lotes", nullptr));
        labelProductoHeader->setText(QCoreApplication::translate("Stock", "Producto", nullptr));
        groupBoxAjuste->setTitle(QCoreApplication::translate("Stock", "A\303\261adir Ajuste / Movimiento", nullptr));
        label_2->setText(QCoreApplication::translate("Stock", "Lote seleccionado:", nullptr));
        label_3->setText(QCoreApplication::translate("Stock", "Nueva Caducidad:", nullptr));
        dateEditNewCad->setDisplayFormat(QCoreApplication::translate("Stock", "yyyy-MM-dd", nullptr));
        label_4->setText(QCoreApplication::translate("Stock", "Cantidad actual:", nullptr));
        label_5->setText(QCoreApplication::translate("Stock", "Nueva Cantidad:", nullptr));
        label_6->setText(QCoreApplication::translate("Stock", "Motivo:", nullptr));
        comboBoxMotivo->setItemText(0, QCoreApplication::translate("Stock", "Correcci\303\263n de Inventario", nullptr));
        comboBoxMotivo->setItemText(1, QCoreApplication::translate("Stock", "Rotura / Merma", nullptr));
        comboBoxMotivo->setItemText(2, QCoreApplication::translate("Stock", "Caducidad", nullptr));
        comboBoxMotivo->setItemText(3, QCoreApplication::translate("Stock", "Error de Fecha", nullptr));

        label_notas->setText(QCoreApplication::translate("Stock", "Notas:", nullptr));
        lineEditNotas->setPlaceholderText(QCoreApplication::translate("Stock", "Aclaraciones adicionales...", nullptr));
        pushButtonValidar->setText(QCoreApplication::translate("Stock", "REGISTRAR AJUSTE", nullptr));
        pushButtonHistory->setText(QCoreApplication::translate("Stock", "Ver Hist\303\263rico de Ajustes", nullptr));
        pushButtonNuevoLote->setText(QCoreApplication::translate("Stock", "Crear Nuevo Lote", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("Stock", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Stock: public Ui_Stock {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STOCK_H
