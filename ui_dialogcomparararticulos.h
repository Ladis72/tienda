/********************************************************************************
** Form generated from reading UI file 'dialogcomparararticulos.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCOMPARARARTICULOS_H
#define UI_DIALOGCOMPARARARTICULOS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogCompararArticulos
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBoxLocal;
    QFormLayout *formLayoutLocal;
    QLabel *label_2;
    QLabel *lblLocalCod;
    QLabel *label_3;
    QLabel *lblLocalDesc;
    QLabel *label_4;
    QLabel *lblLocalPvp;
    QLabel *label_5;
    QLabel *lblLocalIva;
    QGroupBox *groupBoxNube;
    QFormLayout *formLayoutNube;
    QLabel *label_6;
    QLabel *lblNubeCod;
    QLabel *label_7;
    QLabel *lblNubeDesc;
    QLabel *label_8;
    QLabel *lblNubePvp;
    QLabel *label_9;
    QLabel *lblNubeIva;
    QLabel *label_10;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnSincronizarLocal;
    QPushButton *btnSobrescribirNube;
    QPushButton *btnCancelar;

    void setupUi(QDialog *DialogCompararArticulos)
    {
        if (DialogCompararArticulos->objectName().isEmpty())
            DialogCompararArticulos->setObjectName(QString::fromUtf8("DialogCompararArticulos"));
        DialogCompararArticulos->resize(650, 450);
        verticalLayout = new QVBoxLayout(DialogCompararArticulos);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(DialogCompararArticulos);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        label->setFont(font);

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBoxLocal = new QGroupBox(DialogCompararArticulos);
        groupBoxLocal->setObjectName(QString::fromUtf8("groupBoxLocal"));
        formLayoutLocal = new QFormLayout(groupBoxLocal);
        formLayoutLocal->setObjectName(QString::fromUtf8("formLayoutLocal"));
        label_2 = new QLabel(groupBoxLocal);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayoutLocal->setWidget(0, QFormLayout::LabelRole, label_2);

        lblLocalCod = new QLabel(groupBoxLocal);
        lblLocalCod->setObjectName(QString::fromUtf8("lblLocalCod"));

        formLayoutLocal->setWidget(0, QFormLayout::FieldRole, lblLocalCod);

        label_3 = new QLabel(groupBoxLocal);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayoutLocal->setWidget(1, QFormLayout::LabelRole, label_3);

        lblLocalDesc = new QLabel(groupBoxLocal);
        lblLocalDesc->setObjectName(QString::fromUtf8("lblLocalDesc"));
        lblLocalDesc->setWordWrap(true);

        formLayoutLocal->setWidget(1, QFormLayout::FieldRole, lblLocalDesc);

        label_4 = new QLabel(groupBoxLocal);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayoutLocal->setWidget(2, QFormLayout::LabelRole, label_4);

        lblLocalPvp = new QLabel(groupBoxLocal);
        lblLocalPvp->setObjectName(QString::fromUtf8("lblLocalPvp"));

        formLayoutLocal->setWidget(2, QFormLayout::FieldRole, lblLocalPvp);

        label_5 = new QLabel(groupBoxLocal);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayoutLocal->setWidget(3, QFormLayout::LabelRole, label_5);

        lblLocalIva = new QLabel(groupBoxLocal);
        lblLocalIva->setObjectName(QString::fromUtf8("lblLocalIva"));

        formLayoutLocal->setWidget(3, QFormLayout::FieldRole, lblLocalIva);


        horizontalLayout->addWidget(groupBoxLocal);

        groupBoxNube = new QGroupBox(DialogCompararArticulos);
        groupBoxNube->setObjectName(QString::fromUtf8("groupBoxNube"));
        groupBoxNube->setStyleSheet(QString::fromUtf8("QGroupBox { color: blue; }"));
        formLayoutNube = new QFormLayout(groupBoxNube);
        formLayoutNube->setObjectName(QString::fromUtf8("formLayoutNube"));
        label_6 = new QLabel(groupBoxNube);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayoutNube->setWidget(0, QFormLayout::LabelRole, label_6);

        lblNubeCod = new QLabel(groupBoxNube);
        lblNubeCod->setObjectName(QString::fromUtf8("lblNubeCod"));

        formLayoutNube->setWidget(0, QFormLayout::FieldRole, lblNubeCod);

        label_7 = new QLabel(groupBoxNube);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayoutNube->setWidget(1, QFormLayout::LabelRole, label_7);

        lblNubeDesc = new QLabel(groupBoxNube);
        lblNubeDesc->setObjectName(QString::fromUtf8("lblNubeDesc"));
        lblNubeDesc->setWordWrap(true);

        formLayoutNube->setWidget(1, QFormLayout::FieldRole, lblNubeDesc);

        label_8 = new QLabel(groupBoxNube);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayoutNube->setWidget(2, QFormLayout::LabelRole, label_8);

        lblNubePvp = new QLabel(groupBoxNube);
        lblNubePvp->setObjectName(QString::fromUtf8("lblNubePvp"));

        formLayoutNube->setWidget(2, QFormLayout::FieldRole, lblNubePvp);

        label_9 = new QLabel(groupBoxNube);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        formLayoutNube->setWidget(3, QFormLayout::LabelRole, label_9);

        lblNubeIva = new QLabel(groupBoxNube);
        lblNubeIva->setObjectName(QString::fromUtf8("lblNubeIva"));

        formLayoutNube->setWidget(3, QFormLayout::FieldRole, lblNubeIva);


        horizontalLayout->addWidget(groupBoxNube);


        verticalLayout->addLayout(horizontalLayout);

        label_10 = new QLabel(DialogCompararArticulos);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_10);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        btnSincronizarLocal = new QPushButton(DialogCompararArticulos);
        btnSincronizarLocal->setObjectName(QString::fromUtf8("btnSincronizarLocal"));

        horizontalLayout_2->addWidget(btnSincronizarLocal);

        btnSobrescribirNube = new QPushButton(DialogCompararArticulos);
        btnSobrescribirNube->setObjectName(QString::fromUtf8("btnSobrescribirNube"));

        horizontalLayout_2->addWidget(btnSobrescribirNube);

        btnCancelar = new QPushButton(DialogCompararArticulos);
        btnCancelar->setObjectName(QString::fromUtf8("btnCancelar"));

        horizontalLayout_2->addWidget(btnCancelar);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(DialogCompararArticulos);

        QMetaObject::connectSlotsByName(DialogCompararArticulos);
    } // setupUi

    void retranslateUi(QDialog *DialogCompararArticulos)
    {
        DialogCompararArticulos->setWindowTitle(QCoreApplication::translate("DialogCompararArticulos", "Conflicto detectado en la Nube", nullptr));
        label->setText(QCoreApplication::translate("DialogCompararArticulos", "El nuevo c\303\263digo ya existe en la Nube. Compare los datos:", nullptr));
        groupBoxLocal->setTitle(QCoreApplication::translate("DialogCompararArticulos", "Tu Art\303\255culo (Local)", nullptr));
        label_2->setText(QCoreApplication::translate("DialogCompararArticulos", "C\303\263digo:", nullptr));
        lblLocalCod->setText(QCoreApplication::translate("DialogCompararArticulos", "-", nullptr));
        label_3->setText(QCoreApplication::translate("DialogCompararArticulos", "Descripci\303\263n:", nullptr));
        lblLocalDesc->setText(QCoreApplication::translate("DialogCompararArticulos", "-", nullptr));
        label_4->setText(QCoreApplication::translate("DialogCompararArticulos", "PVP:", nullptr));
        lblLocalPvp->setText(QCoreApplication::translate("DialogCompararArticulos", "-", nullptr));
        label_5->setText(QCoreApplication::translate("DialogCompararArticulos", "IVA:", nullptr));
        lblLocalIva->setText(QCoreApplication::translate("DialogCompararArticulos", "-", nullptr));
        groupBoxNube->setTitle(QCoreApplication::translate("DialogCompararArticulos", "Art\303\255culo Existente (Nube)", nullptr));
        label_6->setText(QCoreApplication::translate("DialogCompararArticulos", "C\303\263digo:", nullptr));
        lblNubeCod->setText(QCoreApplication::translate("DialogCompararArticulos", "-", nullptr));
        label_7->setText(QCoreApplication::translate("DialogCompararArticulos", "Descripci\303\263n:", nullptr));
        lblNubeDesc->setText(QCoreApplication::translate("DialogCompararArticulos", "-", nullptr));
        label_8->setText(QCoreApplication::translate("DialogCompararArticulos", "PVP:", nullptr));
        lblNubePvp->setText(QCoreApplication::translate("DialogCompararArticulos", "-", nullptr));
        label_9->setText(QCoreApplication::translate("DialogCompararArticulos", "IVA:", nullptr));
        lblNubeIva->setText(QCoreApplication::translate("DialogCompararArticulos", "-", nullptr));
        label_10->setText(QCoreApplication::translate("DialogCompararArticulos", "\302\277C\303\263mo desea proceder?", nullptr));
        btnSincronizarLocal->setText(QCoreApplication::translate("DialogCompararArticulos", "Usar datos de Nube\n"
"(Tu historial adopta este producto)", nullptr));
        btnSobrescribirNube->setText(QCoreApplication::translate("DialogCompararArticulos", "Sobrescribir Nube\n"
"(Tus datos borran lo de la nube)", nullptr));
        btnCancelar->setText(QCoreApplication::translate("DialogCompararArticulos", "Cancelar Cambio", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogCompararArticulos: public Ui_DialogCompararArticulos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCOMPARARARTICULOS_H
