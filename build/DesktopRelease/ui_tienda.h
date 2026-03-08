/********************************************************************************
** Form generated from reading UI file 'tienda.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
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
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
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
    QHBoxLayout *horizontalLayout;
    QPushButton *ventasButton;
    QPushButton *listadoVentasButton;
    QPushButton *cajasButton;
    QPushButton *movimientosButton;
    QWidget *tabFicheros;
    QGridLayout *gridLayout_3;
    QPushButton *pushButtonFormatos;
    QPushButton *pushButtonFamilias;
    QPushButton *pushButtonTiendas;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QPushButton *pushButtonUsuarios;
    QPushButton *pushButtonFabricantes;
    QPushButton *pushButtonFormasPago;
    QPushButton *pushButtonPrestamistas;
    QPushButton *pushButtonProveedores;
    QPushButton *pushButtonEntradaSalida;
    QWidget *tabListados;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_2;
    QPushButton *pushButton_5;
    QPushButton *pushButtonEtiquetas;
    QPushButton *pushButtonListadoMovimientos;
    QPushButton *pushButtonListadoArqueos;
    QPushButton *pushButtonCaducados_2;
    QWidget *TabPedidos;
    QGridLayout *gridLayout_5;
    QPushButton *pushButtonSalidas;
    QPushButton *pushButtonPreparar;
    QPushButton *pushButtonPrestamos;
    QPushButton *pushButtonFacturar;
    QPushButton *pushButtonGestionar;
    QPushButton *pushButtonEntradas;
    QWidget *tabOperaciones;
    QGridLayout *gridLayout_4;
    QPushButton *pushButtonCaducados;
    QPushButton *pushButtonAlbaranes;
    QPushButton *pushButtonFacturas;
    QPushButton *pushButtonTickets;
    QPushButton *pushButtonGenerarVales;
    QWidget *tabConfig;
    QGridLayout *gridLayout_6;
    QPushButton *pushButtonConfiguracion;
    QPushButton *pushButtonTicket;
    QPushButton *pushButtonInformes;
    QPushButton *pushButtonActualizarClientes;
    QPushButton *pushButtonImpuestos;
    QWidget *tabSalir;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton_4;
    QPushButton *pushButtonCopia;
    QPushButton *pushButtonConectar;
    QPushButton *pushButtonSesion;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Tienda)
    {
        if (Tienda->objectName().isEmpty())
            Tienda->setObjectName("Tienda");
        Tienda->resize(1100, 754);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Tienda->sizePolicy().hasHeightForWidth());
        Tienda->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imagenes/Emeicjac logo.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Tienda->setWindowIcon(icon);
        Tienda->setDocumentMode(false);
        centralWidget = new QWidget(Tienda);
        centralWidget->setObjectName("centralWidget");
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
        logo = new QLabel(centralWidget);
        logo->setObjectName("logo");
        logo->setPixmap(QPixmap(QString::fromUtf8(":/imagenes/Emeicjac logo.jpg")));
        logo->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(logo, 16, 0, 1, 1);

        tab = new QTabWidget(centralWidget);
        tab->setObjectName("tab");
        tab->setIconSize(QSize(16, 16));
        tabVentas = new QWidget();
        tabVentas->setObjectName("tabVentas");
        tabVentas->setAutoFillBackground(false);
        horizontalLayout = new QHBoxLayout(tabVentas);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName("horizontalLayout");
        ventasButton = new QPushButton(tabVentas);
        ventasButton->setObjectName("ventasButton");

        horizontalLayout->addWidget(ventasButton);

        listadoVentasButton = new QPushButton(tabVentas);
        listadoVentasButton->setObjectName("listadoVentasButton");
        listadoVentasButton->setEnabled(true);

        horizontalLayout->addWidget(listadoVentasButton);

        cajasButton = new QPushButton(tabVentas);
        cajasButton->setObjectName("cajasButton");
        cajasButton->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(cajasButton);

        movimientosButton = new QPushButton(tabVentas);
        movimientosButton->setObjectName("movimientosButton");

        horizontalLayout->addWidget(movimientosButton);

        tab->addTab(tabVentas, QString());
        tabFicheros = new QWidget();
        tabFicheros->setObjectName("tabFicheros");
        gridLayout_3 = new QGridLayout(tabFicheros);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName("gridLayout_3");
        pushButtonFormatos = new QPushButton(tabFicheros);
        pushButtonFormatos->setObjectName("pushButtonFormatos");

        gridLayout_3->addWidget(pushButtonFormatos, 1, 4, 1, 1);

        pushButtonFamilias = new QPushButton(tabFicheros);
        pushButtonFamilias->setObjectName("pushButtonFamilias");

        gridLayout_3->addWidget(pushButtonFamilias, 1, 2, 1, 1);

        pushButtonTiendas = new QPushButton(tabFicheros);
        pushButtonTiendas->setObjectName("pushButtonTiendas");

        gridLayout_3->addWidget(pushButtonTiendas, 0, 4, 1, 1);

        pushButton = new QPushButton(tabFicheros);
        pushButton->setObjectName("pushButton");

        gridLayout_3->addWidget(pushButton, 1, 0, 1, 1);

        pushButton_3 = new QPushButton(tabFicheros);
        pushButton_3->setObjectName("pushButton_3");

        gridLayout_3->addWidget(pushButton_3, 1, 1, 1, 1);

        pushButtonUsuarios = new QPushButton(tabFicheros);
        pushButtonUsuarios->setObjectName("pushButtonUsuarios");

        gridLayout_3->addWidget(pushButtonUsuarios, 0, 0, 1, 1);

        pushButtonFabricantes = new QPushButton(tabFicheros);
        pushButtonFabricantes->setObjectName("pushButtonFabricantes");

        gridLayout_3->addWidget(pushButtonFabricantes, 0, 2, 1, 1);

        pushButtonFormasPago = new QPushButton(tabFicheros);
        pushButtonFormasPago->setObjectName("pushButtonFormasPago");

        gridLayout_3->addWidget(pushButtonFormasPago, 0, 3, 1, 1);

        pushButtonPrestamistas = new QPushButton(tabFicheros);
        pushButtonPrestamistas->setObjectName("pushButtonPrestamistas");

        gridLayout_3->addWidget(pushButtonPrestamistas, 1, 3, 1, 1);

        pushButtonProveedores = new QPushButton(tabFicheros);
        pushButtonProveedores->setObjectName("pushButtonProveedores");

        gridLayout_3->addWidget(pushButtonProveedores, 0, 1, 1, 1);

        pushButtonEntradaSalida = new QPushButton(tabFicheros);
        pushButtonEntradaSalida->setObjectName("pushButtonEntradaSalida");

        gridLayout_3->addWidget(pushButtonEntradaSalida, 0, 5, 1, 1);

        tab->addTab(tabFicheros, QString());
        tabListados = new QWidget();
        tabListados->setObjectName("tabListados");
        gridLayout_2 = new QGridLayout(tabListados);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName("gridLayout_2");
        pushButton_2 = new QPushButton(tabListados);
        pushButton_2->setObjectName("pushButton_2");

        gridLayout_2->addWidget(pushButton_2, 0, 0, 1, 1);

        pushButton_5 = new QPushButton(tabListados);
        pushButton_5->setObjectName("pushButton_5");

        gridLayout_2->addWidget(pushButton_5, 0, 1, 1, 1);

        pushButtonEtiquetas = new QPushButton(tabListados);
        pushButtonEtiquetas->setObjectName("pushButtonEtiquetas");

        gridLayout_2->addWidget(pushButtonEtiquetas, 0, 2, 1, 1);

        pushButtonListadoMovimientos = new QPushButton(tabListados);
        pushButtonListadoMovimientos->setObjectName("pushButtonListadoMovimientos");

        gridLayout_2->addWidget(pushButtonListadoMovimientos, 1, 2, 1, 1);

        pushButtonListadoArqueos = new QPushButton(tabListados);
        pushButtonListadoArqueos->setObjectName("pushButtonListadoArqueos");

        gridLayout_2->addWidget(pushButtonListadoArqueos, 1, 0, 1, 1);

        pushButtonCaducados_2 = new QPushButton(tabListados);
        pushButtonCaducados_2->setObjectName("pushButtonCaducados_2");

        gridLayout_2->addWidget(pushButtonCaducados_2, 1, 1, 1, 1);

        tab->addTab(tabListados, QString());
        TabPedidos = new QWidget();
        TabPedidos->setObjectName("TabPedidos");
        gridLayout_5 = new QGridLayout(TabPedidos);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName("gridLayout_5");
        pushButtonSalidas = new QPushButton(TabPedidos);
        pushButtonSalidas->setObjectName("pushButtonSalidas");

        gridLayout_5->addWidget(pushButtonSalidas, 0, 2, 1, 1);

        pushButtonPreparar = new QPushButton(TabPedidos);
        pushButtonPreparar->setObjectName("pushButtonPreparar");

        gridLayout_5->addWidget(pushButtonPreparar, 0, 5, 1, 1);

        pushButtonPrestamos = new QPushButton(TabPedidos);
        pushButtonPrestamos->setObjectName("pushButtonPrestamos");

        gridLayout_5->addWidget(pushButtonPrestamos, 0, 3, 1, 1);

        pushButtonFacturar = new QPushButton(TabPedidos);
        pushButtonFacturar->setObjectName("pushButtonFacturar");

        gridLayout_5->addWidget(pushButtonFacturar, 0, 4, 1, 1);

        pushButtonGestionar = new QPushButton(TabPedidos);
        pushButtonGestionar->setObjectName("pushButtonGestionar");

        gridLayout_5->addWidget(pushButtonGestionar, 0, 0, 1, 1);

        pushButtonEntradas = new QPushButton(TabPedidos);
        pushButtonEntradas->setObjectName("pushButtonEntradas");

        gridLayout_5->addWidget(pushButtonEntradas, 0, 1, 1, 1);

        tab->addTab(TabPedidos, QString());
        tabOperaciones = new QWidget();
        tabOperaciones->setObjectName("tabOperaciones");
        gridLayout_4 = new QGridLayout(tabOperaciones);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName("gridLayout_4");
        pushButtonCaducados = new QPushButton(tabOperaciones);
        pushButtonCaducados->setObjectName("pushButtonCaducados");

        gridLayout_4->addWidget(pushButtonCaducados, 0, 3, 1, 1);

        pushButtonAlbaranes = new QPushButton(tabOperaciones);
        pushButtonAlbaranes->setObjectName("pushButtonAlbaranes");

        gridLayout_4->addWidget(pushButtonAlbaranes, 0, 1, 1, 1);

        pushButtonFacturas = new QPushButton(tabOperaciones);
        pushButtonFacturas->setObjectName("pushButtonFacturas");

        gridLayout_4->addWidget(pushButtonFacturas, 0, 2, 1, 1);

        pushButtonTickets = new QPushButton(tabOperaciones);
        pushButtonTickets->setObjectName("pushButtonTickets");

        gridLayout_4->addWidget(pushButtonTickets, 0, 0, 1, 1);

        pushButtonGenerarVales = new QPushButton(tabOperaciones);
        pushButtonGenerarVales->setObjectName("pushButtonGenerarVales");

        gridLayout_4->addWidget(pushButtonGenerarVales, 0, 4, 1, 1);

        tab->addTab(tabOperaciones, QString());
        tabConfig = new QWidget();
        tabConfig->setObjectName("tabConfig");
        gridLayout_6 = new QGridLayout(tabConfig);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName("gridLayout_6");
        pushButtonConfiguracion = new QPushButton(tabConfig);
        pushButtonConfiguracion->setObjectName("pushButtonConfiguracion");

        gridLayout_6->addWidget(pushButtonConfiguracion, 0, 3, 1, 1);

        pushButtonTicket = new QPushButton(tabConfig);
        pushButtonTicket->setObjectName("pushButtonTicket");

        gridLayout_6->addWidget(pushButtonTicket, 0, 2, 1, 1);

        pushButtonInformes = new QPushButton(tabConfig);
        pushButtonInformes->setObjectName("pushButtonInformes");

        gridLayout_6->addWidget(pushButtonInformes, 0, 0, 1, 1);

        pushButtonActualizarClientes = new QPushButton(tabConfig);
        pushButtonActualizarClientes->setObjectName("pushButtonActualizarClientes");

        gridLayout_6->addWidget(pushButtonActualizarClientes, 0, 4, 1, 1);

        pushButtonImpuestos = new QPushButton(tabConfig);
        pushButtonImpuestos->setObjectName("pushButtonImpuestos");

        gridLayout_6->addWidget(pushButtonImpuestos, 0, 1, 1, 1);

        tab->addTab(tabConfig, QString());
        tabSalir = new QWidget();
        tabSalir->setObjectName("tabSalir");
        horizontalLayout_5 = new QHBoxLayout(tabSalir);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        pushButton_4 = new QPushButton(tabSalir);
        pushButton_4->setObjectName("pushButton_4");

        horizontalLayout_5->addWidget(pushButton_4);

        pushButtonCopia = new QPushButton(tabSalir);
        pushButtonCopia->setObjectName("pushButtonCopia");

        horizontalLayout_5->addWidget(pushButtonCopia);

        pushButtonConectar = new QPushButton(tabSalir);
        pushButtonConectar->setObjectName("pushButtonConectar");
        pushButtonConectar->setEnabled(true);

        horizontalLayout_5->addWidget(pushButtonConectar);

        pushButtonSesion = new QPushButton(tabSalir);
        pushButtonSesion->setObjectName("pushButtonSesion");

        horizontalLayout_5->addWidget(pushButtonSesion);

        tab->addTab(tabSalir, QString());

        gridLayout->addWidget(tab, 1, 0, 1, 1);

        Tienda->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Tienda);
        statusBar->setObjectName("statusBar");
        Tienda->setStatusBar(statusBar);

        retranslateUi(Tienda);
        QObject::connect(pushButton_4, &QPushButton::clicked, Tienda, qOverload<>(&QMainWindow::close));

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
        pushButton->setText(QCoreApplication::translate("Tienda", "Art\303\255culos", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Tienda", "Clientes", nullptr));
        pushButtonUsuarios->setText(QCoreApplication::translate("Tienda", "Usuarios", nullptr));
        pushButtonFabricantes->setText(QCoreApplication::translate("Tienda", "Fabricantes", nullptr));
        pushButtonFormasPago->setText(QCoreApplication::translate("Tienda", "Formas de pago", nullptr));
        pushButtonPrestamistas->setText(QCoreApplication::translate("Tienda", "Prestamos", nullptr));
        pushButtonProveedores->setText(QCoreApplication::translate("Tienda", "Proveedores", nullptr));
        pushButtonEntradaSalida->setText(QCoreApplication::translate("Tienda", "Tipos E/S", nullptr));
        tab->setTabText(tab->indexOf(tabFicheros), QCoreApplication::translate("Tienda", "Ficheros", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Tienda", "Venta art\303\255culos", nullptr));
        pushButton_5->setText(QCoreApplication::translate("Tienda", "Caducidades", nullptr));
        pushButtonEtiquetas->setText(QCoreApplication::translate("Tienda", "Etiquetas", nullptr));
        pushButtonListadoMovimientos->setText(QCoreApplication::translate("Tienda", "Movimientos efectivo", nullptr));
        pushButtonListadoArqueos->setText(QCoreApplication::translate("Tienda", "Arqueos", nullptr));
        pushButtonCaducados_2->setText(QCoreApplication::translate("Tienda", "Caducados", nullptr));
        tab->setTabText(tab->indexOf(tabListados), QCoreApplication::translate("Tienda", "Listados", nullptr));
        pushButtonSalidas->setText(QCoreApplication::translate("Tienda", "Salidas", nullptr));
        pushButtonPreparar->setText(QCoreApplication::translate("Tienda", "Preparar pedidos", nullptr));
        pushButtonPrestamos->setText(QCoreApplication::translate("Tienda", "Prestamos", nullptr));
        pushButtonFacturar->setText(QCoreApplication::translate("Tienda", "Facturar albaranes", nullptr));
        pushButtonGestionar->setText(QCoreApplication::translate("Tienda", "Gestionar", nullptr));
        pushButtonEntradas->setText(QCoreApplication::translate("Tienda", "Entradas", nullptr));
        tab->setTabText(tab->indexOf(TabPedidos), QCoreApplication::translate("Tienda", "Pedidos", nullptr));
        pushButtonCaducados->setText(QCoreApplication::translate("Tienda", "Caducados", nullptr));
        pushButtonAlbaranes->setText(QCoreApplication::translate("Tienda", "Albaranes", nullptr));
        pushButtonFacturas->setText(QCoreApplication::translate("Tienda", "Facturas", nullptr));
        pushButtonTickets->setText(QCoreApplication::translate("Tienda", "Tickets", nullptr));
        pushButtonGenerarVales->setText(QCoreApplication::translate("Tienda", "Generar vales", nullptr));
        tab->setTabText(tab->indexOf(tabOperaciones), QCoreApplication::translate("Tienda", "Operaciones", nullptr));
        pushButtonConfiguracion->setText(QCoreApplication::translate("Tienda", "Otras configuraciones", nullptr));
        pushButtonTicket->setText(QCoreApplication::translate("Tienda", "Tickets", nullptr));
        pushButtonInformes->setText(QCoreApplication::translate("Tienda", "Directorios", nullptr));
        pushButtonActualizarClientes->setText(QCoreApplication::translate("Tienda", "Actualizar clientes", nullptr));
        pushButtonImpuestos->setText(QCoreApplication::translate("Tienda", "Impuestos", nullptr));
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
