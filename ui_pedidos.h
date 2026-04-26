/********************************************************************************
** Form generated from reading UI file 'pedidos.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
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
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_pedidos
{
public:
    QVBoxLayout *mainVerticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QVBoxLayout *verticalLayoutTitleHeader;
    QLabel *labelTitleMain;
    QLabel *labelProveedor;
    QSpacerItem *horizontalSpacerHeader;
    QLabel *labelDocumento;
    QFrame *entryFrame;
    QGridLayout *gridLayoutEdit;
    QLabel *label_11;
    QLabel *label_desc;
    QLabel *label_uds;
    QLabel *label_bon;
    QLabel *label_lote;
    QLabel *label_cad;
    QLineEdit *leCod;
    QLineEdit *leDescripcion;
    QLineEdit *leUds;
    QLineEdit *leBon;
    QLineEdit *leLote;
    QDateEdit *dateEdit;
    QLabel *label_pvt;
    QLabel *label_iva;
    QLabel *label_pvp;
    QLabel *label_desc_p;
    QLabel *label_margen;
    QLabel *label_total_l;
    QLineEdit *lePvt;
    QLineEdit *leIva;
    QLineEdit *lePvp;
    QLineEdit *leDescuento;
    QLineEdit *leMargen;
    QLineEdit *leTotalLinea;
    QHBoxLayout *horizontalLayoutButtons;
    QPushButton *pushButtonAnadir;
    QPushButton *pushButtonModificar;
    QPushButton *pushButtonBorrar;
    QSpacerItem *horizontalSpacerButtons;
    QPushButton *pushButtonImprimir;
    QPushButton *pushButtonCerrar;
    QTableView *tableView;
    QFrame *footerFrame;
    QHBoxLayout *horizontalLayoutFooter;
    QTableView *tableViewTotales;
    QSpacerItem *horizontalSpacerFooter;
    QGridLayout *gridLayoutTotals;
    QLabel *label_l;
    QLineEdit *lineEditLineas;
    QLabel *label_u;
    QLineEdit *lineEditUnidades;
    QLabel *label_b;
    QLineEdit *lineEditBase;
    QLabel *label_i;
    QLineEdit *lineEditIVA;
    QLabel *label_r;
    QLineEdit *lineEditRecargo;
    QLabel *label_t;
    QLineEdit *lineEditTotal;

    void setupUi(QDialog *pedidos)
    {
        if (pedidos->objectName().isEmpty())
            pedidos->setObjectName(QString::fromUtf8("pedidos"));
        pedidos->resize(1182, 850);
        pedidos->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }\n"
"QLabel { color: #424242; font-weight: bold; }\n"
"QLineEdit { padding: 5px; border: 1px solid #ced4da; border-radius: 4px; background: white; }\n"
"QLineEdit:focus { border: 1px solid #f57f17; }\n"
"QPushButton { padding: 8px 15px; border-radius: 4px; font-weight: bold; }\n"
"QPushButton#pushButtonAnadir { background-color: #f57f17; color: white; border: none; }\n"
"QPushButton#pushButtonAnadir:hover { background-color: #e65100; }\n"
"QPushButton#pushButtonModificar { background-color: #fb8c00; color: white; border: none; }\n"
"QPushButton#pushButtonBorrar { background-color: #e53935; color: white; border: none; }\n"
"QPushButton#pushButtonCerrar { background-color: #757575; color: white; border: none; }\n"
"QTableView { background-color: white; border: 1px solid #dee2e6; gridline-color: #f1f3f5; }\n"
""));
        mainVerticalLayout = new QVBoxLayout(pedidos);
        mainVerticalLayout->setSpacing(15);
        mainVerticalLayout->setObjectName(QString::fromUtf8("mainVerticalLayout"));
        mainVerticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(pedidos);
        headerFrame->setObjectName(QString::fromUtf8("headerFrame"));
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame {\n"
"    background-color: #fff9c4;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #fff176;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName(QString::fromUtf8("horizontalLayoutHeader"));
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName(QString::fromUtf8("labelIcono"));
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/pedidos.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        verticalLayoutTitleHeader = new QVBoxLayout();
        verticalLayoutTitleHeader->setObjectName(QString::fromUtf8("verticalLayoutTitleHeader"));
        labelTitleMain = new QLabel(headerFrame);
        labelTitleMain->setObjectName(QString::fromUtf8("labelTitleMain"));
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelTitleMain->setFont(font);
        labelTitleMain->setStyleSheet(QString::fromUtf8("color: #f57f17;"));

        verticalLayoutTitleHeader->addWidget(labelTitleMain);

        labelProveedor = new QLabel(headerFrame);
        labelProveedor->setObjectName(QString::fromUtf8("labelProveedor"));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setItalic(true);
        labelProveedor->setFont(font1);
        labelProveedor->setStyleSheet(QString::fromUtf8("color: #616161;"));

        verticalLayoutTitleHeader->addWidget(labelProveedor);


        horizontalLayoutHeader->addLayout(verticalLayoutTitleHeader);

        horizontalSpacerHeader = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);

        labelDocumento = new QLabel(headerFrame);
        labelDocumento->setObjectName(QString::fromUtf8("labelDocumento"));
        QFont font2;
        font2.setPointSize(14);
        font2.setBold(true);
        labelDocumento->setFont(font2);
        labelDocumento->setStyleSheet(QString::fromUtf8("color: #f57f17;"));

        horizontalLayoutHeader->addWidget(labelDocumento);


        mainVerticalLayout->addWidget(headerFrame);

        entryFrame = new QFrame(pedidos);
        entryFrame->setObjectName(QString::fromUtf8("entryFrame"));
        entryFrame->setStyleSheet(QString::fromUtf8("QFrame#entryFrame {\n"
"    background-color: white;\n"
"    border-radius: 8px;\n"
"    border: 1px solid #ced4da;\n"
"    padding: 10px;\n"
"}"));
        gridLayoutEdit = new QGridLayout(entryFrame);
        gridLayoutEdit->setObjectName(QString::fromUtf8("gridLayoutEdit"));
        label_11 = new QLabel(entryFrame);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayoutEdit->addWidget(label_11, 0, 0, 1, 1);

        label_desc = new QLabel(entryFrame);
        label_desc->setObjectName(QString::fromUtf8("label_desc"));

        gridLayoutEdit->addWidget(label_desc, 0, 1, 1, 1);

        label_uds = new QLabel(entryFrame);
        label_uds->setObjectName(QString::fromUtf8("label_uds"));

        gridLayoutEdit->addWidget(label_uds, 0, 2, 1, 1);

        label_bon = new QLabel(entryFrame);
        label_bon->setObjectName(QString::fromUtf8("label_bon"));

        gridLayoutEdit->addWidget(label_bon, 0, 3, 1, 1);

        label_lote = new QLabel(entryFrame);
        label_lote->setObjectName(QString::fromUtf8("label_lote"));

        gridLayoutEdit->addWidget(label_lote, 0, 4, 1, 1);

        label_cad = new QLabel(entryFrame);
        label_cad->setObjectName(QString::fromUtf8("label_cad"));

        gridLayoutEdit->addWidget(label_cad, 0, 5, 1, 1);

        leCod = new QLineEdit(entryFrame);
        leCod->setObjectName(QString::fromUtf8("leCod"));
        leCod->setMaximumSize(QSize(140, 16777215));

        gridLayoutEdit->addWidget(leCod, 1, 0, 1, 1);

        leDescripcion = new QLineEdit(entryFrame);
        leDescripcion->setObjectName(QString::fromUtf8("leDescripcion"));

        gridLayoutEdit->addWidget(leDescripcion, 1, 1, 1, 1);

        leUds = new QLineEdit(entryFrame);
        leUds->setObjectName(QString::fromUtf8("leUds"));
        leUds->setMaximumSize(QSize(60, 16777215));
        leUds->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayoutEdit->addWidget(leUds, 1, 2, 1, 1);

        leBon = new QLineEdit(entryFrame);
        leBon->setObjectName(QString::fromUtf8("leBon"));
        leBon->setMaximumSize(QSize(60, 16777215));
        leBon->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayoutEdit->addWidget(leBon, 1, 3, 1, 1);

        leLote = new QLineEdit(entryFrame);
        leLote->setObjectName(QString::fromUtf8("leLote"));
        leLote->setMaximumSize(QSize(100, 16777215));

        gridLayoutEdit->addWidget(leLote, 1, 4, 1, 1);

        dateEdit = new QDateEdit(entryFrame);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));
        dateEdit->setCalendarPopup(true);

        gridLayoutEdit->addWidget(dateEdit, 1, 5, 1, 1);

        label_pvt = new QLabel(entryFrame);
        label_pvt->setObjectName(QString::fromUtf8("label_pvt"));

        gridLayoutEdit->addWidget(label_pvt, 2, 0, 1, 1);

        label_iva = new QLabel(entryFrame);
        label_iva->setObjectName(QString::fromUtf8("label_iva"));

        gridLayoutEdit->addWidget(label_iva, 2, 1, 1, 1);

        label_pvp = new QLabel(entryFrame);
        label_pvp->setObjectName(QString::fromUtf8("label_pvp"));

        gridLayoutEdit->addWidget(label_pvp, 2, 2, 1, 1);

        label_desc_p = new QLabel(entryFrame);
        label_desc_p->setObjectName(QString::fromUtf8("label_desc_p"));

        gridLayoutEdit->addWidget(label_desc_p, 2, 3, 1, 1);

        label_margen = new QLabel(entryFrame);
        label_margen->setObjectName(QString::fromUtf8("label_margen"));

        gridLayoutEdit->addWidget(label_margen, 2, 4, 1, 1);

        label_total_l = new QLabel(entryFrame);
        label_total_l->setObjectName(QString::fromUtf8("label_total_l"));

        gridLayoutEdit->addWidget(label_total_l, 2, 5, 1, 1);

        lePvt = new QLineEdit(entryFrame);
        lePvt->setObjectName(QString::fromUtf8("lePvt"));
        lePvt->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        gridLayoutEdit->addWidget(lePvt, 3, 0, 1, 1);

        leIva = new QLineEdit(entryFrame);
        leIva->setObjectName(QString::fromUtf8("leIva"));
        leIva->setMaximumSize(QSize(60, 16777215));
        leIva->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayoutEdit->addWidget(leIva, 3, 1, 1, 1);

        lePvp = new QLineEdit(entryFrame);
        lePvp->setObjectName(QString::fromUtf8("lePvp"));
        lePvp->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        gridLayoutEdit->addWidget(lePvp, 3, 2, 1, 1);

        leDescuento = new QLineEdit(entryFrame);
        leDescuento->setObjectName(QString::fromUtf8("leDescuento"));
        leDescuento->setMaximumSize(QSize(60, 16777215));
        leDescuento->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayoutEdit->addWidget(leDescuento, 3, 3, 1, 1);

        leMargen = new QLineEdit(entryFrame);
        leMargen->setObjectName(QString::fromUtf8("leMargen"));
        leMargen->setEnabled(false);
        leMargen->setStyleSheet(QString::fromUtf8("background-color: #f8f9fa;"));
        leMargen->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayoutEdit->addWidget(leMargen, 3, 4, 1, 1);

        leTotalLinea = new QLineEdit(entryFrame);
        leTotalLinea->setObjectName(QString::fromUtf8("leTotalLinea"));
        leTotalLinea->setEnabled(false);
        QFont font3;
        font3.setBold(true);
        leTotalLinea->setFont(font3);
        leTotalLinea->setStyleSheet(QString::fromUtf8("background-color: #f8f9fa; color: #1b5e20;"));
        leTotalLinea->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        gridLayoutEdit->addWidget(leTotalLinea, 3, 5, 1, 1);


        mainVerticalLayout->addWidget(entryFrame);

        horizontalLayoutButtons = new QHBoxLayout();
        horizontalLayoutButtons->setObjectName(QString::fromUtf8("horizontalLayoutButtons"));
        pushButtonAnadir = new QPushButton(pedidos);
        pushButtonAnadir->setObjectName(QString::fromUtf8("pushButtonAnadir"));
        pushButtonAnadir->setMinimumSize(QSize(120, 0));

        horizontalLayoutButtons->addWidget(pushButtonAnadir);

        pushButtonModificar = new QPushButton(pedidos);
        pushButtonModificar->setObjectName(QString::fromUtf8("pushButtonModificar"));
        pushButtonModificar->setMinimumSize(QSize(120, 0));

        horizontalLayoutButtons->addWidget(pushButtonModificar);

        pushButtonBorrar = new QPushButton(pedidos);
        pushButtonBorrar->setObjectName(QString::fromUtf8("pushButtonBorrar"));
        pushButtonBorrar->setMinimumSize(QSize(120, 0));

        horizontalLayoutButtons->addWidget(pushButtonBorrar);

        horizontalSpacerButtons = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutButtons->addItem(horizontalSpacerButtons);

        pushButtonImprimir = new QPushButton(pedidos);
        pushButtonImprimir->setObjectName(QString::fromUtf8("pushButtonImprimir"));

        horizontalLayoutButtons->addWidget(pushButtonImprimir);

        pushButtonCerrar = new QPushButton(pedidos);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));
        pushButtonCerrar->setMinimumSize(QSize(100, 0));

        horizontalLayoutButtons->addWidget(pushButtonCerrar);


        mainVerticalLayout->addLayout(horizontalLayoutButtons);

        tableView = new QTableView(pedidos);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        mainVerticalLayout->addWidget(tableView);

        footerFrame = new QFrame(pedidos);
        footerFrame->setObjectName(QString::fromUtf8("footerFrame"));
        footerFrame->setStyleSheet(QString::fromUtf8("QFrame#footerFrame {\n"
"    background-color: #e9ecef;\n"
"    border-radius: 8px;\n"
"    padding: 10px;\n"
"}"));
        horizontalLayoutFooter = new QHBoxLayout(footerFrame);
        horizontalLayoutFooter->setObjectName(QString::fromUtf8("horizontalLayoutFooter"));
        tableViewTotales = new QTableView(footerFrame);
        tableViewTotales->setObjectName(QString::fromUtf8("tableViewTotales"));
        tableViewTotales->setMaximumSize(QSize(500, 120));

        horizontalLayoutFooter->addWidget(tableViewTotales);

        horizontalSpacerFooter = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutFooter->addItem(horizontalSpacerFooter);

        gridLayoutTotals = new QGridLayout();
        gridLayoutTotals->setObjectName(QString::fromUtf8("gridLayoutTotals"));
        label_l = new QLabel(footerFrame);
        label_l->setObjectName(QString::fromUtf8("label_l"));

        gridLayoutTotals->addWidget(label_l, 0, 0, 1, 1);

        lineEditLineas = new QLineEdit(footerFrame);
        lineEditLineas->setObjectName(QString::fromUtf8("lineEditLineas"));
        lineEditLineas->setEnabled(false);
        lineEditLineas->setMaximumSize(QSize(80, 16777215));
        lineEditLineas->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayoutTotals->addWidget(lineEditLineas, 0, 1, 1, 1);

        label_u = new QLabel(footerFrame);
        label_u->setObjectName(QString::fromUtf8("label_u"));

        gridLayoutTotals->addWidget(label_u, 0, 2, 1, 1);

        lineEditUnidades = new QLineEdit(footerFrame);
        lineEditUnidades->setObjectName(QString::fromUtf8("lineEditUnidades"));
        lineEditUnidades->setEnabled(false);
        lineEditUnidades->setMaximumSize(QSize(80, 16777215));
        lineEditUnidades->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayoutTotals->addWidget(lineEditUnidades, 0, 3, 1, 1);

        label_b = new QLabel(footerFrame);
        label_b->setObjectName(QString::fromUtf8("label_b"));

        gridLayoutTotals->addWidget(label_b, 1, 0, 1, 1);

        lineEditBase = new QLineEdit(footerFrame);
        lineEditBase->setObjectName(QString::fromUtf8("lineEditBase"));
        lineEditBase->setEnabled(false);
        lineEditBase->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        gridLayoutTotals->addWidget(lineEditBase, 1, 1, 1, 1);

        label_i = new QLabel(footerFrame);
        label_i->setObjectName(QString::fromUtf8("label_i"));

        gridLayoutTotals->addWidget(label_i, 2, 0, 1, 1);

        lineEditIVA = new QLineEdit(footerFrame);
        lineEditIVA->setObjectName(QString::fromUtf8("lineEditIVA"));
        lineEditIVA->setEnabled(false);
        lineEditIVA->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        gridLayoutTotals->addWidget(lineEditIVA, 2, 1, 1, 1);

        label_r = new QLabel(footerFrame);
        label_r->setObjectName(QString::fromUtf8("label_r"));

        gridLayoutTotals->addWidget(label_r, 3, 0, 1, 1);

        lineEditRecargo = new QLineEdit(footerFrame);
        lineEditRecargo->setObjectName(QString::fromUtf8("lineEditRecargo"));
        lineEditRecargo->setEnabled(false);
        lineEditRecargo->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        gridLayoutTotals->addWidget(lineEditRecargo, 3, 1, 1, 1);

        label_t = new QLabel(footerFrame);
        label_t->setObjectName(QString::fromUtf8("label_t"));
        label_t->setFont(font2);

        gridLayoutTotals->addWidget(label_t, 4, 0, 1, 1);

        lineEditTotal = new QLineEdit(footerFrame);
        lineEditTotal->setObjectName(QString::fromUtf8("lineEditTotal"));
        lineEditTotal->setEnabled(false);
        lineEditTotal->setFont(font2);
        lineEditTotal->setStyleSheet(QString::fromUtf8("color: #d32f2f; background-color: white;"));
        lineEditTotal->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        gridLayoutTotals->addWidget(lineEditTotal, 4, 1, 1, 1);


        horizontalLayoutFooter->addLayout(gridLayoutTotals);


        mainVerticalLayout->addWidget(footerFrame);


        retranslateUi(pedidos);

        QMetaObject::connectSlotsByName(pedidos);
    } // setupUi

    void retranslateUi(QDialog *pedidos)
    {
        pedidos->setWindowTitle(QCoreApplication::translate("pedidos", "Gesti\303\263n de pedidos", nullptr));
        labelTitleMain->setText(QCoreApplication::translate("pedidos", "Gesti\303\263n de Pedidos", nullptr));
        labelProveedor->setText(QCoreApplication::translate("pedidos", "Proveedor", nullptr));
        labelDocumento->setText(QCoreApplication::translate("pedidos", "N\303\272mero documento", nullptr));
        label_11->setText(QCoreApplication::translate("pedidos", "Cod.", nullptr));
        label_desc->setText(QCoreApplication::translate("pedidos", "Descripci\303\263n", nullptr));
        label_uds->setText(QCoreApplication::translate("pedidos", "Uds.", nullptr));
        label_bon->setText(QCoreApplication::translate("pedidos", "Bon.", nullptr));
        label_lote->setText(QCoreApplication::translate("pedidos", "Lote", nullptr));
        label_cad->setText(QCoreApplication::translate("pedidos", "Caducidad", nullptr));
        dateEdit->setDisplayFormat(QCoreApplication::translate("pedidos", "yyyy-MM-dd", nullptr));
        label_pvt->setText(QCoreApplication::translate("pedidos", "PVT (Costo)", nullptr));
        label_iva->setText(QCoreApplication::translate("pedidos", "IVA %", nullptr));
        label_pvp->setText(QCoreApplication::translate("pedidos", "PVP", nullptr));
        label_desc_p->setText(QCoreApplication::translate("pedidos", "Desc %", nullptr));
        label_margen->setText(QCoreApplication::translate("pedidos", "Margen %", nullptr));
        label_total_l->setText(QCoreApplication::translate("pedidos", "Total L\303\255nea", nullptr));
        pushButtonAnadir->setText(QCoreApplication::translate("pedidos", "A\303\261adir", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("pedidos", "Modificar", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("pedidos", "Borrar", nullptr));
        pushButtonImprimir->setText(QCoreApplication::translate("pedidos", "Imprimir", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("pedidos", "Cerrar", nullptr));
        label_l->setText(QCoreApplication::translate("pedidos", "L\303\255neas:", nullptr));
        label_u->setText(QCoreApplication::translate("pedidos", "Unidades:", nullptr));
        label_b->setText(QCoreApplication::translate("pedidos", "Base:", nullptr));
        label_i->setText(QCoreApplication::translate("pedidos", "IVA:", nullptr));
        label_r->setText(QCoreApplication::translate("pedidos", "Recargo:", nullptr));
        label_t->setText(QCoreApplication::translate("pedidos", "TOTAL:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class pedidos: public Ui_pedidos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PEDIDOS_H
