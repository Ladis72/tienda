/********************************************************************************
** Form generated from reading UI file 'dialoganadirapedido.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGANADIRAPEDIDO_H
#define UI_DIALOGANADIRAPEDIDO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogAnadirAPedido
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboBoxPedidos;
    QPushButton *pushButtonNuevo;
    QFormLayout *formLayout;
    QLabel *labelCantidad;
    QDoubleSpinBox *doubleSpinBoxCantidad;
    QLabel *labelBonificacion;
    QDoubleSpinBox *doubleSpinBoxBonificacion;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonAceptar;
    QPushButton *pushButtonCancelar;

    void setupUi(QDialog *DialogAnadirAPedido)
    {
        if (DialogAnadirAPedido->objectName().isEmpty())
            DialogAnadirAPedido->setObjectName("DialogAnadirAPedido");
        DialogAnadirAPedido->resize(400, 250);
        verticalLayout = new QVBoxLayout(DialogAnadirAPedido);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(DialogAnadirAPedido);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        comboBoxPedidos = new QComboBox(DialogAnadirAPedido);
        comboBoxPedidos->setObjectName("comboBoxPedidos");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboBoxPedidos->sizePolicy().hasHeightForWidth());
        comboBoxPedidos->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(comboBoxPedidos);

        pushButtonNuevo = new QPushButton(DialogAnadirAPedido);
        pushButtonNuevo->setObjectName("pushButtonNuevo");

        horizontalLayout->addWidget(pushButtonNuevo);


        verticalLayout->addLayout(horizontalLayout);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        labelCantidad = new QLabel(DialogAnadirAPedido);
        labelCantidad->setObjectName("labelCantidad");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, labelCantidad);

        doubleSpinBoxCantidad = new QDoubleSpinBox(DialogAnadirAPedido);
        doubleSpinBoxCantidad->setObjectName("doubleSpinBoxCantidad");
        doubleSpinBoxCantidad->setMinimum(0.010000000000000);
        doubleSpinBoxCantidad->setMaximum(99999.990000000005239);
        doubleSpinBoxCantidad->setValue(1.000000000000000);

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, doubleSpinBoxCantidad);

        labelBonificacion = new QLabel(DialogAnadirAPedido);
        labelBonificacion->setObjectName("labelBonificacion");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, labelBonificacion);

        doubleSpinBoxBonificacion = new QDoubleSpinBox(DialogAnadirAPedido);
        doubleSpinBoxBonificacion->setObjectName("doubleSpinBoxBonificacion");
        doubleSpinBoxBonificacion->setMinimum(0.000000000000000);
        doubleSpinBoxBonificacion->setMaximum(99999.990000000005239);
        doubleSpinBoxBonificacion->setValue(0.000000000000000);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, doubleSpinBoxBonificacion);


        verticalLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButtonAceptar = new QPushButton(DialogAnadirAPedido);
        pushButtonAceptar->setObjectName("pushButtonAceptar");

        horizontalLayout_2->addWidget(pushButtonAceptar);

        pushButtonCancelar = new QPushButton(DialogAnadirAPedido);
        pushButtonCancelar->setObjectName("pushButtonCancelar");

        horizontalLayout_2->addWidget(pushButtonCancelar);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(DialogAnadirAPedido);
        QObject::connect(pushButtonCancelar, &QPushButton::clicked, DialogAnadirAPedido, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(DialogAnadirAPedido);
    } // setupUi

    void retranslateUi(QDialog *DialogAnadirAPedido)
    {
        DialogAnadirAPedido->setWindowTitle(QCoreApplication::translate("DialogAnadirAPedido", "A\303\261adir a Pedido", nullptr));
        label->setText(QCoreApplication::translate("DialogAnadirAPedido", "Seleccione un pedido pendiente o cree uno nuevo:", nullptr));
        pushButtonNuevo->setText(QCoreApplication::translate("DialogAnadirAPedido", "Nuevo", nullptr));
        labelCantidad->setText(QCoreApplication::translate("DialogAnadirAPedido", "Cantidad:", nullptr));
        labelBonificacion->setText(QCoreApplication::translate("DialogAnadirAPedido", "Bonificaci\303\263n:", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("DialogAnadirAPedido", "Aceptar", nullptr));
        pushButtonCancelar->setText(QCoreApplication::translate("DialogAnadirAPedido", "Cancelar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogAnadirAPedido: public Ui_DialogAnadirAPedido {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGANADIRAPEDIDO_H
