/********************************************************************************
** Form generated from reading UI file 'unificarmaestros.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNIFICARMAESTROS_H
#define UI_UNIFICARMAESTROS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UnificarMaestros
{
public:
    QVBoxLayout *mainVerticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *labelTitle;
    QSpacerItem *horizontalSpacerHeader;
    QLabel *labelInfo;
    QTableView *tableView;
    QHBoxLayout *horizontalLayoutButtons;
    QPushButton *pushButtonFusionar;
    QPushButton *pushButtonRenombrar;
    QSpacerItem *horizontalSpacerButtons;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *UnificarMaestros)
    {
        if (UnificarMaestros->objectName().isEmpty())
            UnificarMaestros->setObjectName("UnificarMaestros");
        UnificarMaestros->resize(700, 600);
        UnificarMaestros->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        mainVerticalLayout = new QVBoxLayout(UnificarMaestros);
        mainVerticalLayout->setSpacing(15);
        mainVerticalLayout->setObjectName("mainVerticalLayout");
        mainVerticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(UnificarMaestros);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame {\n"
"    background-color: #e8eaf6;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #c5cae9;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName("horizontalLayoutHeader");
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName("labelIcono");
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/unificar.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelTitle = new QLabel(headerFrame);
        labelTitle->setObjectName("labelTitle");
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelTitle->setFont(font);
        labelTitle->setStyleSheet(QString::fromUtf8("color: #1a237e;"));

        horizontalLayoutHeader->addWidget(labelTitle);

        horizontalSpacerHeader = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);


        mainVerticalLayout->addWidget(headerFrame);

        labelInfo = new QLabel(UnificarMaestros);
        labelInfo->setObjectName("labelInfo");
        labelInfo->setWordWrap(true);

        mainVerticalLayout->addWidget(labelInfo);

        tableView = new QTableView(UnificarMaestros);
        tableView->setObjectName("tableView");
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        mainVerticalLayout->addWidget(tableView);

        horizontalLayoutButtons = new QHBoxLayout();
        horizontalLayoutButtons->setObjectName("horizontalLayoutButtons");
        pushButtonFusionar = new QPushButton(UnificarMaestros);
        pushButtonFusionar->setObjectName("pushButtonFusionar");
        pushButtonFusionar->setMinimumSize(QSize(140, 40));

        horizontalLayoutButtons->addWidget(pushButtonFusionar);

        pushButtonRenombrar = new QPushButton(UnificarMaestros);
        pushButtonRenombrar->setObjectName("pushButtonRenombrar");
        pushButtonRenombrar->setMinimumSize(QSize(120, 40));

        horizontalLayoutButtons->addWidget(pushButtonRenombrar);

        horizontalSpacerButtons = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutButtons->addItem(horizontalSpacerButtons);

        pushButtonCerrar = new QPushButton(UnificarMaestros);
        pushButtonCerrar->setObjectName("pushButtonCerrar");
        pushButtonCerrar->setMinimumSize(QSize(100, 40));

        horizontalLayoutButtons->addWidget(pushButtonCerrar);


        mainVerticalLayout->addLayout(horizontalLayoutButtons);


        retranslateUi(UnificarMaestros);

        QMetaObject::connectSlotsByName(UnificarMaestros);
    } // setupUi

    void retranslateUi(QDialog *UnificarMaestros)
    {
        UnificarMaestros->setWindowTitle(QCoreApplication::translate("UnificarMaestros", "Unificar y Limpiar Duplicados", nullptr));
        labelTitle->setText(QCoreApplication::translate("UnificarMaestros", "Unificar Maestros", nullptr));
        labelInfo->setText(QCoreApplication::translate("UnificarMaestros", "Seleccione dos o m\303\241s registros para fusionarlos. El registro elegido como 'ganador' conservar\303\241 su ID y nombre, y todos los datos asociados (ventas, stock, etc.) de los otros registros se mover\303\241n al ganador antes de borrarlos.", nullptr));
        pushButtonFusionar->setText(QCoreApplication::translate("UnificarMaestros", "Fusionar Seleccionados", nullptr));
        pushButtonRenombrar->setText(QCoreApplication::translate("UnificarMaestros", "Renombrar", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("UnificarMaestros", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UnificarMaestros: public Ui_UnificarMaestros {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNIFICARMAESTROS_H
