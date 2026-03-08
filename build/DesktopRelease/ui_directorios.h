/********************************************************************************
** Form generated from reading UI file 'directorios.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
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
    QGridLayout *gridLayout;
    QLabel *label_6;
    QLineEdit *lineEditEtiquetas;
    QLineEdit *lineEditFactura;
    QLabel *label_3;
    QLineEdit *lineEditCaducados;
    QLabel *label_5;
    QLineEdit *lineEditMovimientos;
    QLabel *label;
    QToolButton *toolButtonEtiquetas;
    QToolButton *toolButtonArqueos;
    QToolButton *toolButtonCaducados;
    QLabel *label_2;
    QToolButton *toolButtonMovimientos;
    QLabel *label_4;
    QToolButton *toolButtonVentas;
    QLabel *label_7;
    QToolButton *toolButtonCseg;
    QLineEdit *lineEditCseg;
    QLineEdit *lineEditVentas;
    QToolButton *toolButtonFactura;
    QLineEdit *lineEditArqueos;
    QLabel *label_8;
    QLineEdit *lineEditLogo;
    QToolButton *toolButtonLogo;
    QPushButton *pushButtonAceptar;
    QPushButton *pushButtonCancelar;

    void setupUi(QDialog *Directorios)
    {
        if (Directorios->objectName().isEmpty())
            Directorios->setObjectName("Directorios");
        Directorios->resize(761, 368);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imagenes/Emeicjac logo.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Directorios->setWindowIcon(icon);
        gridLayout_2 = new QGridLayout(Directorios);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label_6 = new QLabel(Directorios);
        label_6->setObjectName("label_6");

        gridLayout->addWidget(label_6, 5, 0, 1, 1);

        lineEditEtiquetas = new QLineEdit(Directorios);
        lineEditEtiquetas->setObjectName("lineEditEtiquetas");

        gridLayout->addWidget(lineEditEtiquetas, 2, 1, 1, 1);

        lineEditFactura = new QLineEdit(Directorios);
        lineEditFactura->setObjectName("lineEditFactura");

        gridLayout->addWidget(lineEditFactura, 0, 1, 1, 1);

        label_3 = new QLabel(Directorios);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        lineEditCaducados = new QLineEdit(Directorios);
        lineEditCaducados->setObjectName("lineEditCaducados");

        gridLayout->addWidget(lineEditCaducados, 3, 1, 1, 1);

        label_5 = new QLabel(Directorios);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        lineEditMovimientos = new QLineEdit(Directorios);
        lineEditMovimientos->setObjectName("lineEditMovimientos");

        gridLayout->addWidget(lineEditMovimientos, 5, 1, 1, 1);

        label = new QLabel(Directorios);
        label->setObjectName("label");

        gridLayout->addWidget(label, 1, 0, 1, 1);

        toolButtonEtiquetas = new QToolButton(Directorios);
        toolButtonEtiquetas->setObjectName("toolButtonEtiquetas");

        gridLayout->addWidget(toolButtonEtiquetas, 2, 2, 1, 1);

        toolButtonArqueos = new QToolButton(Directorios);
        toolButtonArqueos->setObjectName("toolButtonArqueos");

        gridLayout->addWidget(toolButtonArqueos, 4, 2, 1, 1);

        toolButtonCaducados = new QToolButton(Directorios);
        toolButtonCaducados->setObjectName("toolButtonCaducados");

        gridLayout->addWidget(toolButtonCaducados, 3, 2, 1, 1);

        label_2 = new QLabel(Directorios);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        toolButtonMovimientos = new QToolButton(Directorios);
        toolButtonMovimientos->setObjectName("toolButtonMovimientos");

        gridLayout->addWidget(toolButtonMovimientos, 5, 2, 1, 1);

        label_4 = new QLabel(Directorios);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        toolButtonVentas = new QToolButton(Directorios);
        toolButtonVentas->setObjectName("toolButtonVentas");

        gridLayout->addWidget(toolButtonVentas, 1, 2, 1, 1);

        label_7 = new QLabel(Directorios);
        label_7->setObjectName("label_7");

        gridLayout->addWidget(label_7, 6, 0, 1, 1);

        toolButtonCseg = new QToolButton(Directorios);
        toolButtonCseg->setObjectName("toolButtonCseg");

        gridLayout->addWidget(toolButtonCseg, 6, 2, 1, 1);

        lineEditCseg = new QLineEdit(Directorios);
        lineEditCseg->setObjectName("lineEditCseg");

        gridLayout->addWidget(lineEditCseg, 6, 1, 1, 1);

        lineEditVentas = new QLineEdit(Directorios);
        lineEditVentas->setObjectName("lineEditVentas");

        gridLayout->addWidget(lineEditVentas, 1, 1, 1, 1);

        toolButtonFactura = new QToolButton(Directorios);
        toolButtonFactura->setObjectName("toolButtonFactura");
        toolButtonFactura->setPopupMode(QToolButton::ToolButtonPopupMode::DelayedPopup);

        gridLayout->addWidget(toolButtonFactura, 0, 2, 1, 1);

        lineEditArqueos = new QLineEdit(Directorios);
        lineEditArqueos->setObjectName("lineEditArqueos");

        gridLayout->addWidget(lineEditArqueos, 4, 1, 1, 1);

        label_8 = new QLabel(Directorios);
        label_8->setObjectName("label_8");

        gridLayout->addWidget(label_8, 7, 0, 1, 1);

        lineEditLogo = new QLineEdit(Directorios);
        lineEditLogo->setObjectName("lineEditLogo");

        gridLayout->addWidget(lineEditLogo, 7, 1, 1, 1);

        toolButtonLogo = new QToolButton(Directorios);
        toolButtonLogo->setObjectName("toolButtonLogo");

        gridLayout->addWidget(toolButtonLogo, 7, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 2);

        pushButtonAceptar = new QPushButton(Directorios);
        pushButtonAceptar->setObjectName("pushButtonAceptar");
        pushButtonAceptar->setAutoDefault(false);

        gridLayout_2->addWidget(pushButtonAceptar, 1, 0, 1, 1);

        pushButtonCancelar = new QPushButton(Directorios);
        pushButtonCancelar->setObjectName("pushButtonCancelar");
        pushButtonCancelar->setAutoDefault(false);

        gridLayout_2->addWidget(pushButtonCancelar, 1, 1, 1, 1);

        QWidget::setTabOrder(lineEditFactura, toolButtonFactura);
        QWidget::setTabOrder(toolButtonFactura, lineEditVentas);
        QWidget::setTabOrder(lineEditVentas, toolButtonVentas);
        QWidget::setTabOrder(toolButtonVentas, lineEditEtiquetas);
        QWidget::setTabOrder(lineEditEtiquetas, toolButtonEtiquetas);
        QWidget::setTabOrder(toolButtonEtiquetas, lineEditCaducados);
        QWidget::setTabOrder(lineEditCaducados, toolButtonCaducados);
        QWidget::setTabOrder(toolButtonCaducados, lineEditArqueos);
        QWidget::setTabOrder(lineEditArqueos, toolButtonArqueos);
        QWidget::setTabOrder(toolButtonArqueos, lineEditMovimientos);
        QWidget::setTabOrder(lineEditMovimientos, toolButtonMovimientos);
        QWidget::setTabOrder(toolButtonMovimientos, pushButtonAceptar);
        QWidget::setTabOrder(pushButtonAceptar, pushButtonCancelar);

        retranslateUi(Directorios);
        QObject::connect(pushButtonCancelar, &QPushButton::clicked, Directorios, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(Directorios);
    } // setupUi

    void retranslateUi(QDialog *Directorios)
    {
        Directorios->setWindowTitle(QCoreApplication::translate("Directorios", "Directorios", nullptr));
        label_6->setText(QCoreApplication::translate("Directorios", "Movimientos efectivo:", nullptr));
        label_3->setText(QCoreApplication::translate("Directorios", "Caducados:", nullptr));
        label_5->setText(QCoreApplication::translate("Directorios", "Listado arqueos:", nullptr));
        label->setText(QCoreApplication::translate("Directorios", "Ventas:", nullptr));
        toolButtonEtiquetas->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        toolButtonArqueos->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        toolButtonCaducados->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        label_2->setText(QCoreApplication::translate("Directorios", "Etiquetas:", nullptr));
        toolButtonMovimientos->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        label_4->setText(QCoreApplication::translate("Directorios", "Factura:", nullptr));
        toolButtonVentas->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        label_7->setText(QCoreApplication::translate("Directorios", "Copia de seguridad", nullptr));
        toolButtonCseg->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        toolButtonFactura->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        label_8->setText(QCoreApplication::translate("Directorios", "Logo", nullptr));
        toolButtonLogo->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("Directorios", "Aceptar", nullptr));
        pushButtonCancelar->setText(QCoreApplication::translate("Directorios", "Cancelar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Directorios: public Ui_Directorios {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIRECTORIOS_H
