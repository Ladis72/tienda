/********************************************************************************
** Form generated from reading UI file 'proveedores.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROVEEDORES_H
#define UI_PROVEEDORES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Proveedores
{
public:
    QGridLayout *gridLayout_2;
    QLabel *labelNombre;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout;
    QGridLayout *General;
    QLabel *label_13;
    QLabel *label_11;
    QLineEdit *lineEditDescuento;
    QLabel *label_9;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label;
    QLineEdit *lineEditTelefono;
    QLineEdit *lineEditMailR;
    QLabel *label_12;
    QLineEdit *lineEditRepresentante;
    QLabel *label_10;
    QLabel *label_8;
    QLabel *labelFPago;
    QLabel *label_7;
    QPlainTextEdit *plainTextEdit;
    QLabel *label_15;
    QLabel *label_14;
    QLineEdit *lineEditProvincia;
    QLabel *label_5;
    QLineEdit *lineEditDireccion;
    QLabel *label_4;
    QLineEdit *lineEditTelefonoR;
    QLineEdit *lineEditMail;
    QLineEdit *lineEditFechaUltimaCompra;
    QLineEdit *lineEditCod;
    QLineEdit *lineEditNombre;
    QLabel *label_6;
    QLineEdit *lineEditLocalidad;
    QLineEdit *lineEditCP;
    QLineEdit *lineEditFormaPago;
    QPushButton *pushButtonFPago;
    QLabel *label_16;
    QLineEdit *lineEditNIF;
    QPushButton *pushButton;
    QWidget *Compras;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButtonComprasFacturas;
    QRadioButton *radioButtonComprasMeses;
    QRadioButton *radioButtonComprasAnos;
    QTableView *tableViewCompras;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonNuevo;
    QPushButton *pushButtonModificar;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonRefrescar;
    QPushButton *pushButtonAnterior;
    QPushButton *pushButtonSiguiente;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *Proveedores)
    {
        if (Proveedores->objectName().isEmpty())
            Proveedores->setObjectName(QString::fromUtf8("Proveedores"));
        Proveedores->resize(1128, 533);
        gridLayout_2 = new QGridLayout(Proveedores);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        labelNombre = new QLabel(Proveedores);
        labelNombre->setObjectName(QString::fromUtf8("labelNombre"));
        QFont font;
        font.setPointSize(15);
        labelNombre->setFont(font);

        gridLayout_2->addWidget(labelNombre, 0, 1, 1, 1);

        tabWidget = new QTabWidget(Proveedores);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout = new QGridLayout(tab);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        General = new QGridLayout();
        General->setObjectName(QString::fromUtf8("General"));
        label_13 = new QLabel(tab);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        General->addWidget(label_13, 7, 0, 1, 1);

        label_11 = new QLabel(tab);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        General->addWidget(label_11, 4, 5, 1, 1);

        lineEditDescuento = new QLineEdit(tab);
        lineEditDescuento->setObjectName(QString::fromUtf8("lineEditDescuento"));

        General->addWidget(lineEditDescuento, 6, 1, 1, 1);

        label_9 = new QLabel(tab);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        General->addWidget(label_9, 5, 2, 1, 1);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        General->addWidget(label_3, 2, 0, 1, 1);

        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        General->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));

        General->addWidget(label, 0, 0, 1, 1);

        lineEditTelefono = new QLineEdit(tab);
        lineEditTelefono->setObjectName(QString::fromUtf8("lineEditTelefono"));

        General->addWidget(lineEditTelefono, 5, 1, 1, 1);

        lineEditMailR = new QLineEdit(tab);
        lineEditMailR->setObjectName(QString::fromUtf8("lineEditMailR"));

        General->addWidget(lineEditMailR, 4, 6, 1, 1);

        label_12 = new QLabel(tab);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        General->addWidget(label_12, 6, 0, 1, 1);

        lineEditRepresentante = new QLineEdit(tab);
        lineEditRepresentante->setObjectName(QString::fromUtf8("lineEditRepresentante"));

        General->addWidget(lineEditRepresentante, 4, 1, 1, 1);

        label_10 = new QLabel(tab);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        General->addWidget(label_10, 4, 2, 1, 1);

        label_8 = new QLabel(tab);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        General->addWidget(label_8, 5, 0, 1, 1);

        labelFPago = new QLabel(tab);
        labelFPago->setObjectName(QString::fromUtf8("labelFPago"));

        General->addWidget(labelFPago, 6, 6, 1, 1);

        label_7 = new QLabel(tab);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        General->addWidget(label_7, 4, 0, 1, 1);

        plainTextEdit = new QPlainTextEdit(tab);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        General->addWidget(plainTextEdit, 8, 0, 1, 7);

        label_15 = new QLabel(tab);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        General->addWidget(label_15, 6, 2, 1, 1);

        label_14 = new QLabel(tab);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        General->addWidget(label_14, 7, 2, 1, 1);

        lineEditProvincia = new QLineEdit(tab);
        lineEditProvincia->setObjectName(QString::fromUtf8("lineEditProvincia"));

        General->addWidget(lineEditProvincia, 3, 1, 1, 2);

        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        General->addWidget(label_5, 3, 0, 1, 1);

        lineEditDireccion = new QLineEdit(tab);
        lineEditDireccion->setObjectName(QString::fromUtf8("lineEditDireccion"));

        General->addWidget(lineEditDireccion, 2, 1, 1, 2);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        General->addWidget(label_4, 2, 3, 1, 1);

        lineEditTelefonoR = new QLineEdit(tab);
        lineEditTelefonoR->setObjectName(QString::fromUtf8("lineEditTelefonoR"));

        General->addWidget(lineEditTelefonoR, 4, 3, 1, 1);

        lineEditMail = new QLineEdit(tab);
        lineEditMail->setObjectName(QString::fromUtf8("lineEditMail"));

        General->addWidget(lineEditMail, 5, 3, 1, 1);

        lineEditFechaUltimaCompra = new QLineEdit(tab);
        lineEditFechaUltimaCompra->setObjectName(QString::fromUtf8("lineEditFechaUltimaCompra"));
        lineEditFechaUltimaCompra->setEnabled(false);

        General->addWidget(lineEditFechaUltimaCompra, 7, 3, 1, 1);

        lineEditCod = new QLineEdit(tab);
        lineEditCod->setObjectName(QString::fromUtf8("lineEditCod"));

        General->addWidget(lineEditCod, 0, 1, 1, 1);

        lineEditNombre = new QLineEdit(tab);
        lineEditNombre->setObjectName(QString::fromUtf8("lineEditNombre"));

        General->addWidget(lineEditNombre, 1, 1, 1, 2);

        label_6 = new QLabel(tab);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        General->addWidget(label_6, 3, 3, 1, 1);

        lineEditLocalidad = new QLineEdit(tab);
        lineEditLocalidad->setObjectName(QString::fromUtf8("lineEditLocalidad"));

        General->addWidget(lineEditLocalidad, 2, 5, 1, 1);

        lineEditCP = new QLineEdit(tab);
        lineEditCP->setObjectName(QString::fromUtf8("lineEditCP"));

        General->addWidget(lineEditCP, 3, 5, 1, 1);

        lineEditFormaPago = new QLineEdit(tab);
        lineEditFormaPago->setObjectName(QString::fromUtf8("lineEditFormaPago"));

        General->addWidget(lineEditFormaPago, 6, 3, 1, 1);

        pushButtonFPago = new QPushButton(tab);
        pushButtonFPago->setObjectName(QString::fromUtf8("pushButtonFPago"));
        pushButtonFPago->setAutoDefault(false);

        General->addWidget(pushButtonFPago, 6, 5, 1, 1);

        label_16 = new QLabel(tab);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        General->addWidget(label_16, 1, 3, 1, 1);

        lineEditNIF = new QLineEdit(tab);
        lineEditNIF->setObjectName(QString::fromUtf8("lineEditNIF"));

        General->addWidget(lineEditNIF, 1, 5, 1, 1);

        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setAutoDefault(false);

        General->addWidget(pushButton, 0, 2, 1, 1);


        gridLayout->addLayout(General, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        Compras = new QWidget();
        Compras->setObjectName(QString::fromUtf8("Compras"));
        gridLayout_3 = new QGridLayout(Compras);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox = new QGroupBox(Compras);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        radioButtonComprasFacturas = new QRadioButton(groupBox);
        radioButtonComprasFacturas->setObjectName(QString::fromUtf8("radioButtonComprasFacturas"));

        horizontalLayout_2->addWidget(radioButtonComprasFacturas);

        radioButtonComprasMeses = new QRadioButton(groupBox);
        radioButtonComprasMeses->setObjectName(QString::fromUtf8("radioButtonComprasMeses"));
        radioButtonComprasMeses->setChecked(true);

        horizontalLayout_2->addWidget(radioButtonComprasMeses);

        radioButtonComprasAnos = new QRadioButton(groupBox);
        radioButtonComprasAnos->setObjectName(QString::fromUtf8("radioButtonComprasAnos"));

        horizontalLayout_2->addWidget(radioButtonComprasAnos);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 1);

        tableViewCompras = new QTableView(Compras);
        tableViewCompras->setObjectName(QString::fromUtf8("tableViewCompras"));

        gridLayout_3->addWidget(tableViewCompras, 1, 0, 1, 1);

        tabWidget->addTab(Compras, QString());

        gridLayout_2->addWidget(tabWidget, 2, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonNuevo = new QPushButton(Proveedores);
        pushButtonNuevo->setObjectName(QString::fromUtf8("pushButtonNuevo"));
        pushButtonNuevo->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonNuevo);

        pushButtonModificar = new QPushButton(Proveedores);
        pushButtonModificar->setObjectName(QString::fromUtf8("pushButtonModificar"));
        pushButtonModificar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonModificar);

        pushButtonBorrar = new QPushButton(Proveedores);
        pushButtonBorrar->setObjectName(QString::fromUtf8("pushButtonBorrar"));
        pushButtonBorrar->setEnabled(false);
        pushButtonBorrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonBorrar);

        pushButtonRefrescar = new QPushButton(Proveedores);
        pushButtonRefrescar->setObjectName(QString::fromUtf8("pushButtonRefrescar"));
        pushButtonRefrescar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonRefrescar);

        pushButtonAnterior = new QPushButton(Proveedores);
        pushButtonAnterior->setObjectName(QString::fromUtf8("pushButtonAnterior"));
        pushButtonAnterior->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonAnterior);

        pushButtonSiguiente = new QPushButton(Proveedores);
        pushButtonSiguiente->setObjectName(QString::fromUtf8("pushButtonSiguiente"));
        pushButtonSiguiente->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonSiguiente);

        pushButtonCerrar = new QPushButton(Proveedores);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));
        pushButtonCerrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonCerrar);


        gridLayout_2->addLayout(horizontalLayout, 3, 1, 1, 1);

#if QT_CONFIG(shortcut)
        label_11->setBuddy(lineEditMailR);
        label_9->setBuddy(lineEditMail);
        label_3->setBuddy(lineEditDireccion);
        label_2->setBuddy(lineEditNombre);
        label->setBuddy(lineEditCod);
        label_12->setBuddy(lineEditDescuento);
        label_10->setBuddy(lineEditTelefono);
        label_8->setBuddy(lineEditTelefonoR);
        label_7->setBuddy(lineEditRepresentante);
        label_15->setBuddy(lineEditFormaPago);
        label_5->setBuddy(lineEditProvincia);
        label_4->setBuddy(lineEditLocalidad);
        label_6->setBuddy(lineEditCP);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(lineEditCod, lineEditNombre);
        QWidget::setTabOrder(lineEditNombre, lineEditNIF);
        QWidget::setTabOrder(lineEditNIF, lineEditDireccion);
        QWidget::setTabOrder(lineEditDireccion, lineEditLocalidad);
        QWidget::setTabOrder(lineEditLocalidad, lineEditProvincia);
        QWidget::setTabOrder(lineEditProvincia, lineEditCP);
        QWidget::setTabOrder(lineEditCP, lineEditRepresentante);
        QWidget::setTabOrder(lineEditRepresentante, lineEditTelefonoR);
        QWidget::setTabOrder(lineEditTelefonoR, lineEditMailR);
        QWidget::setTabOrder(lineEditMailR, lineEditTelefono);
        QWidget::setTabOrder(lineEditTelefono, lineEditMail);
        QWidget::setTabOrder(lineEditMail, lineEditDescuento);
        QWidget::setTabOrder(lineEditDescuento, lineEditFormaPago);
        QWidget::setTabOrder(lineEditFormaPago, pushButtonFPago);
        QWidget::setTabOrder(pushButtonFPago, plainTextEdit);
        QWidget::setTabOrder(plainTextEdit, pushButtonNuevo);
        QWidget::setTabOrder(pushButtonNuevo, pushButtonModificar);
        QWidget::setTabOrder(pushButtonModificar, pushButtonBorrar);
        QWidget::setTabOrder(pushButtonBorrar, pushButtonRefrescar);
        QWidget::setTabOrder(pushButtonRefrescar, pushButtonAnterior);
        QWidget::setTabOrder(pushButtonAnterior, pushButtonSiguiente);
        QWidget::setTabOrder(pushButtonSiguiente, pushButtonCerrar);
        QWidget::setTabOrder(pushButtonCerrar, lineEditFechaUltimaCompra);

        retranslateUi(Proveedores);
        QObject::connect(pushButtonCerrar, SIGNAL(clicked()), Proveedores, SLOT(close()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Proveedores);
    } // setupUi

    void retranslateUi(QDialog *Proveedores)
    {
        Proveedores->setWindowTitle(QCoreApplication::translate("Proveedores", "Proveedores", nullptr));
        labelNombre->setText(QCoreApplication::translate("Proveedores", "Nombre proveedor", nullptr));
        label_13->setText(QCoreApplication::translate("Proveedores", "Notas", nullptr));
        label_11->setText(QCoreApplication::translate("Proveedores", "E-mail", nullptr));
        label_9->setText(QCoreApplication::translate("Proveedores", "E-&mail", nullptr));
        label_3->setText(QCoreApplication::translate("Proveedores", "Direcci&\303\263n", nullptr));
        label_2->setText(QCoreApplication::translate("Proveedores", "&Nombre", nullptr));
        label->setText(QCoreApplication::translate("Proveedores", "C&\303\263digo", nullptr));
        label_12->setText(QCoreApplication::translate("Proveedores", "Descuento", nullptr));
        label_10->setText(QCoreApplication::translate("Proveedores", "Te&lefono", nullptr));
        label_8->setText(QCoreApplication::translate("Proveedores", "Telefono", nullptr));
        labelFPago->setText(QCoreApplication::translate("Proveedores", "Forma de pago", nullptr));
        label_7->setText(QCoreApplication::translate("Proveedores", "Representante", nullptr));
        label_15->setText(QCoreApplication::translate("Proveedores", "Forma de pago", nullptr));
        label_14->setText(QCoreApplication::translate("Proveedores", "Fecha ultima compra", nullptr));
        label_5->setText(QCoreApplication::translate("Proveedores", "Pro&vincia", nullptr));
        label_4->setText(QCoreApplication::translate("Proveedores", "&Localidad", nullptr));
        label_6->setText(QCoreApplication::translate("Proveedores", "C.P.", nullptr));
        pushButtonFPago->setText(QCoreApplication::translate("Proveedores", "...", nullptr));
        label_16->setText(QCoreApplication::translate("Proveedores", "N.I.F.", nullptr));
        pushButton->setText(QCoreApplication::translate("Proveedores", "Nuevo c\303\263digo", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("Proveedores", "General", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Proveedores", "Ver por:", nullptr));
        radioButtonComprasFacturas->setText(QCoreApplication::translate("Proveedores", "Facturas", nullptr));
        radioButtonComprasMeses->setText(QCoreApplication::translate("Proveedores", "&Meses", nullptr));
        radioButtonComprasAnos->setText(QCoreApplication::translate("Proveedores", "A&\303\261os", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Compras), QCoreApplication::translate("Proveedores", "Compras", nullptr));
        pushButtonNuevo->setText(QCoreApplication::translate("Proveedores", "Nuevo", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("Proveedores", "Modificar", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("Proveedores", "Borrar", nullptr));
        pushButtonRefrescar->setText(QCoreApplication::translate("Proveedores", "Refescar", nullptr));
        pushButtonAnterior->setText(QCoreApplication::translate("Proveedores", "Anterior", nullptr));
        pushButtonSiguiente->setText(QCoreApplication::translate("Proveedores", "Siguente", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("Proveedores", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Proveedores: public Ui_Proveedores {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROVEEDORES_H
