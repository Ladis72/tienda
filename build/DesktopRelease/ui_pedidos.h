/********************************************************************************
** Form generated from reading UI file 'pedidos.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PEDIDOS_H
#define UI_PEDIDOS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_pedidos
{
public:
    QGridLayout *gridLayout_3;
    QLabel *labelDocumento;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonAnadir;
    QPushButton *pushButtonModificar;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonCerrar;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButtonImprimir;
    QGridLayout *gridLayout_2;
    QLabel *label_21;
    QLabel *label_18;
    QLineEdit *leCod;
    QLineEdit *leLote;
    QLineEdit *leUds;
    QLabel *label_12;
    QLabel *label_16;
    QLineEdit *leIva;
    QLabel *label_15;
    QLineEdit *leDescripcion;
    QLineEdit *lePvp;
    QLineEdit *lePvt;
    QLabel *label_11;
    QLabel *label_17;
    QLabel *labelDescripcion;
    QLineEdit *leTotalLinea;
    QDateEdit *dateEdit;
    QLabel *label_20;
    QLabel *label_13;
    QLineEdit *leBon;
    QLabel *label_19;
    QLabel *label_14;
    QLineEdit *leDescuento;
    QLineEdit *leMargen;
    QLabel *labelProveedor;
    QFrame *line_3;
    QTableView *tableView;
    QTableView *tableViewTotales;
    QGridLayout *gridLayout;
    QLineEdit *lineEditLineas;
    QLineEdit *lineEditUnidades;
    QLineEdit *lineEditBase;
    QLabel *label_3;
    QLabel *label_6;
    QLabel *label_4;
    QLabel *label;
    QLabel *label_5;
    QLabel *label_2;
    QLineEdit *lineEditIVA;
    QLineEdit *lineEditRecargo;
    QLineEdit *lineEditTotal;

    void setupUi(QDialog *pedidos)
    {
        if (pedidos->objectName().isEmpty())
            pedidos->setObjectName("pedidos");
        pedidos->resize(1182, 767);
        pedidos->setStyleSheet(QString::fromUtf8(""));
        gridLayout_3 = new QGridLayout(pedidos);
        gridLayout_3->setObjectName("gridLayout_3");
        labelDocumento = new QLabel(pedidos);
        labelDocumento->setObjectName("labelDocumento");
        QFont font;
        font.setFamilies({QString::fromUtf8("Fira Sans")});
        font.setPointSize(12);
        font.setBold(true);
        font.setItalic(true);
        labelDocumento->setFont(font);

        gridLayout_3->addWidget(labelDocumento, 0, 6, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pushButtonAnadir = new QPushButton(pedidos);
        pushButtonAnadir->setObjectName("pushButtonAnadir");
        pushButtonAnadir->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        pushButtonAnadir->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonAnadir);

        pushButtonModificar = new QPushButton(pedidos);
        pushButtonModificar->setObjectName("pushButtonModificar");
        pushButtonModificar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonModificar);

        pushButtonBorrar = new QPushButton(pedidos);
        pushButtonBorrar->setObjectName("pushButtonBorrar");
        pushButtonBorrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonBorrar);

        pushButtonCerrar = new QPushButton(pedidos);
        pushButtonCerrar->setObjectName("pushButtonCerrar");
        pushButtonCerrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonCerrar);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        pushButtonImprimir = new QPushButton(pedidos);
        pushButtonImprimir->setObjectName("pushButtonImprimir");
        pushButtonImprimir->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonImprimir);


        gridLayout_3->addLayout(horizontalLayout, 4, 0, 1, 7);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        label_21 = new QLabel(pedidos);
        label_21->setObjectName("label_21");

        gridLayout_2->addWidget(label_21, 0, 12, 1, 1);

        label_18 = new QLabel(pedidos);
        label_18->setObjectName("label_18");

        gridLayout_2->addWidget(label_18, 0, 11, 1, 1);

        leCod = new QLineEdit(pedidos);
        leCod->setObjectName("leCod");
        leCod->setMaximumSize(QSize(150, 16777215));
        leCod->setMaxLength(13);

        gridLayout_2->addWidget(leCod, 1, 0, 1, 1);

        leLote = new QLineEdit(pedidos);
        leLote->setObjectName("leLote");
        leLote->setEnabled(false);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(5);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leLote->sizePolicy().hasHeightForWidth());
        leLote->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(leLote, 1, 4, 1, 1);

        leUds = new QLineEdit(pedidos);
        leUds->setObjectName("leUds");
        leUds->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(leUds, 1, 2, 1, 1);

        label_12 = new QLabel(pedidos);
        label_12->setObjectName("label_12");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy1);
        label_12->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(label_12, 0, 4, 1, 1);

        label_16 = new QLabel(pedidos);
        label_16->setObjectName("label_16");

        gridLayout_2->addWidget(label_16, 0, 9, 1, 1);

        leIva = new QLineEdit(pedidos);
        leIva->setObjectName("leIva");
        leIva->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(leIva, 1, 9, 1, 1);

        label_15 = new QLabel(pedidos);
        label_15->setObjectName("label_15");

        gridLayout_2->addWidget(label_15, 0, 8, 1, 1);

        leDescripcion = new QLineEdit(pedidos);
        leDescripcion->setObjectName("leDescripcion");

        gridLayout_2->addWidget(leDescripcion, 1, 1, 1, 1);

        lePvp = new QLineEdit(pedidos);
        lePvp->setObjectName("lePvp");
        lePvp->setMaximumSize(QSize(80, 16777215));

        gridLayout_2->addWidget(lePvp, 1, 10, 1, 1);

        lePvt = new QLineEdit(pedidos);
        lePvt->setObjectName("lePvt");
        lePvt->setMaximumSize(QSize(80, 16777215));

        gridLayout_2->addWidget(lePvt, 1, 8, 1, 1);

        label_11 = new QLabel(pedidos);
        label_11->setObjectName("label_11");

        gridLayout_2->addWidget(label_11, 0, 0, 1, 1);

        label_17 = new QLabel(pedidos);
        label_17->setObjectName("label_17");

        gridLayout_2->addWidget(label_17, 0, 10, 1, 1);

        labelDescripcion = new QLabel(pedidos);
        labelDescripcion->setObjectName("labelDescripcion");

        gridLayout_2->addWidget(labelDescripcion, 0, 1, 1, 1);

        leTotalLinea = new QLineEdit(pedidos);
        leTotalLinea->setObjectName("leTotalLinea");
        leTotalLinea->setEnabled(false);
        leTotalLinea->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(leTotalLinea, 1, 13, 1, 1);

        dateEdit = new QDateEdit(pedidos);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setCalendarPopup(true);

        gridLayout_2->addWidget(dateEdit, 1, 5, 1, 1);

        label_20 = new QLabel(pedidos);
        label_20->setObjectName("label_20");

        gridLayout_2->addWidget(label_20, 0, 5, 1, 1);

        label_13 = new QLabel(pedidos);
        label_13->setObjectName("label_13");

        gridLayout_2->addWidget(label_13, 0, 2, 1, 1);

        leBon = new QLineEdit(pedidos);
        leBon->setObjectName("leBon");
        leBon->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(leBon, 1, 3, 1, 1);

        label_19 = new QLabel(pedidos);
        label_19->setObjectName("label_19");

        gridLayout_2->addWidget(label_19, 0, 13, 1, 1);

        label_14 = new QLabel(pedidos);
        label_14->setObjectName("label_14");

        gridLayout_2->addWidget(label_14, 0, 3, 1, 1);

        leDescuento = new QLineEdit(pedidos);
        leDescuento->setObjectName("leDescuento");
        leDescuento->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(leDescuento, 1, 11, 1, 1);

        leMargen = new QLineEdit(pedidos);
        leMargen->setObjectName("leMargen");
        leMargen->setEnabled(false);
        leMargen->setMaximumSize(QSize(54, 16777215));

        gridLayout_2->addWidget(leMargen, 1, 12, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 3, 0, 1, 7);

        labelProveedor = new QLabel(pedidos);
        labelProveedor->setObjectName("labelProveedor");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Fira Sans")});
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setItalic(true);
        labelProveedor->setFont(font1);

        gridLayout_3->addWidget(labelProveedor, 0, 5, 1, 1);

        line_3 = new QFrame(pedidos);
        line_3->setObjectName("line_3");
        line_3->setFrameShape(QFrame::Shape::HLine);
        line_3->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout_3->addWidget(line_3, 5, 0, 1, 7);

        tableView = new QTableView(pedidos);
        tableView->setObjectName("tableView");
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        gridLayout_3->addWidget(tableView, 2, 0, 1, 7);

        tableViewTotales = new QTableView(pedidos);
        tableViewTotales->setObjectName("tableViewTotales");

        gridLayout_3->addWidget(tableViewTotales, 6, 0, 1, 5);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        lineEditLineas = new QLineEdit(pedidos);
        lineEditLineas->setObjectName("lineEditLineas");
        lineEditLineas->setEnabled(false);

        gridLayout->addWidget(lineEditLineas, 0, 1, 1, 1);

        lineEditUnidades = new QLineEdit(pedidos);
        lineEditUnidades->setObjectName("lineEditUnidades");
        lineEditUnidades->setEnabled(false);

        gridLayout->addWidget(lineEditUnidades, 1, 1, 1, 1);

        lineEditBase = new QLineEdit(pedidos);
        lineEditBase->setObjectName("lineEditBase");
        lineEditBase->setEnabled(false);

        gridLayout->addWidget(lineEditBase, 2, 1, 1, 1);

        label_3 = new QLabel(pedidos);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_6 = new QLabel(pedidos);
        label_6->setObjectName("label_6");

        gridLayout->addWidget(label_6, 5, 0, 1, 1);

        label_4 = new QLabel(pedidos);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label = new QLabel(pedidos);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_5 = new QLabel(pedidos);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        label_2 = new QLabel(pedidos);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEditIVA = new QLineEdit(pedidos);
        lineEditIVA->setObjectName("lineEditIVA");
        lineEditIVA->setEnabled(false);

        gridLayout->addWidget(lineEditIVA, 3, 1, 1, 1);

        lineEditRecargo = new QLineEdit(pedidos);
        lineEditRecargo->setObjectName("lineEditRecargo");
        lineEditRecargo->setEnabled(false);

        gridLayout->addWidget(lineEditRecargo, 4, 1, 1, 1);

        lineEditTotal = new QLineEdit(pedidos);
        lineEditTotal->setObjectName("lineEditTotal");
        lineEditTotal->setEnabled(false);

        gridLayout->addWidget(lineEditTotal, 5, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout, 6, 5, 1, 2);

#if QT_CONFIG(shortcut)
        label_18->setBuddy(leDescuento);
        label_16->setBuddy(leIva);
        label_15->setBuddy(lePvt);
        label_11->setBuddy(leCod);
        label_17->setBuddy(lePvp);
        labelDescripcion->setBuddy(leDescripcion);
        label_13->setBuddy(leUds);
        label_19->setBuddy(leTotalLinea);
        label_14->setBuddy(leBon);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(leCod, leDescripcion);
        QWidget::setTabOrder(leDescripcion, leUds);
        QWidget::setTabOrder(leUds, leBon);
        QWidget::setTabOrder(leBon, leLote);
        QWidget::setTabOrder(leLote, dateEdit);
        QWidget::setTabOrder(dateEdit, lePvt);
        QWidget::setTabOrder(lePvt, leIva);
        QWidget::setTabOrder(leIva, lePvp);
        QWidget::setTabOrder(lePvp, leDescuento);
        QWidget::setTabOrder(leDescuento, pushButtonAnadir);
        QWidget::setTabOrder(pushButtonAnadir, pushButtonModificar);
        QWidget::setTabOrder(pushButtonModificar, pushButtonBorrar);
        QWidget::setTabOrder(pushButtonBorrar, tableView);
        QWidget::setTabOrder(tableView, leTotalLinea);
        QWidget::setTabOrder(leTotalLinea, leMargen);

        retranslateUi(pedidos);
        QObject::connect(pushButtonCerrar, &QPushButton::clicked, pedidos, qOverload<>(&QDialog::close));

        QMetaObject::connectSlotsByName(pedidos);
    } // setupUi

    void retranslateUi(QDialog *pedidos)
    {
        pedidos->setWindowTitle(QCoreApplication::translate("pedidos", "Gest\303\263n pedidos", nullptr));
        labelDocumento->setText(QCoreApplication::translate("pedidos", "N\303\272mero documento", nullptr));
        pushButtonAnadir->setText(QCoreApplication::translate("pedidos", "A\303\261adir", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("pedidos", "Modificar", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("pedidos", "Borrar", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("pedidos", "Cerrar", nullptr));
        pushButtonImprimir->setText(QCoreApplication::translate("pedidos", "Imprimir", nullptr));
        label_21->setText(QCoreApplication::translate("pedidos", "Margen", nullptr));
        label_18->setText(QCoreApplication::translate("pedidos", "Desc.", nullptr));
        label_12->setText(QCoreApplication::translate("pedidos", "Lote", nullptr));
        label_16->setText(QCoreApplication::translate("pedidos", "&I.V.A.", nullptr));
        label_15->setText(QCoreApplication::translate("pedidos", "P&VT", nullptr));
        label_11->setText(QCoreApplication::translate("pedidos", "Cod.", nullptr));
        label_17->setText(QCoreApplication::translate("pedidos", "P.V.P.", nullptr));
        labelDescripcion->setText(QCoreApplication::translate("pedidos", "Descripci&\303\263n", nullptr));
        dateEdit->setDisplayFormat(QCoreApplication::translate("pedidos", "yyyy/MM/dd", nullptr));
        label_20->setText(QCoreApplication::translate("pedidos", "Fecha caducidad", nullptr));
        label_13->setText(QCoreApplication::translate("pedidos", "&Uds.", nullptr));
        label_19->setText(QCoreApplication::translate("pedidos", "Tota&l", nullptr));
        label_14->setText(QCoreApplication::translate("pedidos", "Bo&n.", nullptr));
        labelProveedor->setText(QCoreApplication::translate("pedidos", "Proveedor", nullptr));
        label_3->setText(QCoreApplication::translate("pedidos", "Total base", nullptr));
        label_6->setText(QCoreApplication::translate("pedidos", "TOTAL", nullptr));
        label_4->setText(QCoreApplication::translate("pedidos", "Total IVA", nullptr));
        label->setText(QCoreApplication::translate("pedidos", "Total lineas", nullptr));
        label_5->setText(QCoreApplication::translate("pedidos", "Total Recargo", nullptr));
        label_2->setText(QCoreApplication::translate("pedidos", "Total unidades", nullptr));
    } // retranslateUi

};

namespace Ui {
    class pedidos: public Ui_pedidos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PEDIDOS_H
