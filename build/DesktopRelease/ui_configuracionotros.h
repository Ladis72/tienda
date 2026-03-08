/********************************************************************************
** Form generated from reading UI file 'configuracionotros.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGURACIONOTROS_H
#define UI_CONFIGURACIONOTROS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ConfiguracionOtros
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButtonAceptar;
    QCheckBox *checkBoxRE;
    QPushButton *pushButtonCancelar;

    void setupUi(QDialog *ConfiguracionOtros)
    {
        if (ConfiguracionOtros->objectName().isEmpty())
            ConfiguracionOtros->setObjectName("ConfiguracionOtros");
        ConfiguracionOtros->resize(400, 300);
        gridLayout = new QGridLayout(ConfiguracionOtros);
        gridLayout->setObjectName("gridLayout");
        pushButtonAceptar = new QPushButton(ConfiguracionOtros);
        pushButtonAceptar->setObjectName("pushButtonAceptar");

        gridLayout->addWidget(pushButtonAceptar, 1, 0, 1, 1);

        checkBoxRE = new QCheckBox(ConfiguracionOtros);
        checkBoxRE->setObjectName("checkBoxRE");

        gridLayout->addWidget(checkBoxRE, 0, 0, 1, 1);

        pushButtonCancelar = new QPushButton(ConfiguracionOtros);
        pushButtonCancelar->setObjectName("pushButtonCancelar");

        gridLayout->addWidget(pushButtonCancelar, 1, 1, 1, 1);


        retranslateUi(ConfiguracionOtros);
        QObject::connect(pushButtonCancelar, &QPushButton::clicked, ConfiguracionOtros, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(ConfiguracionOtros);
    } // setupUi

    void retranslateUi(QDialog *ConfiguracionOtros)
    {
        ConfiguracionOtros->setWindowTitle(QCoreApplication::translate("ConfiguracionOtros", "Configuraci\303\263n", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("ConfiguracionOtros", "Aceptar", nullptr));
        checkBoxRE->setText(QCoreApplication::translate("ConfiguracionOtros", "Recargo de equicalencia", nullptr));
        pushButtonCancelar->setText(QCoreApplication::translate("ConfiguracionOtros", "Cancelar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfiguracionOtros: public Ui_ConfiguracionOtros {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGURACIONOTROS_H
