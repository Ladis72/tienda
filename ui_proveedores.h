/********************************************************************************
** Form generated from reading UI file 'proveedores.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROVEEDORES_H
#define UI_PROVEEDORES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Proveedores
{
public:
    QVBoxLayout *mainVerticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *labelNombre;
    QSpacerItem *horizontalSpacerHeader;
    QGroupBox *groupBoxFiltros;
    QHBoxLayout *horizontalLayoutFiltros;
    QLabel *label_17;
    QDateEdit *dateEditDesde;
    QLabel *label_18;
    QDateEdit *dateEditHasta;
    QPushButton *pushButtonRefrescar;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout;
    QGridLayout *General;
    QLabel *label;
    QLineEdit *lineEditCod;
    QPushButton *pushButton;
    QLabel *label_2;
    QLineEdit *lineEditNombre;
    QLabel *label_16;
    QLineEdit *lineEditNIF;
    QLabel *label_3;
    QLineEdit *lineEditDireccion;
    QLabel *label_4;
    QLineEdit *lineEditLocalidad;
    QLabel *label_5;
    QLineEdit *lineEditProvincia;
    QLabel *label_6;
    QLineEdit *lineEditCP;
    QLabel *label_10;
    QLineEdit *lineEditRepresentante;
    QLabel *label_7;
    QLineEdit *lineEditTelefonoR;
    QLabel *label_11;
    QLineEdit *lineEditMailR;
    QLabel *label_8;
    QLineEdit *lineEditTelefono;
    QLabel *label_9;
    QLineEdit *lineEditMail;
    QLabel *label_12;
    QLineEdit *lineEditDescuento;
    QLabel *label_15;
    QLineEdit *lineEditFormaPago;
    QPushButton *pushButtonFPago;
    QLabel *labelFPago;
    QLabel *label_14;
    QLineEdit *lineEditFechaUltimaCompra;
    QLabel *label_13;
    QPlainTextEdit *plainTextEdit;
    QWidget *Compras;
    QVBoxLayout *verticalLayoutCompras;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButtonComprasFacturas;
    QRadioButton *radioButtonComprasMeses;
    QRadioButton *radioButtonComprasAnos;
    QSpacerItem *horizontalSpacer;
    QTableView *tableViewCompras;
    QWidget *Ventas;
    QVBoxLayout *verticalLayoutVentas;
    QTableView *tableViewVentas;
    QHBoxLayout *horizontalLayoutTotales;
    QSpacerItem *horizontalSpacer_2;
    QLabel *labelTotalVentasA;
    QLabel *labelTotalVentasB;
    QLabel *labelTotalVentasSum;
    QHBoxLayout *horizontalLayoutButtons;
    QPushButton *pushButtonNuevo;
    QPushButton *pushButtonModificar;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonUnificar;
    QPushButton *pushButtonSaneador;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButtonAnterior;
    QPushButton *pushButtonSiguiente;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *Proveedores)
    {
        if (Proveedores->objectName().isEmpty())
            Proveedores->setObjectName(QString::fromUtf8("Proveedores"));
        Proveedores->resize(1200, 750);
        Proveedores->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        mainVerticalLayout = new QVBoxLayout(Proveedores);
        mainVerticalLayout->setSpacing(15);
        mainVerticalLayout->setObjectName(QString::fromUtf8("mainVerticalLayout"));
        mainVerticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(Proveedores);
        headerFrame->setObjectName(QString::fromUtf8("headerFrame"));
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame { \n"
"    background-color: #e8f5e9; \n"
"    border-radius: 10px; \n"
"    border: 1px solid #c8e6c9;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName(QString::fromUtf8("horizontalLayoutHeader"));
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName(QString::fromUtf8("labelIcono"));
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/proveedores.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelNombre = new QLabel(headerFrame);
        labelNombre->setObjectName(QString::fromUtf8("labelNombre"));
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelNombre->setFont(font);
        labelNombre->setStyleSheet(QString::fromUtf8("color: #2e7d32;"));

        horizontalLayoutHeader->addWidget(labelNombre);

        horizontalSpacerHeader = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);

        groupBoxFiltros = new QGroupBox(headerFrame);
        groupBoxFiltros->setObjectName(QString::fromUtf8("groupBoxFiltros"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBoxFiltros->sizePolicy().hasHeightForWidth());
        groupBoxFiltros->setSizePolicy(sizePolicy);
        horizontalLayoutFiltros = new QHBoxLayout(groupBoxFiltros);
        horizontalLayoutFiltros->setObjectName(QString::fromUtf8("horizontalLayoutFiltros"));
        label_17 = new QLabel(groupBoxFiltros);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        horizontalLayoutFiltros->addWidget(label_17);

        dateEditDesde = new QDateEdit(groupBoxFiltros);
        dateEditDesde->setObjectName(QString::fromUtf8("dateEditDesde"));
        dateEditDesde->setMinimumSize(QSize(110, 35));
        dateEditDesde->setCalendarPopup(true);

        horizontalLayoutFiltros->addWidget(dateEditDesde);

        label_18 = new QLabel(groupBoxFiltros);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        horizontalLayoutFiltros->addWidget(label_18);

        dateEditHasta = new QDateEdit(groupBoxFiltros);
        dateEditHasta->setObjectName(QString::fromUtf8("dateEditHasta"));
        dateEditHasta->setMinimumSize(QSize(110, 35));
        dateEditHasta->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        dateEditHasta->setCalendarPopup(true);

        horizontalLayoutFiltros->addWidget(dateEditHasta);

        pushButtonRefrescar = new QPushButton(groupBoxFiltros);
        pushButtonRefrescar->setObjectName(QString::fromUtf8("pushButtonRefrescar"));
        pushButtonRefrescar->setMinimumSize(QSize(0, 35));

        horizontalLayoutFiltros->addWidget(pushButtonRefrescar);


        horizontalLayoutHeader->addWidget(groupBoxFiltros);


        mainVerticalLayout->addWidget(headerFrame);

        tabWidget = new QTabWidget(Proveedores);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout = new QGridLayout(tab);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        General = new QGridLayout();
        General->setObjectName(QString::fromUtf8("General"));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));

        General->addWidget(label, 0, 0, 1, 1);

        lineEditCod = new QLineEdit(tab);
        lineEditCod->setObjectName(QString::fromUtf8("lineEditCod"));
        lineEditCod->setMaximumSize(QSize(100, 16777215));

        General->addWidget(lineEditCod, 0, 1, 1, 1);

        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        General->addWidget(pushButton, 0, 2, 1, 1);

        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        General->addWidget(label_2, 1, 0, 1, 1);

        lineEditNombre = new QLineEdit(tab);
        lineEditNombre->setObjectName(QString::fromUtf8("lineEditNombre"));

        General->addWidget(lineEditNombre, 1, 1, 1, 2);

        label_16 = new QLabel(tab);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        General->addWidget(label_16, 1, 3, 1, 1);

        lineEditNIF = new QLineEdit(tab);
        lineEditNIF->setObjectName(QString::fromUtf8("lineEditNIF"));

        General->addWidget(lineEditNIF, 1, 4, 1, 1);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        General->addWidget(label_3, 2, 0, 1, 1);

        lineEditDireccion = new QLineEdit(tab);
        lineEditDireccion->setObjectName(QString::fromUtf8("lineEditDireccion"));

        General->addWidget(lineEditDireccion, 2, 1, 1, 2);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        General->addWidget(label_4, 2, 3, 1, 1);

        lineEditLocalidad = new QLineEdit(tab);
        lineEditLocalidad->setObjectName(QString::fromUtf8("lineEditLocalidad"));

        General->addWidget(lineEditLocalidad, 2, 4, 1, 1);

        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        General->addWidget(label_5, 3, 0, 1, 1);

        lineEditProvincia = new QLineEdit(tab);
        lineEditProvincia->setObjectName(QString::fromUtf8("lineEditProvincia"));

        General->addWidget(lineEditProvincia, 3, 1, 1, 2);

        label_6 = new QLabel(tab);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        General->addWidget(label_6, 3, 3, 1, 1);

        lineEditCP = new QLineEdit(tab);
        lineEditCP->setObjectName(QString::fromUtf8("lineEditCP"));

        General->addWidget(lineEditCP, 3, 4, 1, 1);

        label_10 = new QLabel(tab);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        General->addWidget(label_10, 4, 0, 1, 1);

        lineEditRepresentante = new QLineEdit(tab);
        lineEditRepresentante->setObjectName(QString::fromUtf8("lineEditRepresentante"));

        General->addWidget(lineEditRepresentante, 4, 1, 1, 2);

        label_7 = new QLabel(tab);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        General->addWidget(label_7, 4, 3, 1, 1);

        lineEditTelefonoR = new QLineEdit(tab);
        lineEditTelefonoR->setObjectName(QString::fromUtf8("lineEditTelefonoR"));

        General->addWidget(lineEditTelefonoR, 4, 4, 1, 1);

        label_11 = new QLabel(tab);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        General->addWidget(label_11, 4, 5, 1, 1);

        lineEditMailR = new QLineEdit(tab);
        lineEditMailR->setObjectName(QString::fromUtf8("lineEditMailR"));

        General->addWidget(lineEditMailR, 4, 6, 1, 1);

        label_8 = new QLabel(tab);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        General->addWidget(label_8, 5, 0, 1, 1);

        lineEditTelefono = new QLineEdit(tab);
        lineEditTelefono->setObjectName(QString::fromUtf8("lineEditTelefono"));

        General->addWidget(lineEditTelefono, 5, 1, 1, 2);

        label_9 = new QLabel(tab);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        General->addWidget(label_9, 5, 3, 1, 1);

        lineEditMail = new QLineEdit(tab);
        lineEditMail->setObjectName(QString::fromUtf8("lineEditMail"));

        General->addWidget(lineEditMail, 5, 4, 1, 3);

        label_12 = new QLabel(tab);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        General->addWidget(label_12, 6, 0, 1, 1);

        lineEditDescuento = new QLineEdit(tab);
        lineEditDescuento->setObjectName(QString::fromUtf8("lineEditDescuento"));

        General->addWidget(lineEditDescuento, 6, 1, 1, 1);

        label_15 = new QLabel(tab);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        General->addWidget(label_15, 6, 2, 1, 1);

        lineEditFormaPago = new QLineEdit(tab);
        lineEditFormaPago->setObjectName(QString::fromUtf8("lineEditFormaPago"));

        General->addWidget(lineEditFormaPago, 6, 3, 1, 1);

        pushButtonFPago = new QPushButton(tab);
        pushButtonFPago->setObjectName(QString::fromUtf8("pushButtonFPago"));
        pushButtonFPago->setMaximumSize(QSize(40, 16777215));

        General->addWidget(pushButtonFPago, 6, 4, 1, 1);

        labelFPago = new QLabel(tab);
        labelFPago->setObjectName(QString::fromUtf8("labelFPago"));
        labelFPago->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #1976d2;"));

        General->addWidget(labelFPago, 6, 5, 1, 2);

        label_14 = new QLabel(tab);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        General->addWidget(label_14, 7, 0, 1, 1);

        lineEditFechaUltimaCompra = new QLineEdit(tab);
        lineEditFechaUltimaCompra->setObjectName(QString::fromUtf8("lineEditFechaUltimaCompra"));
        lineEditFechaUltimaCompra->setEnabled(false);

        General->addWidget(lineEditFechaUltimaCompra, 7, 1, 1, 1);

        label_13 = new QLabel(tab);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        General->addWidget(label_13, 8, 0, 1, 1);

        plainTextEdit = new QPlainTextEdit(tab);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        General->addWidget(plainTextEdit, 9, 0, 1, 7);


        gridLayout->addLayout(General, 0, 0, 1, 2);

        tabWidget->addTab(tab, QString());
        Compras = new QWidget();
        Compras->setObjectName(QString::fromUtf8("Compras"));
        verticalLayoutCompras = new QVBoxLayout(Compras);
        verticalLayoutCompras->setObjectName(QString::fromUtf8("verticalLayoutCompras"));
        groupBox = new QGroupBox(Compras);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        radioButtonComprasFacturas = new QRadioButton(groupBox);
        radioButtonComprasFacturas->setObjectName(QString::fromUtf8("radioButtonComprasFacturas"));

        horizontalLayout_2->addWidget(radioButtonComprasFacturas);

        radioButtonComprasMeses = new QRadioButton(groupBox);
        radioButtonComprasMeses->setObjectName(QString::fromUtf8("radioButtonComprasMeses"));
        radioButtonComprasMeses->setChecked(true);

        horizontalLayout_2->addWidget(radioButtonComprasMeses);

        radioButtonComprasAnos = new QRadioButton(groupBox);
        radioButtonComprasAnos->setObjectName(QString::fromUtf8("radioButtonComprasAnos"));

        horizontalLayout_2->addWidget(radioButtonComprasAnos);

        horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayoutCompras->addWidget(groupBox);

        tableViewCompras = new QTableView(Compras);
        tableViewCompras->setObjectName(QString::fromUtf8("tableViewCompras"));
        tableViewCompras->setAlternatingRowColors(true);
        tableViewCompras->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayoutCompras->addWidget(tableViewCompras);

        tabWidget->addTab(Compras, QString());
        Ventas = new QWidget();
        Ventas->setObjectName(QString::fromUtf8("Ventas"));
        verticalLayoutVentas = new QVBoxLayout(Ventas);
        verticalLayoutVentas->setObjectName(QString::fromUtf8("verticalLayoutVentas"));
        tableViewVentas = new QTableView(Ventas);
        tableViewVentas->setObjectName(QString::fromUtf8("tableViewVentas"));
        tableViewVentas->setAlternatingRowColors(true);
        tableViewVentas->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        verticalLayoutVentas->addWidget(tableViewVentas);

        horizontalLayoutTotales = new QHBoxLayout();
        horizontalLayoutTotales->setObjectName(QString::fromUtf8("horizontalLayoutTotales"));
        horizontalSpacer_2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutTotales->addItem(horizontalSpacer_2);

        labelTotalVentasA = new QLabel(Ventas);
        labelTotalVentasA->setObjectName(QString::fromUtf8("labelTotalVentasA"));
        labelTotalVentasA->setStyleSheet(QString::fromUtf8("font-weight: bold;"));

        horizontalLayoutTotales->addWidget(labelTotalVentasA);

        labelTotalVentasB = new QLabel(Ventas);
        labelTotalVentasB->setObjectName(QString::fromUtf8("labelTotalVentasB"));
        labelTotalVentasB->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #c62828;"));

        horizontalLayoutTotales->addWidget(labelTotalVentasB);

        labelTotalVentasSum = new QLabel(Ventas);
        labelTotalVentasSum->setObjectName(QString::fromUtf8("labelTotalVentasSum"));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        labelTotalVentasSum->setFont(font1);
        labelTotalVentasSum->setStyleSheet(QString::fromUtf8("color: #2e7d32;"));

        horizontalLayoutTotales->addWidget(labelTotalVentasSum);


        verticalLayoutVentas->addLayout(horizontalLayoutTotales);

        tabWidget->addTab(Ventas, QString());

        mainVerticalLayout->addWidget(tabWidget);

        horizontalLayoutButtons = new QHBoxLayout();
        horizontalLayoutButtons->setObjectName(QString::fromUtf8("horizontalLayoutButtons"));
        pushButtonNuevo = new QPushButton(Proveedores);
        pushButtonNuevo->setObjectName(QString::fromUtf8("pushButtonNuevo"));

        horizontalLayoutButtons->addWidget(pushButtonNuevo);

        pushButtonModificar = new QPushButton(Proveedores);
        pushButtonModificar->setObjectName(QString::fromUtf8("pushButtonModificar"));

        horizontalLayoutButtons->addWidget(pushButtonModificar);

        pushButtonBorrar = new QPushButton(Proveedores);
        pushButtonBorrar->setObjectName(QString::fromUtf8("pushButtonBorrar"));
        pushButtonBorrar->setEnabled(false);

        horizontalLayoutButtons->addWidget(pushButtonBorrar);

        pushButtonUnificar = new QPushButton(Proveedores);
        pushButtonUnificar->setObjectName(QString::fromUtf8("pushButtonUnificar"));

        horizontalLayoutButtons->addWidget(pushButtonUnificar);

        pushButtonSaneador = new QPushButton(Proveedores);
        pushButtonSaneador->setObjectName(QString::fromUtf8("pushButtonSaneador"));

        horizontalLayoutButtons->addWidget(pushButtonSaneador);

        horizontalSpacer_3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutButtons->addItem(horizontalSpacer_3);

        pushButtonAnterior = new QPushButton(Proveedores);
        pushButtonAnterior->setObjectName(QString::fromUtf8("pushButtonAnterior"));

        horizontalLayoutButtons->addWidget(pushButtonAnterior);

        pushButtonSiguiente = new QPushButton(Proveedores);
        pushButtonSiguiente->setObjectName(QString::fromUtf8("pushButtonSiguiente"));

        horizontalLayoutButtons->addWidget(pushButtonSiguiente);

        pushButtonCerrar = new QPushButton(Proveedores);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));

        horizontalLayoutButtons->addWidget(pushButtonCerrar);


        mainVerticalLayout->addLayout(horizontalLayoutButtons);


        retranslateUi(Proveedores);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Proveedores);
    } // setupUi

    void retranslateUi(QDialog *Proveedores)
    {
        Proveedores->setWindowTitle(QCoreApplication::translate("Proveedores", "Gesti\303\263n de Proveedores", nullptr));
        labelIcono->setText(QString());
        labelNombre->setText(QCoreApplication::translate("Proveedores", "Proveedor", nullptr));
        groupBoxFiltros->setTitle(QCoreApplication::translate("Proveedores", "Filtro de Fechas Global", nullptr));
        label_17->setText(QCoreApplication::translate("Proveedores", "Desde:", nullptr));
        dateEditDesde->setDisplayFormat(QCoreApplication::translate("Proveedores", "yyyy-MM-dd", nullptr));
        label_18->setText(QCoreApplication::translate("Proveedores", "Hasta:", nullptr));
        dateEditHasta->setDisplayFormat(QCoreApplication::translate("Proveedores", "yyyy-MM-dd", nullptr));
        pushButtonRefrescar->setText(QCoreApplication::translate("Proveedores", "Refrescar", nullptr));
        label->setText(QCoreApplication::translate("Proveedores", "C\303\263digo:", nullptr));
        pushButton->setText(QCoreApplication::translate("Proveedores", "Nuevo c\303\263digo", nullptr));
        label_2->setText(QCoreApplication::translate("Proveedores", "Nombre:", nullptr));
        label_16->setText(QCoreApplication::translate("Proveedores", "N.I.F.:", nullptr));
        label_3->setText(QCoreApplication::translate("Proveedores", "Direcci\303\263n:", nullptr));
        label_4->setText(QCoreApplication::translate("Proveedores", "Localidad:", nullptr));
        label_5->setText(QCoreApplication::translate("Proveedores", "Provincia:", nullptr));
        label_6->setText(QCoreApplication::translate("Proveedores", "C.P.:", nullptr));
        label_10->setText(QCoreApplication::translate("Proveedores", "Representante:", nullptr));
        label_7->setText(QCoreApplication::translate("Proveedores", "Tel. Repr.:", nullptr));
        label_11->setText(QCoreApplication::translate("Proveedores", "E-mail R.:", nullptr));
        label_8->setText(QCoreApplication::translate("Proveedores", "Tel\303\251fono Emp:", nullptr));
        label_9->setText(QCoreApplication::translate("Proveedores", "E-mail Emp:", nullptr));
        label_12->setText(QCoreApplication::translate("Proveedores", "DTO Compras:", nullptr));
        label_15->setText(QCoreApplication::translate("Proveedores", "Forma Pago:", nullptr));
        pushButtonFPago->setText(QCoreApplication::translate("Proveedores", "...", nullptr));
        labelFPago->setText(QCoreApplication::translate("Proveedores", "Nombre Pago", nullptr));
        label_14->setText(QCoreApplication::translate("Proveedores", "\303\232ltima Compra:", nullptr));
        label_13->setText(QCoreApplication::translate("Proveedores", "Notas:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("Proveedores", "\360\237\223\246 Datos Generales", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Proveedores", "Agrupar por:", nullptr));
        radioButtonComprasFacturas->setText(QCoreApplication::translate("Proveedores", "Facturas", nullptr));
        radioButtonComprasMeses->setText(QCoreApplication::translate("Proveedores", "Meses", nullptr));
        radioButtonComprasAnos->setText(QCoreApplication::translate("Proveedores", "A\303\261os", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Compras), QCoreApplication::translate("Proveedores", "\360\237\233\222 Historial Compras", nullptr));
        labelTotalVentasA->setText(QCoreApplication::translate("Proveedores", "Ventas A: 0.00 \342\202\254", nullptr));
        labelTotalVentasB->setText(QCoreApplication::translate("Proveedores", "Ventas B: 0.00 \342\202\254", nullptr));
        labelTotalVentasSum->setText(QCoreApplication::translate("Proveedores", "Total: 0.00 \342\202\254", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Ventas), QCoreApplication::translate("Proveedores", "\360\237\223\210 An\303\241lisis de Ventas", nullptr));
        pushButtonNuevo->setText(QCoreApplication::translate("Proveedores", "Nuevo", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("Proveedores", "Guardar", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("Proveedores", "Borrar", nullptr));
        pushButtonUnificar->setText(QCoreApplication::translate("Proveedores", "Unificar y Limpiar", nullptr));
        pushButtonSaneador->setText(QCoreApplication::translate("Proveedores", "Saneador Global", nullptr));
        pushButtonAnterior->setText(QString());
        pushButtonSiguiente->setText(QString());
        pushButtonCerrar->setText(QCoreApplication::translate("Proveedores", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Proveedores: public Ui_Proveedores {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROVEEDORES_H
