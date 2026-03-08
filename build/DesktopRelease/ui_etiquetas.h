/********************************************************************************
** Form generated from reading UI file 'etiquetas.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ETIQUETAS_H
#define UI_ETIQUETAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_Etiquetas
{
public:
    QGridLayout *gridLayout;
    QTableView *tableView;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEditCod;
    QLineEdit *lineEditDesc;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButtonImprimir;
    QPushButton *pushButton;

    void setupUi(QDialog *Etiquetas)
    {
        if (Etiquetas->objectName().isEmpty())
            Etiquetas->setObjectName("Etiquetas");
        Etiquetas->resize(762, 496);
        gridLayout = new QGridLayout(Etiquetas);
        gridLayout->setObjectName("gridLayout");
        tableView = new QTableView(Etiquetas);
        tableView->setObjectName("tableView");

        gridLayout->addWidget(tableView, 0, 0, 1, 3);

        label = new QLabel(Etiquetas);
        label->setObjectName("label");

        gridLayout->addWidget(label, 1, 0, 1, 1);

        label_2 = new QLabel(Etiquetas);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 2, 1, 1);

        lineEditCod = new QLineEdit(Etiquetas);
        lineEditCod->setObjectName("lineEditCod");

        gridLayout->addWidget(lineEditCod, 2, 0, 1, 2);

        lineEditDesc = new QLineEdit(Etiquetas);
        lineEditDesc->setObjectName("lineEditDesc");

        gridLayout->addWidget(lineEditDesc, 2, 2, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pushButton_2 = new QPushButton(Etiquetas);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setAutoDefault(false);

        horizontalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(Etiquetas);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setAutoDefault(false);

        horizontalLayout->addWidget(pushButton_3);

        pushButtonImprimir = new QPushButton(Etiquetas);
        pushButtonImprimir->setObjectName("pushButtonImprimir");
        pushButtonImprimir->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonImprimir);

        pushButton = new QPushButton(Etiquetas);
        pushButton->setObjectName("pushButton");
        pushButton->setAutoDefault(false);

        horizontalLayout->addWidget(pushButton);


        gridLayout->addLayout(horizontalLayout, 3, 0, 1, 3);

#if QT_CONFIG(shortcut)
        label->setBuddy(lineEditCod);
        label_2->setBuddy(lineEditDesc);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(lineEditCod, lineEditDesc);
        QWidget::setTabOrder(lineEditDesc, pushButton_2);
        QWidget::setTabOrder(pushButton_2, pushButton_3);
        QWidget::setTabOrder(pushButton_3, pushButtonImprimir);
        QWidget::setTabOrder(pushButtonImprimir, pushButton);
        QWidget::setTabOrder(pushButton, tableView);

        retranslateUi(Etiquetas);
        QObject::connect(pushButton, &QPushButton::clicked, Etiquetas, qOverload<>(&QDialog::close));
        QObject::connect(pushButton, &QPushButton::clicked, Etiquetas, qOverload<>(&QDialog::close));

        QMetaObject::connectSlotsByName(Etiquetas);
    } // setupUi

    void retranslateUi(QDialog *Etiquetas)
    {
        Etiquetas->setWindowTitle(QCoreApplication::translate("Etiquetas", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("Etiquetas", "C&\303\263digo", nullptr));
        label_2->setText(QCoreApplication::translate("Etiquetas", "Buscar producto", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Etiquetas", "Borrar linea", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Etiquetas", "Borrar todo", nullptr));
        pushButtonImprimir->setText(QCoreApplication::translate("Etiquetas", "Imprimir", nullptr));
        pushButton->setText(QCoreApplication::translate("Etiquetas", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Etiquetas: public Ui_Etiquetas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ETIQUETAS_H
