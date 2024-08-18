/********************************************************************************
** Form generated from reading UI file 'entradasalida.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENTRADASALIDA_H
#define UI_ENTRADASALIDA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_EntradaSalida
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QComboBox *comboBoxMotivo;
    QLineEdit *lineEditCantidad;
    QToolButton *toolButton;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonAceptar;
    QPushButton *pushButtonCAncelar;
    QLineEdit *lineEditDescripcion;

    void setupUi(QDialog *EntradaSalida)
    {
        if (EntradaSalida->objectName().isEmpty())
            EntradaSalida->setObjectName(QString::fromUtf8("EntradaSalida"));
        EntradaSalida->resize(440, 126);
        gridLayout = new QGridLayout(EntradaSalida);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(EntradaSalida);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(EntradaSalida);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        comboBoxMotivo = new QComboBox(EntradaSalida);
        comboBoxMotivo->setObjectName(QString::fromUtf8("comboBoxMotivo"));
        comboBoxMotivo->setMinimumSize(QSize(200, 0));

        gridLayout->addWidget(comboBoxMotivo, 0, 3, 1, 1);

        lineEditCantidad = new QLineEdit(EntradaSalida);
        lineEditCantidad->setObjectName(QString::fromUtf8("lineEditCantidad"));
        lineEditCantidad->setMaximumSize(QSize(100, 16777215));

        gridLayout->addWidget(lineEditCantidad, 0, 1, 1, 1);

        toolButton = new QToolButton(EntradaSalida);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));

        gridLayout->addWidget(toolButton, 0, 4, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonAceptar = new QPushButton(EntradaSalida);
        pushButtonAceptar->setObjectName(QString::fromUtf8("pushButtonAceptar"));

        horizontalLayout->addWidget(pushButtonAceptar);

        pushButtonCAncelar = new QPushButton(EntradaSalida);
        pushButtonCAncelar->setObjectName(QString::fromUtf8("pushButtonCAncelar"));

        horizontalLayout->addWidget(pushButtonCAncelar);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 5);

        lineEditDescripcion = new QLineEdit(EntradaSalida);
        lineEditDescripcion->setObjectName(QString::fromUtf8("lineEditDescripcion"));

        gridLayout->addWidget(lineEditDescripcion, 1, 0, 1, 5);

#if QT_CONFIG(shortcut)
        label->setBuddy(lineEditCantidad);
        label_2->setBuddy(comboBoxMotivo);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(lineEditCantidad, comboBoxMotivo);
        QWidget::setTabOrder(comboBoxMotivo, toolButton);
        QWidget::setTabOrder(toolButton, pushButtonAceptar);
        QWidget::setTabOrder(pushButtonAceptar, pushButtonCAncelar);

        retranslateUi(EntradaSalida);
        QObject::connect(pushButtonCAncelar, SIGNAL(clicked()), EntradaSalida, SLOT(close()));

        QMetaObject::connectSlotsByName(EntradaSalida);
    } // setupUi

    void retranslateUi(QDialog *EntradaSalida)
    {
        EntradaSalida->setWindowTitle(QCoreApplication::translate("EntradaSalida", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("EntradaSalida", "Cantidad", nullptr));
        label_2->setText(QCoreApplication::translate("EntradaSalida", "&Motivo", nullptr));
        toolButton->setText(QCoreApplication::translate("EntradaSalida", "+", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("EntradaSalida", "Aceptar", nullptr));
        pushButtonCAncelar->setText(QCoreApplication::translate("EntradaSalida", "Cancelar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EntradaSalida: public Ui_EntradaSalida {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENTRADASALIDA_H
