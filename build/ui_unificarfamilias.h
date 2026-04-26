/********************************************************************************
** Form generated from reading UI file 'unificarfamilias.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNIFICARFAMILIAS_H
#define UI_UNIFICARFAMILIAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UnificarFamilias
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *labelTitle;
    QLabel *labelInfo;
    QTableView *tableView;
    QHBoxLayout *buttonLayout;
    QPushButton *pushButtonFusionar;
    QPushButton *pushButtonRenombrar;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *UnificarFamilias)
    {
        if (UnificarFamilias->objectName().isEmpty())
            UnificarFamilias->setObjectName("UnificarFamilias");
        UnificarFamilias->resize(700, 550);
        verticalLayout = new QVBoxLayout(UnificarFamilias);
        verticalLayout->setSpacing(15);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(20, 20, 20, 20);
        labelTitle = new QLabel(UnificarFamilias);
        labelTitle->setObjectName("labelTitle");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        labelTitle->setFont(font);

        verticalLayout->addWidget(labelTitle);

        labelInfo = new QLabel(UnificarFamilias);
        labelInfo->setObjectName("labelInfo");
        labelInfo->setWordWrap(true);

        verticalLayout->addWidget(labelInfo);

        tableView = new QTableView(UnificarFamilias);
        tableView->setObjectName("tableView");
        tableView->setSelectionMode(QAbstractItemView::MultiSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setSortingEnabled(true);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(tableView);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(10);
        buttonLayout->setObjectName("buttonLayout");
        pushButtonFusionar = new QPushButton(UnificarFamilias);
        pushButtonFusionar->setObjectName("pushButtonFusionar");
        pushButtonFusionar->setMinimumSize(QSize(140, 35));

        buttonLayout->addWidget(pushButtonFusionar);

        pushButtonRenombrar = new QPushButton(UnificarFamilias);
        pushButtonRenombrar->setObjectName("pushButtonRenombrar");
        pushButtonRenombrar->setMinimumSize(QSize(120, 35));

        buttonLayout->addWidget(pushButtonRenombrar);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        pushButtonCerrar = new QPushButton(UnificarFamilias);
        pushButtonCerrar->setObjectName("pushButtonCerrar");
        pushButtonCerrar->setMinimumSize(QSize(100, 35));

        buttonLayout->addWidget(pushButtonCerrar);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(UnificarFamilias);

        QMetaObject::connectSlotsByName(UnificarFamilias);
    } // setupUi

    void retranslateUi(QDialog *UnificarFamilias)
    {
        UnificarFamilias->setWindowTitle(QCoreApplication::translate("UnificarFamilias", "Herramienta de Unificaci\303\263n de Familias", nullptr));
        labelTitle->setText(QCoreApplication::translate("UnificarFamilias", "Unificaci\303\263n de Familias", nullptr));
        labelInfo->setText(QCoreApplication::translate("UnificarFamilias", "Seleccione las familias que desea fusionar. La fusi\303\263n mover\303\241 todos los art\303\255culos a la familia elegida como principal y eliminar\303\241 las duplicadas.", nullptr));
        pushButtonFusionar->setText(QCoreApplication::translate("UnificarFamilias", "Fusionar Selecci\303\263n", nullptr));
        pushButtonRenombrar->setText(QCoreApplication::translate("UnificarFamilias", "Renombrar", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("UnificarFamilias", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UnificarFamilias: public Ui_UnificarFamilias {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNIFICARFAMILIAS_H
