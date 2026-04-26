/********************************************************************************
** Form generated from reading UI file 'tabwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABWIDGET_H
#define UI_TABWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Ususarios
{
public:
    QVBoxLayout *mainVerticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *labelTitle;
    QSpacerItem *horizontalSpacerHeader;
    QLabel *labelNombre;
    QTabWidget *tabWidget;
    QWidget *General;
    QGridLayout *gridLayout_General;
    QLabel *label_13;
    QLineEdit *lineEditCod;
    QLabel *label_1;
    QLineEdit *lineEditNombre;
    QLabel *label_2;
    QLineEdit *lineEditApellido;
    QLabel *label_3;
    QLineEdit *lineEditDireccion;
    QLabel *label_4;
    QLineEdit *lineEditLocalidad;
    QLabel *label_6;
    QLineEdit *lineEditCp;
    QLabel *label_5;
    QLineEdit *lineEditProvincia;
    QLabel *label_8;
    QLineEdit *lineEditNif;
    QLabel *label_7;
    QLineEdit *lineEditTlfn;
    QLabel *label_9;
    QLineEdit *lineEditMail;
    QFrame *loginFrame;
    QHBoxLayout *horizontalLayout_Login;
    QLabel *label_10;
    QLineEdit *lineEditUsusario;
    QLabel *label_11;
    QLineEdit *lineEditClave;
    QLabel *label_14;
    QLineEdit *lineEditPermisos;
    QPushButton *pushButtonEditarPermisos;
    QLabel *label_12;
    QPlainTextEdit *plainTextEdit;
    QVBoxLayout *verticalLayoutFoto;
    QLabel *labelFoto;
    QLineEdit *lineEditArchivoFoto;
    QHBoxLayout *horizontalLayoutFotoBtns;
    QPushButton *pushButtonCargarImagen;
    QPushButton *pushButtonBorrarImagen;
    QSpacerItem *verticalSpacerFoto;
    QHBoxLayout *horizontalLayoutActions;
    QPushButton *pushButtonNuevo;
    QPushButton *pushButtonModificar;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonUnificar;
    QPushButton *pushButtonSaneador;
    QPushButton *pushButtonRefrescar;
    QSpacerItem *horizontalSpacerActions;
    QPushButton *pushButtonAnterior;
    QPushButton *pushButtonSiguiente;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *Ususarios)
    {
        if (Ususarios->objectName().isEmpty())
            Ususarios->setObjectName("Ususarios");
        Ususarios->resize(950, 700);
        Ususarios->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        mainVerticalLayout = new QVBoxLayout(Ususarios);
        mainVerticalLayout->setSpacing(15);
        mainVerticalLayout->setObjectName("mainVerticalLayout");
        mainVerticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(Ususarios);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame {\n"
"    background-color: #f3e5f5;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #e1bee7;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName("horizontalLayoutHeader");
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName("labelIcono");
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/usuarios.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelTitle = new QLabel(headerFrame);
        labelTitle->setObjectName("labelTitle");
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelTitle->setFont(font);
        labelTitle->setStyleSheet(QString::fromUtf8("color: #4a148c;"));

        horizontalLayoutHeader->addWidget(labelTitle);

        horizontalSpacerHeader = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);

        labelNombre = new QLabel(headerFrame);
        labelNombre->setObjectName("labelNombre");
        QFont font1;
        font1.setPointSize(16);
        font1.setItalic(true);
        labelNombre->setFont(font1);
        labelNombre->setStyleSheet(QString::fromUtf8("color: #7b1fa2;"));

        horizontalLayoutHeader->addWidget(labelNombre);


        mainVerticalLayout->addWidget(headerFrame);

        tabWidget = new QTabWidget(Ususarios);
        tabWidget->setObjectName("tabWidget");
        General = new QWidget();
        General->setObjectName("General");
        gridLayout_General = new QGridLayout(General);
        gridLayout_General->setObjectName("gridLayout_General");
        label_13 = new QLabel(General);
        label_13->setObjectName("label_13");

        gridLayout_General->addWidget(label_13, 0, 0, 1, 1);

        lineEditCod = new QLineEdit(General);
        lineEditCod->setObjectName("lineEditCod");
        lineEditCod->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #4a148c;"));

        gridLayout_General->addWidget(lineEditCod, 0, 1, 1, 1);

        label_1 = new QLabel(General);
        label_1->setObjectName("label_1");

        gridLayout_General->addWidget(label_1, 1, 0, 1, 1);

        lineEditNombre = new QLineEdit(General);
        lineEditNombre->setObjectName("lineEditNombre");

        gridLayout_General->addWidget(lineEditNombre, 1, 1, 1, 1);

        label_2 = new QLabel(General);
        label_2->setObjectName("label_2");

        gridLayout_General->addWidget(label_2, 1, 2, 1, 1);

        lineEditApellido = new QLineEdit(General);
        lineEditApellido->setObjectName("lineEditApellido");

        gridLayout_General->addWidget(lineEditApellido, 1, 3, 1, 1);

        label_3 = new QLabel(General);
        label_3->setObjectName("label_3");

        gridLayout_General->addWidget(label_3, 2, 0, 1, 1);

        lineEditDireccion = new QLineEdit(General);
        lineEditDireccion->setObjectName("lineEditDireccion");

        gridLayout_General->addWidget(lineEditDireccion, 2, 1, 1, 3);

        label_4 = new QLabel(General);
        label_4->setObjectName("label_4");

        gridLayout_General->addWidget(label_4, 3, 0, 1, 1);

        lineEditLocalidad = new QLineEdit(General);
        lineEditLocalidad->setObjectName("lineEditLocalidad");

        gridLayout_General->addWidget(lineEditLocalidad, 3, 1, 1, 1);

        label_6 = new QLabel(General);
        label_6->setObjectName("label_6");

        gridLayout_General->addWidget(label_6, 3, 2, 1, 1);

        lineEditCp = new QLineEdit(General);
        lineEditCp->setObjectName("lineEditCp");

        gridLayout_General->addWidget(lineEditCp, 3, 3, 1, 1);

        label_5 = new QLabel(General);
        label_5->setObjectName("label_5");

        gridLayout_General->addWidget(label_5, 4, 0, 1, 1);

        lineEditProvincia = new QLineEdit(General);
        lineEditProvincia->setObjectName("lineEditProvincia");

        gridLayout_General->addWidget(lineEditProvincia, 4, 1, 1, 1);

        label_8 = new QLabel(General);
        label_8->setObjectName("label_8");

        gridLayout_General->addWidget(label_8, 4, 2, 1, 1);

        lineEditNif = new QLineEdit(General);
        lineEditNif->setObjectName("lineEditNif");

        gridLayout_General->addWidget(lineEditNif, 4, 3, 1, 1);

        label_7 = new QLabel(General);
        label_7->setObjectName("label_7");

        gridLayout_General->addWidget(label_7, 5, 0, 1, 1);

        lineEditTlfn = new QLineEdit(General);
        lineEditTlfn->setObjectName("lineEditTlfn");

        gridLayout_General->addWidget(lineEditTlfn, 5, 1, 1, 1);

        label_9 = new QLabel(General);
        label_9->setObjectName("label_9");

        gridLayout_General->addWidget(label_9, 5, 2, 1, 1);

        lineEditMail = new QLineEdit(General);
        lineEditMail->setObjectName("lineEditMail");

        gridLayout_General->addWidget(lineEditMail, 5, 3, 1, 1);

        loginFrame = new QFrame(General);
        loginFrame->setObjectName("loginFrame");
        loginFrame->setStyleSheet(QString::fromUtf8("QFrame#loginFrame { background-color: #f3e5f5; border-radius: 5px; margin-top: 10px; }"));
        horizontalLayout_Login = new QHBoxLayout(loginFrame);
        horizontalLayout_Login->setObjectName("horizontalLayout_Login");
        label_10 = new QLabel(loginFrame);
        label_10->setObjectName("label_10");

        horizontalLayout_Login->addWidget(label_10);

        lineEditUsusario = new QLineEdit(loginFrame);
        lineEditUsusario->setObjectName("lineEditUsusario");

        horizontalLayout_Login->addWidget(lineEditUsusario);

        label_11 = new QLabel(loginFrame);
        label_11->setObjectName("label_11");

        horizontalLayout_Login->addWidget(label_11);

        lineEditClave = new QLineEdit(loginFrame);
        lineEditClave->setObjectName("lineEditClave");
        lineEditClave->setEchoMode(QLineEdit::EchoMode::Password);

        horizontalLayout_Login->addWidget(lineEditClave);

        label_14 = new QLabel(loginFrame);
        label_14->setObjectName("label_14");

        horizontalLayout_Login->addWidget(label_14);

        lineEditPermisos = new QLineEdit(loginFrame);
        lineEditPermisos->setObjectName("lineEditPermisos");
        lineEditPermisos->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_Login->addWidget(lineEditPermisos);

        pushButtonEditarPermisos = new QPushButton(loginFrame);
        pushButtonEditarPermisos->setObjectName("pushButtonEditarPermisos");

        horizontalLayout_Login->addWidget(pushButtonEditarPermisos);


        gridLayout_General->addWidget(loginFrame, 6, 0, 1, 4);

        label_12 = new QLabel(General);
        label_12->setObjectName("label_12");

        gridLayout_General->addWidget(label_12, 7, 0, 1, 4);

        plainTextEdit = new QPlainTextEdit(General);
        plainTextEdit->setObjectName("plainTextEdit");

        gridLayout_General->addWidget(plainTextEdit, 8, 0, 1, 4);

        verticalLayoutFoto = new QVBoxLayout();
        verticalLayoutFoto->setObjectName("verticalLayoutFoto");
        labelFoto = new QLabel(General);
        labelFoto->setObjectName("labelFoto");
        labelFoto->setMinimumSize(QSize(160, 160));
        labelFoto->setMaximumSize(QSize(160, 160));
        labelFoto->setStyleSheet(QString::fromUtf8("border: 1px solid #e1bee7; border-radius: 5px; background: white;"));
        labelFoto->setAlignment(Qt::AlignmentFlag::AlignCenter);
        labelFoto->setScaledContents(true);

        verticalLayoutFoto->addWidget(labelFoto);

        lineEditArchivoFoto = new QLineEdit(General);
        lineEditArchivoFoto->setObjectName("lineEditArchivoFoto");
        lineEditArchivoFoto->setReadOnly(true);

        verticalLayoutFoto->addWidget(lineEditArchivoFoto);

        horizontalLayoutFotoBtns = new QHBoxLayout();
        horizontalLayoutFotoBtns->setObjectName("horizontalLayoutFotoBtns");
        pushButtonCargarImagen = new QPushButton(General);
        pushButtonCargarImagen->setObjectName("pushButtonCargarImagen");

        horizontalLayoutFotoBtns->addWidget(pushButtonCargarImagen);

        pushButtonBorrarImagen = new QPushButton(General);
        pushButtonBorrarImagen->setObjectName("pushButtonBorrarImagen");

        horizontalLayoutFotoBtns->addWidget(pushButtonBorrarImagen);


        verticalLayoutFoto->addLayout(horizontalLayoutFotoBtns);

        verticalSpacerFoto = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayoutFoto->addItem(verticalSpacerFoto);


        gridLayout_General->addLayout(verticalLayoutFoto, 0, 4, 7, 2);

        tabWidget->addTab(General, QString());

        mainVerticalLayout->addWidget(tabWidget);

        horizontalLayoutActions = new QHBoxLayout();
        horizontalLayoutActions->setObjectName("horizontalLayoutActions");
        pushButtonNuevo = new QPushButton(Ususarios);
        pushButtonNuevo->setObjectName("pushButtonNuevo");
        pushButtonNuevo->setMinimumSize(QSize(100, 40));

        horizontalLayoutActions->addWidget(pushButtonNuevo);

        pushButtonModificar = new QPushButton(Ususarios);
        pushButtonModificar->setObjectName("pushButtonModificar");
        pushButtonModificar->setMinimumSize(QSize(100, 40));

        horizontalLayoutActions->addWidget(pushButtonModificar);

        pushButtonBorrar = new QPushButton(Ususarios);
        pushButtonBorrar->setObjectName("pushButtonBorrar");
        pushButtonBorrar->setMinimumSize(QSize(100, 40));

        horizontalLayoutActions->addWidget(pushButtonBorrar);

        pushButtonUnificar = new QPushButton(Ususarios);
        pushButtonUnificar->setObjectName("pushButtonUnificar");
        pushButtonUnificar->setMinimumSize(QSize(120, 40));

        horizontalLayoutActions->addWidget(pushButtonUnificar);

        pushButtonSaneador = new QPushButton(Ususarios);
        pushButtonSaneador->setObjectName("pushButtonSaneador");
        pushButtonSaneador->setMinimumSize(QSize(120, 40));

        horizontalLayoutActions->addWidget(pushButtonSaneador);

        pushButtonRefrescar = new QPushButton(Ususarios);
        pushButtonRefrescar->setObjectName("pushButtonRefrescar");
        pushButtonRefrescar->setMinimumSize(QSize(100, 40));

        horizontalLayoutActions->addWidget(pushButtonRefrescar);

        horizontalSpacerActions = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutActions->addItem(horizontalSpacerActions);

        pushButtonAnterior = new QPushButton(Ususarios);
        pushButtonAnterior->setObjectName("pushButtonAnterior");

        horizontalLayoutActions->addWidget(pushButtonAnterior);

        pushButtonSiguiente = new QPushButton(Ususarios);
        pushButtonSiguiente->setObjectName("pushButtonSiguiente");

        horizontalLayoutActions->addWidget(pushButtonSiguiente);

        pushButtonCerrar = new QPushButton(Ususarios);
        pushButtonCerrar->setObjectName("pushButtonCerrar");
        pushButtonCerrar->setMinimumSize(QSize(100, 40));

        horizontalLayoutActions->addWidget(pushButtonCerrar);


        mainVerticalLayout->addLayout(horizontalLayoutActions);


        retranslateUi(Ususarios);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Ususarios);
    } // setupUi

    void retranslateUi(QDialog *Ususarios)
    {
        Ususarios->setWindowTitle(QCoreApplication::translate("Ususarios", "Gesti\303\263n de Usuarios y Permisos", nullptr));
        labelTitle->setText(QCoreApplication::translate("Ususarios", "Ficha de Usuario", nullptr));
        labelNombre->setText(QCoreApplication::translate("Ususarios", "Nombre del Usuario", nullptr));
        label_13->setText(QCoreApplication::translate("Ususarios", "C\303\263digo:", nullptr));
        label_1->setText(QCoreApplication::translate("Ususarios", "Nombre:", nullptr));
        label_2->setText(QCoreApplication::translate("Ususarios", "Apellidos:", nullptr));
        label_3->setText(QCoreApplication::translate("Ususarios", "Direcci\303\263n:", nullptr));
        label_4->setText(QCoreApplication::translate("Ususarios", "Localidad:", nullptr));
        label_6->setText(QCoreApplication::translate("Ususarios", "C.P.:", nullptr));
        label_5->setText(QCoreApplication::translate("Ususarios", "Provincia:", nullptr));
        label_8->setText(QCoreApplication::translate("Ususarios", "NIF:", nullptr));
        label_7->setText(QCoreApplication::translate("Ususarios", "Tel\303\251fono:", nullptr));
        label_9->setText(QCoreApplication::translate("Ususarios", "Email:", nullptr));
        label_10->setText(QCoreApplication::translate("Ususarios", "Usuario:", nullptr));
        label_11->setText(QCoreApplication::translate("Ususarios", "Clave:", nullptr));
        label_14->setText(QCoreApplication::translate("Ususarios", "Rol Nivel:", nullptr));
        pushButtonEditarPermisos->setText(QCoreApplication::translate("Ususarios", "Editar Permisos", nullptr));
        label_12->setText(QCoreApplication::translate("Ususarios", "Notas:", nullptr));
        labelFoto->setText(QCoreApplication::translate("Ususarios", "Sin Foto", nullptr));
        lineEditArchivoFoto->setPlaceholderText(QCoreApplication::translate("Ususarios", "Ruta de imagen...", nullptr));
        pushButtonCargarImagen->setText(QCoreApplication::translate("Ususarios", "Cargar", nullptr));
        pushButtonBorrarImagen->setText(QCoreApplication::translate("Ususarios", "Borrar", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(General), QCoreApplication::translate("Ususarios", "Datos Personales", nullptr));
        pushButtonNuevo->setText(QCoreApplication::translate("Ususarios", "Nuevo", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("Ususarios", "Guardar", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("Ususarios", "Borrar", nullptr));
        pushButtonUnificar->setText(QCoreApplication::translate("Ususarios", "Unificar", nullptr));
        pushButtonSaneador->setText(QCoreApplication::translate("Ususarios", "Saneador", nullptr));
        pushButtonRefrescar->setText(QCoreApplication::translate("Ususarios", "Refrescar", nullptr));
        pushButtonAnterior->setText(QCoreApplication::translate("Ususarios", "<", nullptr));
        pushButtonSiguiente->setText(QCoreApplication::translate("Ususarios", ">", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("Ususarios", "Cerrar", nullptr));
#if QT_CONFIG(shortcut)
        pushButtonCerrar->setShortcut(QCoreApplication::translate("Ususarios", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
    } // retranslateUi

};

namespace Ui {
    class Ususarios: public Ui_Ususarios {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABWIDGET_H
