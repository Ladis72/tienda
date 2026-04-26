/********************************************************************************
** Form generated from reading UI file 'directorios.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIRECTORIOS_H
#define UI_DIRECTORIOS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_Directorios
{
public:
    QGridLayout *gridLayout_2;
    QPushButton *pushButtonAceptar;
    QGridLayout *gridLayout;
    QLabel *label_7;
    QToolButton *toolButtonCseg;
    QLineEdit *lineEditLogFactura;
    QLineEdit *lineEditCseg;
    QLabel *label_4;
    QLineEdit *lineEditLogo;
    QLabel *label_6;
    QToolButton *toolButtonLogo;
    QLabel *label_8;
    QLineEdit *lineEditDocumentos;
    QToolButton *toolButtonDocumentos;
    QToolButton *toolButtonLogoFactura;
    QLabel *label;
    QLineEdit *lineEditImagenes;
    QToolButton *toolButton;
    QPushButton *pushButtonCancelar;

    void setupUi(QDialog *Directorios)
    {
        if (Directorios->objectName().isEmpty())
            Directorios->setObjectName(QString::fromUtf8("Directorios"));
        Directorios->resize(761, 368);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imagenes/Emeicjac logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        Directorios->setWindowIcon(icon);
        gridLayout_2 = new QGridLayout(Directorios);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        pushButtonAceptar = new QPushButton(Directorios);
        pushButtonAceptar->setObjectName(QString::fromUtf8("pushButtonAceptar"));
        pushButtonAceptar->setAutoDefault(false);

        gridLayout_2->addWidget(pushButtonAceptar, 3, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_7 = new QLabel(Directorios);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 2, 0, 1, 1);

        toolButtonCseg = new QToolButton(Directorios);
        toolButtonCseg->setObjectName(QString::fromUtf8("toolButtonCseg"));

        gridLayout->addWidget(toolButtonCseg, 2, 2, 1, 1);

        lineEditLogFactura = new QLineEdit(Directorios);
        lineEditLogFactura->setObjectName(QString::fromUtf8("lineEditLogFactura"));

        gridLayout->addWidget(lineEditLogFactura, 1, 1, 1, 1);

        lineEditCseg = new QLineEdit(Directorios);
        lineEditCseg->setObjectName(QString::fromUtf8("lineEditCseg"));

        gridLayout->addWidget(lineEditCseg, 2, 1, 1, 1);

        label_4 = new QLabel(Directorios);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        lineEditLogo = new QLineEdit(Directorios);
        lineEditLogo->setObjectName(QString::fromUtf8("lineEditLogo"));

        gridLayout->addWidget(lineEditLogo, 3, 1, 1, 1);

        label_6 = new QLabel(Directorios);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 1, 0, 1, 1);

        toolButtonLogo = new QToolButton(Directorios);
        toolButtonLogo->setObjectName(QString::fromUtf8("toolButtonLogo"));

        gridLayout->addWidget(toolButtonLogo, 3, 2, 1, 1);

        label_8 = new QLabel(Directorios);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 3, 0, 1, 1);

        lineEditDocumentos = new QLineEdit(Directorios);
        lineEditDocumentos->setObjectName(QString::fromUtf8("lineEditDocumentos"));

        gridLayout->addWidget(lineEditDocumentos, 0, 1, 1, 1);

        toolButtonDocumentos = new QToolButton(Directorios);
        toolButtonDocumentos->setObjectName(QString::fromUtf8("toolButtonDocumentos"));
        toolButtonDocumentos->setPopupMode(QToolButton::ToolButtonPopupMode::DelayedPopup);

        gridLayout->addWidget(toolButtonDocumentos, 0, 2, 1, 1);

        toolButtonLogoFactura = new QToolButton(Directorios);
        toolButtonLogoFactura->setObjectName(QString::fromUtf8("toolButtonLogoFactura"));

        gridLayout->addWidget(toolButtonLogoFactura, 1, 2, 1, 1);

        label = new QLabel(Directorios);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 4, 0, 1, 1);

        lineEditImagenes = new QLineEdit(Directorios);
        lineEditImagenes->setObjectName(QString::fromUtf8("lineEditImagenes"));

        gridLayout->addWidget(lineEditImagenes, 4, 1, 1, 1);

        toolButton = new QToolButton(Directorios);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));

        gridLayout->addWidget(toolButton, 4, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 2);

        pushButtonCancelar = new QPushButton(Directorios);
        pushButtonCancelar->setObjectName(QString::fromUtf8("pushButtonCancelar"));
        pushButtonCancelar->setAutoDefault(false);

        gridLayout_2->addWidget(pushButtonCancelar, 3, 1, 1, 1);

        QWidget::setTabOrder(lineEditDocumentos, toolButtonDocumentos);
        QWidget::setTabOrder(toolButtonDocumentos, lineEditLogFactura);
        QWidget::setTabOrder(lineEditLogFactura, toolButtonLogoFactura);
        QWidget::setTabOrder(toolButtonLogoFactura, pushButtonAceptar);
        QWidget::setTabOrder(pushButtonAceptar, pushButtonCancelar);

        retranslateUi(Directorios);
        QObject::connect(pushButtonCancelar, SIGNAL(clicked()), Directorios, SLOT(reject()));

        QMetaObject::connectSlotsByName(Directorios);
    } // setupUi

    void retranslateUi(QDialog *Directorios)
    {
        Directorios->setWindowTitle(QCoreApplication::translate("Directorios", "Directorios", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("Directorios", "Aceptar", nullptr));
        label_7->setText(QCoreApplication::translate("Directorios", "Copia de seguridad", nullptr));
        toolButtonCseg->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        label_4->setText(QCoreApplication::translate("Directorios", "Documentos", nullptr));
        label_6->setText(QCoreApplication::translate("Directorios", "Logo factura", nullptr));
        toolButtonLogo->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        label_8->setText(QCoreApplication::translate("Directorios", "Logo", nullptr));
        toolButtonDocumentos->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        toolButtonLogoFactura->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        label->setText(QCoreApplication::translate("Directorios", "Imagenes", nullptr));
        toolButton->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        pushButtonCancelar->setText(QCoreApplication::translate("Directorios", "Cancelar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Directorios: public Ui_Directorios {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIRECTORIOS_H
