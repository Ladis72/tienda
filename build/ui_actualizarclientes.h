/********************************************************************************
** Form generated from reading UI file 'actualizarclientes.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACTUALIZARCLIENTES_H
#define UI_ACTUALIZARCLIENTES_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ActualizarClientes
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QPushButton *pushButtonActualizar;
    QPushButton *pushButton;
    QListWidget *listWidget;

    void setupUi(QDialog *ActualizarClientes)
    {
        if (ActualizarClientes->objectName().isEmpty())
            ActualizarClientes->setObjectName("ActualizarClientes");
        ActualizarClientes->resize(400, 300);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imagenes/Emeicjac logo.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        ActualizarClientes->setWindowIcon(icon);
        gridLayout = new QGridLayout(ActualizarClientes);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(ActualizarClientes);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        pushButtonActualizar = new QPushButton(ActualizarClientes);
        pushButtonActualizar->setObjectName("pushButtonActualizar");

        gridLayout->addWidget(pushButtonActualizar, 2, 0, 1, 1);

        pushButton = new QPushButton(ActualizarClientes);
        pushButton->setObjectName("pushButton");

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);

        listWidget = new QListWidget(ActualizarClientes);
        listWidget->setObjectName("listWidget");
        listWidget->setItemAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(listWidget, 1, 0, 1, 2);


        retranslateUi(ActualizarClientes);
        QObject::connect(pushButton, &QPushButton::clicked, ActualizarClientes, qOverload<>(&QDialog::close));

        QMetaObject::connectSlotsByName(ActualizarClientes);
    } // setupUi

    void retranslateUi(QDialog *ActualizarClientes)
    {
        ActualizarClientes->setWindowTitle(QCoreApplication::translate("ActualizarClientes", "Actualizar clientes", nullptr));
        label->setText(QCoreApplication::translate("ActualizarClientes", "Lista de ordenadores remotos", nullptr));
        pushButtonActualizar->setText(QCoreApplication::translate("ActualizarClientes", "Actualizar", nullptr));
        pushButton->setText(QCoreApplication::translate("ActualizarClientes", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ActualizarClientes: public Ui_ActualizarClientes {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACTUALIZARCLIENTES_H
