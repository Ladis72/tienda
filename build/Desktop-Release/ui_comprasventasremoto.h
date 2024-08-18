/********************************************************************************
** Form generated from reading UI file 'comprasventasremoto.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPRASVENTASREMOTO_H
#define UI_COMPRASVENTASREMOTO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_comprasVentasRemoto
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tabCompras;
    QGridLayout *gridLayout_3;
    QTableView *tableViewCompras;
    QGroupBox *groupBoxCompras;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButtonComprasDia;
    QRadioButton *radioButtonComprasMes;
    QRadioButton *radioButtonComprasano;
    QWidget *tab_2;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBoxVentas;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioButtonVentasDia;
    QRadioButton *radioButtonVentaMes;
    QRadioButton *radioButtonVentasAno;
    QTableView *tableViewVentas;
    QPushButton *pushButton;

    void setupUi(QWidget *comprasVentasRemoto)
    {
        if (comprasVentasRemoto->objectName().isEmpty())
            comprasVentasRemoto->setObjectName(QString::fromUtf8("comprasVentasRemoto"));
        comprasVentasRemoto->resize(640, 557);
        gridLayout = new QGridLayout(comprasVentasRemoto);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(comprasVentasRemoto);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabCompras = new QWidget();
        tabCompras->setObjectName(QString::fromUtf8("tabCompras"));
        gridLayout_3 = new QGridLayout(tabCompras);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tableViewCompras = new QTableView(tabCompras);
        tableViewCompras->setObjectName(QString::fromUtf8("tableViewCompras"));

        gridLayout_3->addWidget(tableViewCompras, 1, 0, 1, 1);

        groupBoxCompras = new QGroupBox(tabCompras);
        groupBoxCompras->setObjectName(QString::fromUtf8("groupBoxCompras"));
        horizontalLayout_2 = new QHBoxLayout(groupBoxCompras);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        radioButtonComprasDia = new QRadioButton(groupBoxCompras);
        radioButtonComprasDia->setObjectName(QString::fromUtf8("radioButtonComprasDia"));

        horizontalLayout_2->addWidget(radioButtonComprasDia);

        radioButtonComprasMes = new QRadioButton(groupBoxCompras);
        radioButtonComprasMes->setObjectName(QString::fromUtf8("radioButtonComprasMes"));
        radioButtonComprasMes->setChecked(true);

        horizontalLayout_2->addWidget(radioButtonComprasMes);

        radioButtonComprasano = new QRadioButton(groupBoxCompras);
        radioButtonComprasano->setObjectName(QString::fromUtf8("radioButtonComprasano"));

        horizontalLayout_2->addWidget(radioButtonComprasano);


        gridLayout_3->addWidget(groupBoxCompras, 0, 0, 1, 1);

        tabWidget->addTab(tabCompras, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_2 = new QGridLayout(tab_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBoxVentas = new QGroupBox(tab_2);
        groupBoxVentas->setObjectName(QString::fromUtf8("groupBoxVentas"));
        horizontalLayout = new QHBoxLayout(groupBoxVentas);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        radioButtonVentasDia = new QRadioButton(groupBoxVentas);
        radioButtonVentasDia->setObjectName(QString::fromUtf8("radioButtonVentasDia"));

        horizontalLayout->addWidget(radioButtonVentasDia);

        radioButtonVentaMes = new QRadioButton(groupBoxVentas);
        radioButtonVentaMes->setObjectName(QString::fromUtf8("radioButtonVentaMes"));
        radioButtonVentaMes->setChecked(true);

        horizontalLayout->addWidget(radioButtonVentaMes);

        radioButtonVentasAno = new QRadioButton(groupBoxVentas);
        radioButtonVentasAno->setObjectName(QString::fromUtf8("radioButtonVentasAno"));

        horizontalLayout->addWidget(radioButtonVentasAno);


        gridLayout_2->addWidget(groupBoxVentas, 0, 0, 1, 1);

        tableViewVentas = new QTableView(tab_2);
        tableViewVentas->setObjectName(QString::fromUtf8("tableViewVentas"));

        gridLayout_2->addWidget(tableViewVentas, 1, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        pushButton = new QPushButton(comprasVentasRemoto);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 1, 0, 1, 1);


        retranslateUi(comprasVentasRemoto);
        QObject::connect(pushButton, SIGNAL(clicked()), comprasVentasRemoto, SLOT(close()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(comprasVentasRemoto);
    } // setupUi

    void retranslateUi(QWidget *comprasVentasRemoto)
    {
        comprasVentasRemoto->setWindowTitle(QCoreApplication::translate("comprasVentasRemoto", "Form", nullptr));
        groupBoxCompras->setTitle(QCoreApplication::translate("comprasVentasRemoto", "Fechas", nullptr));
        radioButtonComprasDia->setText(QCoreApplication::translate("comprasVentasRemoto", "D\303\255a", nullptr));
        radioButtonComprasMes->setText(QCoreApplication::translate("comprasVentasRemoto", "Mes", nullptr));
        radioButtonComprasano->setText(QCoreApplication::translate("comprasVentasRemoto", "A\303\261o", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabCompras), QCoreApplication::translate("comprasVentasRemoto", "Compras", nullptr));
        groupBoxVentas->setTitle(QCoreApplication::translate("comprasVentasRemoto", "Fechas", nullptr));
        radioButtonVentasDia->setText(QCoreApplication::translate("comprasVentasRemoto", "D\303\255a", nullptr));
        radioButtonVentaMes->setText(QCoreApplication::translate("comprasVentasRemoto", "Mes", nullptr));
        radioButtonVentasAno->setText(QCoreApplication::translate("comprasVentasRemoto", "A\303\261o", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("comprasVentasRemoto", "Ventas", nullptr));
        pushButton->setText(QCoreApplication::translate("comprasVentasRemoto", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class comprasVentasRemoto: public Ui_comprasVentasRemoto {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPRASVENTASREMOTO_H
