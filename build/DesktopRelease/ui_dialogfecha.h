/********************************************************************************
** Form generated from reading UI file 'dialogfecha.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGFECHA_H
#define UI_DIALOGFECHA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_DialogFecha
{
public:
    QGridLayout *gridLayout;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLabel *label_3;
    QDateEdit *dateEdit;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *DialogFecha)
    {
        if (DialogFecha->objectName().isEmpty())
            DialogFecha->setObjectName("DialogFecha");
        DialogFecha->resize(347, 144);
        gridLayout = new QGridLayout(DialogFecha);
        gridLayout->setObjectName("gridLayout");
        lineEdit = new QLineEdit(DialogFecha);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setEnabled(false);

        gridLayout->addWidget(lineEdit, 2, 0, 1, 1);

        label_2 = new QLabel(DialogFecha);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(DialogFecha);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 1, 1, 1, 1);

        dateEdit = new QDateEdit(DialogFecha);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setCalendarPopup(true);

        gridLayout->addWidget(dateEdit, 2, 1, 1, 1);

        label = new QLabel(DialogFecha);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(12);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 2, 1, 1);

        pushButton = new QPushButton(DialogFecha);
        pushButton->setObjectName("pushButton");

        gridLayout->addWidget(pushButton, 3, 0, 1, 3);


        retranslateUi(DialogFecha);

        QMetaObject::connectSlotsByName(DialogFecha);
    } // setupUi

    void retranslateUi(QDialog *DialogFecha)
    {
        DialogFecha->setWindowTitle(QCoreApplication::translate("DialogFecha", "Devoluci\303\263n de art\303\255culo", nullptr));
        label_2->setText(QCoreApplication::translate("DialogFecha", "Lote", nullptr));
        label_3->setText(QCoreApplication::translate("DialogFecha", "Fecha", nullptr));
        dateEdit->setDisplayFormat(QCoreApplication::translate("DialogFecha", "yyyy-MM-dd", nullptr));
        label->setText(QCoreApplication::translate("DialogFecha", "TextLabel", nullptr));
        pushButton->setText(QCoreApplication::translate("DialogFecha", "Devolver", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogFecha: public Ui_DialogFecha {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGFECHA_H
