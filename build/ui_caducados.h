/********************************************************************************
** Form generated from reading UI file 'caducados.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CADUCADOS_H
#define UI_CADUCADOS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_Caducados
{
public:
    QGridLayout *gridLayout;
    QLineEdit *lineEditCodigo;
    QSpinBox *spinBox;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEditDescripcion;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QLabel *label_4;
    QComboBox *comboBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *Caducados)
    {
        if (Caducados->objectName().isEmpty())
            Caducados->setObjectName("Caducados");
        Caducados->resize(616, 160);
        gridLayout = new QGridLayout(Caducados);
        gridLayout->setObjectName("gridLayout");
        lineEditCodigo = new QLineEdit(Caducados);
        lineEditCodigo->setObjectName("lineEditCodigo");

        gridLayout->addWidget(lineEditCodigo, 0, 1, 1, 1);

        spinBox = new QSpinBox(Caducados);
        spinBox->setObjectName("spinBox");
        spinBox->setMinimum(1);

        gridLayout->addWidget(spinBox, 4, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 4, 4, 1, 1);

        lineEditDescripcion = new QLineEdit(Caducados);
        lineEditDescripcion->setObjectName("lineEditDescripcion");

        gridLayout->addWidget(lineEditDescripcion, 2, 1, 1, 4);

        label_3 = new QLabel(Caducados);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        label_2 = new QLabel(Caducados);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label = new QLabel(Caducados);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        label_4 = new QLabel(Caducados);
        label_4->setObjectName("label_4");
        label_4->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        gridLayout->addWidget(label_4, 4, 2, 1, 1);

        comboBox = new QComboBox(Caducados);
        comboBox->setObjectName("comboBox");

        gridLayout->addWidget(comboBox, 4, 1, 1, 1);

        pushButton = new QPushButton(Caducados);
        pushButton->setObjectName("pushButton");
        pushButton->setAutoDefault(false);

        gridLayout->addWidget(pushButton, 5, 1, 1, 1);

        pushButton_2 = new QPushButton(Caducados);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setAutoDefault(false);

        gridLayout->addWidget(pushButton_2, 5, 2, 1, 1);

#if QT_CONFIG(shortcut)
        label_2->setBuddy(lineEditDescripcion);
        label->setBuddy(lineEditCodigo);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(lineEditCodigo, spinBox);
        QWidget::setTabOrder(spinBox, pushButton);
        QWidget::setTabOrder(pushButton, pushButton_2);
        QWidget::setTabOrder(pushButton_2, comboBox);
        QWidget::setTabOrder(comboBox, lineEditDescripcion);

        retranslateUi(Caducados);
        QObject::connect(pushButton_2, &QPushButton::clicked, Caducados, qOverload<>(&QDialog::close));

        QMetaObject::connectSlotsByName(Caducados);
    } // setupUi

    void retranslateUi(QDialog *Caducados)
    {
        Caducados->setWindowTitle(QCoreApplication::translate("Caducados", "Dialog", nullptr));
        label_3->setText(QCoreApplication::translate("Caducados", "Fecha", nullptr));
        label_2->setText(QCoreApplication::translate("Caducados", "Descripci&\303\263n", nullptr));
        label->setText(QCoreApplication::translate("Caducados", "C&\303\263digo", nullptr));
        label_4->setText(QCoreApplication::translate("Caducados", "Cantidad", nullptr));
        pushButton->setText(QCoreApplication::translate("Caducados", "Procesar", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Caducados", "Salir", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Caducados: public Ui_Caducados {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CADUCADOS_H
