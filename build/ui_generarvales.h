/********************************************************************************
** Form generated from reading UI file 'generarvales.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENERARVALES_H
#define UI_GENERARVALES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GenerarVales
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QDateEdit *dateEdit;
    QSpacerItem *horizontalSpacer;
    QTableWidget *tableWidget;
    QLabel *label;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *checkBoxSimular;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButtonGenerar;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *GenerarVales)
    {
        if (GenerarVales->objectName().isEmpty())
            GenerarVales->setObjectName("GenerarVales");
        GenerarVales->resize(500, 450);
        verticalLayout = new QVBoxLayout(GenerarVales);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_2 = new QLabel(GenerarVales);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        dateEdit = new QDateEdit(GenerarVales);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setCalendarPopup(true);

        horizontalLayout->addWidget(dateEdit);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        tableWidget = new QTableWidget(GenerarVales);
        if (tableWidget->columnCount() < 5)
            tableWidget->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tableWidget->setObjectName("tableWidget");

        verticalLayout->addWidget(tableWidget);

        label = new QLabel(GenerarVales);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(label);

        progressBar = new QProgressBar(GenerarVales);
        progressBar->setObjectName("progressBar");
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        checkBoxSimular = new QCheckBox(GenerarVales);
        checkBoxSimular->setObjectName("checkBoxSimular");
        checkBoxSimular->setChecked(true);

        horizontalLayout_2->addWidget(checkBoxSimular);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButtonGenerar = new QPushButton(GenerarVales);
        pushButtonGenerar->setObjectName("pushButtonGenerar");

        horizontalLayout_2->addWidget(pushButtonGenerar);

        pushButtonCerrar = new QPushButton(GenerarVales);
        pushButtonCerrar->setObjectName("pushButtonCerrar");

        horizontalLayout_2->addWidget(pushButtonCerrar);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(GenerarVales);
        QObject::connect(pushButtonCerrar, &QPushButton::clicked, GenerarVales, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(GenerarVales);
    } // setupUi

    void retranslateUi(QDialog *GenerarVales)
    {
        GenerarVales->setWindowTitle(QCoreApplication::translate("GenerarVales", "Generar vales de fidelidad", nullptr));
        label_2->setText(QCoreApplication::translate("GenerarVales", "Mes de las ventas:", nullptr));
        dateEdit->setDisplayFormat(QCoreApplication::translate("GenerarVales", "yyyy/MM", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("GenerarVales", "ID Cliente", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("GenerarVales", "Nombre", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("GenerarVales", "Total Ventas", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("GenerarVales", "% Dto", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("GenerarVales", "Importe Vale", nullptr));
        label->setText(QCoreApplication::translate("GenerarVales", "Seleccione el mes y pulse Generar para previsualizar.", nullptr));
        checkBoxSimular->setText(QCoreApplication::translate("GenerarVales", "Modo simulaci\303\263n (no grabar)", nullptr));
        pushButtonGenerar->setText(QCoreApplication::translate("GenerarVales", "Generar / Calcular vales", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("GenerarVales", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GenerarVales: public Ui_GenerarVales {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENERARVALES_H
