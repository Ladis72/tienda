/********************************************************************************
** Form generated from reading UI file 'aceptarpedido.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACEPTARPEDIDO_H
#define UI_ACEPTARPEDIDO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_AceptarPedido
{
public:
    QGridLayout *gridLayout_2;
    QDialogButtonBox *buttonBox;
    QPushButton *pushButton;
    QFrame *line_4;
    QGridLayout *gridLayout_3;
    QLabel *label_11;
    QLabel *label_10;
    QLabel *label;
    QComboBox *comboBox;
    QLabel *label_12;
    QDateEdit *dateEditDocumento;
    QLabel *labelProveedor;
    QDateEdit *dateEditVencimiento;
    QDoubleSpinBox *doubleSpinBox;
    QLineEdit *lineEditNDoc;
    QFormLayout *formLayout;
    QLabel *label_2;
    QLineEdit *leLineas;
    QLabel *label_7;
    QLineEdit *leTotalBase;
    QLabel *label_6;
    QLineEdit *leTotalIva;
    QLabel *label_5;
    QLineEdit *leTotalRe;
    QLabel *label_4;
    QLineEdit *leTotal;
    QTableView *tableView;

    void setupUi(QDialog *AceptarPedido)
    {
        if (AceptarPedido->objectName().isEmpty())
            AceptarPedido->setObjectName(QString::fromUtf8("AceptarPedido"));
        AceptarPedido->resize(1031, 351);
        gridLayout_2 = new QGridLayout(AceptarPedido);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        buttonBox = new QDialogButtonBox(AceptarPedido);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::NoButton);

        gridLayout_2->addWidget(buttonBox, 3, 1, 1, 1);

        pushButton = new QPushButton(AceptarPedido);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout_2->addWidget(pushButton, 2, 1, 1, 1);

        line_4 = new QFrame(AceptarPedido);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_4, 1, 1, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_11 = new QLabel(AceptarPedido);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_3->addWidget(label_11, 1, 5, 1, 1);

        label_10 = new QLabel(AceptarPedido);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_3->addWidget(label_10, 1, 2, 1, 1);

        label = new QLabel(AceptarPedido);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 1, 7, 1, 1);

        comboBox = new QComboBox(AceptarPedido);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        gridLayout_3->addWidget(comboBox, 2, 0, 1, 1);

        label_12 = new QLabel(AceptarPedido);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_3->addWidget(label_12, 1, 1, 1, 1);

        dateEditDocumento = new QDateEdit(AceptarPedido);
        dateEditDocumento->setObjectName(QString::fromUtf8("dateEditDocumento"));
        dateEditDocumento->setCalendarPopup(true);

        gridLayout_3->addWidget(dateEditDocumento, 2, 1, 1, 1);

        labelProveedor = new QLabel(AceptarPedido);
        labelProveedor->setObjectName(QString::fromUtf8("labelProveedor"));

        gridLayout_3->addWidget(labelProveedor, 2, 3, 1, 1);

        dateEditVencimiento = new QDateEdit(AceptarPedido);
        dateEditVencimiento->setObjectName(QString::fromUtf8("dateEditVencimiento"));
        dateEditVencimiento->setEnabled(false);
        dateEditVencimiento->setCalendarPopup(true);

        gridLayout_3->addWidget(dateEditVencimiento, 2, 5, 1, 1);

        doubleSpinBox = new QDoubleSpinBox(AceptarPedido);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));

        gridLayout_3->addWidget(doubleSpinBox, 2, 7, 1, 1);

        lineEditNDoc = new QLineEdit(AceptarPedido);
        lineEditNDoc->setObjectName(QString::fromUtf8("lineEditNDoc"));
        lineEditNDoc->setMaximumSize(QSize(120, 16777215));

        gridLayout_3->addWidget(lineEditNDoc, 2, 2, 1, 1);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_2 = new QLabel(AceptarPedido);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        leLineas = new QLineEdit(AceptarPedido);
        leLineas->setObjectName(QString::fromUtf8("leLineas"));
        leLineas->setEnabled(false);

        formLayout->setWidget(0, QFormLayout::FieldRole, leLineas);

        label_7 = new QLabel(AceptarPedido);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_7);

        leTotalBase = new QLineEdit(AceptarPedido);
        leTotalBase->setObjectName(QString::fromUtf8("leTotalBase"));
        leTotalBase->setEnabled(false);

        formLayout->setWidget(1, QFormLayout::FieldRole, leTotalBase);

        label_6 = new QLabel(AceptarPedido);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_6);

        leTotalIva = new QLineEdit(AceptarPedido);
        leTotalIva->setObjectName(QString::fromUtf8("leTotalIva"));
        leTotalIva->setEnabled(false);

        formLayout->setWidget(2, QFormLayout::FieldRole, leTotalIva);

        label_5 = new QLabel(AceptarPedido);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_5);

        leTotalRe = new QLineEdit(AceptarPedido);
        leTotalRe->setObjectName(QString::fromUtf8("leTotalRe"));
        leTotalRe->setEnabled(false);

        formLayout->setWidget(3, QFormLayout::FieldRole, leTotalRe);

        label_4 = new QLabel(AceptarPedido);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QFont font;
        font.setPointSize(14);
        label_4->setFont(font);

        formLayout->setWidget(4, QFormLayout::LabelRole, label_4);

        leTotal = new QLineEdit(AceptarPedido);
        leTotal->setObjectName(QString::fromUtf8("leTotal"));
        leTotal->setEnabled(false);
        leTotal->setFont(font);

        formLayout->setWidget(4, QFormLayout::FieldRole, leTotal);


        gridLayout_3->addLayout(formLayout, 3, 6, 1, 2);

        tableView = new QTableView(AceptarPedido);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout_3->addWidget(tableView, 3, 0, 1, 6);


        gridLayout_2->addLayout(gridLayout_3, 0, 1, 1, 1);

        QWidget::setTabOrder(comboBox, dateEditDocumento);
        QWidget::setTabOrder(dateEditDocumento, lineEditNDoc);
        QWidget::setTabOrder(lineEditNDoc, dateEditVencimiento);
        QWidget::setTabOrder(dateEditVencimiento, doubleSpinBox);
        QWidget::setTabOrder(doubleSpinBox, pushButton);

        retranslateUi(AceptarPedido);
        QObject::connect(buttonBox, SIGNAL(accepted()), AceptarPedido, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AceptarPedido, SLOT(reject()));

        QMetaObject::connectSlotsByName(AceptarPedido);
    } // setupUi

    void retranslateUi(QDialog *AceptarPedido)
    {
        AceptarPedido->setWindowTitle(QCoreApplication::translate("AceptarPedido", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("AceptarPedido", "Aceptar pedido", nullptr));
        label_11->setText(QCoreApplication::translate("AceptarPedido", "Fecha vencimiento", nullptr));
        label_10->setText(QCoreApplication::translate("AceptarPedido", "N\302\272 Documento", nullptr));
        label->setText(QCoreApplication::translate("AceptarPedido", "Descuento", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("AceptarPedido", "Seleccionar", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("AceptarPedido", "Factura", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("AceptarPedido", "Albar\303\241n", nullptr));

        label_12->setText(QCoreApplication::translate("AceptarPedido", "Fecha Documento", nullptr));
        dateEditDocumento->setDisplayFormat(QCoreApplication::translate("AceptarPedido", "yyyy/MM/dd", nullptr));
        labelProveedor->setText(QCoreApplication::translate("AceptarPedido", "Proveedor", nullptr));
        dateEditVencimiento->setDisplayFormat(QCoreApplication::translate("AceptarPedido", "yyyy/MM/dd", nullptr));
        label_2->setText(QCoreApplication::translate("AceptarPedido", "Lineas", nullptr));
        label_7->setText(QCoreApplication::translate("AceptarPedido", "Base", nullptr));
        label_6->setText(QCoreApplication::translate("AceptarPedido", "IVA", nullptr));
        label_5->setText(QCoreApplication::translate("AceptarPedido", "Recargo", nullptr));
        label_4->setText(QCoreApplication::translate("AceptarPedido", "Total", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AceptarPedido: public Ui_AceptarPedido {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACEPTARPEDIDO_H
