/********************************************************************************
** Form generated from reading UI file 'cambiodatosproducto.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMBIODATOSPRODUCTO_H
#define UI_CAMBIODATOSPRODUCTO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_CambioDatosProducto
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout_3;
    QRadioButton *radioButtonNombrePedido;
    QRadioButton *radioButtonNombreAlmacen;
    QRadioButton *radioButtonNombrePersonal;
    QLineEdit *lineEditNombrePersonal;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QRadioButton *radioButtonPVPPedido;
    QRadioButton *radioButtonPVPAlmacen;
    QRadioButton *radioButtonPVPPersonal;
    QLineEdit *lineEditPVPPersonal;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QRadioButton *radioButtonDatosAlmacen;
    QRadioButton *radioButtonDatosPedido;
    QRadioButton *radioButton;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_3;
    QLabel *label_2;
    QLabel *label;
    QLabel *label_3;
    QPushButton *pushButton;

    void setupUi(QDialog *CambioDatosProducto)
    {
        if (CambioDatosProducto->objectName().isEmpty())
            CambioDatosProducto->setObjectName(QString::fromUtf8("CambioDatosProducto"));
        CambioDatosProducto->resize(700, 283);
        gridLayout = new QGridLayout(CambioDatosProducto);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_2 = new QGroupBox(CambioDatosProducto);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        formLayout_3 = new QFormLayout(groupBox_2);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy::AllNonFixedFieldsGrow);
        radioButtonNombrePedido = new QRadioButton(groupBox_2);
        radioButtonNombrePedido->setObjectName(QString::fromUtf8("radioButtonNombrePedido"));
        radioButtonNombrePedido->setChecked(true);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, radioButtonNombrePedido);

        radioButtonNombreAlmacen = new QRadioButton(groupBox_2);
        radioButtonNombreAlmacen->setObjectName(QString::fromUtf8("radioButtonNombreAlmacen"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, radioButtonNombreAlmacen);

        radioButtonNombrePersonal = new QRadioButton(groupBox_2);
        radioButtonNombrePersonal->setObjectName(QString::fromUtf8("radioButtonNombrePersonal"));

        formLayout_3->setWidget(2, QFormLayout::FieldRole, radioButtonNombrePersonal);

        lineEditNombrePersonal = new QLineEdit(groupBox_2);
        lineEditNombrePersonal->setObjectName(QString::fromUtf8("lineEditNombrePersonal"));

        formLayout_3->setWidget(3, QFormLayout::FieldRole, lineEditNombrePersonal);


        gridLayout->addWidget(groupBox_2, 0, 0, 3, 1);

        groupBox = new QGroupBox(CambioDatosProducto);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy::AllNonFixedFieldsGrow);
        radioButtonPVPPedido = new QRadioButton(groupBox);
        radioButtonPVPPedido->setObjectName(QString::fromUtf8("radioButtonPVPPedido"));
        radioButtonPVPPedido->setChecked(true);

        formLayout->setWidget(0, QFormLayout::LabelRole, radioButtonPVPPedido);

        radioButtonPVPAlmacen = new QRadioButton(groupBox);
        radioButtonPVPAlmacen->setObjectName(QString::fromUtf8("radioButtonPVPAlmacen"));

        formLayout->setWidget(1, QFormLayout::LabelRole, radioButtonPVPAlmacen);

        radioButtonPVPPersonal = new QRadioButton(groupBox);
        radioButtonPVPPersonal->setObjectName(QString::fromUtf8("radioButtonPVPPersonal"));

        formLayout->setWidget(2, QFormLayout::LabelRole, radioButtonPVPPersonal);

        lineEditPVPPersonal = new QLineEdit(groupBox);
        lineEditPVPPersonal->setObjectName(QString::fromUtf8("lineEditPVPPersonal"));

        formLayout->setWidget(3, QFormLayout::LabelRole, lineEditPVPPersonal);


        gridLayout->addWidget(groupBox, 0, 2, 3, 1);

        groupBox_3 = new QGroupBox(CambioDatosProducto);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        radioButtonDatosAlmacen = new QRadioButton(groupBox_3);
        radioButtonDatosAlmacen->setObjectName(QString::fromUtf8("radioButtonDatosAlmacen"));

        gridLayout_2->addWidget(radioButtonDatosAlmacen, 0, 1, 1, 1);

        radioButtonDatosPedido = new QRadioButton(groupBox_3);
        radioButtonDatosPedido->setObjectName(QString::fromUtf8("radioButtonDatosPedido"));

        gridLayout_2->addWidget(radioButtonDatosPedido, 0, 0, 1, 1);

        radioButton = new QRadioButton(groupBox_3);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setChecked(true);

        gridLayout_2->addWidget(radioButton, 0, 2, 1, 1);


        gridLayout->addWidget(groupBox_3, 3, 0, 1, 3);

        groupBox_4 = new QGroupBox(CambioDatosProducto);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_3 = new QGridLayout(groupBox_4);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 2, 0, 1, 1);

        label = new QLabel(groupBox_4);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 1, 0, 1, 1);

        label_3 = new QLabel(groupBox_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_3->addWidget(label_3, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_4, 0, 1, 2, 1);

        pushButton = new QPushButton(CambioDatosProducto);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 4, 2, 1, 1);


        retranslateUi(CambioDatosProducto);
        QObject::connect(pushButton, SIGNAL(clicked()), CambioDatosProducto, SLOT(accept()));

        QMetaObject::connectSlotsByName(CambioDatosProducto);
    } // setupUi

    void retranslateUi(QDialog *CambioDatosProducto)
    {
        CambioDatosProducto->setWindowTitle(QCoreApplication::translate("CambioDatosProducto", "Dialog", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("CambioDatosProducto", "Nombre", nullptr));
        radioButtonNombrePedido->setText(QCoreApplication::translate("CambioDatosProducto", "RadioButton", nullptr));
        radioButtonNombreAlmacen->setText(QCoreApplication::translate("CambioDatosProducto", "RadioButton", nullptr));
        radioButtonNombrePersonal->setText(QCoreApplication::translate("CambioDatosProducto", "Personalizado", nullptr));
        groupBox->setTitle(QCoreApplication::translate("CambioDatosProducto", "P.V.P.", nullptr));
        radioButtonPVPPedido->setText(QCoreApplication::translate("CambioDatosProducto", "RadioButton", nullptr));
        radioButtonPVPAlmacen->setText(QCoreApplication::translate("CambioDatosProducto", "RadioButton", nullptr));
        radioButtonPVPPersonal->setText(QCoreApplication::translate("CambioDatosProducto", "Personalizado", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("CambioDatosProducto", "Datos automaticos", nullptr));
        radioButtonDatosAlmacen->setText(QCoreApplication::translate("CambioDatosProducto", "&Utilizar siempre datos del almacen", nullptr));
        radioButtonDatosPedido->setText(QCoreApplication::translate("CambioDatosProducto", "Utili&zar siempre datos del pedido", nullptr));
        radioButton->setText(QCoreApplication::translate("CambioDatosProducto", "Preguntar sie&mpre", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("CambioDatosProducto", "Origen", nullptr));
        label_2->setText(QCoreApplication::translate("CambioDatosProducto", "Personal", nullptr));
        label->setText(QCoreApplication::translate("CambioDatosProducto", "Almacen", nullptr));
        label_3->setText(QCoreApplication::translate("CambioDatosProducto", "Pedido", nullptr));
        pushButton->setText(QCoreApplication::translate("CambioDatosProducto", "Aceptar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CambioDatosProducto: public Ui_CambioDatosProducto {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMBIODATOSPRODUCTO_H
