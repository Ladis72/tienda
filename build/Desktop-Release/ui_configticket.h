/********************************************************************************
** Form generated from reading UI file 'configticket.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
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
            ConfigTicket->setObjectName(QString::fromUtf8("ConfigTicket"));
        ConfigTicket->resize(898, 527);
        horizontalLayout = new QHBoxLayout(ConfigTicket);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBoxCabecera = new QGroupBox(ConfigTicket);
        groupBoxCabecera->setObjectName(QString::fromUtf8("groupBoxCabecera"));
        QFont font;
        font.setBold(true);
        groupBoxCabecera->setFont(font);
        groupBoxCabecera->setFlat(true);
        groupBoxCabecera->setCheckable(false);
        gridLayout = new QGridLayout(groupBoxCabecera);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        plainTextEditCabecera = new QPlainTextEdit(groupBoxCabecera);
        plainTextEditCabecera->setObjectName(QString::fromUtf8("plainTextEditCabecera"));
        plainTextEditCabecera->setFrameShape(QFrame::Shape::StyledPanel);
        plainTextEditCabecera->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(plainTextEditCabecera, 0, 0, 1, 1);

        groupBoxPie = new QGroupBox(groupBoxCabecera);
        groupBoxPie->setObjectName(QString::fromUtf8("groupBoxPie"));
        groupBoxPie->setFont(font);
        groupBoxPie->setFlat(true);
        groupBoxPie->setCheckable(false);
        gridLayout_2 = new QGridLayout(groupBoxPie);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        plainTextEditPie = new QPlainTextEdit(groupBoxPie);
        plainTextEditPie->setObjectName(QString::fromUtf8("plainTextEditPie"));

        gridLayout_2->addWidget(plainTextEditPie, 0, 0, 1, 1);


        gridLayout->addWidget(groupBoxPie, 1, 0, 1, 1);


        verticalLayout_2->addWidget(groupBoxCabecera);


        horizontalLayout->addLayout(verticalLayout_2);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(0, QFormLayout::LabelRole, verticalSpacer);

        label = new QLabel(ConfigTicket);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label);

        lineEditCola = new QLineEdit(ConfigTicket);
        lineEditCola->setObjectName(QString::fromUtf8("lineEditCola"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEditCola);

        label_2 = new QLabel(ConfigTicket);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        lineEditCodApertura = new QLineEdit(ConfigTicket);
        lineEditCodApertura->setObjectName(QString::fromUtf8("lineEditCodApertura"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEditCodApertura);

        label_3 = new QLabel(ConfigTicket);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_3);

        lineEditCodCorte = new QLineEdit(ConfigTicket);
        lineEditCodCorte->setObjectName(QString::fromUtf8("lineEditCodCorte"));

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEditCodCorte);

        groupBoxPromo = new QGroupBox(ConfigTicket);
        groupBoxPromo->setObjectName(QString::fromUtf8("groupBoxPromo"));
        groupBoxPromo->setFont(font);
        groupBoxPromo->setFlat(true);
        gridLayout_3 = new QGridLayout(groupBoxPromo);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        checkBoxPromo = new QCheckBox(groupBoxPromo);
        checkBoxPromo->setObjectName(QString::fromUtf8("checkBoxPromo"));
        QFont font1;
        font1.setBold(false);
        checkBoxPromo->setFont(font1);

        gridLayout_3->addWidget(checkBoxPromo, 0, 0, 1, 1);

        plainTextEditPromo = new QPlainTextEdit(groupBoxPromo);
        plainTextEditPromo->setObjectName(QString::fromUtf8("plainTextEditPromo"));

        gridLayout_3->addWidget(plainTextEditPromo, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(verticalSpacer_2, 2, 0, 1, 1);


        formLayout->setWidget(4, QFormLayout::SpanningRole, groupBoxPromo);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(5, QFormLayout::FieldRole, verticalSpacer_3);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(6, QFormLayout::FieldRole, verticalSpacer_4);

        pushButtonAceptar = new QPushButton(ConfigTicket);
        pushButtonAceptar->setObjectName(QString::fromUtf8("pushButtonAceptar"));

        formLayout->setWidget(8, QFormLayout::LabelRole, pushButtonAceptar);

        pushButtonCancelar = new QPushButton(ConfigTicket);
        pushButtonCancelar->setObjectName(QString::fromUtf8("pushButtonCancelar"));

        formLayout->setWidget(8, QFormLayout::FieldRole, pushButtonCancelar);


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
