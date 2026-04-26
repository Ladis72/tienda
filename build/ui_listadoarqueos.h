/********************************************************************************
** Form generated from reading UI file 'listadoarqueos.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTADOARQUEOS_H
#define UI_LISTADOARQUEOS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_ListadoArqueos
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QDateEdit *dateEditDesde;
    QLabel *label_2;
    QDateEdit *dateEditHasta;
    QPushButton *pushButtonImprimir;
    QPushButton *pushButtonCerrar;
    QTableView *tableView;
    QPushButton *pushButtonConsultar;

    void setupUi(QDialog *ListadoArqueos)
    {
        if (ListadoArqueos->objectName().isEmpty())
            ListadoArqueos->setObjectName("ListadoArqueos");
        ListadoArqueos->resize(897, 550);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imagenes/Emeicjac logo.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        ListadoArqueos->setWindowIcon(icon);
        gridLayout = new QGridLayout(ListadoArqueos);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(ListadoArqueos);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        dateEditDesde = new QDateEdit(ListadoArqueos);
        dateEditDesde->setObjectName("dateEditDesde");
        dateEditDesde->setCalendarPopup(true);

        gridLayout->addWidget(dateEditDesde, 0, 1, 1, 1);

        label_2 = new QLabel(ListadoArqueos);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        dateEditHasta = new QDateEdit(ListadoArqueos);
        dateEditHasta->setObjectName("dateEditHasta");
        dateEditHasta->setCalendarPopup(true);

        gridLayout->addWidget(dateEditHasta, 0, 3, 1, 1);

        pushButtonImprimir = new QPushButton(ListadoArqueos);
        pushButtonImprimir->setObjectName("pushButtonImprimir");

        gridLayout->addWidget(pushButtonImprimir, 2, 4, 1, 1);

        pushButtonCerrar = new QPushButton(ListadoArqueos);
        pushButtonCerrar->setObjectName("pushButtonCerrar");

        gridLayout->addWidget(pushButtonCerrar, 2, 5, 1, 1);

        tableView = new QTableView(ListadoArqueos);
        tableView->setObjectName("tableView");

        gridLayout->addWidget(tableView, 1, 0, 1, 6);

        pushButtonConsultar = new QPushButton(ListadoArqueos);
        pushButtonConsultar->setObjectName("pushButtonConsultar");

        gridLayout->addWidget(pushButtonConsultar, 0, 4, 1, 1);

#if QT_CONFIG(shortcut)
        label->setBuddy(dateEditDesde);
        label_2->setBuddy(dateEditHasta);
#endif // QT_CONFIG(shortcut)

        retranslateUi(ListadoArqueos);
        QObject::connect(pushButtonCerrar, &QPushButton::clicked, ListadoArqueos, qOverload<>(&QDialog::close));

        QMetaObject::connectSlotsByName(ListadoArqueos);
    } // setupUi

    void retranslateUi(QDialog *ListadoArqueos)
    {
        ListadoArqueos->setWindowTitle(QCoreApplication::translate("ListadoArqueos", "Listado arqueos", nullptr));
        label->setText(QCoreApplication::translate("ListadoArqueos", "Desde", nullptr));
        dateEditDesde->setDisplayFormat(QCoreApplication::translate("ListadoArqueos", "yyyy-MM-dd", nullptr));
        label_2->setText(QCoreApplication::translate("ListadoArqueos", "Hasta", nullptr));
        dateEditHasta->setDisplayFormat(QCoreApplication::translate("ListadoArqueos", "yyyy-MM-dd", nullptr));
        pushButtonImprimir->setText(QCoreApplication::translate("ListadoArqueos", "Imprimir", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("ListadoArqueos", "Cerrar", nullptr));
        pushButtonConsultar->setText(QCoreApplication::translate("ListadoArqueos", "Consultar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ListadoArqueos: public Ui_ListadoArqueos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTADOARQUEOS_H
