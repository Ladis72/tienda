/********************************************************************************
** Form generated from reading UI file 'encargosdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENCARGOSDIALOG_H
#define UI_ENCARGOSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_EncargosDialog
{
public:
    QFormLayout *formLayout;
    QLabel *labelCliente;
    QHBoxLayout *horizontalLayout_cliente;
    QLineEdit *lineEditCliente;
    QPushButton *btnBuscarCliente;
    QLabel *labelArticulo;
    QHBoxLayout *horizontalLayout_articulo;
    QLineEdit *lineEditArticulo;
    QPushButton *btnBuscarArticulo;
    QLabel *labelCantidad;
    QSpinBox *spinBoxCantidad;
    QLabel *labelAnticipo;
    QDoubleSpinBox *doubleSpinBoxAnticipo;
    QLabel *labelEmpleado;
    QLineEdit *lineEditEmpleado;
    QLabel *labelNotas;
    QTextEdit *textEditNotas;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *EncargosDialog)
    {
        if (EncargosDialog->objectName().isEmpty())
            EncargosDialog->setObjectName(QString::fromUtf8("EncargosDialog"));
        EncargosDialog->resize(500, 350);
        formLayout = new QFormLayout(EncargosDialog);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        labelCliente = new QLabel(EncargosDialog);
        labelCliente->setObjectName(QString::fromUtf8("labelCliente"));

        formLayout->setWidget(0, QFormLayout::LabelRole, labelCliente);

        horizontalLayout_cliente = new QHBoxLayout();
        horizontalLayout_cliente->setObjectName(QString::fromUtf8("horizontalLayout_cliente"));
        lineEditCliente = new QLineEdit(EncargosDialog);
        lineEditCliente->setObjectName(QString::fromUtf8("lineEditCliente"));
        lineEditCliente->setReadOnly(true);

        horizontalLayout_cliente->addWidget(lineEditCliente);

        btnBuscarCliente = new QPushButton(EncargosDialog);
        btnBuscarCliente->setObjectName(QString::fromUtf8("btnBuscarCliente"));
        btnBuscarCliente->setMaximumWidth(30);

        horizontalLayout_cliente->addWidget(btnBuscarCliente);


        formLayout->setLayout(0, QFormLayout::FieldRole, horizontalLayout_cliente);

        labelArticulo = new QLabel(EncargosDialog);
        labelArticulo->setObjectName(QString::fromUtf8("labelArticulo"));

        formLayout->setWidget(1, QFormLayout::LabelRole, labelArticulo);

        horizontalLayout_articulo = new QHBoxLayout();
        horizontalLayout_articulo->setObjectName(QString::fromUtf8("horizontalLayout_articulo"));
        lineEditArticulo = new QLineEdit(EncargosDialog);
        lineEditArticulo->setObjectName(QString::fromUtf8("lineEditArticulo"));
        lineEditArticulo->setReadOnly(true);

        horizontalLayout_articulo->addWidget(lineEditArticulo);

        btnBuscarArticulo = new QPushButton(EncargosDialog);
        btnBuscarArticulo->setObjectName(QString::fromUtf8("btnBuscarArticulo"));
        btnBuscarArticulo->setMaximumWidth(30);

        horizontalLayout_articulo->addWidget(btnBuscarArticulo);


        formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout_articulo);

        labelCantidad = new QLabel(EncargosDialog);
        labelCantidad->setObjectName(QString::fromUtf8("labelCantidad"));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelCantidad);

        spinBoxCantidad = new QSpinBox(EncargosDialog);
        spinBoxCantidad->setObjectName(QString::fromUtf8("spinBoxCantidad"));
        spinBoxCantidad->setMinimum(1);
        spinBoxCantidad->setMaximum(9999);

        formLayout->setWidget(2, QFormLayout::FieldRole, spinBoxCantidad);

        labelAnticipo = new QLabel(EncargosDialog);
        labelAnticipo->setObjectName(QString::fromUtf8("labelAnticipo"));

        formLayout->setWidget(3, QFormLayout::LabelRole, labelAnticipo);

        doubleSpinBoxAnticipo = new QDoubleSpinBox(EncargosDialog);
        doubleSpinBoxAnticipo->setObjectName(QString::fromUtf8("doubleSpinBoxAnticipo"));
        doubleSpinBoxAnticipo->setMaximum(9999.989999999999782);

        formLayout->setWidget(3, QFormLayout::FieldRole, doubleSpinBoxAnticipo);

        labelEmpleado = new QLabel(EncargosDialog);
        labelEmpleado->setObjectName(QString::fromUtf8("labelEmpleado"));

        formLayout->setWidget(4, QFormLayout::LabelRole, labelEmpleado);

        lineEditEmpleado = new QLineEdit(EncargosDialog);
        lineEditEmpleado->setObjectName(QString::fromUtf8("lineEditEmpleado"));
        lineEditEmpleado->setReadOnly(true);

        formLayout->setWidget(4, QFormLayout::FieldRole, lineEditEmpleado);

        labelNotas = new QLabel(EncargosDialog);
        labelNotas->setObjectName(QString::fromUtf8("labelNotas"));

        formLayout->setWidget(5, QFormLayout::LabelRole, labelNotas);

        textEditNotas = new QTextEdit(EncargosDialog);
        textEditNotas->setObjectName(QString::fromUtf8("textEditNotas"));

        formLayout->setWidget(5, QFormLayout::FieldRole, textEditNotas);

        buttonBox = new QDialogButtonBox(EncargosDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        formLayout->setWidget(6, QFormLayout::SpanningRole, buttonBox);


        retranslateUi(EncargosDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), EncargosDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), EncargosDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(EncargosDialog);
    } // setupUi

    void retranslateUi(QDialog *EncargosDialog)
    {
        EncargosDialog->setWindowTitle(QCoreApplication::translate("EncargosDialog", "Nuevo Encargo", nullptr));
        labelCliente->setText(QCoreApplication::translate("EncargosDialog", "Cliente:", nullptr));
        btnBuscarCliente->setText(QCoreApplication::translate("EncargosDialog", "...", nullptr));
        labelArticulo->setText(QCoreApplication::translate("EncargosDialog", "Art\303\255culo:", nullptr));
        btnBuscarArticulo->setText(QCoreApplication::translate("EncargosDialog", "...", nullptr));
        labelCantidad->setText(QCoreApplication::translate("EncargosDialog", "Cantidad:", nullptr));
        labelAnticipo->setText(QCoreApplication::translate("EncargosDialog", "Anticipo a cuenta (\342\202\254):", nullptr));
        labelEmpleado->setText(QCoreApplication::translate("EncargosDialog", "Empleado:", nullptr));
        labelNotas->setText(QCoreApplication::translate("EncargosDialog", "Notas:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EncargosDialog: public Ui_EncargosDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENCARGOSDIALOG_H
