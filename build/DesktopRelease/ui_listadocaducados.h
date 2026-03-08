/********************************************************************************
** Form generated from reading UI file 'listadocaducados.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTADOCADUCADOS_H
#define UI_LISTADOCADUCADOS_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_ListadoCaducados
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDateEdit *dateEditDesde;
    QLabel *label_2;
    QDateEdit *dateEditHasta;
    QSpacerItem *horizontalSpacer;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButtonImprimir;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *ListadoCaducados)
    {
        if (ListadoCaducados->objectName().isEmpty())
            ListadoCaducados->setObjectName("ListadoCaducados");
        ListadoCaducados->resize(978, 562);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imagenes/Emeicjac logo.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        ListadoCaducados->setWindowIcon(icon);
        gridLayout = new QGridLayout(ListadoCaducados);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(ListadoCaducados);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        dateEditDesde = new QDateEdit(ListadoCaducados);
        dateEditDesde->setObjectName("dateEditDesde");
        dateEditDesde->setCalendarPopup(true);
        dateEditDesde->setDate(QDate(2025, 1, 1));

        horizontalLayout->addWidget(dateEditDesde);

        label_2 = new QLabel(ListadoCaducados);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        dateEditHasta = new QDateEdit(ListadoCaducados);
        dateEditHasta->setObjectName("dateEditHasta");
        dateEditHasta->setCalendarPopup(true);
        dateEditHasta->setDate(QDate(2025, 1, 1));

        horizontalLayout->addWidget(dateEditHasta);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        tableView = new QTableView(ListadoCaducados);
        tableView->setObjectName("tableView");
        tableView->setAlternatingRowColors(true);

        gridLayout->addWidget(tableView, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButtonImprimir = new QPushButton(ListadoCaducados);
        pushButtonImprimir->setObjectName("pushButtonImprimir");
        pushButtonImprimir->setAutoDefault(false);

        horizontalLayout_2->addWidget(pushButtonImprimir);

        pushButtonCerrar = new QPushButton(ListadoCaducados);
        pushButtonCerrar->setObjectName("pushButtonCerrar");
        pushButtonCerrar->setAutoDefault(false);

        horizontalLayout_2->addWidget(pushButtonCerrar);


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 1);

#if QT_CONFIG(shortcut)
        label->setBuddy(dateEditDesde);
        label_2->setBuddy(dateEditHasta);
#endif // QT_CONFIG(shortcut)

        retranslateUi(ListadoCaducados);
        QObject::connect(pushButtonCerrar, &QPushButton::clicked, ListadoCaducados, qOverload<>(&QDialog::close));

        QMetaObject::connectSlotsByName(ListadoCaducados);
    } // setupUi

    void retranslateUi(QDialog *ListadoCaducados)
    {
        ListadoCaducados->setWindowTitle(QCoreApplication::translate("ListadoCaducados", "Listado productos caducados", nullptr));
        label->setText(QCoreApplication::translate("ListadoCaducados", "Desde", nullptr));
        dateEditDesde->setDisplayFormat(QCoreApplication::translate("ListadoCaducados", "yyyy-MM-dd", nullptr));
        label_2->setText(QCoreApplication::translate("ListadoCaducados", "Hasta", nullptr));
        dateEditHasta->setDisplayFormat(QCoreApplication::translate("ListadoCaducados", "yyyy-MM-dd", nullptr));
        pushButtonImprimir->setText(QCoreApplication::translate("ListadoCaducados", "Imprimir", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("ListadoCaducados", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ListadoCaducados: public Ui_ListadoCaducados {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTADOCADUCADOS_H
