/********************************************************************************
** Form generated from reading UI file 'dialogcambiocodigo.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCAMBIOCODIGO_H
#define UI_DIALOGCAMBIOCODIGO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogCambioCodigo
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *labelInfo;
    QLabel *labelArticulo;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLineEdit *lineEditNuevoCodigo;
    QComboBox *comboBoxFuente;
    QPushButton *pushButtonBuscarNube;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogCambioCodigo)
    {
        if (DialogCambioCodigo->objectName().isEmpty())
            DialogCambioCodigo->setObjectName("DialogCambioCodigo");
        DialogCambioCodigo->resize(500, 200);
        verticalLayout = new QVBoxLayout(DialogCambioCodigo);
        verticalLayout->setObjectName("verticalLayout");
        labelInfo = new QLabel(DialogCambioCodigo);
        labelInfo->setObjectName("labelInfo");

        verticalLayout->addWidget(labelInfo);

        labelArticulo = new QLabel(DialogCambioCodigo);
        labelArticulo->setObjectName("labelArticulo");
        QFont font;
        font.setBold(true);
        labelArticulo->setFont(font);
        labelArticulo->setWordWrap(true);

        verticalLayout->addWidget(labelArticulo);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_3 = new QLabel(DialogCambioCodigo);
        label_3->setObjectName("label_3");

        horizontalLayout->addWidget(label_3);

        lineEditNuevoCodigo = new QLineEdit(DialogCambioCodigo);
        lineEditNuevoCodigo->setObjectName("lineEditNuevoCodigo");

        horizontalLayout->addWidget(lineEditNuevoCodigo);

        comboBoxFuente = new QComboBox(DialogCambioCodigo);
        comboBoxFuente->setObjectName("comboBoxFuente");
        comboBoxFuente->setMinimumSize(QSize(120, 0));

        horizontalLayout->addWidget(comboBoxFuente);

        pushButtonBuscarNube = new QPushButton(DialogCambioCodigo);
        pushButtonBuscarNube->setObjectName("pushButtonBuscarNube");

        horizontalLayout->addWidget(pushButtonBuscarNube);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(DialogCambioCodigo);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DialogCambioCodigo);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, DialogCambioCodigo, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, DialogCambioCodigo, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(DialogCambioCodigo);
    } // setupUi

    void retranslateUi(QDialog *DialogCambioCodigo)
    {
        DialogCambioCodigo->setWindowTitle(QCoreApplication::translate("DialogCambioCodigo", "Cambiar C\303\263digo de Art\303\255culo", nullptr));
        labelInfo->setText(QCoreApplication::translate("DialogCambioCodigo", "C\303\263digo actual:", nullptr));
        labelArticulo->setText(QCoreApplication::translate("DialogCambioCodigo", "Producto...", nullptr));
        label_3->setText(QCoreApplication::translate("DialogCambioCodigo", "Nuevo C\303\263digo:", nullptr));
        pushButtonBuscarNube->setText(QCoreApplication::translate("DialogCambioCodigo", "Buscar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogCambioCodigo: public Ui_DialogCambioCodigo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCAMBIOCODIGO_H
