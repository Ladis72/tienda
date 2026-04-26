/********************************************************************************
** Form generated from reading UI file 'buscarcliente.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUSCARCLIENTE_H
#define UI_BUSCARCLIENTE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_BuscarCliente
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_top;
    QLineEdit *lineEdit_buscar;
    QHBoxLayout *horizontalLayout_content;
    QTableView *tableView;
    QFrame *frame_detalles;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_info;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonCancelar;
    QPushButton *pushButtonAceptar;

    void setupUi(QDialog *BuscarCliente)
    {
        if (BuscarCliente->objectName().isEmpty())
            BuscarCliente->setObjectName("BuscarCliente");
        BuscarCliente->resize(1024, 600);
        verticalLayout = new QVBoxLayout(BuscarCliente);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_top = new QHBoxLayout();
        horizontalLayout_top->setObjectName("horizontalLayout_top");
        lineEdit_buscar = new QLineEdit(BuscarCliente);
        lineEdit_buscar->setObjectName("lineEdit_buscar");

        horizontalLayout_top->addWidget(lineEdit_buscar);


        verticalLayout->addLayout(horizontalLayout_top);

        horizontalLayout_content = new QHBoxLayout();
        horizontalLayout_content->setObjectName("horizontalLayout_content");
        tableView = new QTableView(BuscarCliente);
        tableView->setObjectName("tableView");
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        horizontalLayout_content->addWidget(tableView);

        frame_detalles = new QFrame(BuscarCliente);
        frame_detalles->setObjectName("frame_detalles");
        frame_detalles->setMinimumSize(QSize(250, 0));
        frame_detalles->setMaximumSize(QSize(300, 16777215));
        frame_detalles->setFrameShape(QFrame::Shape::StyledPanel);
        frame_detalles->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_detalles);
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_info = new QLabel(frame_detalles);
        label_info->setObjectName("label_info");
        label_info->setWordWrap(true);

        verticalLayout_2->addWidget(label_info);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_content->addWidget(frame_detalles);


        verticalLayout->addLayout(horizontalLayout_content);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonCancelar = new QPushButton(BuscarCliente);
        pushButtonCancelar->setObjectName("pushButtonCancelar");

        horizontalLayout->addWidget(pushButtonCancelar);

        pushButtonAceptar = new QPushButton(BuscarCliente);
        pushButtonAceptar->setObjectName("pushButtonAceptar");

        horizontalLayout->addWidget(pushButtonAceptar);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(BuscarCliente);

        QMetaObject::connectSlotsByName(BuscarCliente);
    } // setupUi

    void retranslateUi(QDialog *BuscarCliente)
    {
        BuscarCliente->setWindowTitle(QCoreApplication::translate("BuscarCliente", "Buscar clientes", nullptr));
        lineEdit_buscar->setPlaceholderText(QCoreApplication::translate("BuscarCliente", "Buscar cliente...", nullptr));
        label_info->setText(QCoreApplication::translate("BuscarCliente", "Seleccione un cliente", nullptr));
        pushButtonCancelar->setText(QCoreApplication::translate("BuscarCliente", "Cancelar", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("BuscarCliente", "Aceptar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BuscarCliente: public Ui_BuscarCliente {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUSCARCLIENTE_H
