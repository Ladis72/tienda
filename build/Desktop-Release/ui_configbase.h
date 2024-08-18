/********************************************************************************
** Form generated from reading UI file 'configbase.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGBASE_H
#define UI_CONFIGBASE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ConfigBase
{
public:
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineEditDireccion;
    QLabel *label_2;
    QLineEdit *lineEditPuerto;
    QLabel *label_3;
    QLineEdit *lineEditUsuario;
    QLabel *label_4;
    QLineEdit *lineEditClave;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label_5;
    QLineEdit *lineEditBaseDatos;

    void setupUi(QDialog *ConfigBase)
    {
        if (ConfigBase->objectName().isEmpty())
            ConfigBase->setObjectName(QString::fromUtf8("ConfigBase"));
        ConfigBase->resize(324, 236);
        formLayout = new QFormLayout(ConfigBase);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(ConfigBase);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lineEditDireccion = new QLineEdit(ConfigBase);
        lineEditDireccion->setObjectName(QString::fromUtf8("lineEditDireccion"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEditDireccion);

        label_2 = new QLabel(ConfigBase);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        lineEditPuerto = new QLineEdit(ConfigBase);
        lineEditPuerto->setObjectName(QString::fromUtf8("lineEditPuerto"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEditPuerto);

        label_3 = new QLabel(ConfigBase);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        lineEditUsuario = new QLineEdit(ConfigBase);
        lineEditUsuario->setObjectName(QString::fromUtf8("lineEditUsuario"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEditUsuario);

        label_4 = new QLabel(ConfigBase);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_4);

        lineEditClave = new QLineEdit(ConfigBase);
        lineEditClave->setObjectName(QString::fromUtf8("lineEditClave"));
        lineEditClave->setEchoMode(QLineEdit::EchoMode::Password);

        formLayout->setWidget(4, QFormLayout::FieldRole, lineEditClave);

        pushButton = new QPushButton(ConfigBase);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        formLayout->setWidget(5, QFormLayout::LabelRole, pushButton);

        pushButton_2 = new QPushButton(ConfigBase);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        formLayout->setWidget(5, QFormLayout::FieldRole, pushButton_2);

        label_5 = new QLabel(ConfigBase);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_5);

        lineEditBaseDatos = new QLineEdit(ConfigBase);
        lineEditBaseDatos->setObjectName(QString::fromUtf8("lineEditBaseDatos"));

        formLayout->setWidget(3, QFormLayout::FieldRole, lineEditBaseDatos);

#if QT_CONFIG(shortcut)
        label->setBuddy(lineEditDireccion);
        label_2->setBuddy(lineEditPuerto);
        label_3->setBuddy(lineEditUsuario);
        label_4->setBuddy(lineEditClave);
#endif // QT_CONFIG(shortcut)

        retranslateUi(ConfigBase);
        QObject::connect(pushButton_2, SIGNAL(clicked()), ConfigBase, SLOT(reject()));

        QMetaObject::connectSlotsByName(ConfigBase);
    } // setupUi

    void retranslateUi(QDialog *ConfigBase)
    {
        ConfigBase->setWindowTitle(QCoreApplication::translate("ConfigBase", "Configuraci\303\263n base de datos", nullptr));
        label->setText(QCoreApplication::translate("ConfigBase", "Direcci&\303\263n del sevidor", nullptr));
        label_2->setText(QCoreApplication::translate("ConfigBase", "Puerto", nullptr));
        label_3->setText(QCoreApplication::translate("ConfigBase", "Usuario", nullptr));
        label_4->setText(QCoreApplication::translate("ConfigBase", "Contrase&\303\261a", nullptr));
        pushButton->setText(QCoreApplication::translate("ConfigBase", "Aceptar", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ConfigBase", "Cancelar", nullptr));
        label_5->setText(QCoreApplication::translate("ConfigBase", "Base de datos", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfigBase: public Ui_ConfigBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGBASE_H
