/********************************************************************************
** Form generated from reading UI file 'pedidonuevo.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PEDIDONUEVO_H
#define UI_PEDIDONUEVO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_PedidoNuevo
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEditIdProveedor;
    QLineEdit *lineEditProveedor;
    QLabel *label_2;
    QLineEdit *lineEditDocumento;
    QLabel *label_3;
    QDateEdit *dateEditDocumento;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonAceptar;
    QPushButton *pushButtonCancelar;

    void setupUi(QDialog *PedidoNuevo)
    {
        if (PedidoNuevo->objectName().isEmpty())
            PedidoNuevo->setObjectName("PedidoNuevo");
        PedidoNuevo->resize(324, 162);
        PedidoNuevo->setSizeGripEnabled(false);
        gridLayout = new QGridLayout(PedidoNuevo);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(PedidoNuevo);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEditIdProveedor = new QLineEdit(PedidoNuevo);
        lineEditIdProveedor->setObjectName("lineEditIdProveedor");
        lineEditIdProveedor->setMaximumSize(QSize(30, 16777215));

        gridLayout->addWidget(lineEditIdProveedor, 0, 1, 1, 1);

        lineEditProveedor = new QLineEdit(PedidoNuevo);
        lineEditProveedor->setObjectName("lineEditProveedor");

        gridLayout->addWidget(lineEditProveedor, 0, 2, 1, 1);

        label_2 = new QLabel(PedidoNuevo);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEditDocumento = new QLineEdit(PedidoNuevo);
        lineEditDocumento->setObjectName("lineEditDocumento");

        gridLayout->addWidget(lineEditDocumento, 1, 2, 1, 1);

        label_3 = new QLabel(PedidoNuevo);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        dateEditDocumento = new QDateEdit(PedidoNuevo);
        dateEditDocumento->setObjectName("dateEditDocumento");
        dateEditDocumento->setWrapping(false);
        dateEditDocumento->setCalendarPopup(true);

        gridLayout->addWidget(dateEditDocumento, 2, 2, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonAceptar = new QPushButton(PedidoNuevo);
        pushButtonAceptar->setObjectName("pushButtonAceptar");
        pushButtonAceptar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonAceptar);

        pushButtonCancelar = new QPushButton(PedidoNuevo);
        pushButtonCancelar->setObjectName("pushButtonCancelar");
        pushButtonCancelar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonCancelar);


        gridLayout->addLayout(horizontalLayout, 3, 0, 1, 3);

#if QT_CONFIG(shortcut)
        label->setBuddy(lineEditIdProveedor);
        label_2->setBuddy(lineEditDocumento);
        label_3->setBuddy(dateEditDocumento);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(lineEditIdProveedor, lineEditProveedor);
        QWidget::setTabOrder(lineEditProveedor, lineEditDocumento);
        QWidget::setTabOrder(lineEditDocumento, dateEditDocumento);

        retranslateUi(PedidoNuevo);
        QObject::connect(pushButtonAceptar, &QPushButton::clicked, PedidoNuevo, qOverload<>(&QDialog::accept));
        QObject::connect(pushButtonCancelar, &QPushButton::clicked, PedidoNuevo, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(PedidoNuevo);
    } // setupUi

    void retranslateUi(QDialog *PedidoNuevo)
    {
        PedidoNuevo->setWindowTitle(QCoreApplication::translate("PedidoNuevo", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("PedidoNuevo", "Proveedor", nullptr));
        label_2->setText(QCoreApplication::translate("PedidoNuevo", "N\302\272. documento", nullptr));
        label_3->setText(QCoreApplication::translate("PedidoNuevo", "Fecha documento", nullptr));
        dateEditDocumento->setDisplayFormat(QCoreApplication::translate("PedidoNuevo", "yyyy-MM-dd", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("PedidoNuevo", "Aceptar", nullptr));
        pushButtonCancelar->setText(QCoreApplication::translate("PedidoNuevo", "Cancelar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PedidoNuevo: public Ui_PedidoNuevo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PEDIDONUEVO_H
