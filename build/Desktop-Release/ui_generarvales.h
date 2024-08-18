/********************************************************************************
** Form generated from reading UI file 'generarvales.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENERARVALES_H
#define UI_GENERARVALES_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_GenerarVales
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QPushButton *pushButtonCerrar;
    QProgressBar *progressBar;
    QLabel *label_2;
    QDateEdit *dateEdit;
    QPushButton *pushButtonGenerar;

    void setupUi(QDialog *GenerarVales)
    {
        if (GenerarVales->objectName().isEmpty())
            GenerarVales->setObjectName(QString::fromUtf8("GenerarVales"));
        GenerarVales->resize(299, 204);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imagenes/Emeicjac logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        GenerarVales->setWindowIcon(icon);
        gridLayout = new QGridLayout(GenerarVales);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(GenerarVales);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label, 3, 0, 1, 1);

        pushButtonCerrar = new QPushButton(GenerarVales);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));

        gridLayout->addWidget(pushButtonCerrar, 5, 0, 1, 1);

        progressBar = new QProgressBar(GenerarVales);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setBaseSize(QSize(0, 0));
        progressBar->setValue(0);

        gridLayout->addWidget(progressBar, 4, 0, 1, 1);

        label_2 = new QLabel(GenerarVales);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        dateEdit = new QDateEdit(GenerarVales);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));
        dateEdit->setWrapping(false);
        dateEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);
        dateEdit->setReadOnly(false);
        dateEdit->setButtonSymbols(QAbstractSpinBox::ButtonSymbols::PlusMinus);
        dateEdit->setProperty("showGroupSeparator", QVariant(false));
        dateEdit->setCalendarPopup(true);

        gridLayout->addWidget(dateEdit, 1, 0, 1, 1);

        pushButtonGenerar = new QPushButton(GenerarVales);
        pushButtonGenerar->setObjectName(QString::fromUtf8("pushButtonGenerar"));

        gridLayout->addWidget(pushButtonGenerar, 2, 0, 1, 1);


        retranslateUi(GenerarVales);
        QObject::connect(pushButtonCerrar, SIGNAL(clicked()), GenerarVales, SLOT(reject()));

        QMetaObject::connectSlotsByName(GenerarVales);
    } // setupUi

    void retranslateUi(QDialog *GenerarVales)
    {
        GenerarVales->setWindowTitle(QCoreApplication::translate("GenerarVales", "Generar vales", nullptr));
        label->setText(QCoreApplication::translate("GenerarVales", "TextLabel", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("GenerarVales", "Cerrar", nullptr));
        label_2->setText(QCoreApplication::translate("GenerarVales", "Fecha", nullptr));
        dateEdit->setDisplayFormat(QCoreApplication::translate("GenerarVales", "yyyy/MM", nullptr));
        pushButtonGenerar->setText(QCoreApplication::translate("GenerarVales", "Generar vales", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GenerarVales: public Ui_GenerarVales {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENERARVALES_H
