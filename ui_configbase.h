/********************************************************************************
** Form generated from reading UI file 'configbase.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGBASE_H
#define UI_CONFIGBASE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConfigBase
{
public:
    QVBoxLayout *mainVerticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *labelTitle;
    QSpacerItem *horizontalSpacerHeader;
    QGroupBox *groupBoxConfig;
    QFormLayout *formLayoutConfig;
    QLabel *label;
    QLineEdit *lineEditDireccion;
    QLabel *label_2;
    QLineEdit *lineEditPuerto;
    QLabel *label_3;
    QLineEdit *lineEditUsuario;
    QLabel *label_5;
    QLineEdit *lineEditBaseDatos;
    QLabel *label_4;
    QLineEdit *lineEditClave;
    QLabel *label_6;
    QHBoxLayout *horizontalLayoutSsl;
    QLineEdit *lineEditSSLCA;
    QPushButton *pushButtonBuscarCa;
    QPushButton *pushButtonTest;
    QHBoxLayout *horizontalLayoutButtons;
    QSpacerItem *horizontalSpacerButtons;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QDialog *ConfigBase)
    {
        if (ConfigBase->objectName().isEmpty())
            ConfigBase->setObjectName(QString::fromUtf8("ConfigBase"));
        ConfigBase->resize(500, 550);
        ConfigBase->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        mainVerticalLayout = new QVBoxLayout(ConfigBase);
        mainVerticalLayout->setSpacing(15);
        mainVerticalLayout->setObjectName(QString::fromUtf8("mainVerticalLayout"));
        mainVerticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(ConfigBase);
        headerFrame->setObjectName(QString::fromUtf8("headerFrame"));
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame {\n"
"    background-color: #e3f2fd;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #bbdefb;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName(QString::fromUtf8("horizontalLayoutHeader"));
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName(QString::fromUtf8("labelIcono"));
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/base_datos.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelTitle = new QLabel(headerFrame);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelTitle->setFont(font);
        labelTitle->setStyleSheet(QString::fromUtf8("color: #0d47a1;"));

        horizontalLayoutHeader->addWidget(labelTitle);

        horizontalSpacerHeader = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);


        mainVerticalLayout->addWidget(headerFrame);

        groupBoxConfig = new QGroupBox(ConfigBase);
        groupBoxConfig->setObjectName(QString::fromUtf8("groupBoxConfig"));
        formLayoutConfig = new QFormLayout(groupBoxConfig);
        formLayoutConfig->setObjectName(QString::fromUtf8("formLayoutConfig"));
        formLayoutConfig->setVerticalSpacing(12);
        label = new QLabel(groupBoxConfig);
        label->setObjectName(QString::fromUtf8("label"));

        formLayoutConfig->setWidget(0, QFormLayout::LabelRole, label);

        lineEditDireccion = new QLineEdit(groupBoxConfig);
        lineEditDireccion->setObjectName(QString::fromUtf8("lineEditDireccion"));

        formLayoutConfig->setWidget(0, QFormLayout::FieldRole, lineEditDireccion);

        label_2 = new QLabel(groupBoxConfig);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayoutConfig->setWidget(1, QFormLayout::LabelRole, label_2);

        lineEditPuerto = new QLineEdit(groupBoxConfig);
        lineEditPuerto->setObjectName(QString::fromUtf8("lineEditPuerto"));

        formLayoutConfig->setWidget(1, QFormLayout::FieldRole, lineEditPuerto);

        label_3 = new QLabel(groupBoxConfig);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayoutConfig->setWidget(2, QFormLayout::LabelRole, label_3);

        lineEditUsuario = new QLineEdit(groupBoxConfig);
        lineEditUsuario->setObjectName(QString::fromUtf8("lineEditUsuario"));

        formLayoutConfig->setWidget(2, QFormLayout::FieldRole, lineEditUsuario);

        label_5 = new QLabel(groupBoxConfig);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayoutConfig->setWidget(3, QFormLayout::LabelRole, label_5);

        lineEditBaseDatos = new QLineEdit(groupBoxConfig);
        lineEditBaseDatos->setObjectName(QString::fromUtf8("lineEditBaseDatos"));

        formLayoutConfig->setWidget(3, QFormLayout::FieldRole, lineEditBaseDatos);

        label_4 = new QLabel(groupBoxConfig);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayoutConfig->setWidget(4, QFormLayout::LabelRole, label_4);

        lineEditClave = new QLineEdit(groupBoxConfig);
        lineEditClave->setObjectName(QString::fromUtf8("lineEditClave"));
        lineEditClave->setEchoMode(QLineEdit::EchoMode::Password);

        formLayoutConfig->setWidget(4, QFormLayout::FieldRole, lineEditClave);

        label_6 = new QLabel(groupBoxConfig);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayoutConfig->setWidget(5, QFormLayout::LabelRole, label_6);

        horizontalLayoutSsl = new QHBoxLayout();
        horizontalLayoutSsl->setObjectName(QString::fromUtf8("horizontalLayoutSsl"));
        lineEditSSLCA = new QLineEdit(groupBoxConfig);
        lineEditSSLCA->setObjectName(QString::fromUtf8("lineEditSSLCA"));

        horizontalLayoutSsl->addWidget(lineEditSSLCA);

        pushButtonBuscarCa = new QPushButton(groupBoxConfig);
        pushButtonBuscarCa->setObjectName(QString::fromUtf8("pushButtonBuscarCa"));
        pushButtonBuscarCa->setMaximumSize(QSize(30, 16777215));

        horizontalLayoutSsl->addWidget(pushButtonBuscarCa);


        formLayoutConfig->setLayout(5, QFormLayout::FieldRole, horizontalLayoutSsl);


        mainVerticalLayout->addWidget(groupBoxConfig);

        pushButtonTest = new QPushButton(ConfigBase);
        pushButtonTest->setObjectName(QString::fromUtf8("pushButtonTest"));
        pushButtonTest->setMinimumSize(QSize(0, 40));

        mainVerticalLayout->addWidget(pushButtonTest);

        horizontalLayoutButtons = new QHBoxLayout();
        horizontalLayoutButtons->setObjectName(QString::fromUtf8("horizontalLayoutButtons"));
        horizontalSpacerButtons = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutButtons->addItem(horizontalSpacerButtons);

        pushButton_2 = new QPushButton(ConfigBase);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(120, 40));

        horizontalLayoutButtons->addWidget(pushButton_2);

        pushButton = new QPushButton(ConfigBase);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(120, 40));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #1976d2; color: white; font-weight: bold; border-radius: 4px; }\n"
"QPushButton:hover { background-color: #1565c0; }"));

        horizontalLayoutButtons->addWidget(pushButton);


        mainVerticalLayout->addLayout(horizontalLayoutButtons);


        retranslateUi(ConfigBase);

        QMetaObject::connectSlotsByName(ConfigBase);
    } // setupUi

    void retranslateUi(QDialog *ConfigBase)
    {
        ConfigBase->setWindowTitle(QCoreApplication::translate("ConfigBase", "Configuraci\303\263n Servidor en la Nube", nullptr));
        labelTitle->setText(QCoreApplication::translate("ConfigBase", "Servidor Nube", nullptr));
        groupBoxConfig->setTitle(QCoreApplication::translate("ConfigBase", "Par\303\241metros de Conexi\303\263n", nullptr));
        label->setText(QCoreApplication::translate("ConfigBase", "Host (IP/DNS):", nullptr));
        label_2->setText(QCoreApplication::translate("ConfigBase", "Puerto:", nullptr));
        label_3->setText(QCoreApplication::translate("ConfigBase", "Usuario:", nullptr));
        label_5->setText(QCoreApplication::translate("ConfigBase", "Base de Datos:", nullptr));
        label_4->setText(QCoreApplication::translate("ConfigBase", "Contrase\303\261a:", nullptr));
        label_6->setText(QCoreApplication::translate("ConfigBase", "Certificado CA:", nullptr));
        lineEditSSLCA->setPlaceholderText(QCoreApplication::translate("ConfigBase", ".pem file path", nullptr));
        pushButtonBuscarCa->setText(QCoreApplication::translate("ConfigBase", "...", nullptr));
        pushButtonTest->setText(QCoreApplication::translate("ConfigBase", "Probar Conexi\303\263n", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ConfigBase", "Cancelar", nullptr));
        pushButton->setText(QCoreApplication::translate("ConfigBase", "Aceptar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfigBase: public Ui_ConfigBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGBASE_H
