/********************************************************************************
** Form generated from reading UI file 'buscarproducto.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUSCARPRODUCTO_H
#define UI_BUSCARPRODUCTO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
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

class Ui_BuscarProducto
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_top;
    QLineEdit *lineEdit_buscar;
    QCheckBox *checkBox_conStock;
    QHBoxLayout *horizontalLayout_content;
    QTableView *tableView;
    QFrame *frame_detalles;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_foto;
    QLabel *label_info;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonCancelar;
    QPushButton *pushButtonAceptar;

    void setupUi(QDialog *BuscarProducto)
    {
        if (BuscarProducto->objectName().isEmpty())
            BuscarProducto->setObjectName("BuscarProducto");
        BuscarProducto->resize(1024, 600);
        verticalLayout = new QVBoxLayout(BuscarProducto);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_top = new QHBoxLayout();
        horizontalLayout_top->setObjectName("horizontalLayout_top");
        lineEdit_buscar = new QLineEdit(BuscarProducto);
        lineEdit_buscar->setObjectName("lineEdit_buscar");

        horizontalLayout_top->addWidget(lineEdit_buscar);

        checkBox_conStock = new QCheckBox(BuscarProducto);
        checkBox_conStock->setObjectName("checkBox_conStock");

        horizontalLayout_top->addWidget(checkBox_conStock);


        verticalLayout->addLayout(horizontalLayout_top);

        horizontalLayout_content = new QHBoxLayout();
        horizontalLayout_content->setObjectName("horizontalLayout_content");
        tableView = new QTableView(BuscarProducto);
        tableView->setObjectName("tableView");
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        horizontalLayout_content->addWidget(tableView);

        frame_detalles = new QFrame(BuscarProducto);
        frame_detalles->setObjectName("frame_detalles");
        frame_detalles->setMinimumSize(QSize(250, 0));
        frame_detalles->setMaximumSize(QSize(300, 16777215));
        frame_detalles->setFrameShape(QFrame::Shape::StyledPanel);
        frame_detalles->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_detalles);
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_foto = new QLabel(frame_detalles);
        label_foto->setObjectName("label_foto");
        label_foto->setMinimumSize(QSize(200, 200));
        label_foto->setMaximumSize(QSize(300, 300));
        label_foto->setStyleSheet(QString::fromUtf8("border: 1px solid #ccc; background-color: #f9f9f9;"));
        label_foto->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(label_foto);

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

        pushButtonCancelar = new QPushButton(BuscarProducto);
        pushButtonCancelar->setObjectName("pushButtonCancelar");

        horizontalLayout->addWidget(pushButtonCancelar);

        pushButtonAceptar = new QPushButton(BuscarProducto);
        pushButtonAceptar->setObjectName("pushButtonAceptar");

        horizontalLayout->addWidget(pushButtonAceptar);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(BuscarProducto);

        QMetaObject::connectSlotsByName(BuscarProducto);
    } // setupUi

    void retranslateUi(QDialog *BuscarProducto)
    {
        BuscarProducto->setWindowTitle(QCoreApplication::translate("BuscarProducto", "Buscar productos", nullptr));
        lineEdit_buscar->setPlaceholderText(QCoreApplication::translate("BuscarProducto", "Buscar producto...", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_conStock->setToolTip(QCoreApplication::translate("BuscarProducto", "crtl+S", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_conStock->setText(QCoreApplication::translate("BuscarProducto", "Con stock", nullptr));
#if QT_CONFIG(shortcut)
        checkBox_conStock->setShortcut(QCoreApplication::translate("BuscarProducto", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        label_foto->setText(QCoreApplication::translate("BuscarProducto", "Sin foto", nullptr));
        label_info->setText(QCoreApplication::translate("BuscarProducto", "Seleccione un producto", nullptr));
        pushButtonCancelar->setText(QCoreApplication::translate("BuscarProducto", "Cancelar", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("BuscarProducto", "Aceptar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BuscarProducto: public Ui_BuscarProducto {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUSCARPRODUCTO_H
