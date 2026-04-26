/********************************************************************************
** Form generated from reading UI file 'prestamos.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRESTAMOS_H
#define UI_PRESTAMOS_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_Prestamos
{
public:
    QGridLayout *gridLayout_2;
    QTableView *tableView;
    QPushButton *pushButtonCerrar;
    QGridLayout *gridLayout;
    QPushButton *pushButtonProcesar;
    QComboBox *comboBox;
    QLineEdit *lineEditEan;
    QLabel *label_4;
    QLabel *label_7;
    QLineEdit *lineEditDescripcion;
    QLabel *label_5;
    QLabel *label_3;
    QDateEdit *dateEdit;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEditNotas;
    QSpinBox *spinBox;
    QLineEdit *lineEditCod;
    QLabel *label_8;
    QLineEdit *lineEdit;
    QLabel *label_9;

    void setupUi(QDialog *Prestamos)
    {
        if (Prestamos->objectName().isEmpty())
            Prestamos->setObjectName("Prestamos");
        Prestamos->resize(1044, 606);
        gridLayout_2 = new QGridLayout(Prestamos);
        gridLayout_2->setObjectName("gridLayout_2");
        tableView = new QTableView(Prestamos);
        tableView->setObjectName("tableView");

        gridLayout_2->addWidget(tableView, 0, 0, 1, 5);

        pushButtonCerrar = new QPushButton(Prestamos);
        pushButtonCerrar->setObjectName("pushButtonCerrar");
        pushButtonCerrar->setAutoDefault(false);

        gridLayout_2->addWidget(pushButtonCerrar, 4, 4, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        pushButtonProcesar = new QPushButton(Prestamos);
        pushButtonProcesar->setObjectName("pushButtonProcesar");
        pushButtonProcesar->setAutoDefault(false);

        gridLayout->addWidget(pushButtonProcesar, 1, 5, 1, 1);

        comboBox = new QComboBox(Prestamos);
        comboBox->setObjectName("comboBox");

        gridLayout->addWidget(comboBox, 1, 4, 1, 1);

        lineEditEan = new QLineEdit(Prestamos);
        lineEditEan->setObjectName("lineEditEan");
        lineEditEan->setMaximumSize(QSize(120, 16777215));
        lineEditEan->setMaxLength(13);

        gridLayout->addWidget(lineEditEan, 1, 0, 1, 1);

        label_4 = new QLabel(Prestamos);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 0, 3, 1, 1);

        label_7 = new QLabel(Prestamos);
        label_7->setObjectName("label_7");

        gridLayout->addWidget(label_7, 2, 0, 1, 1);

        lineEditDescripcion = new QLineEdit(Prestamos);
        lineEditDescripcion->setObjectName("lineEditDescripcion");

        gridLayout->addWidget(lineEditDescripcion, 1, 1, 1, 1);

        label_5 = new QLabel(Prestamos);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 0, 4, 1, 1);

        label_3 = new QLabel(Prestamos);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        dateEdit = new QDateEdit(Prestamos);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setCalendarPopup(true);
        dateEdit->setDate(QDate(2017, 1, 1));

        gridLayout->addWidget(dateEdit, 1, 3, 1, 1);

        label = new QLabel(Prestamos);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(Prestamos);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        lineEditNotas = new QLineEdit(Prestamos);
        lineEditNotas->setObjectName("lineEditNotas");

        gridLayout->addWidget(lineEditNotas, 2, 1, 1, 5);

        spinBox = new QSpinBox(Prestamos);
        spinBox->setObjectName("spinBox");
        spinBox->setMinimum(-999);
        spinBox->setMaximum(999);

        gridLayout->addWidget(spinBox, 1, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 3, 0, 1, 5);

        lineEditCod = new QLineEdit(Prestamos);
        lineEditCod->setObjectName("lineEditCod");
        lineEditCod->setMaximumSize(QSize(240, 16777215));

        gridLayout_2->addWidget(lineEditCod, 2, 0, 1, 1);

        label_8 = new QLabel(Prestamos);
        label_8->setObjectName("label_8");

        gridLayout_2->addWidget(label_8, 1, 0, 1, 1);

        lineEdit = new QLineEdit(Prestamos);
        lineEdit->setObjectName("lineEdit");

        gridLayout_2->addWidget(lineEdit, 2, 1, 1, 4);

        label_9 = new QLabel(Prestamos);
        label_9->setObjectName("label_9");

        gridLayout_2->addWidget(label_9, 1, 1, 1, 1);

#if QT_CONFIG(shortcut)
        label_4->setBuddy(dateEdit);
        label->setBuddy(lineEditEan);
        label_2->setBuddy(lineEditDescripcion);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(lineEditEan, lineEditDescripcion);
        QWidget::setTabOrder(lineEditDescripcion, spinBox);
        QWidget::setTabOrder(spinBox, dateEdit);
        QWidget::setTabOrder(dateEdit, comboBox);
        QWidget::setTabOrder(comboBox, pushButtonProcesar);
        QWidget::setTabOrder(pushButtonProcesar, pushButtonCerrar);
        QWidget::setTabOrder(pushButtonCerrar, lineEditCod);
        QWidget::setTabOrder(lineEditCod, tableView);
        QWidget::setTabOrder(tableView, lineEdit);
        QWidget::setTabOrder(lineEdit, lineEditNotas);

        retranslateUi(Prestamos);
        QObject::connect(pushButtonCerrar, &QPushButton::clicked, Prestamos, qOverload<>(&QDialog::close));

        QMetaObject::connectSlotsByName(Prestamos);
    } // setupUi

    void retranslateUi(QDialog *Prestamos)
    {
        Prestamos->setWindowTitle(QCoreApplication::translate("Prestamos", "Pr\303\251stamos", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("Prestamos", "Cerrar", nullptr));
        pushButtonProcesar->setText(QCoreApplication::translate("Prestamos", "Procesar", nullptr));
        label_4->setText(QCoreApplication::translate("Prestamos", "Fecha caducidad", nullptr));
        label_7->setText(QCoreApplication::translate("Prestamos", "Notas", nullptr));
        label_5->setText(QCoreApplication::translate("Prestamos", "Procedencia-destino", nullptr));
        label_3->setText(QCoreApplication::translate("Prestamos", "Cantidad", nullptr));
        dateEdit->setDisplayFormat(QCoreApplication::translate("Prestamos", "yyyy/MM/dd", nullptr));
        label->setText(QCoreApplication::translate("Prestamos", "C&\303\263digo", nullptr));
        label_2->setText(QCoreApplication::translate("Prestamos", "Descripci\303\263n", nullptr));
        label_8->setText(QCoreApplication::translate("Prestamos", "C\303\263digo", nullptr));
        label_9->setText(QCoreApplication::translate("Prestamos", "Producto", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Prestamos: public Ui_Prestamos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRESTAMOS_H
