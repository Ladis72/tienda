/********************************************************************************
** Form generated from reading UI file 'configuracionotros.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGURACIONOTROS_H
#define UI_CONFIGURACIONOTROS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConfiguracionOtros
{
public:
    QVBoxLayout *mainVerticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *labelTitle;
    QSpacerItem *horizontalSpacerHeader;
    QGroupBox *groupBoxSettings;
    QVBoxLayout *verticalLayoutSettings;
    QCheckBox *checkBoxRE;
    QCheckBox *checkBoxNube;
    QSpacerItem *verticalSpacerBody;
    QGroupBox *groupBoxHotkeys;
    QGridLayout *gridLayoutHotkeys;
    QLabel *labelF1;
    QComboBox *comboBoxF1;
    QLabel *labelF2;
    QComboBox *comboBoxF2;
    QLabel *labelF3;
    QComboBox *comboBoxF3;
    QLabel *labelF4;
    QComboBox *comboBoxF4;
    QHBoxLayout *horizontalLayoutButtons;
    QSpacerItem *horizontalSpacerButtons;
    QPushButton *pushButtonCancelar;
    QPushButton *pushButtonAceptar;

    void setupUi(QDialog *ConfiguracionOtros)
    {
        if (ConfiguracionOtros->objectName().isEmpty())
            ConfiguracionOtros->setObjectName(QString::fromUtf8("ConfiguracionOtros"));
        ConfiguracionOtros->resize(550, 350);
        ConfiguracionOtros->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        mainVerticalLayout = new QVBoxLayout(ConfiguracionOtros);
        mainVerticalLayout->setSpacing(20);
        mainVerticalLayout->setObjectName(QString::fromUtf8("mainVerticalLayout"));
        mainVerticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(ConfiguracionOtros);
        headerFrame->setObjectName(QString::fromUtf8("headerFrame"));
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame {\n"
"    background-color: #eceff1;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #cfd8dc;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName(QString::fromUtf8("horizontalLayoutHeader"));
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName(QString::fromUtf8("labelIcono"));
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/configuracion.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelTitle = new QLabel(headerFrame);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelTitle->setFont(font);
        labelTitle->setStyleSheet(QString::fromUtf8("color: #263238;"));

        horizontalLayoutHeader->addWidget(labelTitle);

        horizontalSpacerHeader = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);


        mainVerticalLayout->addWidget(headerFrame);

        groupBoxSettings = new QGroupBox(ConfiguracionOtros);
        groupBoxSettings->setObjectName(QString::fromUtf8("groupBoxSettings"));
        verticalLayoutSettings = new QVBoxLayout(groupBoxSettings);
        verticalLayoutSettings->setObjectName(QString::fromUtf8("verticalLayoutSettings"));
        checkBoxRE = new QCheckBox(groupBoxSettings);
        checkBoxRE->setObjectName(QString::fromUtf8("checkBoxRE"));

        verticalLayoutSettings->addWidget(checkBoxRE);

        checkBoxNube = new QCheckBox(groupBoxSettings);
        checkBoxNube->setObjectName(QString::fromUtf8("checkBoxNube"));

        verticalLayoutSettings->addWidget(checkBoxNube);

        verticalSpacerBody = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayoutSettings->addItem(verticalSpacerBody);


        mainVerticalLayout->addWidget(groupBoxSettings);

        groupBoxHotkeys = new QGroupBox(ConfiguracionOtros);
        groupBoxHotkeys->setObjectName(QString::fromUtf8("groupBoxHotkeys"));
        gridLayoutHotkeys = new QGridLayout(groupBoxHotkeys);
        gridLayoutHotkeys->setObjectName(QString::fromUtf8("gridLayoutHotkeys"));
        labelF1 = new QLabel(groupBoxHotkeys);
        labelF1->setObjectName(QString::fromUtf8("labelF1"));

        gridLayoutHotkeys->addWidget(labelF1, 0, 0, 1, 1);

        comboBoxF1 = new QComboBox(groupBoxHotkeys);
        comboBoxF1->setObjectName(QString::fromUtf8("comboBoxF1"));

        gridLayoutHotkeys->addWidget(comboBoxF1, 0, 1, 1, 1);

        labelF2 = new QLabel(groupBoxHotkeys);
        labelF2->setObjectName(QString::fromUtf8("labelF2"));

        gridLayoutHotkeys->addWidget(labelF2, 1, 0, 1, 1);

        comboBoxF2 = new QComboBox(groupBoxHotkeys);
        comboBoxF2->setObjectName(QString::fromUtf8("comboBoxF2"));

        gridLayoutHotkeys->addWidget(comboBoxF2, 1, 1, 1, 1);

        labelF3 = new QLabel(groupBoxHotkeys);
        labelF3->setObjectName(QString::fromUtf8("labelF3"));

        gridLayoutHotkeys->addWidget(labelF3, 2, 0, 1, 1);

        comboBoxF3 = new QComboBox(groupBoxHotkeys);
        comboBoxF3->setObjectName(QString::fromUtf8("comboBoxF3"));

        gridLayoutHotkeys->addWidget(comboBoxF3, 2, 1, 1, 1);

        labelF4 = new QLabel(groupBoxHotkeys);
        labelF4->setObjectName(QString::fromUtf8("labelF4"));

        gridLayoutHotkeys->addWidget(labelF4, 3, 0, 1, 1);

        comboBoxF4 = new QComboBox(groupBoxHotkeys);
        comboBoxF4->setObjectName(QString::fromUtf8("comboBoxF4"));

        gridLayoutHotkeys->addWidget(comboBoxF4, 3, 1, 1, 1);


        mainVerticalLayout->addWidget(groupBoxHotkeys);

        horizontalLayoutButtons = new QHBoxLayout();
        horizontalLayoutButtons->setObjectName(QString::fromUtf8("horizontalLayoutButtons"));
        horizontalSpacerButtons = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutButtons->addItem(horizontalSpacerButtons);

        pushButtonCancelar = new QPushButton(ConfiguracionOtros);
        pushButtonCancelar->setObjectName(QString::fromUtf8("pushButtonCancelar"));
        pushButtonCancelar->setMinimumSize(QSize(120, 40));

        horizontalLayoutButtons->addWidget(pushButtonCancelar);

        pushButtonAceptar = new QPushButton(ConfiguracionOtros);
        pushButtonAceptar->setObjectName(QString::fromUtf8("pushButtonAceptar"));
        pushButtonAceptar->setMinimumSize(QSize(120, 40));
        pushButtonAceptar->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #455a64; color: white; font-weight: bold; border-radius: 4px; }\n"
"QPushButton:hover { background-color: #37474f; }"));

        horizontalLayoutButtons->addWidget(pushButtonAceptar);


        mainVerticalLayout->addLayout(horizontalLayoutButtons);


        retranslateUi(ConfiguracionOtros);

        QMetaObject::connectSlotsByName(ConfiguracionOtros);
    } // setupUi

    void retranslateUi(QDialog *ConfiguracionOtros)
    {
        ConfiguracionOtros->setWindowTitle(QCoreApplication::translate("ConfiguracionOtros", "Configuraci\303\263n del Sistema", nullptr));
        labelTitle->setText(QCoreApplication::translate("ConfiguracionOtros", "Configuraci\303\263n", nullptr));
        groupBoxSettings->setTitle(QCoreApplication::translate("ConfiguracionOtros", "Opciones Generales", nullptr));
        checkBoxRE->setText(QCoreApplication::translate("ConfiguracionOtros", "Habilitar Recargo de Equivalencia (R.E.)", nullptr));
        checkBoxNube->setText(QCoreApplication::translate("ConfiguracionOtros", "Usar precios espec\303\255ficos de esta tienda (No sincronizar PVP con la red)", nullptr));
        groupBoxHotkeys->setTitle(QCoreApplication::translate("ConfiguracionOtros", "Teclas R\303\241pidas de Vendedores (F1-F4)", nullptr));
        labelF1->setText(QCoreApplication::translate("ConfiguracionOtros", "Tecla F1:", nullptr));
        labelF2->setText(QCoreApplication::translate("ConfiguracionOtros", "Tecla F2:", nullptr));
        labelF3->setText(QCoreApplication::translate("ConfiguracionOtros", "Tecla F3:", nullptr));
        labelF4->setText(QCoreApplication::translate("ConfiguracionOtros", "Tecla F4:", nullptr));
        pushButtonCancelar->setText(QCoreApplication::translate("ConfiguracionOtros", "Cancelar", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("ConfiguracionOtros", "Aceptar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfiguracionOtros: public Ui_ConfiguracionOtros {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGURACIONOTROS_H
