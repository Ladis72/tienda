/********************************************************************************
** Form generated from reading UI file 'estadisticas.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ESTADISTICAS_H
#define UI_ESTADISTICAS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Estadisticas
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBoxFiltros;
    QHBoxLayout *horizontalLayout;
    QLabel *labelTienda;
    QComboBox *comboTienda;
    QLabel *labelDesde;
    QDateEdit *dateDesde;
    QLabel *labelHasta;
    QDateEdit *dateHasta;
    QPushButton *btnActualizar;
    QGroupBox *groupBoxKPIs;
    QGridLayout *gridLayoutKPIs;
    QLabel *labelKpiVentasTitle;
    QLabel *lblVentasTotal;
    QLabel *labelKpiTicketsTitle;
    QLabel *lblNumTickets;
    QLabel *labelKpiMedioTitle;
    QLabel *lblTicketMedio;
    QLabel *labelKpiComprasTitle;
    QLabel *lblTotalCompras;
    QLabel *labelKpiStockTitle;
    QLabel *lblValorStock;
    QLabel *labelKpiClientesTitle;
    QLabel *lblClientesActivos;
    QTabWidget *tabWidgetEstadisticas;
    QWidget *tabGraficos;
    QVBoxLayout *verticalLayout_2;
    QWidget *widgetContenedorGrafico;
    QVBoxLayout *layoutGrafico;
    QWidget *tabProductos;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBoxTopVentas;
    QVBoxLayout *verticalLayout_3;
    QTableView *tablaTopVendidos;
    QGroupBox *groupBoxTopRentables;
    QVBoxLayout *verticalLayout_4;
    QTableView *tablaTopRentables;
    QWidget *tabFamilias;
    QVBoxLayout *verticalLayout_5;
    QTableView *tablaFamilias;
    QWidget *tabClientes;
    QVBoxLayout *verticalLayout_6;
    QTableView *tablaMejoresClientes;
    QWidget *tabVendedores;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBoxVendedores;
    QVBoxLayout *verticalLayout_7;
    QTableView *tablaVendedores;
    QGroupBox *groupBoxFormaPago;
    QVBoxLayout *verticalLayout_8;
    QTableView *tablaFormaPago;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Estadisticas)
    {
        if (Estadisticas->objectName().isEmpty())
            Estadisticas->setObjectName(QString::fromUtf8("Estadisticas"));
        Estadisticas->resize(1024, 768);
        verticalLayout = new QVBoxLayout(Estadisticas);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBoxFiltros = new QGroupBox(Estadisticas);
        groupBoxFiltros->setObjectName(QString::fromUtf8("groupBoxFiltros"));
        horizontalLayout = new QHBoxLayout(groupBoxFiltros);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        labelTienda = new QLabel(groupBoxFiltros);
        labelTienda->setObjectName(QString::fromUtf8("labelTienda"));

        horizontalLayout->addWidget(labelTienda);

        comboTienda = new QComboBox(groupBoxFiltros);
        comboTienda->setObjectName(QString::fromUtf8("comboTienda"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboTienda->sizePolicy().hasHeightForWidth());
        comboTienda->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(comboTienda);

        labelDesde = new QLabel(groupBoxFiltros);
        labelDesde->setObjectName(QString::fromUtf8("labelDesde"));

        horizontalLayout->addWidget(labelDesde);

        dateDesde = new QDateEdit(groupBoxFiltros);
        dateDesde->setObjectName(QString::fromUtf8("dateDesde"));
        dateDesde->setCalendarPopup(true);

        horizontalLayout->addWidget(dateDesde);

        labelHasta = new QLabel(groupBoxFiltros);
        labelHasta->setObjectName(QString::fromUtf8("labelHasta"));

        horizontalLayout->addWidget(labelHasta);

        dateHasta = new QDateEdit(groupBoxFiltros);
        dateHasta->setObjectName(QString::fromUtf8("dateHasta"));
        dateHasta->setCalendarPopup(true);

        horizontalLayout->addWidget(dateHasta);

        btnActualizar = new QPushButton(groupBoxFiltros);
        btnActualizar->setObjectName(QString::fromUtf8("btnActualizar"));
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("view-refresh")));
        btnActualizar->setIcon(icon);

        horizontalLayout->addWidget(btnActualizar);


        verticalLayout->addWidget(groupBoxFiltros);

        groupBoxKPIs = new QGroupBox(Estadisticas);
        groupBoxKPIs->setObjectName(QString::fromUtf8("groupBoxKPIs"));
        gridLayoutKPIs = new QGridLayout(groupBoxKPIs);
        gridLayoutKPIs->setObjectName(QString::fromUtf8("gridLayoutKPIs"));
        labelKpiVentasTitle = new QLabel(groupBoxKPIs);
        labelKpiVentasTitle->setObjectName(QString::fromUtf8("labelKpiVentasTitle"));
        QFont font;
        font.setBold(true);
        labelKpiVentasTitle->setFont(font);

        gridLayoutKPIs->addWidget(labelKpiVentasTitle, 0, 0, 1, 1);

        lblVentasTotal = new QLabel(groupBoxKPIs);
        lblVentasTotal->setObjectName(QString::fromUtf8("lblVentasTotal"));
        QFont font1;
        font1.setPointSize(14);
        font1.setBold(true);
        lblVentasTotal->setFont(font1);
        lblVentasTotal->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayoutKPIs->addWidget(lblVentasTotal, 0, 1, 1, 1);

        labelKpiTicketsTitle = new QLabel(groupBoxKPIs);
        labelKpiTicketsTitle->setObjectName(QString::fromUtf8("labelKpiTicketsTitle"));
        labelKpiTicketsTitle->setFont(font);

        gridLayoutKPIs->addWidget(labelKpiTicketsTitle, 0, 2, 1, 1);

        lblNumTickets = new QLabel(groupBoxKPIs);
        lblNumTickets->setObjectName(QString::fromUtf8("lblNumTickets"));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(true);
        lblNumTickets->setFont(font2);
        lblNumTickets->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayoutKPIs->addWidget(lblNumTickets, 0, 3, 1, 1);

        labelKpiMedioTitle = new QLabel(groupBoxKPIs);
        labelKpiMedioTitle->setObjectName(QString::fromUtf8("labelKpiMedioTitle"));
        labelKpiMedioTitle->setFont(font);

        gridLayoutKPIs->addWidget(labelKpiMedioTitle, 0, 4, 1, 1);

        lblTicketMedio = new QLabel(groupBoxKPIs);
        lblTicketMedio->setObjectName(QString::fromUtf8("lblTicketMedio"));
        lblTicketMedio->setFont(font2);
        lblTicketMedio->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayoutKPIs->addWidget(lblTicketMedio, 0, 5, 1, 1);

        labelKpiComprasTitle = new QLabel(groupBoxKPIs);
        labelKpiComprasTitle->setObjectName(QString::fromUtf8("labelKpiComprasTitle"));
        labelKpiComprasTitle->setFont(font);

        gridLayoutKPIs->addWidget(labelKpiComprasTitle, 1, 0, 1, 1);

        lblTotalCompras = new QLabel(groupBoxKPIs);
        lblTotalCompras->setObjectName(QString::fromUtf8("lblTotalCompras"));
        lblTotalCompras->setFont(font2);
        lblTotalCompras->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayoutKPIs->addWidget(lblTotalCompras, 1, 1, 1, 1);

        labelKpiStockTitle = new QLabel(groupBoxKPIs);
        labelKpiStockTitle->setObjectName(QString::fromUtf8("labelKpiStockTitle"));
        labelKpiStockTitle->setFont(font);

        gridLayoutKPIs->addWidget(labelKpiStockTitle, 1, 2, 1, 1);

        lblValorStock = new QLabel(groupBoxKPIs);
        lblValorStock->setObjectName(QString::fromUtf8("lblValorStock"));
        lblValorStock->setFont(font2);
        lblValorStock->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayoutKPIs->addWidget(lblValorStock, 1, 3, 1, 1);

        labelKpiClientesTitle = new QLabel(groupBoxKPIs);
        labelKpiClientesTitle->setObjectName(QString::fromUtf8("labelKpiClientesTitle"));
        labelKpiClientesTitle->setFont(font);

        gridLayoutKPIs->addWidget(labelKpiClientesTitle, 1, 4, 1, 1);

        lblClientesActivos = new QLabel(groupBoxKPIs);
        lblClientesActivos->setObjectName(QString::fromUtf8("lblClientesActivos"));
        QFont font3;
        font3.setPointSize(12);
        font3.setBold(true);
        font3.setWeight(75);
        lblClientesActivos->setFont(font3);
        lblClientesActivos->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayoutKPIs->addWidget(lblClientesActivos, 1, 5, 1, 1);


        verticalLayout->addWidget(groupBoxKPIs);

        tabWidgetEstadisticas = new QTabWidget(Estadisticas);
        tabWidgetEstadisticas->setObjectName(QString::fromUtf8("tabWidgetEstadisticas"));
        tabGraficos = new QWidget();
        tabGraficos->setObjectName(QString::fromUtf8("tabGraficos"));
        verticalLayout_2 = new QVBoxLayout(tabGraficos);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        widgetContenedorGrafico = new QWidget(tabGraficos);
        widgetContenedorGrafico->setObjectName(QString::fromUtf8("widgetContenedorGrafico"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widgetContenedorGrafico->sizePolicy().hasHeightForWidth());
        widgetContenedorGrafico->setSizePolicy(sizePolicy1);
        layoutGrafico = new QVBoxLayout(widgetContenedorGrafico);
        layoutGrafico->setObjectName(QString::fromUtf8("layoutGrafico"));

        verticalLayout_2->addWidget(widgetContenedorGrafico);

        tabWidgetEstadisticas->addTab(tabGraficos, QString());
        tabProductos = new QWidget();
        tabProductos->setObjectName(QString::fromUtf8("tabProductos"));
        horizontalLayout_2 = new QHBoxLayout(tabProductos);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBoxTopVentas = new QGroupBox(tabProductos);
        groupBoxTopVentas->setObjectName(QString::fromUtf8("groupBoxTopVentas"));
        verticalLayout_3 = new QVBoxLayout(groupBoxTopVentas);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tablaTopVendidos = new QTableView(groupBoxTopVentas);
        tablaTopVendidos->setObjectName(QString::fromUtf8("tablaTopVendidos"));
        tablaTopVendidos->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tablaTopVendidos->setSelectionBehavior(QAbstractItemView::SelectRows);
        tablaTopVendidos->setSortingEnabled(true);

        verticalLayout_3->addWidget(tablaTopVendidos);


        horizontalLayout_2->addWidget(groupBoxTopVentas);

        groupBoxTopRentables = new QGroupBox(tabProductos);
        groupBoxTopRentables->setObjectName(QString::fromUtf8("groupBoxTopRentables"));
        verticalLayout_4 = new QVBoxLayout(groupBoxTopRentables);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        tablaTopRentables = new QTableView(groupBoxTopRentables);
        tablaTopRentables->setObjectName(QString::fromUtf8("tablaTopRentables"));
        tablaTopRentables->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tablaTopRentables->setSelectionBehavior(QAbstractItemView::SelectRows);
        tablaTopRentables->setSortingEnabled(true);

        verticalLayout_4->addWidget(tablaTopRentables);


        horizontalLayout_2->addWidget(groupBoxTopRentables);

        tabWidgetEstadisticas->addTab(tabProductos, QString());
        tabFamilias = new QWidget();
        tabFamilias->setObjectName(QString::fromUtf8("tabFamilias"));
        verticalLayout_5 = new QVBoxLayout(tabFamilias);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        tablaFamilias = new QTableView(tabFamilias);
        tablaFamilias->setObjectName(QString::fromUtf8("tablaFamilias"));
        tablaFamilias->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tablaFamilias->setSelectionBehavior(QAbstractItemView::SelectRows);
        tablaFamilias->setSortingEnabled(true);

        verticalLayout_5->addWidget(tablaFamilias);

        tabWidgetEstadisticas->addTab(tabFamilias, QString());
        tabClientes = new QWidget();
        tabClientes->setObjectName(QString::fromUtf8("tabClientes"));
        verticalLayout_6 = new QVBoxLayout(tabClientes);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        tablaMejoresClientes = new QTableView(tabClientes);
        tablaMejoresClientes->setObjectName(QString::fromUtf8("tablaMejoresClientes"));
        tablaMejoresClientes->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tablaMejoresClientes->setSelectionBehavior(QAbstractItemView::SelectRows);
        tablaMejoresClientes->setSortingEnabled(true);

        verticalLayout_6->addWidget(tablaMejoresClientes);

        tabWidgetEstadisticas->addTab(tabClientes, QString());
        tabVendedores = new QWidget();
        tabVendedores->setObjectName(QString::fromUtf8("tabVendedores"));
        horizontalLayout_3 = new QHBoxLayout(tabVendedores);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        groupBoxVendedores = new QGroupBox(tabVendedores);
        groupBoxVendedores->setObjectName(QString::fromUtf8("groupBoxVendedores"));
        verticalLayout_7 = new QVBoxLayout(groupBoxVendedores);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        tablaVendedores = new QTableView(groupBoxVendedores);
        tablaVendedores->setObjectName(QString::fromUtf8("tablaVendedores"));
        tablaVendedores->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tablaVendedores->setSelectionBehavior(QAbstractItemView::SelectRows);
        tablaVendedores->setSortingEnabled(true);

        verticalLayout_7->addWidget(tablaVendedores);


        horizontalLayout_3->addWidget(groupBoxVendedores);

        groupBoxFormaPago = new QGroupBox(tabVendedores);
        groupBoxFormaPago->setObjectName(QString::fromUtf8("groupBoxFormaPago"));
        verticalLayout_8 = new QVBoxLayout(groupBoxFormaPago);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        tablaFormaPago = new QTableView(groupBoxFormaPago);
        tablaFormaPago->setObjectName(QString::fromUtf8("tablaFormaPago"));
        tablaFormaPago->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tablaFormaPago->setSelectionBehavior(QAbstractItemView::SelectRows);
        tablaFormaPago->setSortingEnabled(true);

        verticalLayout_8->addWidget(tablaFormaPago);


        horizontalLayout_3->addWidget(groupBoxFormaPago);

        tabWidgetEstadisticas->addTab(tabVendedores, QString());

        verticalLayout->addWidget(tabWidgetEstadisticas);

        buttonBox = new QDialogButtonBox(Estadisticas);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(Estadisticas);
        QObject::connect(buttonBox, SIGNAL(accepted()), Estadisticas, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Estadisticas, SLOT(reject()));

        tabWidgetEstadisticas->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Estadisticas);
    } // setupUi

    void retranslateUi(QDialog *Estadisticas)
    {
        Estadisticas->setWindowTitle(QCoreApplication::translate("Estadisticas", "Estad\303\255sticas Avanzadas", nullptr));
        groupBoxFiltros->setTitle(QCoreApplication::translate("Estadisticas", "Filtros de consulta", nullptr));
        labelTienda->setText(QCoreApplication::translate("Estadisticas", "Tienda:", nullptr));
        labelDesde->setText(QCoreApplication::translate("Estadisticas", "Desde:", nullptr));
        labelHasta->setText(QCoreApplication::translate("Estadisticas", "Hasta:", nullptr));
        btnActualizar->setText(QCoreApplication::translate("Estadisticas", "Actualizar", nullptr));
        groupBoxKPIs->setTitle(QCoreApplication::translate("Estadisticas", "Resumen (KPIs)", nullptr));
        labelKpiVentasTitle->setText(QCoreApplication::translate("Estadisticas", "Ventas Totales:", nullptr));
        lblVentasTotal->setText(QCoreApplication::translate("Estadisticas", "0.00 \342\202\254", nullptr));
        lblVentasTotal->setStyleSheet(QCoreApplication::translate("Estadisticas", "color: #2E7D32;", nullptr));
        labelKpiTicketsTitle->setText(QCoreApplication::translate("Estadisticas", "N\302\272 Tickets:", nullptr));
        lblNumTickets->setText(QCoreApplication::translate("Estadisticas", "0", nullptr));
        lblNumTickets->setStyleSheet(QCoreApplication::translate("Estadisticas", "color: #1565C0;", nullptr));
        labelKpiMedioTitle->setText(QCoreApplication::translate("Estadisticas", "Ticket Medio:", nullptr));
        lblTicketMedio->setText(QCoreApplication::translate("Estadisticas", "0.00 \342\202\254", nullptr));
        lblTicketMedio->setStyleSheet(QCoreApplication::translate("Estadisticas", "color: #EF6C00;", nullptr));
        labelKpiComprasTitle->setText(QCoreApplication::translate("Estadisticas", "Total Compras:", nullptr));
        lblTotalCompras->setText(QCoreApplication::translate("Estadisticas", "0.00 \342\202\254", nullptr));
        lblTotalCompras->setStyleSheet(QCoreApplication::translate("Estadisticas", "color: #C62828;", nullptr));
        labelKpiStockTitle->setText(QCoreApplication::translate("Estadisticas", "Stock Valor:", nullptr));
        lblValorStock->setText(QCoreApplication::translate("Estadisticas", "0.00 \342\202\254", nullptr));
        lblValorStock->setStyleSheet(QCoreApplication::translate("Estadisticas", "color: #6A1B9A;", nullptr));
        labelKpiClientesTitle->setText(QCoreApplication::translate("Estadisticas", "Clientes Act.:", nullptr));
        lblClientesActivos->setText(QCoreApplication::translate("Estadisticas", "0", nullptr));
        lblClientesActivos->setStyleSheet(QCoreApplication::translate("Estadisticas", "color: #00838F;", nullptr));
        tabWidgetEstadisticas->setTabText(tabWidgetEstadisticas->indexOf(tabGraficos), QCoreApplication::translate("Estadisticas", "Ventas (Gr\303\241fico)", nullptr));
        groupBoxTopVentas->setTitle(QCoreApplication::translate("Estadisticas", "M\303\241s Vendidos (Cantidad)", nullptr));
        groupBoxTopRentables->setTitle(QCoreApplication::translate("Estadisticas", "M\303\241s Rentables (Importe)", nullptr));
        tabWidgetEstadisticas->setTabText(tabWidgetEstadisticas->indexOf(tabProductos), QCoreApplication::translate("Estadisticas", "Productos", nullptr));
        tabWidgetEstadisticas->setTabText(tabWidgetEstadisticas->indexOf(tabFamilias), QCoreApplication::translate("Estadisticas", "Familias", nullptr));
        tabWidgetEstadisticas->setTabText(tabWidgetEstadisticas->indexOf(tabClientes), QCoreApplication::translate("Estadisticas", "Clientes", nullptr));
        groupBoxVendedores->setTitle(QCoreApplication::translate("Estadisticas", "Ventas por Usuario/Vendedor", nullptr));
        groupBoxFormaPago->setTitle(QCoreApplication::translate("Estadisticas", "Ventas por Forma de Pago", nullptr));
        tabWidgetEstadisticas->setTabText(tabWidgetEstadisticas->indexOf(tabVendedores), QCoreApplication::translate("Estadisticas", "Vendedores / Forma Pago", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Estadisticas: public Ui_Estadisticas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ESTADISTICAS_H
