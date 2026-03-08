/********************************************************************************
** Form generated from reading UI file 'configticket.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGTICKET_H
#define UI_CONFIGTICKET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConfigTicket
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBoxCabecera;
    QGridLayout *gridLayout;
    QPlainTextEdit *plainTextEditCabecera;
    QGroupBox *groupBoxPie;
    QGridLayout *gridLayout_2;
    QPlainTextEdit *plainTextEditPie;
    QFormLayout *formLayout;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QLineEdit *lineEditCola;
    QLabel *label_2;
    QLineEdit *lineEditCodApertura;
    QLabel *label_3;
    QLineEdit *lineEditCodCorte;
    QGroupBox *groupBoxPromo;
    QGridLayout *gridLayout_3;
    QCheckBox *checkBoxPromo;
    QPlainTextEdit *plainTextEditPromo;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_4;
    QPushButton *pushButtonAceptar;
    QPushButton *pushButtonCancelar;

    void setupUi(QDialog *ConfigTicket)
    {
        if (ConfigTicket->objectName().isEmpty())
            ConfigTicket->setObjectName("ConfigTicket");
        ConfigTicket->resize(898, 527);
        horizontalLayout = new QHBoxLayout(ConfigTicket);
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        groupBoxCabecera = new QGroupBox(ConfigTicket);
        groupBoxCabecera->setObjectName("groupBoxCabecera");
        QFont font;
        font.setBold(true);
        groupBoxCabecera->setFont(font);
        groupBoxCabecera->setFlat(true);
        groupBoxCabecera->setCheckable(false);
        gridLayout = new QGridLayout(groupBoxCabecera);
        gridLayout->setObjectName("gridLayout");
        plainTextEditCabecera = new QPlainTextEdit(groupBoxCabecera);
        plainTextEditCabecera->setObjectName("plainTextEditCabecera");
        plainTextEditCabecera->setFrameShape(QFrame::Shape::StyledPanel);
        plainTextEditCabecera->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(plainTextEditCabecera, 0, 0, 1, 1);

        groupBoxPie = new QGroupBox(groupBoxCabecera);
        groupBoxPie->setObjectName("groupBoxPie");
        groupBoxPie->setFont(font);
        groupBoxPie->setFlat(true);
        groupBoxPie->setCheckable(false);
        gridLayout_2 = new QGridLayout(groupBoxPie);
        gridLayout_2->setObjectName("gridLayout_2");
        plainTextEditPie = new QPlainTextEdit(groupBoxPie);
        plainTextEditPie->setObjectName("plainTextEditPie");

        gridLayout_2->addWidget(plainTextEditPie, 0, 0, 1, 1);


        gridLayout->addWidget(groupBoxPie, 1, 0, 1, 1);


        verticalLayout_2->addWidget(groupBoxCabecera);


        horizontalLayout->addLayout(verticalLayout_2);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        formLayout->setItem(0, QFormLayout::ItemRole::LabelRole, verticalSpacer);

        label = new QLabel(ConfigTicket);
        label->setObjectName("label");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label);

        lineEditCola = new QLineEdit(ConfigTicket);
        lineEditCola->setObjectName("lineEditCola");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, lineEditCola);

        label_2 = new QLabel(ConfigTicket);
        label_2->setObjectName("label_2");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_2);

        lineEditCodApertura = new QLineEdit(ConfigTicket);
        lineEditCodApertura->setObjectName("lineEditCodApertura");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, lineEditCodApertura);

        label_3 = new QLabel(ConfigTicket);
        label_3->setObjectName("label_3");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_3);

        lineEditCodCorte = new QLineEdit(ConfigTicket);
        lineEditCodCorte->setObjectName("lineEditCodCorte");

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, lineEditCodCorte);

        groupBoxPromo = new QGroupBox(ConfigTicket);
        groupBoxPromo->setObjectName("groupBoxPromo");
        groupBoxPromo->setFont(font);
        groupBoxPromo->setFlat(true);
        gridLayout_3 = new QGridLayout(groupBoxPromo);
        gridLayout_3->setObjectName("gridLayout_3");
        checkBoxPromo = new QCheckBox(groupBoxPromo);
        checkBoxPromo->setObjectName("checkBoxPromo");
        QFont font1;
        font1.setBold(false);
        checkBoxPromo->setFont(font1);

        gridLayout_3->addWidget(checkBoxPromo, 0, 0, 1, 1);

        plainTextEditPromo = new QPlainTextEdit(groupBoxPromo);
        plainTextEditPromo->setObjectName("plainTextEditPromo");

        gridLayout_3->addWidget(plainTextEditPromo, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 2, 0, 1, 1);


        formLayout->setWidget(4, QFormLayout::ItemRole::SpanningRole, groupBoxPromo);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        formLayout->setItem(5, QFormLayout::ItemRole::FieldRole, verticalSpacer_3);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        formLayout->setItem(6, QFormLayout::ItemRole::FieldRole, verticalSpacer_4);

        pushButtonAceptar = new QPushButton(ConfigTicket);
        pushButtonAceptar->setObjectName("pushButtonAceptar");

        formLayout->setWidget(8, QFormLayout::ItemRole::LabelRole, pushButtonAceptar);

        pushButtonCancelar = new QPushButton(ConfigTicket);
        pushButtonCancelar->setObjectName("pushButtonCancelar");

        formLayout->setWidget(8, QFormLayout::ItemRole::FieldRole, pushButtonCancelar);


        horizontalLayout->addLayout(formLayout);


        retranslateUi(ConfigTicket);

        QMetaObject::connectSlotsByName(ConfigTicket);
    } // setupUi

    void retranslateUi(QDialog *ConfigTicket)
    {
        ConfigTicket->setWindowTitle(QCoreApplication::translate("ConfigTicket", "Configurar tickets", nullptr));
        groupBoxCabecera->setTitle(QCoreApplication::translate("ConfigTicket", "Cabecera", nullptr));
        groupBoxPie->setTitle(QCoreApplication::translate("ConfigTicket", "Pie", nullptr));
        label->setText(QCoreApplication::translate("ConfigTicket", "Ruta impresora de tickets:", nullptr));
        label_2->setText(QCoreApplication::translate("ConfigTicket", "C\303\263digo apertura del caj\303\263n", nullptr));
        label_3->setText(QCoreApplication::translate("ConfigTicket", "C\303\263digo de corte del papel", nullptr));
        groupBoxPromo->setTitle(QCoreApplication::translate("ConfigTicket", "Ticket promocional", nullptr));
        checkBoxPromo->setText(QCoreApplication::translate("ConfigTicket", "Imprimir Ticket promocinal", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("ConfigTicket", "Guardar", nullptr));
        pushButtonCancelar->setText(QCoreApplication::translate("ConfigTicket", "Cancelar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfigTicket: public Ui_ConfigTicket {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGTICKET_H
