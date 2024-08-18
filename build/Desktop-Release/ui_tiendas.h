/********************************************************************************
** Form generated from reading UI file 'tiendas.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIENDAS_H
#define UI_TIENDAS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_tiendas
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonNuevo;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonModificar;
    QPushButton *pushButtonRefrescar;
    QPushButton *pushButtonAnterior;
    QPushButton *pushButtonSiguiente;
    QPushButton *pushButtonCerrar;
    QWidget *general_2;
    QGridLayout *general;
    QLabel *label_5;
    QLabel *label_8;
    QLineEdit *lineEditIP;
    QLineEdit *lineEditEmail;
    QLabel *label_4;
    QLabel *label_7;
    QLineEdit *lineEditWharsapp;
    QLabel *label_10;
    QLineEdit *lineEditPassword;
    QLabel *label_2;
    QLineEdit *lineEditTlfn;
    QLineEdit *lineEditId;
    QSpacerItem *horizontalSpacer;
    QLabel *label_9;
    QLineEdit *lineEditUsusario;
    QLabel *label_6;
    QLabel *label_3;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEditNombre;
    QLineEdit *lineEditDireccion;
    QLineEdit *lineEditCiudad;
    QCheckBox *checkBoxMaster;
    QCheckBox *checkBoxLocal;

    void setupUi(QDialog *tiendas)
    {
        if (tiendas->objectName().isEmpty())
            tiendas->setObjectName(QString::fromUtf8("tiendas"));
        tiendas->resize(910, 342);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imagenes/Emeicjac logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        tiendas->setWindowIcon(icon);
        gridLayout = new QGridLayout(tiendas);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonNuevo = new QPushButton(tiendas);
        pushButtonNuevo->setObjectName(QString::fromUtf8("pushButtonNuevo"));
        pushButtonNuevo->setEnabled(true);
        pushButtonNuevo->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonNuevo);

        pushButtonBorrar = new QPushButton(tiendas);
        pushButtonBorrar->setObjectName(QString::fromUtf8("pushButtonBorrar"));
        pushButtonBorrar->setEnabled(true);
        pushButtonBorrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonBorrar);

        pushButtonModificar = new QPushButton(tiendas);
        pushButtonModificar->setObjectName(QString::fromUtf8("pushButtonModificar"));
        pushButtonModificar->setEnabled(true);
        pushButtonModificar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonModificar);

        pushButtonRefrescar = new QPushButton(tiendas);
        pushButtonRefrescar->setObjectName(QString::fromUtf8("pushButtonRefrescar"));
        pushButtonRefrescar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonRefrescar);

        pushButtonAnterior = new QPushButton(tiendas);
        pushButtonAnterior->setObjectName(QString::fromUtf8("pushButtonAnterior"));
        pushButtonAnterior->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonAnterior);

        pushButtonSiguiente = new QPushButton(tiendas);
        pushButtonSiguiente->setObjectName(QString::fromUtf8("pushButtonSiguiente"));
        pushButtonSiguiente->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonSiguiente);

        pushButtonCerrar = new QPushButton(tiendas);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));
        pushButtonCerrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonCerrar);


        gridLayout->addLayout(horizontalLayout, 3, 0, 1, 8);

        general_2 = new QWidget(tiendas);
        general_2->setObjectName(QString::fromUtf8("general_2"));
        general = new QGridLayout(general_2);
        general->setObjectName(QString::fromUtf8("general"));
        label_5 = new QLabel(general_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        general->addWidget(label_5, 6, 0, 1, 1);

        label_8 = new QLabel(general_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        general->addWidget(label_8, 3, 3, 1, 1);

        lineEditIP = new QLineEdit(general_2);
        lineEditIP->setObjectName(QString::fromUtf8("lineEditIP"));

        general->addWidget(lineEditIP, 2, 4, 1, 1);

        lineEditEmail = new QLineEdit(general_2);
        lineEditEmail->setObjectName(QString::fromUtf8("lineEditEmail"));

        general->addWidget(lineEditEmail, 7, 1, 1, 1);

        label_4 = new QLabel(general_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        general->addWidget(label_4, 4, 0, 1, 1);

        label_7 = new QLabel(general_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        general->addWidget(label_7, 2, 3, 1, 1);

        lineEditWharsapp = new QLineEdit(general_2);
        lineEditWharsapp->setObjectName(QString::fromUtf8("lineEditWharsapp"));

        general->addWidget(lineEditWharsapp, 6, 1, 1, 1);

        label_10 = new QLabel(general_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        general->addWidget(label_10, 1, 0, 1, 1);

        lineEditPassword = new QLineEdit(general_2);
        lineEditPassword->setObjectName(QString::fromUtf8("lineEditPassword"));
        lineEditPassword->setEchoMode(QLineEdit::EchoMode::Password);

        general->addWidget(lineEditPassword, 4, 4, 1, 1);

        label_2 = new QLabel(general_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        general->addWidget(label_2, 3, 0, 1, 1);

        lineEditTlfn = new QLineEdit(general_2);
        lineEditTlfn->setObjectName(QString::fromUtf8("lineEditTlfn"));

        general->addWidget(lineEditTlfn, 5, 1, 1, 1);

        lineEditId = new QLineEdit(general_2);
        lineEditId->setObjectName(QString::fromUtf8("lineEditId"));
        lineEditId->setEnabled(false);

        general->addWidget(lineEditId, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        general->addItem(horizontalSpacer, 1, 4, 1, 1);

        label_9 = new QLabel(general_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        general->addWidget(label_9, 4, 3, 1, 1);

        lineEditUsusario = new QLineEdit(general_2);
        lineEditUsusario->setObjectName(QString::fromUtf8("lineEditUsusario"));

        general->addWidget(lineEditUsusario, 3, 4, 1, 1);

        label_6 = new QLabel(general_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        general->addWidget(label_6, 7, 0, 1, 1);

        label_3 = new QLabel(general_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        general->addWidget(label_3, 5, 0, 1, 1);

        label = new QLabel(general_2);
        label->setObjectName(QString::fromUtf8("label"));

        general->addWidget(label, 2, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        general->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        lineEditNombre = new QLineEdit(general_2);
        lineEditNombre->setObjectName(QString::fromUtf8("lineEditNombre"));

        general->addWidget(lineEditNombre, 2, 1, 1, 2);

        lineEditDireccion = new QLineEdit(general_2);
        lineEditDireccion->setObjectName(QString::fromUtf8("lineEditDireccion"));

        general->addWidget(lineEditDireccion, 3, 1, 1, 2);

        lineEditCiudad = new QLineEdit(general_2);
        lineEditCiudad->setObjectName(QString::fromUtf8("lineEditCiudad"));

        general->addWidget(lineEditCiudad, 4, 1, 1, 2);

        checkBoxMaster = new QCheckBox(general_2);
        checkBoxMaster->setObjectName(QString::fromUtf8("checkBoxMaster"));

        general->addWidget(checkBoxMaster, 7, 3, 1, 1);

        checkBoxLocal = new QCheckBox(general_2);
        checkBoxLocal->setObjectName(QString::fromUtf8("checkBoxLocal"));

        general->addWidget(checkBoxLocal, 6, 3, 1, 1);


        gridLayout->addWidget(general_2, 0, 0, 3, 8);

        QWidget::setTabOrder(lineEditNombre, lineEditDireccion);
        QWidget::setTabOrder(lineEditDireccion, lineEditCiudad);
        QWidget::setTabOrder(lineEditCiudad, lineEditTlfn);
        QWidget::setTabOrder(lineEditTlfn, lineEditWharsapp);
        QWidget::setTabOrder(lineEditWharsapp, lineEditEmail);
        QWidget::setTabOrder(lineEditEmail, lineEditIP);
        QWidget::setTabOrder(lineEditIP, lineEditUsusario);
        QWidget::setTabOrder(lineEditUsusario, lineEditPassword);
        QWidget::setTabOrder(lineEditPassword, pushButtonNuevo);
        QWidget::setTabOrder(pushButtonNuevo, pushButtonBorrar);
        QWidget::setTabOrder(pushButtonBorrar, pushButtonModificar);
        QWidget::setTabOrder(pushButtonModificar, pushButtonRefrescar);
        QWidget::setTabOrder(pushButtonRefrescar, pushButtonAnterior);
        QWidget::setTabOrder(pushButtonAnterior, pushButtonSiguiente);
        QWidget::setTabOrder(pushButtonSiguiente, pushButtonCerrar);
        QWidget::setTabOrder(pushButtonCerrar, lineEditId);

        retranslateUi(tiendas);
        QObject::connect(pushButtonCerrar, SIGNAL(clicked()), tiendas, SLOT(accept()));

        QMetaObject::connectSlotsByName(tiendas);
    } // setupUi

    void retranslateUi(QDialog *tiendas)
    {
        tiendas->setWindowTitle(QCoreApplication::translate("tiendas", "Tiendas", nullptr));
        pushButtonNuevo->setText(QCoreApplication::translate("tiendas", "Nuevo", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("tiendas", "Borrar", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("tiendas", "Modificar", nullptr));
        pushButtonRefrescar->setText(QCoreApplication::translate("tiendas", "Refrescar", nullptr));
        pushButtonAnterior->setText(QCoreApplication::translate("tiendas", "Anterior", nullptr));
        pushButtonSiguiente->setText(QCoreApplication::translate("tiendas", "Siguiente", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("tiendas", "Cerrar", nullptr));
        label_5->setText(QCoreApplication::translate("tiendas", "Whatsapp", nullptr));
        label_8->setText(QCoreApplication::translate("tiendas", "Ususario", nullptr));
        lineEditIP->setInputMask(QString());
        label_4->setText(QCoreApplication::translate("tiendas", "Ciudad", nullptr));
        label_7->setText(QCoreApplication::translate("tiendas", "I.P.", nullptr));
        label_10->setText(QCoreApplication::translate("tiendas", "Id", nullptr));
        label_2->setText(QCoreApplication::translate("tiendas", "Direcci&\303\263n", nullptr));
        label_9->setText(QCoreApplication::translate("tiendas", "Contrase&\303\261a", nullptr));
        label_6->setText(QCoreApplication::translate("tiendas", "Correo", nullptr));
        label_3->setText(QCoreApplication::translate("tiendas", "Tel\303\251fono", nullptr));
        label->setText(QCoreApplication::translate("tiendas", "Nombre", nullptr));
        checkBoxMaster->setText(QCoreApplication::translate("tiendas", "\302\277Es la tienda principal?", nullptr));
        checkBoxLocal->setText(QCoreApplication::translate("tiendas", "\302\277Es la tienda local?", nullptr));
    } // retranslateUi

};

namespace Ui {
    class tiendas: public Ui_tiendas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIENDAS_H
