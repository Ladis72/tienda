/********************************************************************************
** Form generated from reading UI file 'configticket.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGTICKET_H
#define UI_CONFIGTICKET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConfigTicket
{
public:
    QVBoxLayout *mainVerticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *labelTitle;
    QSpacerItem *horizontalSpacerHeader;
    QHBoxLayout *horizontalLayoutBody;
    QVBoxLayout *verticalLayoutLeft;
    QGroupBox *groupBoxCabecera;
    QVBoxLayout *verticalLayoutCabecera;
    QPlainTextEdit *plainTextEditCabecera;
    QHBoxLayout *horizontalLayoutImgCabecera;
    QLabel *labelImgCabecera;
    QLineEdit *lineEditImgCabecera;
    QToolButton *toolButtonImgCabecera;
    QLabel *labelPreviewCabecera;
    QGroupBox *groupBoxPie;
    QVBoxLayout *verticalLayoutPie;
    QPlainTextEdit *plainTextEditPie;
    QVBoxLayout *verticalLayoutRight;
    QGroupBox *groupBoxImpresora;
    QFormLayout *formLayoutImpresora;
    QLabel *label;
    QLineEdit *lineEditCola;
    QLabel *label_2;
    QLineEdit *lineEditCodApertura;
    QLabel *label_3;
    QLineEdit *lineEditCodCorte;
    QGroupBox *groupBoxPromo;
    QVBoxLayout *verticalLayoutPromo;
    QCheckBox *checkBoxPromo;
    QHBoxLayout *horizontalLayoutImgPromo;
    QLabel *labelImgPromo;
    QLineEdit *lineEditImgPromo;
    QToolButton *toolButtonImgPromo;
    QLabel *labelPreviewPromo;
    QSpacerItem *verticalSpacerRight;
    QHBoxLayout *horizontalLayoutActions;
    QSpacerItem *horizontalSpacerActions;
    QPushButton *pushButtonCancelar;
    QPushButton *pushButtonAceptar;

    void setupUi(QDialog *ConfigTicket)
    {
        if (ConfigTicket->objectName().isEmpty())
            ConfigTicket->setObjectName("ConfigTicket");
        ConfigTicket->resize(1024, 768);
        ConfigTicket->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        mainVerticalLayout = new QVBoxLayout(ConfigTicket);
        mainVerticalLayout->setSpacing(20);
        mainVerticalLayout->setObjectName("mainVerticalLayout");
        mainVerticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(ConfigTicket);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame {\n"
"    background-color: #f5f5f5;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #e0e0e0;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName("horizontalLayoutHeader");
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName("labelIcono");
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/tickets.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelTitle = new QLabel(headerFrame);
        labelTitle->setObjectName("labelTitle");
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelTitle->setFont(font);
        labelTitle->setStyleSheet(QString::fromUtf8("color: #424242;"));

        horizontalLayoutHeader->addWidget(labelTitle);

        horizontalSpacerHeader = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);


        mainVerticalLayout->addWidget(headerFrame);

        horizontalLayoutBody = new QHBoxLayout();
        horizontalLayoutBody->setObjectName("horizontalLayoutBody");
        verticalLayoutLeft = new QVBoxLayout();
        verticalLayoutLeft->setObjectName("verticalLayoutLeft");
        groupBoxCabecera = new QGroupBox(ConfigTicket);
        groupBoxCabecera->setObjectName("groupBoxCabecera");
        verticalLayoutCabecera = new QVBoxLayout(groupBoxCabecera);
        verticalLayoutCabecera->setObjectName("verticalLayoutCabecera");
        plainTextEditCabecera = new QPlainTextEdit(groupBoxCabecera);
        plainTextEditCabecera->setObjectName("plainTextEditCabecera");

        verticalLayoutCabecera->addWidget(plainTextEditCabecera);

        horizontalLayoutImgCabecera = new QHBoxLayout();
        horizontalLayoutImgCabecera->setObjectName("horizontalLayoutImgCabecera");
        labelImgCabecera = new QLabel(groupBoxCabecera);
        labelImgCabecera->setObjectName("labelImgCabecera");

        horizontalLayoutImgCabecera->addWidget(labelImgCabecera);

        lineEditImgCabecera = new QLineEdit(groupBoxCabecera);
        lineEditImgCabecera->setObjectName("lineEditImgCabecera");

        horizontalLayoutImgCabecera->addWidget(lineEditImgCabecera);

        toolButtonImgCabecera = new QToolButton(groupBoxCabecera);
        toolButtonImgCabecera->setObjectName("toolButtonImgCabecera");

        horizontalLayoutImgCabecera->addWidget(toolButtonImgCabecera);


        verticalLayoutCabecera->addLayout(horizontalLayoutImgCabecera);

        labelPreviewCabecera = new QLabel(groupBoxCabecera);
        labelPreviewCabecera->setObjectName("labelPreviewCabecera");
        labelPreviewCabecera->setMinimumSize(QSize(0, 80));
        labelPreviewCabecera->setAlignment(Qt::AlignmentFlag::AlignCenter);
        labelPreviewCabecera->setStyleSheet(QString::fromUtf8("border: 1px dashed #bdbdbd;"));

        verticalLayoutCabecera->addWidget(labelPreviewCabecera);


        verticalLayoutLeft->addWidget(groupBoxCabecera);

        groupBoxPie = new QGroupBox(ConfigTicket);
        groupBoxPie->setObjectName("groupBoxPie");
        verticalLayoutPie = new QVBoxLayout(groupBoxPie);
        verticalLayoutPie->setObjectName("verticalLayoutPie");
        plainTextEditPie = new QPlainTextEdit(groupBoxPie);
        plainTextEditPie->setObjectName("plainTextEditPie");

        verticalLayoutPie->addWidget(plainTextEditPie);


        verticalLayoutLeft->addWidget(groupBoxPie);


        horizontalLayoutBody->addLayout(verticalLayoutLeft);

        verticalLayoutRight = new QVBoxLayout();
        verticalLayoutRight->setObjectName("verticalLayoutRight");
        groupBoxImpresora = new QGroupBox(ConfigTicket);
        groupBoxImpresora->setObjectName("groupBoxImpresora");
        formLayoutImpresora = new QFormLayout(groupBoxImpresora);
        formLayoutImpresora->setObjectName("formLayoutImpresora");
        label = new QLabel(groupBoxImpresora);
        label->setObjectName("label");

        formLayoutImpresora->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        lineEditCola = new QLineEdit(groupBoxImpresora);
        lineEditCola->setObjectName("lineEditCola");

        formLayoutImpresora->setWidget(0, QFormLayout::ItemRole::FieldRole, lineEditCola);

        label_2 = new QLabel(groupBoxImpresora);
        label_2->setObjectName("label_2");

        formLayoutImpresora->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        lineEditCodApertura = new QLineEdit(groupBoxImpresora);
        lineEditCodApertura->setObjectName("lineEditCodApertura");

        formLayoutImpresora->setWidget(1, QFormLayout::ItemRole::FieldRole, lineEditCodApertura);

        label_3 = new QLabel(groupBoxImpresora);
        label_3->setObjectName("label_3");

        formLayoutImpresora->setWidget(2, QFormLayout::ItemRole::LabelRole, label_3);

        lineEditCodCorte = new QLineEdit(groupBoxImpresora);
        lineEditCodCorte->setObjectName("lineEditCodCorte");

        formLayoutImpresora->setWidget(2, QFormLayout::ItemRole::FieldRole, lineEditCodCorte);


        verticalLayoutRight->addWidget(groupBoxImpresora);

        groupBoxPromo = new QGroupBox(ConfigTicket);
        groupBoxPromo->setObjectName("groupBoxPromo");
        verticalLayoutPromo = new QVBoxLayout(groupBoxPromo);
        verticalLayoutPromo->setObjectName("verticalLayoutPromo");
        checkBoxPromo = new QCheckBox(groupBoxPromo);
        checkBoxPromo->setObjectName("checkBoxPromo");

        verticalLayoutPromo->addWidget(checkBoxPromo);

        horizontalLayoutImgPromo = new QHBoxLayout();
        horizontalLayoutImgPromo->setObjectName("horizontalLayoutImgPromo");
        labelImgPromo = new QLabel(groupBoxPromo);
        labelImgPromo->setObjectName("labelImgPromo");

        horizontalLayoutImgPromo->addWidget(labelImgPromo);

        lineEditImgPromo = new QLineEdit(groupBoxPromo);
        lineEditImgPromo->setObjectName("lineEditImgPromo");

        horizontalLayoutImgPromo->addWidget(lineEditImgPromo);

        toolButtonImgPromo = new QToolButton(groupBoxPromo);
        toolButtonImgPromo->setObjectName("toolButtonImgPromo");

        horizontalLayoutImgPromo->addWidget(toolButtonImgPromo);


        verticalLayoutPromo->addLayout(horizontalLayoutImgPromo);

        labelPreviewPromo = new QLabel(groupBoxPromo);
        labelPreviewPromo->setObjectName("labelPreviewPromo");
        labelPreviewPromo->setMinimumSize(QSize(0, 100));
        labelPreviewPromo->setAlignment(Qt::AlignmentFlag::AlignCenter);
        labelPreviewPromo->setStyleSheet(QString::fromUtf8("border: 1px dashed #bdbdbd;"));

        verticalLayoutPromo->addWidget(labelPreviewPromo);


        verticalLayoutRight->addWidget(groupBoxPromo);

        verticalSpacerRight = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayoutRight->addItem(verticalSpacerRight);


        horizontalLayoutBody->addLayout(verticalLayoutRight);


        mainVerticalLayout->addLayout(horizontalLayoutBody);

        horizontalLayoutActions = new QHBoxLayout();
        horizontalLayoutActions->setObjectName("horizontalLayoutActions");
        horizontalSpacerActions = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutActions->addItem(horizontalSpacerActions);

        pushButtonCancelar = new QPushButton(ConfigTicket);
        pushButtonCancelar->setObjectName("pushButtonCancelar");
        pushButtonCancelar->setMinimumSize(QSize(120, 40));

        horizontalLayoutActions->addWidget(pushButtonCancelar);

        pushButtonAceptar = new QPushButton(ConfigTicket);
        pushButtonAceptar->setObjectName("pushButtonAceptar");
        pushButtonAceptar->setMinimumSize(QSize(120, 40));
        pushButtonAceptar->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #616161; color: white; font-weight: bold; border-radius: 4px; }\n"
"QPushButton:hover { background-color: #424242; }"));

        horizontalLayoutActions->addWidget(pushButtonAceptar);


        mainVerticalLayout->addLayout(horizontalLayoutActions);


        retranslateUi(ConfigTicket);

        QMetaObject::connectSlotsByName(ConfigTicket);
    } // setupUi

    void retranslateUi(QDialog *ConfigTicket)
    {
        ConfigTicket->setWindowTitle(QCoreApplication::translate("ConfigTicket", "Configuraci\303\263n de Tickets", nullptr));
        labelTitle->setText(QCoreApplication::translate("ConfigTicket", "Configurar Tickets e Impresora", nullptr));
        groupBoxCabecera->setTitle(QCoreApplication::translate("ConfigTicket", "Texto de Cabecera", nullptr));
        labelImgCabecera->setText(QCoreApplication::translate("ConfigTicket", "Logo Cabecera:", nullptr));
        toolButtonImgCabecera->setText(QCoreApplication::translate("ConfigTicket", "...", nullptr));
        labelPreviewCabecera->setText(QCoreApplication::translate("ConfigTicket", "Vista previa logo", nullptr));
        groupBoxPie->setTitle(QCoreApplication::translate("ConfigTicket", "Texto de Pie de P\303\241gina", nullptr));
        groupBoxImpresora->setTitle(QCoreApplication::translate("ConfigTicket", "Hardware / Impresora", nullptr));
        label->setText(QCoreApplication::translate("ConfigTicket", "Cola Impresi\303\263n:", nullptr));
        label_2->setText(QCoreApplication::translate("ConfigTicket", "C\303\263d. Apertura:", nullptr));
        label_3->setText(QCoreApplication::translate("ConfigTicket", "C\303\263d. Corte:", nullptr));
        groupBoxPromo->setTitle(QCoreApplication::translate("ConfigTicket", "Ticket Promocional", nullptr));
        checkBoxPromo->setText(QCoreApplication::translate("ConfigTicket", "Activar publicidad en ticket", nullptr));
        labelImgPromo->setText(QCoreApplication::translate("ConfigTicket", "Imagen Promo:", nullptr));
        toolButtonImgPromo->setText(QCoreApplication::translate("ConfigTicket", "...", nullptr));
        labelPreviewPromo->setText(QCoreApplication::translate("ConfigTicket", "Vista previa promo", nullptr));
        pushButtonCancelar->setText(QCoreApplication::translate("ConfigTicket", "Cancelar", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("ConfigTicket", "Guardar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfigTicket: public Ui_ConfigTicket {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGTICKET_H
