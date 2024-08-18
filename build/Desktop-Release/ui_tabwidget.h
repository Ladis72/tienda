/********************************************************************************
** Form generated from reading UI file 'tabwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABWIDGET_H
#define UI_TABWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Ususarios
{
public:
    QWidget *General;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLineEdit *lineEditCp;
    QLabel *label_9;
    QLineEdit *lineEditUsusario;
    QLabel *label_6;
    QLabel *label_11;
    QLabel *label_10;
    QLabel *label;
    QLabel *label_8;
    QLabel *label_7;
    QLineEdit *lineEditTlfn;
    QLineEdit *lineEditClave;
    QLineEdit *lineEditNombre;
    QLabel *label_14;
    QLineEdit *lineEditProvincia;
    QLabel *label_5;
    QLineEdit *lineEditNif;
    QLabel *label_4;
    QLabel *label_3;
    QLineEdit *lineEditDireccion;
    QLineEdit *lineEditMail;
    QLabel *label_2;
    QPlainTextEdit *plainTextEdit;
    QLineEdit *lineEditLocalidad;
    QLabel *label_12;
    QLineEdit *lineEditArchivoFoto;
    QPushButton *pushButtonCargarImagen;
    QPushButton *pushButtonBorrarImagen;
    QLineEdit *lineEditPermisos;
    QLineEdit *lineEditApellido;
    QLabel *labelFoto;
    QGridLayout *gridLayout_3;
    QLabel *label_13;
    QLineEdit *lineEditCod;
    QLabel *labelNombre;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonNuevo;
    QPushButton *pushButtonModificar;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonRefrescar;
    QPushButton *pushButtonAnterior;
    QPushButton *pushButtonSiguiente;
    QPushButton *pushButtonCerrar;
    QWidget *tab;

    void setupUi(QTabWidget *Ususarios)
    {
        if (Ususarios->objectName().isEmpty())
            Ususarios->setObjectName(QString::fromUtf8("Ususarios"));
        Ususarios->setWindowModality(Qt::WindowModality::WindowModal);
        Ususarios->resize(1016, 549);
        General = new QWidget();
        General->setObjectName(QString::fromUtf8("General"));
        gridLayout_2 = new QGridLayout(General);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineEditCp = new QLineEdit(General);
        lineEditCp->setObjectName(QString::fromUtf8("lineEditCp"));

        gridLayout->addWidget(lineEditCp, 2, 3, 1, 1);

        label_9 = new QLabel(General);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 4, 0, 1, 1);

        lineEditUsusario = new QLineEdit(General);
        lineEditUsusario->setObjectName(QString::fromUtf8("lineEditUsusario"));

        gridLayout->addWidget(lineEditUsusario, 5, 1, 1, 1);

        label_6 = new QLabel(General);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 2, 2, 1, 1);

        label_11 = new QLabel(General);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 5, 2, 1, 1);

        label_10 = new QLabel(General);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 5, 0, 1, 1);

        label = new QLabel(General);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_8 = new QLabel(General);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 3, 2, 1, 1);

        label_7 = new QLabel(General);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 3, 0, 1, 1);

        lineEditTlfn = new QLineEdit(General);
        lineEditTlfn->setObjectName(QString::fromUtf8("lineEditTlfn"));

        gridLayout->addWidget(lineEditTlfn, 3, 1, 1, 1);

        lineEditClave = new QLineEdit(General);
        lineEditClave->setObjectName(QString::fromUtf8("lineEditClave"));
        lineEditClave->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout->addWidget(lineEditClave, 5, 3, 1, 1);

        lineEditNombre = new QLineEdit(General);
        lineEditNombre->setObjectName(QString::fromUtf8("lineEditNombre"));

        gridLayout->addWidget(lineEditNombre, 0, 1, 1, 1);

        label_14 = new QLabel(General);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout->addWidget(label_14, 6, 0, 1, 1);

        lineEditProvincia = new QLineEdit(General);
        lineEditProvincia->setObjectName(QString::fromUtf8("lineEditProvincia"));

        gridLayout->addWidget(lineEditProvincia, 2, 1, 1, 1);

        label_5 = new QLabel(General);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 2, 0, 1, 1);

        lineEditNif = new QLineEdit(General);
        lineEditNif->setObjectName(QString::fromUtf8("lineEditNif"));

        gridLayout->addWidget(lineEditNif, 3, 3, 1, 1);

        label_4 = new QLabel(General);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 2, 1, 1);

        label_3 = new QLabel(General);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        lineEditDireccion = new QLineEdit(General);
        lineEditDireccion->setObjectName(QString::fromUtf8("lineEditDireccion"));

        gridLayout->addWidget(lineEditDireccion, 1, 1, 1, 1);

        lineEditMail = new QLineEdit(General);
        lineEditMail->setObjectName(QString::fromUtf8("lineEditMail"));

        gridLayout->addWidget(lineEditMail, 4, 1, 1, 3);

        label_2 = new QLabel(General);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        plainTextEdit = new QPlainTextEdit(General);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        gridLayout->addWidget(plainTextEdit, 8, 0, 1, 6);

        lineEditLocalidad = new QLineEdit(General);
        lineEditLocalidad->setObjectName(QString::fromUtf8("lineEditLocalidad"));

        gridLayout->addWidget(lineEditLocalidad, 1, 3, 1, 1);

        label_12 = new QLabel(General);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout->addWidget(label_12, 7, 0, 1, 1);

        lineEditArchivoFoto = new QLineEdit(General);
        lineEditArchivoFoto->setObjectName(QString::fromUtf8("lineEditArchivoFoto"));
        lineEditArchivoFoto->setEnabled(false);
        lineEditArchivoFoto->setAlignment(Qt::AlignmentFlag::AlignBottom|Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft);

        gridLayout->addWidget(lineEditArchivoFoto, 7, 3, 1, 1);

        pushButtonCargarImagen = new QPushButton(General);
        pushButtonCargarImagen->setObjectName(QString::fromUtf8("pushButtonCargarImagen"));

        gridLayout->addWidget(pushButtonCargarImagen, 7, 4, 1, 1);

        pushButtonBorrarImagen = new QPushButton(General);
        pushButtonBorrarImagen->setObjectName(QString::fromUtf8("pushButtonBorrarImagen"));

        gridLayout->addWidget(pushButtonBorrarImagen, 7, 5, 1, 1);

        lineEditPermisos = new QLineEdit(General);
        lineEditPermisos->setObjectName(QString::fromUtf8("lineEditPermisos"));

        gridLayout->addWidget(lineEditPermisos, 6, 1, 1, 1);

        lineEditApellido = new QLineEdit(General);
        lineEditApellido->setObjectName(QString::fromUtf8("lineEditApellido"));

        gridLayout->addWidget(lineEditApellido, 0, 3, 1, 1);

        labelFoto = new QLabel(General);
        labelFoto->setObjectName(QString::fromUtf8("labelFoto"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelFoto->sizePolicy().hasHeightForWidth());
        labelFoto->setSizePolicy(sizePolicy);
        labelFoto->setMaximumSize(QSize(200, 200));
        labelFoto->setScaledContents(true);
        labelFoto->setAlignment(Qt::AlignmentFlag::AlignJustify|Qt::AlignmentFlag::AlignVCenter);
        labelFoto->setWordWrap(false);

        gridLayout->addWidget(labelFoto, 0, 4, 7, 2);


        gridLayout_2->addLayout(gridLayout, 1, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_13 = new QLabel(General);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_3->addWidget(label_13, 0, 0, 1, 1);

        lineEditCod = new QLineEdit(General);
        lineEditCod->setObjectName(QString::fromUtf8("lineEditCod"));
        lineEditCod->setEnabled(true);
        lineEditCod->setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);

        gridLayout_3->addWidget(lineEditCod, 0, 1, 1, 1);

        labelNombre = new QLabel(General);
        labelNombre->setObjectName(QString::fromUtf8("labelNombre"));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(14);
        font.setItalic(true);
        labelNombre->setFont(font);

        gridLayout_3->addWidget(labelNombre, 0, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout_3, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonNuevo = new QPushButton(General);
        pushButtonNuevo->setObjectName(QString::fromUtf8("pushButtonNuevo"));

        horizontalLayout->addWidget(pushButtonNuevo);

        pushButtonModificar = new QPushButton(General);
        pushButtonModificar->setObjectName(QString::fromUtf8("pushButtonModificar"));

        horizontalLayout->addWidget(pushButtonModificar);

        pushButtonBorrar = new QPushButton(General);
        pushButtonBorrar->setObjectName(QString::fromUtf8("pushButtonBorrar"));

        horizontalLayout->addWidget(pushButtonBorrar);

        pushButtonRefrescar = new QPushButton(General);
        pushButtonRefrescar->setObjectName(QString::fromUtf8("pushButtonRefrescar"));

        horizontalLayout->addWidget(pushButtonRefrescar);

        pushButtonAnterior = new QPushButton(General);
        pushButtonAnterior->setObjectName(QString::fromUtf8("pushButtonAnterior"));

        horizontalLayout->addWidget(pushButtonAnterior);

        pushButtonSiguiente = new QPushButton(General);
        pushButtonSiguiente->setObjectName(QString::fromUtf8("pushButtonSiguiente"));

        horizontalLayout->addWidget(pushButtonSiguiente);

        pushButtonCerrar = new QPushButton(General);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));

        horizontalLayout->addWidget(pushButtonCerrar);


        gridLayout_2->addLayout(horizontalLayout, 3, 0, 1, 1);

        Ususarios->addTab(General, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        Ususarios->addTab(tab, QString());
#if QT_CONFIG(shortcut)
        label_9->setBuddy(lineEditMail);
        label_6->setBuddy(lineEditCp);
        label_11->setBuddy(lineEditClave);
        label_10->setBuddy(lineEditUsusario);
        label->setBuddy(lineEditNombre);
        label_8->setBuddy(lineEditNif);
        label_7->setBuddy(lineEditTlfn);
        label_14->setBuddy(lineEditPermisos);
        label_5->setBuddy(lineEditProvincia);
        label_4->setBuddy(lineEditLocalidad);
        label_3->setBuddy(lineEditDireccion);
        label_2->setBuddy(lineEditApellido);
        label_13->setBuddy(lineEditCod);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(lineEditCod, lineEditNombre);
        QWidget::setTabOrder(lineEditNombre, lineEditApellido);
        QWidget::setTabOrder(lineEditApellido, lineEditDireccion);
        QWidget::setTabOrder(lineEditDireccion, lineEditLocalidad);
        QWidget::setTabOrder(lineEditLocalidad, lineEditProvincia);
        QWidget::setTabOrder(lineEditProvincia, lineEditCp);
        QWidget::setTabOrder(lineEditCp, lineEditTlfn);
        QWidget::setTabOrder(lineEditTlfn, lineEditNif);
        QWidget::setTabOrder(lineEditNif, lineEditMail);
        QWidget::setTabOrder(lineEditMail, lineEditUsusario);
        QWidget::setTabOrder(lineEditUsusario, lineEditClave);
        QWidget::setTabOrder(lineEditClave, pushButtonCargarImagen);
        QWidget::setTabOrder(pushButtonCargarImagen, pushButtonBorrarImagen);
        QWidget::setTabOrder(pushButtonBorrarImagen, plainTextEdit);
        QWidget::setTabOrder(plainTextEdit, lineEditArchivoFoto);
        QWidget::setTabOrder(lineEditArchivoFoto, pushButtonNuevo);
        QWidget::setTabOrder(pushButtonNuevo, pushButtonModificar);
        QWidget::setTabOrder(pushButtonModificar, pushButtonBorrar);
        QWidget::setTabOrder(pushButtonBorrar, pushButtonRefrescar);
        QWidget::setTabOrder(pushButtonRefrescar, pushButtonAnterior);
        QWidget::setTabOrder(pushButtonAnterior, pushButtonSiguiente);
        QWidget::setTabOrder(pushButtonSiguiente, pushButtonCerrar);

        retranslateUi(Ususarios);
        QObject::connect(pushButtonCerrar, SIGNAL(clicked()), Ususarios, SLOT(close()));

        QMetaObject::connectSlotsByName(Ususarios);
    } // setupUi

    void retranslateUi(QTabWidget *Ususarios)
    {
        Ususarios->setWindowTitle(QCoreApplication::translate("Ususarios", "Usuarios", nullptr));
        label_9->setText(QCoreApplication::translate("Ususarios", "Correo", nullptr));
        label_6->setText(QCoreApplication::translate("Ususarios", "C.P.", nullptr));
        label_11->setText(QCoreApplication::translate("Ususarios", "Cla&ve", nullptr));
        label_10->setText(QCoreApplication::translate("Ususarios", "&Usuario", nullptr));
        label->setText(QCoreApplication::translate("Ususarios", "&Nombre", nullptr));
        label_8->setText(QCoreApplication::translate("Ususarios", "NIF", nullptr));
        label_7->setText(QCoreApplication::translate("Ususarios", "Telefono", nullptr));
        lineEditTlfn->setInputMask(QCoreApplication::translate("Ususarios", "999-99-99-99", nullptr));
        label_14->setText(QCoreApplication::translate("Ususarios", "Per&misos", nullptr));
        label_5->setText(QCoreApplication::translate("Ususarios", "Provincia", nullptr));
        lineEditNif->setInputMask(QCoreApplication::translate("Ususarios", "99.999.999->A", nullptr));
        label_4->setText(QCoreApplication::translate("Ususarios", "&Localidad", nullptr));
        label_3->setText(QCoreApplication::translate("Ususarios", "Direccion", nullptr));
        label_2->setText(QCoreApplication::translate("Ususarios", "Apellidos", nullptr));
        label_12->setText(QCoreApplication::translate("Ususarios", "Notas", nullptr));
        pushButtonCargarImagen->setText(QCoreApplication::translate("Ususarios", "Cargar Imagen", nullptr));
        pushButtonBorrarImagen->setText(QCoreApplication::translate("Ususarios", "Borrar Imagen", nullptr));
        labelFoto->setText(QCoreApplication::translate("Ususarios", "Sin foto", nullptr));
        label_13->setText(QCoreApplication::translate("Ususarios", "Codigo", nullptr));
        labelNombre->setText(QCoreApplication::translate("Ususarios", "Nombre de usuario", nullptr));
        pushButtonNuevo->setText(QCoreApplication::translate("Ususarios", "Nuevo", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("Ususarios", "Modificar", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("Ususarios", "Borrar", nullptr));
        pushButtonRefrescar->setText(QCoreApplication::translate("Ususarios", "Refrescar", nullptr));
        pushButtonAnterior->setText(QCoreApplication::translate("Ususarios", "Anterior", nullptr));
        pushButtonSiguiente->setText(QCoreApplication::translate("Ususarios", "Siguiente", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("Ususarios", "Cerrar", nullptr));
#if QT_CONFIG(shortcut)
        pushButtonCerrar->setShortcut(QCoreApplication::translate("Ususarios", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        Ususarios->setTabText(Ususarios->indexOf(General), QCoreApplication::translate("Ususarios", "General", nullptr));
        Ususarios->setTabText(Ususarios->indexOf(tab), QCoreApplication::translate("Ususarios", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Ususarios: public Ui_Ususarios {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABWIDGET_H
