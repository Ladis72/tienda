/********************************************************************************
** Form generated from reading UI file 'tienda.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIENDA_H
#define UI_TIENDA_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tienda
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QLabel *logo;
    QTabWidget *tab;
    QWidget *tabVentas;
    QVBoxLayout *verticalLayout_Ventas;
    QHBoxLayout *horizontalLayout_VentasButtons;
    QPushButton *ventasButton;
    QPushButton *listadoVentasButton;
    QPushButton *cajasButton;
    QPushButton *movimientosButton;
    QSpacerItem *verticalSpacer_Ventas;
    QWidget *tabFicheros;
    QGridLayout *gridLayout_3;
    QPushButton *pushButtonFormatos;
    QPushButton *pushButtonFamilias;
    QPushButton *pushButtonTiendas;
    QPushButton *pushButtonArticulos;
    QPushButton *pushButtonClientes;
    QPushButton *pushButtonUsuarios;
    QPushButton *pushButtonFabricantes;
    QPushButton *pushButtonFormasPago;
    QPushButton *pushButtonProveedores;
    QPushButton *pushButtonEntradaSalida;
    QSpacerItem *verticalSpacer_tabFicheros;
    QWidget *tabListados;
    QGridLayout *gridLayout_2;
    QPushButton *pushButtonVentaArticulos;
    QPushButton *pushButtonCaducidades;
    QPushButton *pushButtonEtiquetas;
    QPushButton *pushButtonListadoMovimientos;
    QPushButton *pushButtonListadoArqueos;
    QPushButton *pushButtonCaducados_2;
    QSpacerItem *verticalSpacer_tabListados;
    QWidget *TabPedidos;
    QGridLayout *gridLayout_5;
    QPushButton *pushButtonSalidas;
    QPushButton *pushButtonPreparar;
    QPushButton *pushButtonFacturar;
    QPushButton *pushButtonGestionar;
    QPushButton *pushButtonEntradas;
    QSpacerItem *verticalSpacer_TabPedidos;
    QWidget *tabOperaciones;
    QGridLayout *gridLayout_4;
    QPushButton *pushButtonAlbaranes;
    QPushButton *pushButtonFacturas;
    QPushButton *pushButtonTickets;
    QPushButton *pushButtonGenerarVales;
    QSpacerItem *verticalSpacer_tabOperaciones;
    QWidget *tabEstadisticas;
    QGridLayout *gridLayout_Estadisticas;
    QPushButton *pushButtonEstadisticas;
    QSpacerItem *verticalSpacer_tabEstadisticas;
    QWidget *tabConfig;
    QGridLayout *gridLayout_6;
    QPushButton *pushButtonConfiguracion;
    QPushButton *pushButtonTicket;
    QPushButton *pushButtonInformes;
    QPushButton *pushButtonImpuestos;
    QPushButton *pushButtonConfigDB;
    QPushButton *pushButtonUnificarGlobal;
    QSpacerItem *verticalSpacer_tabConfig;
    QWidget *tabSalir;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton_4;
    QPushButton *pushButtonCopia;
    QPushButton *pushButtonConectar;
    QPushButton *pushButtonSesion;
    QSpacerItem *verticalSpacer_tabSalir;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Tienda)
    {
        if (Tienda->objectName().isEmpty())
            Tienda->setObjectName(QString::fromUtf8("Tienda"));
        Tienda->resize(1100, 754);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Tienda->sizePolicy().hasHeightForWidth());
        Tienda->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imagenes/Emeicjac logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        Tienda->setWindowIcon(icon);
        Tienda->setDocumentMode(false);
        centralWidget = new QWidget(Tienda);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        logo = new QLabel(centralWidget);
        logo->setObjectName(QString::fromUtf8("logo"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(logo->sizePolicy().hasHeightForWidth());
        logo->setSizePolicy(sizePolicy1);
        logo->setPixmap(QPixmap(QString::fromUtf8(":/imagenes/Emeicjac logo.jpg")));
        logo->setScaledContents(true);
        logo->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(logo, 1, 0, 1, 1);

        tab = new QTabWidget(centralWidget);
        tab->setObjectName(QString::fromUtf8("tab"));
        tab->setIconSize(QSize(16, 16));
        tabVentas = new QWidget();
        tabVentas->setObjectName(QString::fromUtf8("tabVentas"));
        tabVentas->setAutoFillBackground(false);
        verticalLayout_Ventas = new QVBoxLayout(tabVentas);
        verticalLayout_Ventas->setSpacing(6);
        verticalLayout_Ventas->setContentsMargins(11, 11, 11, 11);
        verticalLayout_Ventas->setObjectName(QString::fromUtf8("verticalLayout_Ventas"));
        horizontalLayout_VentasButtons = new QHBoxLayout();
        horizontalLayout_VentasButtons->setSpacing(6);
        horizontalLayout_VentasButtons->setObjectName(QString::fromUtf8("horizontalLayout_VentasButtons"));
        ventasButton = new QPushButton(tabVentas);
        ventasButton->setObjectName(QString::fromUtf8("ventasButton"));

        horizontalLayout_VentasButtons->addWidget(ventasButton);

        listadoVentasButton = new QPushButton(tabVentas);
        listadoVentasButton->setObjectName(QString::fromUtf8("listadoVentasButton"));

        horizontalLayout_VentasButtons->addWidget(listadoVentasButton);

        cajasButton = new QPushButton(tabVentas);
        cajasButton->setObjectName(QString::fromUtf8("cajasButton"));

        horizontalLayout_VentasButtons->addWidget(cajasButton);

        movimientosButton = new QPushButton(tabVentas);
        movimientosButton->setObjectName(QString::fromUtf8("movimientosButton"));

        horizontalLayout_VentasButtons->addWidget(movimientosButton);


        verticalLayout_Ventas->addLayout(horizontalLayout_VentasButtons);

        verticalSpacer_Ventas = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_Ventas->addItem(verticalSpacer_Ventas);

        tab->addTab(tabVentas, QString());
        tabFicheros = new QWidget();
        tabFicheros->setObjectName(QString::fromUtf8("tabFicheros"));
        gridLayout_3 = new QGridLayout(tabFicheros);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        pushButtonFormatos = new QPushButton(tabFicheros);
        pushButtonFormatos->setObjectName(QString::fromUtf8("pushButtonFormatos"));

        gridLayout_3->addWidget(pushButtonFormatos, 1, 4, 1, 1);

        pushButtonFamilias = new QPushButton(tabFicheros);
        pushButtonFamilias->setObjectName(QString::fromUtf8("pushButtonFamilias"));

        gridLayout_3->addWidget(pushButtonFamilias, 1, 2, 1, 1);

        pushButtonTiendas = new QPushButton(tabFicheros);
        pushButtonTiendas->setObjectName(QString::fromUtf8("pushButtonTiendas"));

        gridLayout_3->addWidget(pushButtonTiendas, 0, 4, 1, 1);

        pushButtonArticulos = new QPushButton(tabFicheros);
        pushButtonArticulos->setObjectName(QString::fromUtf8("pushButtonArticulos"));

        gridLayout_3->addWidget(pushButtonArticulos, 1, 0, 1, 1);

        pushButtonClientes = new QPushButton(tabFicheros);
        pushButtonClientes->setObjectName(QString::fromUtf8("pushButtonClientes"));

        gridLayout_3->addWidget(pushButtonClientes, 1, 1, 1, 1);

        pushButtonUsuarios = new QPushButton(tabFicheros);
        pushButtonUsuarios->setObjectName(QString::fromUtf8("pushButtonUsuarios"));

        gridLayout_3->addWidget(pushButtonUsuarios, 0, 0, 1, 1);

        pushButtonFabricantes = new QPushButton(tabFicheros);
        pushButtonFabricantes->setObjectName(QString::fromUtf8("pushButtonFabricantes"));

        gridLayout_3->addWidget(pushButtonFabricantes, 0, 2, 1, 1);

        pushButtonFormasPago = new QPushButton(tabFicheros);
        pushButtonFormasPago->setObjectName(QString::fromUtf8("pushButtonFormasPago"));

        gridLayout_3->addWidget(pushButtonFormasPago, 0, 3, 1, 1);

        pushButtonProveedores = new QPushButton(tabFicheros);
        pushButtonProveedores->setObjectName(QString::fromUtf8("pushButtonProveedores"));

        gridLayout_3->addWidget(pushButtonProveedores, 0, 1, 1, 1);

        pushButtonEntradaSalida = new QPushButton(tabFicheros);
        pushButtonEntradaSalida->setObjectName(QString::fromUtf8("pushButtonEntradaSalida"));

        gridLayout_3->addWidget(pushButtonEntradaSalida, 0, 5, 1, 1);

        verticalSpacer_tabFicheros = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_tabFicheros, 2, 0, 1, 1);

        tab->addTab(tabFicheros, QString());
        tabListados = new QWidget();
        tabListados->setObjectName(QString::fromUtf8("tabListados"));
        gridLayout_2 = new QGridLayout(tabListados);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        pushButtonVentaArticulos = new QPushButton(tabListados);
        pushButtonVentaArticulos->setObjectName(QString::fromUtf8("pushButtonVentaArticulos"));

        gridLayout_2->addWidget(pushButtonVentaArticulos, 0, 0, 1, 1);

        pushButtonCaducidades = new QPushButton(tabListados);
        pushButtonCaducidades->setObjectName(QString::fromUtf8("pushButtonCaducidades"));

        gridLayout_2->addWidget(pushButtonCaducidades, 0, 1, 1, 1);

        pushButtonEtiquetas = new QPushButton(tabListados);
        pushButtonEtiquetas->setObjectName(QString::fromUtf8("pushButtonEtiquetas"));

        gridLayout_2->addWidget(pushButtonEtiquetas, 0, 2, 1, 1);

        pushButtonListadoMovimientos = new QPushButton(tabListados);
        pushButtonListadoMovimientos->setObjectName(QString::fromUtf8("pushButtonListadoMovimientos"));

        gridLayout_2->addWidget(pushButtonListadoMovimientos, 1, 2, 1, 1);

        pushButtonListadoArqueos = new QPushButton(tabListados);
        pushButtonListadoArqueos->setObjectName(QString::fromUtf8("pushButtonListadoArqueos"));

        gridLayout_2->addWidget(pushButtonListadoArqueos, 1, 0, 1, 1);

        pushButtonCaducados_2 = new QPushButton(tabListados);
        pushButtonCaducados_2->setObjectName(QString::fromUtf8("pushButtonCaducados_2"));

        gridLayout_2->addWidget(pushButtonCaducados_2, 1, 1, 1, 1);

        verticalSpacer_tabListados = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_tabListados, 2, 0, 1, 1);

        tab->addTab(tabListados, QString());
        TabPedidos = new QWidget();
        TabPedidos->setObjectName(QString::fromUtf8("TabPedidos"));
        gridLayout_5 = new QGridLayout(TabPedidos);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        pushButtonSalidas = new QPushButton(TabPedidos);
        pushButtonSalidas->setObjectName(QString::fromUtf8("pushButtonSalidas"));

        gridLayout_5->addWidget(pushButtonSalidas, 0, 2, 1, 1);

        pushButtonPreparar = new QPushButton(TabPedidos);
        pushButtonPreparar->setObjectName(QString::fromUtf8("pushButtonPreparar"));

        gridLayout_5->addWidget(pushButtonPreparar, 0, 5, 1, 1);

        pushButtonFacturar = new QPushButton(TabPedidos);
        pushButtonFacturar->setObjectName(QString::fromUtf8("pushButtonFacturar"));

        gridLayout_5->addWidget(pushButtonFacturar, 0, 4, 1, 1);

        pushButtonGestionar = new QPushButton(TabPedidos);
        pushButtonGestionar->setObjectName(QString::fromUtf8("pushButtonGestionar"));

        gridLayout_5->addWidget(pushButtonGestionar, 0, 0, 1, 1);

        pushButtonEntradas = new QPushButton(TabPedidos);
        pushButtonEntradas->setObjectName(QString::fromUtf8("pushButtonEntradas"));

        gridLayout_5->addWidget(pushButtonEntradas, 0, 1, 1, 1);

        verticalSpacer_TabPedidos = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_TabPedidos, 1, 0, 1, 1);

        tab->addTab(TabPedidos, QString());
        tabOperaciones = new QWidget();
        tabOperaciones->setObjectName(QString::fromUtf8("tabOperaciones"));
        gridLayout_4 = new QGridLayout(tabOperaciones);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        pushButtonAlbaranes = new QPushButton(tabOperaciones);
        pushButtonAlbaranes->setObjectName(QString::fromUtf8("pushButtonAlbaranes"));

        gridLayout_4->addWidget(pushButtonAlbaranes, 0, 1, 1, 1);

        pushButtonFacturas = new QPushButton(tabOperaciones);
        pushButtonFacturas->setObjectName(QString::fromUtf8("pushButtonFacturas"));

        gridLayout_4->addWidget(pushButtonFacturas, 0, 2, 1, 1);

        pushButtonTickets = new QPushButton(tabOperaciones);
        pushButtonTickets->setObjectName(QString::fromUtf8("pushButtonTickets"));

        gridLayout_4->addWidget(pushButtonTickets, 0, 0, 1, 1);

        pushButtonGenerarVales = new QPushButton(tabOperaciones);
        pushButtonGenerarVales->setObjectName(QString::fromUtf8("pushButtonGenerarVales"));

        gridLayout_4->addWidget(pushButtonGenerarVales, 0, 4, 1, 1);

        verticalSpacer_tabOperaciones = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_tabOperaciones, 1, 0, 1, 1);

        tab->addTab(tabOperaciones, QString());
        tabEstadisticas = new QWidget();
        tabEstadisticas->setObjectName(QString::fromUtf8("tabEstadisticas"));
        gridLayout_Estadisticas = new QGridLayout(tabEstadisticas);
        gridLayout_Estadisticas->setSpacing(6);
        gridLayout_Estadisticas->setContentsMargins(11, 11, 11, 11);
        gridLayout_Estadisticas->setObjectName(QString::fromUtf8("gridLayout_Estadisticas"));
        pushButtonEstadisticas = new QPushButton(tabEstadisticas);
        pushButtonEstadisticas->setObjectName(QString::fromUtf8("pushButtonEstadisticas"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButtonEstadisticas->sizePolicy().hasHeightForWidth());
        pushButtonEstadisticas->setSizePolicy(sizePolicy2);
        pushButtonEstadisticas->setMinimumSize(QSize(200, 40));

        gridLayout_Estadisticas->addWidget(pushButtonEstadisticas, 0, 0, 1, 1);

        verticalSpacer_tabEstadisticas = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_Estadisticas->addItem(verticalSpacer_tabEstadisticas, 1, 0, 1, 1);

        tab->addTab(tabEstadisticas, QString());
        tabConfig = new QWidget();
        tabConfig->setObjectName(QString::fromUtf8("tabConfig"));
        gridLayout_6 = new QGridLayout(tabConfig);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        pushButtonConfiguracion = new QPushButton(tabConfig);
        pushButtonConfiguracion->setObjectName(QString::fromUtf8("pushButtonConfiguracion"));

        gridLayout_6->addWidget(pushButtonConfiguracion, 0, 3, 1, 1);

        pushButtonTicket = new QPushButton(tabConfig);
        pushButtonTicket->setObjectName(QString::fromUtf8("pushButtonTicket"));

        gridLayout_6->addWidget(pushButtonTicket, 0, 2, 1, 1);

        pushButtonInformes = new QPushButton(tabConfig);
        pushButtonInformes->setObjectName(QString::fromUtf8("pushButtonInformes"));

        gridLayout_6->addWidget(pushButtonInformes, 0, 0, 1, 1);

        pushButtonImpuestos = new QPushButton(tabConfig);
        pushButtonImpuestos->setObjectName(QString::fromUtf8("pushButtonImpuestos"));

        gridLayout_6->addWidget(pushButtonImpuestos, 0, 1, 1, 1);

        pushButtonConfigDB = new QPushButton(tabConfig);
        pushButtonConfigDB->setObjectName(QString::fromUtf8("pushButtonConfigDB"));

        gridLayout_6->addWidget(pushButtonConfigDB, 0, 5, 1, 1);

        pushButtonUnificarGlobal = new QPushButton(tabConfig);
        pushButtonUnificarGlobal->setObjectName(QString::fromUtf8("pushButtonUnificarGlobal"));
        pushButtonUnificarGlobal->setStyleSheet(QString::fromUtf8("background-color: #ff9800; color: white; font-weight: bold;"));

        gridLayout_6->addWidget(pushButtonUnificarGlobal, 0, 6, 1, 1);

        verticalSpacer_tabConfig = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_tabConfig, 1, 0, 1, 1);

        tab->addTab(tabConfig, QString());
        tabSalir = new QWidget();
        tabSalir->setObjectName(QString::fromUtf8("tabSalir"));
        horizontalLayout_5 = new QHBoxLayout(tabSalir);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        pushButton_4 = new QPushButton(tabSalir);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        horizontalLayout_5->addWidget(pushButton_4);

        pushButtonCopia = new QPushButton(tabSalir);
        pushButtonCopia->setObjectName(QString::fromUtf8("pushButtonCopia"));

        horizontalLayout_5->addWidget(pushButtonCopia);

        pushButtonConectar = new QPushButton(tabSalir);
        pushButtonConectar->setObjectName(QString::fromUtf8("pushButtonConectar"));
        pushButtonConectar->setEnabled(true);

        horizontalLayout_5->addWidget(pushButtonConectar);

        pushButtonSesion = new QPushButton(tabSalir);
        pushButtonSesion->setObjectName(QString::fromUtf8("pushButtonSesion"));

        horizontalLayout_5->addWidget(pushButtonSesion);

        verticalSpacer_tabSalir = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout_5->addItem(verticalSpacer_tabSalir);

        tab->addTab(tabSalir, QString());

        gridLayout->addWidget(tab, 0, 0, 1, 1);

        gridLayout->setRowStretch(1, 1);
        Tienda->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Tienda);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Tienda->setStatusBar(statusBar);

        retranslateUi(Tienda);
        QObject::connect(pushButton_4, SIGNAL(clicked()), Tienda, SLOT(close()));

        tab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Tienda);
    } // setupUi

    void retranslateUi(QMainWindow *Tienda)
    {
        Tienda->setWindowTitle(QCoreApplication::translate("Tienda", "Tienda 1.99", nullptr));
        logo->setText(QString());
        ventasButton->setText(QCoreApplication::translate("Tienda", "Ventas", nullptr));
        listadoVentasButton->setText(QCoreApplication::translate("Tienda", "Cajas", nullptr));
        cajasButton->setText(QCoreApplication::translate("Tienda", "Arqueo", nullptr));
        movimientosButton->setText(QCoreApplication::translate("Tienda", "Movimientos", nullptr));
        tab->setTabText(tab->indexOf(tabVentas), QCoreApplication::translate("Tienda", "Ventas", nullptr));
        pushButtonFormatos->setText(QCoreApplication::translate("Tienda", "Formatos", nullptr));
        pushButtonFamilias->setText(QCoreApplication::translate("Tienda", "Familias", nullptr));
        pushButtonTiendas->setText(QCoreApplication::translate("Tienda", "Tiendas", nullptr));
        pushButtonArticulos->setText(QCoreApplication::translate("Tienda", "Art\303\255culos", nullptr));
        pushButtonClientes->setText(QCoreApplication::translate("Tienda", "Clientes", nullptr));
        pushButtonUsuarios->setText(QCoreApplication::translate("Tienda", "Usuarios", nullptr));
        pushButtonFabricantes->setText(QCoreApplication::translate("Tienda", "Fabricantes", nullptr));
        pushButtonFormasPago->setText(QCoreApplication::translate("Tienda", "Formas de pago", nullptr));
        pushButtonProveedores->setText(QCoreApplication::translate("Tienda", "Proveedores", nullptr));
        pushButtonEntradaSalida->setText(QCoreApplication::translate("Tienda", "Tipos E/S", nullptr));
        tab->setTabText(tab->indexOf(tabFicheros), QCoreApplication::translate("Tienda", "Ficheros", nullptr));
        pushButtonVentaArticulos->setText(QCoreApplication::translate("Tienda", "Venta art\303\255culos", nullptr));
        pushButtonCaducidades->setText(QCoreApplication::translate("Tienda", "Caducidades", nullptr));
        pushButtonEtiquetas->setText(QCoreApplication::translate("Tienda", "Etiquetas", nullptr));
        pushButtonListadoMovimientos->setText(QCoreApplication::translate("Tienda", "Movimientos efectivo", nullptr));
        pushButtonListadoArqueos->setText(QCoreApplication::translate("Tienda", "Arqueos", nullptr));
        pushButtonCaducados_2->setText(QCoreApplication::translate("Tienda", "Caducados", nullptr));
        tab->setTabText(tab->indexOf(tabListados), QCoreApplication::translate("Tienda", "Listados", nullptr));
        pushButtonSalidas->setText(QCoreApplication::translate("Tienda", "Salidas", nullptr));
        pushButtonPreparar->setText(QCoreApplication::translate("Tienda", "Preparar pedidos", nullptr));
        pushButtonFacturar->setText(QCoreApplication::translate("Tienda", "Facturar albaranes", nullptr));
        pushButtonGestionar->setText(QCoreApplication::translate("Tienda", "Gestionar", nullptr));
        pushButtonEntradas->setText(QCoreApplication::translate("Tienda", "Entradas", nullptr));
        tab->setTabText(tab->indexOf(TabPedidos), QCoreApplication::translate("Tienda", "Pedidos", nullptr));
        pushButtonAlbaranes->setText(QCoreApplication::translate("Tienda", "Albaranes", nullptr));
        pushButtonFacturas->setText(QCoreApplication::translate("Tienda", "Facturas", nullptr));
        pushButtonTickets->setText(QCoreApplication::translate("Tienda", "Tickets", nullptr));
        pushButtonGenerarVales->setText(QCoreApplication::translate("Tienda", "Generar vales", nullptr));
        tab->setTabText(tab->indexOf(tabOperaciones), QCoreApplication::translate("Tienda", "Operaciones", nullptr));
        pushButtonEstadisticas->setText(QCoreApplication::translate("Tienda", "Abrir Panel de Estad\303\255sticas", nullptr));
        tab->setTabText(tab->indexOf(tabEstadisticas), QCoreApplication::translate("Tienda", "Estad\303\255sticas", nullptr));
        pushButtonConfiguracion->setText(QCoreApplication::translate("Tienda", "Otras configuraciones", nullptr));
        pushButtonTicket->setText(QCoreApplication::translate("Tienda", "Tickets", nullptr));
        pushButtonInformes->setText(QCoreApplication::translate("Tienda", "Directorios", nullptr));
        pushButtonImpuestos->setText(QCoreApplication::translate("Tienda", "Impuestos", nullptr));
        pushButtonConfigDB->setText(QCoreApplication::translate("Tienda", "Servidor Nube", nullptr));
        pushButtonUnificarGlobal->setText(QCoreApplication::translate("Tienda", "Unificar Proveedores", nullptr));
        tab->setTabText(tab->indexOf(tabConfig), QCoreApplication::translate("Tienda", "Configuraci\303\263n", nullptr));
        pushButton_4->setText(QCoreApplication::translate("Tienda", "Salir", nullptr));
        pushButtonCopia->setText(QCoreApplication::translate("Tienda", "Copia y salir", nullptr));
        pushButtonConectar->setText(QCoreApplication::translate("Tienda", "Conectar", nullptr));
        pushButtonSesion->setText(QCoreApplication::translate("Tienda", "Cerrar sesion", nullptr));
        tab->setTabText(tab->indexOf(tabSalir), QCoreApplication::translate("Tienda", "Salir", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tienda: public Ui_Tienda {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIENDA_H
