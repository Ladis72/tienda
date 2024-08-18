/********************************************************************************
** Form generated from reading UI file 'aceptarpedido.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACEPTARPEDIDO_H
#define UI_ACEPTARPEDIDO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_AceptarPedido
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout_3;
    QLineEdit *lineEditNDoc;
    QSpacerItem *horizontalSpacer;
    QComboBox *comboBox;
    QDateEdit *dateEditVencimiento;
    QDoubleSpinBox *doubleSpinBox;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label;
    QLabel *label_12;
    QDateEdit *dateEditDocumento;
    QLabel *labelProveedor;
    QGridLayout *gridLayout;
    QLineEdit *leTotalIva;
    QLineEdit *leRe5;
    QLabel *label_5;
    QLabel *label_9;
    QLineEdit *leRe10;
    QLabel *label_6;
    QFrame *line;
    QLineEdit *leBase5;
    QLineEdit *leIva21;
    QLabel *label_13;
    QLineEdit *leTotalBase;
    QLineEdit *leRe21;
    QLineEdit *leTotalRe;
    QLabel *label_8;
    QLabel *label_2;
    QLineEdit *leTotal10;
    QLabel *label_7;
    QLineEdit *leBase10;
    QLineEdit *leIva5;
    QLineEdit *leTotal;
    QFrame *line_2;
    QLineEdit *leTotal5;
    QLineEdit *leTotal21;
    QLineEdit *leBase21;
    QLineEdit *leIva10;
    QLabel *label_3;
    QFrame *line_3;
    QLabel *label_4;
    QLabel *label_14;
    QLineEdit *leBase4;
    QLineEdit *leBase0;
    QLineEdit *leIva4;
    QLineEdit *leIva0;
    QLineEdit *leRe4;
    QLineEdit *leRe0;
    QLineEdit *leTotal4;
    QLineEdit *leTotal0;
    QDialogButtonBox *buttonBox;
    QPushButton *pushButton;
    QFrame *line_4;

    void setupUi(QDialog *AceptarPedido)
    {
        if (AceptarPedido->objectName().isEmpty())
            AceptarPedido->setObjectName(QString::fromUtf8("AceptarPedido"));
        AceptarPedido->resize(796, 424);
        gridLayout_2 = new QGridLayout(AceptarPedido);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        lineEditNDoc = new QLineEdit(AceptarPedido);
        lineEditNDoc->setObjectName(QString::fromUtf8("lineEditNDoc"));
        lineEditNDoc->setMaximumSize(QSize(120, 16777215));

        gridLayout_3->addWidget(lineEditNDoc, 2, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 1, 3, 1, 1);

        comboBox = new QComboBox(AceptarPedido);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        gridLayout_3->addWidget(comboBox, 2, 0, 1, 1);

        dateEditVencimiento = new QDateEdit(AceptarPedido);
        dateEditVencimiento->setObjectName(QString::fromUtf8("dateEditVencimiento"));
        dateEditVencimiento->setEnabled(false);
        dateEditVencimiento->setCalendarPopup(true);

        gridLayout_3->addWidget(dateEditVencimiento, 2, 5, 1, 1);

        doubleSpinBox = new QDoubleSpinBox(AceptarPedido);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));

        gridLayout_3->addWidget(doubleSpinBox, 2, 7, 1, 1);

        label_10 = new QLabel(AceptarPedido);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_3->addWidget(label_10, 1, 2, 1, 1);

        label_11 = new QLabel(AceptarPedido);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_3->addWidget(label_11, 1, 5, 1, 1);

        label = new QLabel(AceptarPedido);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 1, 7, 1, 1);

        label_12 = new QLabel(AceptarPedido);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_3->addWidget(label_12, 1, 1, 1, 1);

        dateEditDocumento = new QDateEdit(AceptarPedido);
        dateEditDocumento->setObjectName(QString::fromUtf8("dateEditDocumento"));
        dateEditDocumento->setCalendarPopup(true);

        gridLayout_3->addWidget(dateEditDocumento, 2, 1, 1, 1);

        labelProveedor = new QLabel(AceptarPedido);
        labelProveedor->setObjectName(QString::fromUtf8("labelProveedor"));

        gridLayout_3->addWidget(labelProveedor, 2, 3, 1, 1);


        gridLayout_2->addLayout(gridLayout_3, 0, 1, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        leTotalIva = new QLineEdit(AceptarPedido);
        leTotalIva->setObjectName(QString::fromUtf8("leTotalIva"));
        leTotalIva->setEnabled(false);

        gridLayout->addWidget(leTotalIva, 7, 2, 1, 1);

        leRe5 = new QLineEdit(AceptarPedido);
        leRe5->setObjectName(QString::fromUtf8("leRe5"));
        leRe5->setEnabled(false);

        gridLayout->addWidget(leRe5, 3, 3, 1, 1);

        label_5 = new QLabel(AceptarPedido);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 0, 5, 1, 1);

        label_9 = new QLabel(AceptarPedido);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 7, 0, 1, 1);

        leRe10 = new QLineEdit(AceptarPedido);
        leRe10->setObjectName(QString::fromUtf8("leRe10"));
        leRe10->setEnabled(false);

        gridLayout->addWidget(leRe10, 2, 3, 1, 1);

        label_6 = new QLabel(AceptarPedido);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_6, 1, 0, 1, 1);

        line = new QFrame(AceptarPedido);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 1, 4, 3, 1);

        leBase5 = new QLineEdit(AceptarPedido);
        leBase5->setObjectName(QString::fromUtf8("leBase5"));
        leBase5->setEnabled(false);

        gridLayout->addWidget(leBase5, 3, 1, 1, 1);

        leIva21 = new QLineEdit(AceptarPedido);
        leIva21->setObjectName(QString::fromUtf8("leIva21"));
        leIva21->setEnabled(false);

        gridLayout->addWidget(leIva21, 1, 2, 1, 1);

        label_13 = new QLabel(AceptarPedido);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_13, 4, 0, 1, 1);

        leTotalBase = new QLineEdit(AceptarPedido);
        leTotalBase->setObjectName(QString::fromUtf8("leTotalBase"));
        leTotalBase->setEnabled(false);

        gridLayout->addWidget(leTotalBase, 7, 1, 1, 1);

        leRe21 = new QLineEdit(AceptarPedido);
        leRe21->setObjectName(QString::fromUtf8("leRe21"));
        leRe21->setEnabled(false);

        gridLayout->addWidget(leRe21, 1, 3, 1, 1);

        leTotalRe = new QLineEdit(AceptarPedido);
        leTotalRe->setObjectName(QString::fromUtf8("leTotalRe"));
        leTotalRe->setEnabled(false);

        gridLayout->addWidget(leTotalRe, 7, 3, 1, 1);

        label_8 = new QLabel(AceptarPedido);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_8, 3, 0, 1, 1);

        label_2 = new QLabel(AceptarPedido);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        leTotal10 = new QLineEdit(AceptarPedido);
        leTotal10->setObjectName(QString::fromUtf8("leTotal10"));
        leTotal10->setEnabled(false);

        gridLayout->addWidget(leTotal10, 2, 5, 1, 1);

        label_7 = new QLabel(AceptarPedido);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_7, 2, 0, 1, 1);

        leBase10 = new QLineEdit(AceptarPedido);
        leBase10->setObjectName(QString::fromUtf8("leBase10"));
        leBase10->setEnabled(false);

        gridLayout->addWidget(leBase10, 2, 1, 1, 1);

        leIva5 = new QLineEdit(AceptarPedido);
        leIva5->setObjectName(QString::fromUtf8("leIva5"));
        leIva5->setEnabled(false);

        gridLayout->addWidget(leIva5, 3, 2, 1, 1);

        leTotal = new QLineEdit(AceptarPedido);
        leTotal->setObjectName(QString::fromUtf8("leTotal"));
        leTotal->setEnabled(false);

        gridLayout->addWidget(leTotal, 7, 5, 1, 1);

        line_2 = new QFrame(AceptarPedido);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 6, 0, 1, 6);

        leTotal5 = new QLineEdit(AceptarPedido);
        leTotal5->setObjectName(QString::fromUtf8("leTotal5"));
        leTotal5->setEnabled(false);

        gridLayout->addWidget(leTotal5, 3, 5, 1, 1);

        leTotal21 = new QLineEdit(AceptarPedido);
        leTotal21->setObjectName(QString::fromUtf8("leTotal21"));
        leTotal21->setEnabled(false);

        gridLayout->addWidget(leTotal21, 1, 5, 1, 1);

        leBase21 = new QLineEdit(AceptarPedido);
        leBase21->setObjectName(QString::fromUtf8("leBase21"));
        leBase21->setEnabled(false);

        gridLayout->addWidget(leBase21, 1, 1, 1, 1);

        leIva10 = new QLineEdit(AceptarPedido);
        leIva10->setObjectName(QString::fromUtf8("leIva10"));
        leIva10->setEnabled(false);

        gridLayout->addWidget(leIva10, 2, 2, 1, 1);

        label_3 = new QLabel(AceptarPedido);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        line_3 = new QFrame(AceptarPedido);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_3, 7, 4, 1, 1);

        label_4 = new QLabel(AceptarPedido);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 0, 3, 1, 1);

        label_14 = new QLabel(AceptarPedido);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_14, 5, 0, 1, 1);

        leBase4 = new QLineEdit(AceptarPedido);
        leBase4->setObjectName(QString::fromUtf8("leBase4"));
        leBase4->setEnabled(false);

        gridLayout->addWidget(leBase4, 4, 1, 1, 1);

        leBase0 = new QLineEdit(AceptarPedido);
        leBase0->setObjectName(QString::fromUtf8("leBase0"));
        leBase0->setEnabled(false);

        gridLayout->addWidget(leBase0, 5, 1, 1, 1);

        leIva4 = new QLineEdit(AceptarPedido);
        leIva4->setObjectName(QString::fromUtf8("leIva4"));
        leIva4->setEnabled(false);

        gridLayout->addWidget(leIva4, 4, 2, 1, 1);

        leIva0 = new QLineEdit(AceptarPedido);
        leIva0->setObjectName(QString::fromUtf8("leIva0"));
        leIva0->setEnabled(false);

        gridLayout->addWidget(leIva0, 5, 2, 1, 1);

        leRe4 = new QLineEdit(AceptarPedido);
        leRe4->setObjectName(QString::fromUtf8("leRe4"));
        leRe4->setEnabled(false);

        gridLayout->addWidget(leRe4, 4, 3, 1, 1);

        leRe0 = new QLineEdit(AceptarPedido);
        leRe0->setObjectName(QString::fromUtf8("leRe0"));
        leRe0->setEnabled(false);

        gridLayout->addWidget(leRe0, 5, 3, 1, 1);

        leTotal4 = new QLineEdit(AceptarPedido);
        leTotal4->setObjectName(QString::fromUtf8("leTotal4"));
        leTotal4->setEnabled(false);

        gridLayout->addWidget(leTotal4, 4, 5, 1, 1);

        leTotal0 = new QLineEdit(AceptarPedido);
        leTotal0->setObjectName(QString::fromUtf8("leTotal0"));
        leTotal0->setEnabled(false);

        gridLayout->addWidget(leTotal0, 5, 5, 1, 1);


        gridLayout_2->addLayout(gridLayout, 2, 1, 1, 1);

        buttonBox = new QDialogButtonBox(AceptarPedido);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout_2->addWidget(buttonBox, 4, 1, 1, 1);

        pushButton = new QPushButton(AceptarPedido);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout_2->addWidget(pushButton, 3, 1, 1, 1);

        line_4 = new QFrame(AceptarPedido);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_4, 1, 1, 1, 1);

        QWidget::setTabOrder(comboBox, dateEditDocumento);
        QWidget::setTabOrder(dateEditDocumento, lineEditNDoc);
        QWidget::setTabOrder(lineEditNDoc, dateEditVencimiento);
        QWidget::setTabOrder(dateEditVencimiento, doubleSpinBox);
        QWidget::setTabOrder(doubleSpinBox, pushButton);
        QWidget::setTabOrder(pushButton, leBase21);
        QWidget::setTabOrder(leBase21, leIva21);
        QWidget::setTabOrder(leIva21, leRe21);
        QWidget::setTabOrder(leRe21, leTotal21);
        QWidget::setTabOrder(leTotal21, leBase10);
        QWidget::setTabOrder(leBase10, leRe10);
        QWidget::setTabOrder(leRe10, leIva10);
        QWidget::setTabOrder(leIva10, leTotal10);
        QWidget::setTabOrder(leTotal10, leBase5);
        QWidget::setTabOrder(leBase5, leIva5);
        QWidget::setTabOrder(leIva5, leRe5);
        QWidget::setTabOrder(leRe5, leTotal5);
        QWidget::setTabOrder(leTotal5, leTotalBase);
        QWidget::setTabOrder(leTotalBase, leTotalIva);
        QWidget::setTabOrder(leTotalIva, leTotalRe);
        QWidget::setTabOrder(leTotalRe, leTotal);

        retranslateUi(AceptarPedido);
        QObject::connect(buttonBox, SIGNAL(accepted()), AceptarPedido, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AceptarPedido, SLOT(reject()));

        QMetaObject::connectSlotsByName(AceptarPedido);
    } // setupUi

    void retranslateUi(QDialog *AceptarPedido)
    {
        AceptarPedido->setWindowTitle(QCoreApplication::translate("AceptarPedido", "Dialog", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("AceptarPedido", "Seleccionar", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("AceptarPedido", "Factura", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("AceptarPedido", "Albar\303\241n", nullptr));

        dateEditVencimiento->setDisplayFormat(QCoreApplication::translate("AceptarPedido", "yyyy/MM/dd", nullptr));
        label_10->setText(QCoreApplication::translate("AceptarPedido", "N\302\272 Documento", nullptr));
        label_11->setText(QCoreApplication::translate("AceptarPedido", "Fecha vencimiento", nullptr));
        label->setText(QCoreApplication::translate("AceptarPedido", "Descuento", nullptr));
        label_12->setText(QCoreApplication::translate("AceptarPedido", "Fecha Documento", nullptr));
        dateEditDocumento->setDisplayFormat(QCoreApplication::translate("AceptarPedido", "yyyy/MM/dd", nullptr));
        labelProveedor->setText(QCoreApplication::translate("AceptarPedido", "Proveedor", nullptr));
        label_5->setText(QCoreApplication::translate("AceptarPedido", "Total", nullptr));
        label_9->setText(QCoreApplication::translate("AceptarPedido", "Total", nullptr));
        label_6->setText(QCoreApplication::translate("AceptarPedido", "21", nullptr));
        label_13->setText(QCoreApplication::translate("AceptarPedido", "4", nullptr));
        label_8->setText(QCoreApplication::translate("AceptarPedido", "5", nullptr));
        label_2->setText(QCoreApplication::translate("AceptarPedido", "Base", nullptr));
        label_7->setText(QCoreApplication::translate("AceptarPedido", "10", nullptr));
        label_3->setText(QCoreApplication::translate("AceptarPedido", "I.V.A.", nullptr));
        label_4->setText(QCoreApplication::translate("AceptarPedido", "R.E.", nullptr));
        label_14->setText(QCoreApplication::translate("AceptarPedido", "0", nullptr));
        pushButton->setText(QCoreApplication::translate("AceptarPedido", "Aceptar pedido", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AceptarPedido: public Ui_AceptarPedido {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACEPTARPEDIDO_H
