/********************************************************************************
** Form generated from reading UI file 'directorios.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
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
    QToolButton *toolButtonEtiquetas;
    QLabel *label_5;
    QToolButton *toolButtonCaducados;
    QLineEdit *lineEditEtiquetas;
    QToolButton *toolButtonMovimientos;
    QLineEdit *lineEditMovimientos;
    QLineEdit *lineEditFactura;
    QLabel *label_4;
    QLabel *label_3;
    QLabel *label_2;
    QLineEdit *lineEditCaducados;
    QLineEdit *lineEditArqueos;
    QToolButton *toolButtonArqueos;
    QLineEdit *lineEditVentas;
    QLabel *label;
    QToolButton *toolButtonFactura;
    QLabel *label_6;
    QToolButton *toolButtonVentas;
    QLabel *label_7;
    QLineEdit *lineEditCseg;
    QToolButton *toolButtonCseg;
    QPushButton *pushButtonAceptar;
    QPushButton *pushButtonCancelar;

    void setupUi(QDialog *Directorios)
    {
        if (Directorios->objectName().isEmpty())
            Directorios->setObjectName(QString::fromUtf8("Directorios"));
        Directorios->resize(761, 328);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imagenes/Emeicjac logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        Directorios->setWindowIcon(icon);
        gridLayout_2 = new QGridLayout(Directorios);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        toolButtonEtiquetas = new QToolButton(Directorios);
        toolButtonEtiquetas->setObjectName(QString::fromUtf8("toolButtonEtiquetas"));

        gridLayout->addWidget(toolButtonEtiquetas, 2, 2, 1, 1);

        label_5 = new QLabel(Directorios);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        toolButtonCaducados = new QToolButton(Directorios);
        toolButtonCaducados->setObjectName(QString::fromUtf8("toolButtonCaducados"));

        gridLayout->addWidget(toolButtonCaducados, 3, 2, 1, 1);

        lineEditEtiquetas = new QLineEdit(Directorios);
        lineEditEtiquetas->setObjectName(QString::fromUtf8("lineEditEtiquetas"));

        gridLayout->addWidget(lineEditEtiquetas, 2, 1, 1, 1);

        toolButtonMovimientos = new QToolButton(Directorios);
        toolButtonMovimientos->setObjectName(QString::fromUtf8("toolButtonMovimientos"));

        gridLayout->addWidget(toolButtonMovimientos, 5, 2, 1, 1);

        lineEditMovimientos = new QLineEdit(Directorios);
        lineEditMovimientos->setObjectName(QString::fromUtf8("lineEditMovimientos"));

        gridLayout->addWidget(lineEditMovimientos, 5, 1, 1, 1);

        lineEditFactura = new QLineEdit(Directorios);
        lineEditFactura->setObjectName(QString::fromUtf8("lineEditFactura"));

        gridLayout->addWidget(lineEditFactura, 0, 1, 1, 1);

        label_4 = new QLabel(Directorios);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        label_3 = new QLabel(Directorios);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        label_2 = new QLabel(Directorios);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        lineEditCaducados = new QLineEdit(Directorios);
        lineEditCaducados->setObjectName(QString::fromUtf8("lineEditCaducados"));

        gridLayout->addWidget(lineEditCaducados, 3, 1, 1, 1);

        lineEditArqueos = new QLineEdit(Directorios);
        lineEditArqueos->setObjectName(QString::fromUtf8("lineEditArqueos"));

        gridLayout->addWidget(lineEditArqueos, 4, 1, 1, 1);

        toolButtonArqueos = new QToolButton(Directorios);
        toolButtonArqueos->setObjectName(QString::fromUtf8("toolButtonArqueos"));

        gridLayout->addWidget(toolButtonArqueos, 4, 2, 1, 1);

        lineEditVentas = new QLineEdit(Directorios);
        lineEditVentas->setObjectName(QString::fromUtf8("lineEditVentas"));

        gridLayout->addWidget(lineEditVentas, 1, 1, 1, 1);

        label = new QLabel(Directorios);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        toolButtonFactura = new QToolButton(Directorios);
        toolButtonFactura->setObjectName(QString::fromUtf8("toolButtonFactura"));
        toolButtonFactura->setPopupMode(QToolButton::ToolButtonPopupMode::DelayedPopup);

        gridLayout->addWidget(toolButtonFactura, 0, 2, 1, 1);

        label_6 = new QLabel(Directorios);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 5, 0, 1, 1);

        toolButtonVentas = new QToolButton(Directorios);
        toolButtonVentas->setObjectName(QString::fromUtf8("toolButtonVentas"));

        gridLayout->addWidget(toolButtonVentas, 1, 2, 1, 1);

        label_7 = new QLabel(Directorios);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 6, 0, 1, 1);

        lineEditCseg = new QLineEdit(Directorios);
        lineEditCseg->setObjectName(QString::fromUtf8("lineEditCseg"));

        gridLayout->addWidget(lineEditCseg, 6, 1, 1, 1);

        toolButtonCseg = new QToolButton(Directorios);
        toolButtonCseg->setObjectName(QString::fromUtf8("toolButtonCseg"));

        gridLayout->addWidget(toolButtonCseg, 6, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 2);

        pushButtonAceptar = new QPushButton(Directorios);
        pushButtonAceptar->setObjectName(QString::fromUtf8("pushButtonAceptar"));
        pushButtonAceptar->setAutoDefault(false);

        gridLayout_2->addWidget(pushButtonAceptar, 1, 0, 1, 1);

        pushButtonCancelar = new QPushButton(Directorios);
        pushButtonCancelar->setObjectName(QString::fromUtf8("pushButtonCancelar"));
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
        QObject::connect(pushButtonCancelar, SIGNAL(clicked()), Directorios, SLOT(reject()));

        QMetaObject::connectSlotsByName(Directorios);
    } // setupUi

    void retranslateUi(QDialog *Directorios)
    {
        Directorios->setWindowTitle(QCoreApplication::translate("Directorios", "Directorios", nullptr));
        toolButtonEtiquetas->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        label_5->setText(QCoreApplication::translate("Directorios", "Listado arqueos:", nullptr));
        toolButtonCaducados->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        toolButtonMovimientos->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        label_4->setText(QCoreApplication::translate("Directorios", "Factura:", nullptr));
        label_3->setText(QCoreApplication::translate("Directorios", "Caducados:", nullptr));
        label_2->setText(QCoreApplication::translate("Directorios", "Etiquetas:", nullptr));
        toolButtonArqueos->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        label->setText(QCoreApplication::translate("Directorios", "Ventas:", nullptr));
        toolButtonFactura->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        label_6->setText(QCoreApplication::translate("Directorios", "Movimientos efectivo:", nullptr));
        toolButtonVentas->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        label_7->setText(QCoreApplication::translate("Directorios", "Copia de seguridad", nullptr));
        toolButtonCseg->setText(QCoreApplication::translate("Directorios", "...", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("Directorios", "Aceptar", nullptr));
        pushButtonCancelar->setText(QCoreApplication::translate("Directorios", "Cancelar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Directorios: public Ui_Directorios {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIRECTORIOS_H
