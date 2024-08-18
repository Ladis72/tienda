/********************************************************************************
** Form generated from reading UI file 'copiaseguridad.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COPIASEGURIDAD_H
#define UI_COPIASEGURIDAD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>

QT_BEGIN_NAMESPACE

class Ui_CopiaSeguridad
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CopiaSeguridad)
    {
        if (CopiaSeguridad->objectName().isEmpty())
            CopiaSeguridad->setObjectName(QString::fromUtf8("CopiaSeguridad"));
        CopiaSeguridad->resize(400, 300);
        buttonBox = new QDialogButtonBox(CopiaSeguridad);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        retranslateUi(CopiaSeguridad);
        QObject::connect(buttonBox, SIGNAL(accepted()), CopiaSeguridad, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CopiaSeguridad, SLOT(reject()));

        QMetaObject::connectSlotsByName(CopiaSeguridad);
    } // setupUi

    void retranslateUi(QDialog *CopiaSeguridad)
    {
        CopiaSeguridad->setWindowTitle(QCoreApplication::translate("CopiaSeguridad", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CopiaSeguridad: public Ui_CopiaSeguridad {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COPIASEGURIDAD_H
