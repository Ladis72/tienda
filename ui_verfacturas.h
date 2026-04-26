/********************************************************************************
** Form generated from reading UI file 'verfacturas.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VERFACTURAS_H
#define UI_VERFACTURAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_VerFacturas
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
    QDateEdit *dateEditDesde;
    QLabel *label_2;
    QDateEdit *dateEditHasta;
    QSpacerItem *horizontalSpacerFilters1;
    QLabel *label_3;
    QComboBox *comboBoxProceedores;
    QCheckBox *checkBoxTodosProveedores;
    QSpacerItem *horizontalSpacerFilters2;
    QPushButton *pushButtonVer;
    QTableView *tableView;
    QHBoxLayout *horizontalLayoutActions;
    QPushButton *pushButtonVerFactura;
    QPushButton *pushButtonPagar;
    QSpacerItem *horizontalSpacerActions;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *VerFacturas)
    {
        if (VerFacturas->objectName().isEmpty())
            VerFacturas->setObjectName(QString::fromUtf8("VerFacturas"));
        VerFacturas->resize(1100, 750);
        VerFacturas->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        mainVerticalLayout = new QVBoxLayout(VerFacturas);
        mainVerticalLayout->setSpacing(15);
        mainVerticalLayout->setObjectName(QString::fromUtf8("mainVerticalLayout"));
        mainVerticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(VerFacturas);
        headerFrame->setObjectName(QString::fromUtf8("headerFrame"));
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame {\n"
"    background-color: #e3f2fd;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #bbdefb;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName(QString::fromUtf8("horizontalLayoutHeader"));
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName(QString::fromUtf8("labelIcono"));
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/facturas.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelTitle = new QLabel(headerFrame);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelTitle->setFont(font);
        labelTitle->setStyleSheet(QString::fromUtf8("color: #0d47a1;"));

        horizontalLayoutHeader->addWidget(labelTitle);

        horizontalSpacerHeader = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);


        mainVerticalLayout->addWidget(headerFrame);

        filterFrame = new QFrame(VerFacturas);
        filterFrame->setObjectName(QString::fromUtf8("filterFrame"));
        filterFrame->setStyleSheet(QString::fromUtf8("QFrame#filterFrame { background-color: white; border-radius: 5px; border: 1px solid #e0e0e0; }"));
        horizontalLayoutFilters = new QHBoxLayout(filterFrame);
        horizontalLayoutFilters->setObjectName(QString::fromUtf8("horizontalLayoutFilters"));
        label = new QLabel(filterFrame);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayoutFilters->addWidget(label);

        dateEditDesde = new QDateEdit(filterFrame);
        dateEditDesde->setObjectName(QString::fromUtf8("dateEditDesde"));
        dateEditDesde->setCalendarPopup(true);

        horizontalLayoutFilters->addWidget(dateEditDesde);

        label_2 = new QLabel(filterFrame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayoutFilters->addWidget(label_2);

        dateEditHasta = new QDateEdit(filterFrame);
        dateEditHasta->setObjectName(QString::fromUtf8("dateEditHasta"));
        dateEditHasta->setCalendarPopup(true);

        horizontalLayoutFilters->addWidget(dateEditHasta);

        horizontalSpacerFilters1 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutFilters->addItem(horizontalSpacerFilters1);

        label_3 = new QLabel(filterFrame);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayoutFilters->addWidget(label_3);

        comboBoxProceedores = new QComboBox(filterFrame);
        comboBoxProceedores->setObjectName(QString::fromUtf8("comboBoxProceedores"));
        comboBoxProceedores->setMinimumSize(QSize(200, 0));

        horizontalLayoutFilters->addWidget(comboBoxProceedores);

        checkBoxTodosProveedores = new QCheckBox(filterFrame);
        checkBoxTodosProveedores->setObjectName(QString::fromUtf8("checkBoxTodosProveedores"));
        checkBoxTodosProveedores->setChecked(true);

        horizontalLayoutFilters->addWidget(checkBoxTodosProveedores);

        horizontalSpacerFilters2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutFilters->addItem(horizontalSpacerFilters2);

        pushButtonVer = new QPushButton(filterFrame);
        pushButtonVer->setObjectName(QString::fromUtf8("pushButtonVer"));
        pushButtonVer->setMinimumSize(QSize(100, 35));
        pushButtonVer->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #1976d2; color: white; font-weight: bold; border-radius: 4px; }\n"
"QPushButton:hover { background-color: #1565c0; }"));

        horizontalLayoutFilters->addWidget(pushButtonVer);


        mainVerticalLayout->addWidget(filterFrame);

        tableView = new QTableView(VerFacturas);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        mainVerticalLayout->addWidget(tableView);

        horizontalLayoutActions = new QHBoxLayout();
        horizontalLayoutActions->setObjectName(QString::fromUtf8("horizontalLayoutActions"));
        pushButtonVerFactura = new QPushButton(VerFacturas);
        pushButtonVerFactura->setObjectName(QString::fromUtf8("pushButtonVerFactura"));
        pushButtonVerFactura->setMinimumSize(QSize(150, 40));

        horizontalLayoutActions->addWidget(pushButtonVerFactura);

        pushButtonPagar = new QPushButton(VerFacturas);
        pushButtonPagar->setObjectName(QString::fromUtf8("pushButtonPagar"));
        pushButtonPagar->setMinimumSize(QSize(180, 40));

        horizontalLayoutActions->addWidget(pushButtonPagar);

        horizontalSpacerActions = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutActions->addItem(horizontalSpacerActions);

        pushButtonCerrar = new QPushButton(VerFacturas);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));
        pushButtonCerrar->setMinimumSize(QSize(100, 40));

        horizontalLayoutActions->addWidget(pushButtonCerrar);


        mainVerticalLayout->addLayout(horizontalLayoutActions);


        retranslateUi(VerFacturas);

        QMetaObject::connectSlotsByName(VerFacturas);
    } // setupUi

    void retranslateUi(QDialog *VerFacturas)
    {
        VerFacturas->setWindowTitle(QCoreApplication::translate("VerFacturas", "Listado de Documentos", nullptr));
        labelTitle->setText(QCoreApplication::translate("VerFacturas", "Consulta de Facturas / Albaranes", nullptr));
        label->setText(QCoreApplication::translate("VerFacturas", "Desde:", nullptr));
        dateEditDesde->setDisplayFormat(QCoreApplication::translate("VerFacturas", "yyyy-MM-dd", nullptr));
        label_2->setText(QCoreApplication::translate("VerFacturas", "Hasta:", nullptr));
        dateEditHasta->setDisplayFormat(QCoreApplication::translate("VerFacturas", "yyyy-MM-dd", nullptr));
        label_3->setText(QCoreApplication::translate("VerFacturas", "Proveedor:", nullptr));
        checkBoxTodosProveedores->setText(QCoreApplication::translate("VerFacturas", "Todos", nullptr));
        pushButtonVer->setText(QCoreApplication::translate("VerFacturas", "Consultar", nullptr));
        pushButtonVerFactura->setText(QCoreApplication::translate("VerFacturas", "Ver Documento", nullptr));
        pushButtonPagar->setText(QCoreApplication::translate("VerFacturas", "Marcar Pagada", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("VerFacturas", "Cerrar", nullptr));
#if QT_CONFIG(shortcut)
        pushButtonCerrar->setShortcut(QCoreApplication::translate("VerFacturas", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
    } // retranslateUi

};

namespace Ui {
    class VerFacturas: public Ui_VerFacturas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VERFACTURAS_H
