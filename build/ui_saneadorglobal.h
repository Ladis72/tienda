/********************************************************************************
** Form generated from reading UI file 'saneadorglobal.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SANEADORGLOBAL_H
#define UI_SANEADORGLOBAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SaneadorGlobal
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *labelTitle;
    QSpacerItem *horizontalSpacerHeader;
    QHBoxLayout *layoutTiendaSelector;
    QLabel *label_3;
    QComboBox *comboBoxTienda;
    QLabel *labelInfo;
    QTableWidget *tableWidget;
    QTextEdit *textEditLog;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonAnalizar;
    QPushButton *pushButtonIgualar;
    QPushButton *pushButtonIgualarLocal;
    QPushButton *pushButtonClonar;
    QPushButton *pushButtonClonarLocal;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *SaneadorGlobal)
    {
        if (SaneadorGlobal->objectName().isEmpty())
            SaneadorGlobal->setObjectName("SaneadorGlobal");
        SaneadorGlobal->resize(850, 650);
        SaneadorGlobal->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        verticalLayout = new QVBoxLayout(SaneadorGlobal);
        verticalLayout->setSpacing(15);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(SaneadorGlobal);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame {\n"
"    background-color: #e3f2fd;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #bbdefb;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName("horizontalLayoutHeader");
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName("labelIcono");
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/saneador.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelTitle = new QLabel(headerFrame);
        labelTitle->setObjectName("labelTitle");
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelTitle->setFont(font);
        labelTitle->setStyleSheet(QString::fromUtf8("color: #0d47a1;"));

        horizontalLayoutHeader->addWidget(labelTitle);

        horizontalSpacerHeader = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);

        layoutTiendaSelector = new QHBoxLayout();
        layoutTiendaSelector->setObjectName("layoutTiendaSelector");
        label_3 = new QLabel(headerFrame);
        label_3->setObjectName("label_3");

        layoutTiendaSelector->addWidget(label_3);

        comboBoxTienda = new QComboBox(headerFrame);
        comboBoxTienda->setObjectName("comboBoxTienda");
        comboBoxTienda->setMinimumSize(QSize(200, 35));

        layoutTiendaSelector->addWidget(comboBoxTienda);


        horizontalLayoutHeader->addLayout(layoutTiendaSelector);


        verticalLayout->addWidget(headerFrame);

        labelInfo = new QLabel(SaneadorGlobal);
        labelInfo->setObjectName("labelInfo");
        labelInfo->setWordWrap(true);

        verticalLayout->addWidget(labelInfo);

        tableWidget = new QTableWidget(SaneadorGlobal);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget->setColumnCount(4);

        verticalLayout->addWidget(tableWidget);

        textEditLog = new QTextEdit(SaneadorGlobal);
        textEditLog->setObjectName("textEditLog");
        textEditLog->setMaximumSize(QSize(16777215, 100));
        textEditLog->setReadOnly(true);

        verticalLayout->addWidget(textEditLog);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pushButtonAnalizar = new QPushButton(SaneadorGlobal);
        pushButtonAnalizar->setObjectName("pushButtonAnalizar");
        pushButtonAnalizar->setMinimumSize(QSize(120, 40));

        horizontalLayout->addWidget(pushButtonAnalizar);

        pushButtonIgualar = new QPushButton(SaneadorGlobal);
        pushButtonIgualar->setObjectName("pushButtonIgualar");
        pushButtonIgualar->setMinimumSize(QSize(140, 40));

        horizontalLayout->addWidget(pushButtonIgualar);

        pushButtonIgualarLocal = new QPushButton(SaneadorGlobal);
        pushButtonIgualarLocal->setObjectName("pushButtonIgualarLocal");
        pushButtonIgualarLocal->setMinimumSize(QSize(140, 40));

        horizontalLayout->addWidget(pushButtonIgualarLocal);

        pushButtonClonar = new QPushButton(SaneadorGlobal);
        pushButtonClonar->setObjectName("pushButtonClonar");
        pushButtonClonar->setMinimumSize(QSize(140, 40));

        horizontalLayout->addWidget(pushButtonClonar);

        pushButtonClonarLocal = new QPushButton(SaneadorGlobal);
        pushButtonClonarLocal->setObjectName("pushButtonClonarLocal");
        pushButtonClonarLocal->setMinimumSize(QSize(140, 40));

        horizontalLayout->addWidget(pushButtonClonarLocal);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonCerrar = new QPushButton(SaneadorGlobal);
        pushButtonCerrar->setObjectName("pushButtonCerrar");
        pushButtonCerrar->setMinimumSize(QSize(100, 40));

        horizontalLayout->addWidget(pushButtonCerrar);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(SaneadorGlobal);

        QMetaObject::connectSlotsByName(SaneadorGlobal);
    } // setupUi

    void retranslateUi(QDialog *SaneadorGlobal)
    {
        SaneadorGlobal->setWindowTitle(QCoreApplication::translate("SaneadorGlobal", "Saneador Maestro Global - Alineaci\303\263n Multi-Tienda", nullptr));
        labelTitle->setText(QCoreApplication::translate("SaneadorGlobal", "Alineaci\303\263n de Maestros", nullptr));
        label_3->setText(QCoreApplication::translate("SaneadorGlobal", "Tienda a comparar:", nullptr));
        labelInfo->setText(QCoreApplication::translate("SaneadorGlobal", "Esta herramienta permite igualar los IDs de la tienda remota con los de esta tienda local (maestra). Seleccione los registros marcados para alinear e igualar sus IDs y datos hist\303\263ricos.", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("SaneadorGlobal", "Concepto", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("SaneadorGlobal", "ID Local", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("SaneadorGlobal", "ID Remoto", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("SaneadorGlobal", "Estado / Acci\303\263n", nullptr));
        textEditLog->setPlaceholderText(QCoreApplication::translate("SaneadorGlobal", "Consola de operaciones...", nullptr));
        pushButtonAnalizar->setText(QCoreApplication::translate("SaneadorGlobal", "Refrescar / Analizar", nullptr));
        pushButtonIgualar->setText(QCoreApplication::translate("SaneadorGlobal", "Igualar en Remoto", nullptr));
        pushButtonIgualarLocal->setText(QCoreApplication::translate("SaneadorGlobal", "Igualar en Local", nullptr));
        pushButtonClonar->setText(QCoreApplication::translate("SaneadorGlobal", "Clonar a Remoto", nullptr));
        pushButtonClonarLocal->setText(QCoreApplication::translate("SaneadorGlobal", "Clonar a Local", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("SaneadorGlobal", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SaneadorGlobal: public Ui_SaneadorGlobal {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SANEADORGLOBAL_H
