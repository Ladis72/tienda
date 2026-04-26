/********************************************************************************
** Form generated from reading UI file 'tiendas.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIENDAS_H
#define UI_TIENDAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_tiendas
{
public:
    QVBoxLayout *mainVerticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *headerTitle;
    QSpacerItem *h_sp_h;
    QHBoxLayout *bodyHorizontalLayout;
    QTableView *tableViewTiendas;
    QGroupBox *general_2;
    QGridLayout *gridLayoutForm;
    QLabel *l_id;
    QLineEdit *lineEditId;
    QLabel *l_nom;
    QLineEdit *lineEditNombre;
    QLabel *l_dir;
    QLineEdit *lineEditDireccion;
    QLabel *l_ciu;
    QLineEdit *lineEditCiudad;
    QLabel *l_tlf;
    QLineEdit *lineEditTlfn;
    QLabel *l_wa;
    QLineEdit *lineEditWharsapp;
    QLabel *l_ema;
    QLineEdit *lineEditEmail;
    QFrame *line_1;
    QLabel *l_ip;
    QLineEdit *lineEditIP;
    QLabel *l_pue;
    QLineEdit *lineEditPuerto;
    QLabel *l_usu;
    QLineEdit *lineEditUsusario;
    QLabel *l_pas;
    QLineEdit *lineEditPassword;
    QLabel *l_bas;
    QLineEdit *lineEditBase;
    QHBoxLayout *h_ssl;
    QLabel *l_ssl;
    QLineEdit *lineEditSslCa;
    QPushButton *pushButtonSslCa;
    QCheckBox *checkBoxMaster;
    QCheckBox *checkBoxLocal;
    QHBoxLayout *buttonLayout;
    QPushButton *pushButtonNuevo;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonRefrescar;
    QSpacerItem *h_sp_b;
    QPushButton *pushButtonCancelar;
    QPushButton *pushButtonGuardar;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *tiendas)
    {
        if (tiendas->objectName().isEmpty())
            tiendas->setObjectName(QString::fromUtf8("tiendas"));
        tiendas->resize(1100, 600);
        tiendas->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }\n"
"QFrame#headerFrame { background-color: #2e7d32; border-bottom-left-radius: 15px; border-bottom-right-radius: 15px; }\n"
"QLabel#headerTitle { color: white; font-size: 20pt; font-weight: bold; }\n"
"QGroupBox { font-weight: bold; border: 1px solid #dee2e6; border-radius: 8px; margin-top: 10px; padding: 10px; background-color: white; }\n"
"QLineEdit { padding: 6px; border: 1px solid #ced4da; border-radius: 4px; }\n"
"QLineEdit:focus { border: 1px solid #2e7d32; }\n"
"QPushButton { padding: 8px 15px; border-radius: 4px; font-weight: bold; min-width: 100px; }\n"
"QPushButton#pushButtonAceptar, QPushButton#pushButtonGuardar { background-color: #2e7d32; color: white; }\n"
"QPushButton#pushButtonNuevo { background-color: #1976d2; color: white; }\n"
"QPushButton#pushButtonBorrar { background-color: #d32f2f; color: white; }\n"
"QPushButton#pushButtonCerrar { background-color: #e0e0e0; color: #424242; }"));
        mainVerticalLayout = new QVBoxLayout(tiendas);
        mainVerticalLayout->setSpacing(0);
        mainVerticalLayout->setObjectName(QString::fromUtf8("mainVerticalLayout"));
        mainVerticalLayout->setContentsMargins(0, 0, 0, 0);
        headerFrame = new QFrame(tiendas);
        headerFrame->setObjectName(QString::fromUtf8("headerFrame"));
        headerFrame->setMinimumSize(QSize(0, 80));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName(QString::fromUtf8("horizontalLayoutHeader"));
        horizontalLayoutHeader->setContentsMargins(30, -1, -1, -1);
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName(QString::fromUtf8("labelIcono"));
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/tiendas.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        headerTitle = new QLabel(headerFrame);
        headerTitle->setObjectName(QString::fromUtf8("headerTitle"));

        horizontalLayoutHeader->addWidget(headerTitle);

        h_sp_h = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutHeader->addItem(h_sp_h);


        mainVerticalLayout->addWidget(headerFrame);

        bodyHorizontalLayout = new QHBoxLayout();
        bodyHorizontalLayout->setSpacing(20);
        bodyHorizontalLayout->setObjectName(QString::fromUtf8("bodyHorizontalLayout"));
        bodyHorizontalLayout->setContentsMargins(20, 20, 20, 10);
        tableViewTiendas = new QTableView(tiendas);
        tableViewTiendas->setObjectName(QString::fromUtf8("tableViewTiendas"));
        tableViewTiendas->setMinimumSize(QSize(350, 0));
        tableViewTiendas->setMaximumSize(QSize(400, 16777215));
        tableViewTiendas->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableViewTiendas->setStyleSheet(QString::fromUtf8("QTableView { border: 1px solid #dee2e6; border-radius: 4px; background-color: white; }"));

        bodyHorizontalLayout->addWidget(tableViewTiendas);

        general_2 = new QGroupBox(tiendas);
        general_2->setObjectName(QString::fromUtf8("general_2"));
        gridLayoutForm = new QGridLayout(general_2);
        gridLayoutForm->setObjectName(QString::fromUtf8("gridLayoutForm"));
        l_id = new QLabel(general_2);
        l_id->setObjectName(QString::fromUtf8("l_id"));

        gridLayoutForm->addWidget(l_id, 0, 0, 1, 1);

        lineEditId = new QLineEdit(general_2);
        lineEditId->setObjectName(QString::fromUtf8("lineEditId"));
        lineEditId->setEnabled(false);

        gridLayoutForm->addWidget(lineEditId, 0, 1, 1, 1);

        l_nom = new QLabel(general_2);
        l_nom->setObjectName(QString::fromUtf8("l_nom"));

        gridLayoutForm->addWidget(l_nom, 1, 0, 1, 1);

        lineEditNombre = new QLineEdit(general_2);
        lineEditNombre->setObjectName(QString::fromUtf8("lineEditNombre"));

        gridLayoutForm->addWidget(lineEditNombre, 1, 1, 1, 3);

        l_dir = new QLabel(general_2);
        l_dir->setObjectName(QString::fromUtf8("l_dir"));

        gridLayoutForm->addWidget(l_dir, 2, 0, 1, 1);

        lineEditDireccion = new QLineEdit(general_2);
        lineEditDireccion->setObjectName(QString::fromUtf8("lineEditDireccion"));

        gridLayoutForm->addWidget(lineEditDireccion, 2, 1, 1, 3);

        l_ciu = new QLabel(general_2);
        l_ciu->setObjectName(QString::fromUtf8("l_ciu"));

        gridLayoutForm->addWidget(l_ciu, 3, 0, 1, 1);

        lineEditCiudad = new QLineEdit(general_2);
        lineEditCiudad->setObjectName(QString::fromUtf8("lineEditCiudad"));

        gridLayoutForm->addWidget(lineEditCiudad, 3, 1, 1, 3);

        l_tlf = new QLabel(general_2);
        l_tlf->setObjectName(QString::fromUtf8("l_tlf"));

        gridLayoutForm->addWidget(l_tlf, 4, 0, 1, 1);

        lineEditTlfn = new QLineEdit(general_2);
        lineEditTlfn->setObjectName(QString::fromUtf8("lineEditTlfn"));

        gridLayoutForm->addWidget(lineEditTlfn, 4, 1, 1, 1);

        l_wa = new QLabel(general_2);
        l_wa->setObjectName(QString::fromUtf8("l_wa"));

        gridLayoutForm->addWidget(l_wa, 4, 2, 1, 1);

        lineEditWharsapp = new QLineEdit(general_2);
        lineEditWharsapp->setObjectName(QString::fromUtf8("lineEditWharsapp"));

        gridLayoutForm->addWidget(lineEditWharsapp, 4, 3, 1, 1);

        l_ema = new QLabel(general_2);
        l_ema->setObjectName(QString::fromUtf8("l_ema"));

        gridLayoutForm->addWidget(l_ema, 5, 0, 1, 1);

        lineEditEmail = new QLineEdit(general_2);
        lineEditEmail->setObjectName(QString::fromUtf8("lineEditEmail"));

        gridLayoutForm->addWidget(lineEditEmail, 5, 1, 1, 3);

        line_1 = new QFrame(general_2);
        line_1->setObjectName(QString::fromUtf8("line_1"));
        line_1->setFrameShape(QFrame::HLine);
        line_1->setFrameShadow(QFrame::Sunken);

        gridLayoutForm->addWidget(line_1, 6, 0, 1, 4);

        l_ip = new QLabel(general_2);
        l_ip->setObjectName(QString::fromUtf8("l_ip"));

        gridLayoutForm->addWidget(l_ip, 7, 0, 1, 1);

        lineEditIP = new QLineEdit(general_2);
        lineEditIP->setObjectName(QString::fromUtf8("lineEditIP"));

        gridLayoutForm->addWidget(lineEditIP, 7, 1, 1, 1);

        l_pue = new QLabel(general_2);
        l_pue->setObjectName(QString::fromUtf8("l_pue"));

        gridLayoutForm->addWidget(l_pue, 7, 2, 1, 1);

        lineEditPuerto = new QLineEdit(general_2);
        lineEditPuerto->setObjectName(QString::fromUtf8("lineEditPuerto"));

        gridLayoutForm->addWidget(lineEditPuerto, 7, 3, 1, 1);

        l_usu = new QLabel(general_2);
        l_usu->setObjectName(QString::fromUtf8("l_usu"));

        gridLayoutForm->addWidget(l_usu, 8, 0, 1, 1);

        lineEditUsusario = new QLineEdit(general_2);
        lineEditUsusario->setObjectName(QString::fromUtf8("lineEditUsusario"));

        gridLayoutForm->addWidget(lineEditUsusario, 8, 1, 1, 1);

        l_pas = new QLabel(general_2);
        l_pas->setObjectName(QString::fromUtf8("l_pas"));

        gridLayoutForm->addWidget(l_pas, 8, 2, 1, 1);

        lineEditPassword = new QLineEdit(general_2);
        lineEditPassword->setObjectName(QString::fromUtf8("lineEditPassword"));
        lineEditPassword->setEchoMode(QLineEdit::Password);

        gridLayoutForm->addWidget(lineEditPassword, 8, 3, 1, 1);

        l_bas = new QLabel(general_2);
        l_bas->setObjectName(QString::fromUtf8("l_bas"));

        gridLayoutForm->addWidget(l_bas, 9, 0, 1, 1);

        lineEditBase = new QLineEdit(general_2);
        lineEditBase->setObjectName(QString::fromUtf8("lineEditBase"));

        gridLayoutForm->addWidget(lineEditBase, 9, 1, 1, 1);

        h_ssl = new QHBoxLayout();
        h_ssl->setObjectName(QString::fromUtf8("h_ssl"));
        l_ssl = new QLabel(general_2);
        l_ssl->setObjectName(QString::fromUtf8("l_ssl"));

        h_ssl->addWidget(l_ssl);

        lineEditSslCa = new QLineEdit(general_2);
        lineEditSslCa->setObjectName(QString::fromUtf8("lineEditSslCa"));
        lineEditSslCa->setReadOnly(true);

        h_ssl->addWidget(lineEditSslCa);

        pushButtonSslCa = new QPushButton(general_2);
        pushButtonSslCa->setObjectName(QString::fromUtf8("pushButtonSslCa"));
        pushButtonSslCa->setMaximumWidth(35);

        h_ssl->addWidget(pushButtonSslCa);


        gridLayoutForm->addLayout(h_ssl, 10, 0, 1, 4);

        checkBoxMaster = new QCheckBox(general_2);
        checkBoxMaster->setObjectName(QString::fromUtf8("checkBoxMaster"));

        gridLayoutForm->addWidget(checkBoxMaster, 11, 1, 1, 1);

        checkBoxLocal = new QCheckBox(general_2);
        checkBoxLocal->setObjectName(QString::fromUtf8("checkBoxLocal"));

        gridLayoutForm->addWidget(checkBoxLocal, 11, 3, 1, 1);


        bodyHorizontalLayout->addWidget(general_2);


        mainVerticalLayout->addLayout(bodyHorizontalLayout);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(10);
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        buttonLayout->setContentsMargins(20, -1, 20, 20);
        pushButtonNuevo = new QPushButton(tiendas);
        pushButtonNuevo->setObjectName(QString::fromUtf8("pushButtonNuevo"));

        buttonLayout->addWidget(pushButtonNuevo);

        pushButtonBorrar = new QPushButton(tiendas);
        pushButtonBorrar->setObjectName(QString::fromUtf8("pushButtonBorrar"));

        buttonLayout->addWidget(pushButtonBorrar);

        pushButtonRefrescar = new QPushButton(tiendas);
        pushButtonRefrescar->setObjectName(QString::fromUtf8("pushButtonRefrescar"));

        buttonLayout->addWidget(pushButtonRefrescar);

        h_sp_b = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(h_sp_b);

        pushButtonCancelar = new QPushButton(tiendas);
        pushButtonCancelar->setObjectName(QString::fromUtf8("pushButtonCancelar"));

        buttonLayout->addWidget(pushButtonCancelar);

        pushButtonGuardar = new QPushButton(tiendas);
        pushButtonGuardar->setObjectName(QString::fromUtf8("pushButtonGuardar"));

        buttonLayout->addWidget(pushButtonGuardar);

        pushButtonCerrar = new QPushButton(tiendas);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));

        buttonLayout->addWidget(pushButtonCerrar);


        mainVerticalLayout->addLayout(buttonLayout);


        retranslateUi(tiendas);

        QMetaObject::connectSlotsByName(tiendas);
    } // setupUi

    void retranslateUi(QDialog *tiendas)
    {
        tiendas->setWindowTitle(QCoreApplication::translate("tiendas", "Gesti\303\263n de Tiendas", nullptr));
        headerTitle->setText(QCoreApplication::translate("tiendas", "GESTI\303\223N DE TIENDAS", nullptr));
        general_2->setTitle(QCoreApplication::translate("tiendas", "DETALLES DE LA TIENDA", nullptr));
        l_id->setText(QCoreApplication::translate("tiendas", "ID:", nullptr));
        l_nom->setText(QCoreApplication::translate("tiendas", "Nombre:", nullptr));
        l_dir->setText(QCoreApplication::translate("tiendas", "Direcci\303\263n:", nullptr));
        l_ciu->setText(QCoreApplication::translate("tiendas", "Ciudad:", nullptr));
        l_tlf->setText(QCoreApplication::translate("tiendas", "Tel\303\251fono:", nullptr));
        l_wa->setText(QCoreApplication::translate("tiendas", "WhatsApp:", nullptr));
        l_ema->setText(QCoreApplication::translate("tiendas", "Email:", nullptr));
        l_ip->setText(QCoreApplication::translate("tiendas", "I.P. Servidor:", nullptr));
        l_pue->setText(QCoreApplication::translate("tiendas", "Puerto:", nullptr));
        l_usu->setText(QCoreApplication::translate("tiendas", "Usuario DB:", nullptr));
        l_pas->setText(QCoreApplication::translate("tiendas", "Contrase\303\261a:", nullptr));
        l_bas->setText(QCoreApplication::translate("tiendas", "Base de datos:", nullptr));
        l_ssl->setText(QCoreApplication::translate("tiendas", "Certificado CA (SSL):", nullptr));
        pushButtonSslCa->setText(QCoreApplication::translate("tiendas", "...", nullptr));
        checkBoxMaster->setText(QCoreApplication::translate("tiendas", "\302\277Es la tienda principal?", nullptr));
        checkBoxLocal->setText(QCoreApplication::translate("tiendas", "\302\277Es la tienda local?", nullptr));
        pushButtonNuevo->setText(QCoreApplication::translate("tiendas", "NUEVO", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("tiendas", "BORRAR", nullptr));
        pushButtonRefrescar->setText(QCoreApplication::translate("tiendas", "REFRESCAR", nullptr));
        pushButtonCancelar->setText(QCoreApplication::translate("tiendas", "CANCELAR", nullptr));
        pushButtonGuardar->setText(QCoreApplication::translate("tiendas", "GUARDAR", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("tiendas", "CERRAR", nullptr));
    } // retranslateUi

};

namespace Ui {
    class tiendas: public Ui_tiendas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIENDAS_H
