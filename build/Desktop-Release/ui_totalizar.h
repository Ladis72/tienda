/********************************************************************************
** Form generated from reading UI file 'totalizar.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOTALIZAR_H
#define UI_TOTALIZAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_totalizar
{
public:
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QGridLayout *gridLayout;
    QLabel *label_6;
    QLabel *label_5;
    QLabel *label_3;
    QLineEdit *lineEditTotal;
    QLineEdit *lineEditEntrega;
    QLabel *label_4;
    QLineEdit *lineEditCambio;
    QLineEdit *lineEditImporte;
    QLineEdit *lineEditDescuento;
    QLabel *label_2;
    QLabel *label_7;
    QLineEdit *lineEditVale;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButtonCobrar;
    QPushButton *pushButtonTicket;
    QPushButton *pushButtonFactura;
    QPushButton *pushButtonSalir;
    QCheckBox *checkBoxVale;

    void setupUi(QDialog *totalizar)
    {
        if (totalizar->objectName().isEmpty())
            totalizar->setObjectName(QString::fromUtf8("totalizar"));
        totalizar->resize(442, 344);
        totalizar->setAutoFillBackground(true);
        gridLayout_2 = new QGridLayout(totalizar);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(totalizar);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(totalizar);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout->addWidget(comboBox);


        gridLayout_2->addLayout(horizontalLayout, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_6 = new QLabel(totalizar);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font);

        gridLayout->addWidget(label_6, 4, 2, 1, 1);

        label_5 = new QLabel(totalizar);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);

        gridLayout->addWidget(label_5, 3, 2, 1, 1);

        label_3 = new QLabel(totalizar);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        gridLayout->addWidget(label_3, 1, 2, 1, 1);

        lineEditTotal = new QLineEdit(totalizar);
        lineEditTotal->setObjectName(QString::fromUtf8("lineEditTotal"));
        lineEditTotal->setEnabled(false);
        QFont font1;
        font1.setPointSize(18);
        lineEditTotal->setFont(font1);

        gridLayout->addWidget(lineEditTotal, 2, 3, 1, 1);

        lineEditEntrega = new QLineEdit(totalizar);
        lineEditEntrega->setObjectName(QString::fromUtf8("lineEditEntrega"));
        lineEditEntrega->setFont(font1);

        gridLayout->addWidget(lineEditEntrega, 3, 3, 1, 1);

        label_4 = new QLabel(totalizar);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);

        gridLayout->addWidget(label_4, 1, 3, 1, 1);

        lineEditCambio = new QLineEdit(totalizar);
        lineEditCambio->setObjectName(QString::fromUtf8("lineEditCambio"));
        lineEditCambio->setEnabled(false);
        lineEditCambio->setFont(font1);

        gridLayout->addWidget(lineEditCambio, 4, 3, 1, 1);

        lineEditImporte = new QLineEdit(totalizar);
        lineEditImporte->setObjectName(QString::fromUtf8("lineEditImporte"));
        lineEditImporte->setEnabled(false);
        lineEditImporte->setFont(font1);

        gridLayout->addWidget(lineEditImporte, 2, 0, 1, 1);

        lineEditDescuento = new QLineEdit(totalizar);
        lineEditDescuento->setObjectName(QString::fromUtf8("lineEditDescuento"));
        lineEditDescuento->setFont(font1);
        lineEditDescuento->setMaxLength(3);

        gridLayout->addWidget(lineEditDescuento, 2, 2, 1, 1);

        label_2 = new QLabel(totalizar);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_7 = new QLabel(totalizar);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font);

        gridLayout->addWidget(label_7, 1, 1, 1, 1);

        lineEditVale = new QLineEdit(totalizar);
        lineEditVale->setObjectName(QString::fromUtf8("lineEditVale"));
        lineEditVale->setEnabled(false);
        QFont font2;
        font2.setPointSize(18);
        font2.setBold(true);
        lineEditVale->setFont(font2);

        gridLayout->addWidget(lineEditVale, 2, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButtonCobrar = new QPushButton(totalizar);
        pushButtonCobrar->setObjectName(QString::fromUtf8("pushButtonCobrar"));

        horizontalLayout_2->addWidget(pushButtonCobrar);

        pushButtonTicket = new QPushButton(totalizar);
        pushButtonTicket->setObjectName(QString::fromUtf8("pushButtonTicket"));

        horizontalLayout_2->addWidget(pushButtonTicket);

        pushButtonFactura = new QPushButton(totalizar);
        pushButtonFactura->setObjectName(QString::fromUtf8("pushButtonFactura"));

        horizontalLayout_2->addWidget(pushButtonFactura);

        pushButtonSalir = new QPushButton(totalizar);
        pushButtonSalir->setObjectName(QString::fromUtf8("pushButtonSalir"));

        horizontalLayout_2->addWidget(pushButtonSalir);


        gridLayout_2->addLayout(horizontalLayout_2, 3, 0, 1, 1);

        checkBoxVale = new QCheckBox(totalizar);
        checkBoxVale->setObjectName(QString::fromUtf8("checkBoxVale"));

        gridLayout_2->addWidget(checkBoxVale, 1, 0, 1, 1);

#if QT_CONFIG(shortcut)
        label_6->setBuddy(lineEditCambio);
        label_5->setBuddy(lineEditEntrega);
        label_3->setBuddy(lineEditDescuento);
        label_4->setBuddy(lineEditTotal);
        label_2->setBuddy(lineEditImporte);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(lineEditEntrega, pushButtonCobrar);
        QWidget::setTabOrder(pushButtonCobrar, pushButtonTicket);
        QWidget::setTabOrder(pushButtonTicket, pushButtonFactura);
        QWidget::setTabOrder(pushButtonFactura, pushButtonSalir);
        QWidget::setTabOrder(pushButtonSalir, lineEditCambio);
        QWidget::setTabOrder(lineEditCambio, lineEditDescuento);
        QWidget::setTabOrder(lineEditDescuento, lineEditImporte);
        QWidget::setTabOrder(lineEditImporte, lineEditTotal);
        QWidget::setTabOrder(lineEditTotal, comboBox);

        retranslateUi(totalizar);
        QObject::connect(pushButtonSalir, SIGNAL(clicked()), totalizar, SLOT(reject()));
        QObject::connect(pushButtonCobrar, SIGNAL(clicked()), totalizar, SLOT(accept()));

        QMetaObject::connectSlotsByName(totalizar);
    } // setupUi

    void retranslateUi(QDialog *totalizar)
    {
        totalizar->setWindowTitle(QCoreApplication::translate("totalizar", "Totalizaci\303\263n", nullptr));
        label->setText(QCoreApplication::translate("totalizar", "Forma de pago", nullptr));
        label_6->setText(QCoreApplication::translate("totalizar", "Cambio", nullptr));
        label_5->setText(QCoreApplication::translate("totalizar", "E&ntrega", nullptr));
        label_3->setText(QCoreApplication::translate("totalizar", "Descuento", nullptr));
        label_4->setText(QCoreApplication::translate("totalizar", "Tota&l", nullptr));
        lineEditDescuento->setText(QCoreApplication::translate("totalizar", "0", nullptr));
        label_2->setText(QCoreApplication::translate("totalizar", "I&mporte", nullptr));
        label_7->setText(QCoreApplication::translate("totalizar", "Vale", nullptr));
        pushButtonCobrar->setText(QCoreApplication::translate("totalizar", "Cobrar", nullptr));
#if QT_CONFIG(shortcut)
        pushButtonCobrar->setShortcut(QCoreApplication::translate("totalizar", "F8", nullptr));
#endif // QT_CONFIG(shortcut)
        pushButtonTicket->setText(QCoreApplication::translate("totalizar", "Ticket", nullptr));
#if QT_CONFIG(shortcut)
        pushButtonTicket->setShortcut(QCoreApplication::translate("totalizar", "F9", nullptr));
#endif // QT_CONFIG(shortcut)
        pushButtonFactura->setText(QCoreApplication::translate("totalizar", "Factura", nullptr));
        pushButtonSalir->setText(QCoreApplication::translate("totalizar", "Salir", nullptr));
#if QT_CONFIG(shortcut)
        pushButtonSalir->setShortcut(QCoreApplication::translate("totalizar", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        checkBoxVale->setText(QCoreApplication::translate("totalizar", "Usar vale", nullptr));
    } // retranslateUi

};

namespace Ui {
    class totalizar: public Ui_totalizar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOTALIZAR_H
