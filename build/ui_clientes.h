/********************************************************************************
** Form generated from reading UI file 'clientes.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTES_H
#define UI_CLIENTES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
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
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Clientes
{
public:
    QVBoxLayout *mainVerticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *labelNombreCliente;
    QCheckBox *checkBoxTiendasConectadas;
    QSpacerItem *horizontalSpacerHeader;
    QGroupBox *groupBoxFiltros;
    QHBoxLayout *horizontalLayoutFiltros;
    QLabel *label_17;
    QDateEdit *dateEditDesde;
    QLabel *label_18;
    QDateEdit *dateEditHasta;
    QPushButton *pushButtonRefrescar;
    QTabWidget *tabWidgetProductos;
    QWidget *tab_3;
    QWidget *Principal;
    QGridLayout *General;
    QLabel *label;
    QLineEdit *lineEditCod;
    QLabel *label_2;
    QLineEdit *lineEditNIF;
    QSpacerItem *verticalSpacer;
    QLabel *label_3;
    QLineEdit *lineEditNombre;
    QLabel *label_4;
    QLineEdit *lineEditApellidos;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_5;
    QLineEdit *lineEditDireccion;
    QLabel *label_6;
    QLineEdit *lineEditCP;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_7;
    QLineEdit *lineEditLocalidad;
    QLabel *label_8;
    QLineEdit *lineEditProvincia;
    QSpacerItem *verticalSpacer_4;
    QLabel *label_9;
    QLineEdit *lineEditTlfn1;
    QLabel *label_10;
    QLineEdit *lineEditTlfn2;
    QSpacerItem *verticalSpacer_5;
    QLabel *label_11;
    QLineEdit *lineEditMail;
    QSpacerItem *verticalSpacer_6;
    QLabel *label_12;
    QLineEdit *lineEditDescuento;
    QLabel *label_13;
    QDateEdit *dateEdit;
    QLabel *label_14;
    QPlainTextEdit *plainTextEdit;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_Ventas;
    QGroupBox *groupBoxKPIs;
    QHBoxLayout *horizontalLayoutKPI;
    QLabel *labelKpiGasto;
    QLabel *labelKpiTotalGasto;
    QLabel *labelKpiTickets;
    QLabel *labelKpiTotalTickets;
    QLabel *labelKpiUltima;
    QLabel *labelKpiFechaUltima;
    QLabel *labelAgrupacion;
    QComboBox *comboBoxAgrupacion;
    QLabel *labelTipoGrafico;
    QComboBox *comboBoxTipoGrafico;
    QSpacerItem *horizontalSpacerKPI;
    QWidget *widgetGraficoVentas;
    QVBoxLayout *verticalLayoutGrafico;
    QSplitter *splitterVentas;
    QTableView *tableView2;
    QTableView *tableViewDetalleTicket;
    QWidget *Productos;
    QGridLayout *gridLayout_3;
    QRadioButton *radioButtonCantidad;
    QRadioButton *radioButtonFecha;
    QLabel *labelDesde_2;
    QDateEdit *dateEditDesde_2;
    QLabel *labelHasta_2;
    QDateEdit *dateEditHasta_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *labelBuscar;
    QLineEdit *lineEditBuscarProducto;
    QTableView *tableViewProductos;
    QHBoxLayout *horizontalLayoutButtons;
    QPushButton *pushButtonNuevo;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonModificar;
    QPushButton *pushButtonUnificar;
    QPushButton *pushButtonSaneador;
    QSpacerItem *horizontalSpacerButtons;
    QPushButton *pushButtonAnterior;
    QPushButton *pushButtonSiguiente;
    QPushButton *btn_encargos_cliente;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *Clientes)
    {
        if (Clientes->objectName().isEmpty())
            Clientes->setObjectName("Clientes");
        Clientes->resize(1290, 700);
        Clientes->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        mainVerticalLayout = new QVBoxLayout(Clientes);
        mainVerticalLayout->setSpacing(15);
        mainVerticalLayout->setObjectName("mainVerticalLayout");
        mainVerticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(Clientes);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame {\n"
"    background-color: #e8f5e9;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #c8e6c9;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName("horizontalLayoutHeader");
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName("labelIcono");
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/clientes.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelNombreCliente = new QLabel(headerFrame);
        labelNombreCliente->setObjectName("labelNombreCliente");
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelNombreCliente->setFont(font);
        labelNombreCliente->setStyleSheet(QString::fromUtf8("color: #2e7d32;"));

        horizontalLayoutHeader->addWidget(labelNombreCliente);

        checkBoxTiendasConectadas = new QCheckBox(headerFrame);
        checkBoxTiendasConectadas->setObjectName("checkBoxTiendasConectadas");

        horizontalLayoutHeader->addWidget(checkBoxTiendasConectadas);

        horizontalSpacerHeader = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);

        groupBoxFiltros = new QGroupBox(headerFrame);
        groupBoxFiltros->setObjectName("groupBoxFiltros");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBoxFiltros->sizePolicy().hasHeightForWidth());
        groupBoxFiltros->setSizePolicy(sizePolicy);
        horizontalLayoutFiltros = new QHBoxLayout(groupBoxFiltros);
        horizontalLayoutFiltros->setObjectName("horizontalLayoutFiltros");
        label_17 = new QLabel(groupBoxFiltros);
        label_17->setObjectName("label_17");

        horizontalLayoutFiltros->addWidget(label_17);

        dateEditDesde = new QDateEdit(groupBoxFiltros);
        dateEditDesde->setObjectName("dateEditDesde");
        dateEditDesde->setMinimumSize(QSize(110, 35));
        dateEditDesde->setCalendarPopup(true);

        horizontalLayoutFiltros->addWidget(dateEditDesde);

        label_18 = new QLabel(groupBoxFiltros);
        label_18->setObjectName("label_18");

        horizontalLayoutFiltros->addWidget(label_18);

        dateEditHasta = new QDateEdit(groupBoxFiltros);
        dateEditHasta->setObjectName("dateEditHasta");
        dateEditHasta->setMinimumSize(QSize(110, 35));
        dateEditHasta->setCalendarPopup(true);

        horizontalLayoutFiltros->addWidget(dateEditHasta);

        pushButtonRefrescar = new QPushButton(groupBoxFiltros);
        pushButtonRefrescar->setObjectName("pushButtonRefrescar");
        pushButtonRefrescar->setMinimumSize(QSize(0, 35));

        horizontalLayoutFiltros->addWidget(pushButtonRefrescar);


        horizontalLayoutHeader->addWidget(groupBoxFiltros);


        mainVerticalLayout->addWidget(headerFrame);

        tabWidgetProductos = new QTabWidget(Clientes);
        tabWidgetProductos->setObjectName("tabWidgetProductos");
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        Principal = new QWidget(tab_3);
        Principal->setObjectName("Principal");
        Principal->setGeometry(QRect(0, 10, 1037, 449));
        General = new QGridLayout(Principal);
        General->setObjectName("General");
        label = new QLabel(Principal);
        label->setObjectName("label");

        General->addWidget(label, 1, 0, 1, 1);

        lineEditCod = new QLineEdit(Principal);
        lineEditCod->setObjectName("lineEditCod");
        lineEditCod->setEnabled(true);

        General->addWidget(lineEditCod, 1, 1, 1, 1);

        label_2 = new QLabel(Principal);
        label_2->setObjectName("label_2");

        General->addWidget(label_2, 1, 2, 1, 1);

        lineEditNIF = new QLineEdit(Principal);
        lineEditNIF->setObjectName("lineEditNIF");

        General->addWidget(lineEditNIF, 1, 3, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        General->addItem(verticalSpacer, 2, 1, 1, 1);

        label_3 = new QLabel(Principal);
        label_3->setObjectName("label_3");

        General->addWidget(label_3, 3, 0, 1, 1);

        lineEditNombre = new QLineEdit(Principal);
        lineEditNombre->setObjectName("lineEditNombre");

        General->addWidget(lineEditNombre, 3, 1, 1, 1);

        label_4 = new QLabel(Principal);
        label_4->setObjectName("label_4");

        General->addWidget(label_4, 3, 2, 1, 1);

        lineEditApellidos = new QLineEdit(Principal);
        lineEditApellidos->setObjectName("lineEditApellidos");

        General->addWidget(lineEditApellidos, 3, 3, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        General->addItem(verticalSpacer_2, 4, 1, 1, 1);

        label_5 = new QLabel(Principal);
        label_5->setObjectName("label_5");

        General->addWidget(label_5, 5, 0, 1, 1);

        lineEditDireccion = new QLineEdit(Principal);
        lineEditDireccion->setObjectName("lineEditDireccion");

        General->addWidget(lineEditDireccion, 5, 1, 1, 1);

        label_6 = new QLabel(Principal);
        label_6->setObjectName("label_6");

        General->addWidget(label_6, 5, 2, 1, 1);

        lineEditCP = new QLineEdit(Principal);
        lineEditCP->setObjectName("lineEditCP");

        General->addWidget(lineEditCP, 5, 3, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        General->addItem(verticalSpacer_3, 6, 1, 1, 1);

        label_7 = new QLabel(Principal);
        label_7->setObjectName("label_7");

        General->addWidget(label_7, 7, 0, 1, 1);

        lineEditLocalidad = new QLineEdit(Principal);
        lineEditLocalidad->setObjectName("lineEditLocalidad");

        General->addWidget(lineEditLocalidad, 7, 1, 1, 1);

        label_8 = new QLabel(Principal);
        label_8->setObjectName("label_8");

        General->addWidget(label_8, 7, 2, 1, 1);

        lineEditProvincia = new QLineEdit(Principal);
        lineEditProvincia->setObjectName("lineEditProvincia");

        General->addWidget(lineEditProvincia, 7, 3, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        General->addItem(verticalSpacer_4, 8, 1, 1, 1);

        label_9 = new QLabel(Principal);
        label_9->setObjectName("label_9");

        General->addWidget(label_9, 9, 0, 1, 1);

        lineEditTlfn1 = new QLineEdit(Principal);
        lineEditTlfn1->setObjectName("lineEditTlfn1");

        General->addWidget(lineEditTlfn1, 9, 1, 1, 1);

        label_10 = new QLabel(Principal);
        label_10->setObjectName("label_10");

        General->addWidget(label_10, 9, 2, 1, 1);

        lineEditTlfn2 = new QLineEdit(Principal);
        lineEditTlfn2->setObjectName("lineEditTlfn2");

        General->addWidget(lineEditTlfn2, 9, 3, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        General->addItem(verticalSpacer_5, 10, 1, 1, 1);

        label_11 = new QLabel(Principal);
        label_11->setObjectName("label_11");

        General->addWidget(label_11, 11, 0, 1, 1);

        lineEditMail = new QLineEdit(Principal);
        lineEditMail->setObjectName("lineEditMail");

        General->addWidget(lineEditMail, 11, 1, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        General->addItem(verticalSpacer_6, 12, 1, 1, 1);

        label_12 = new QLabel(Principal);
        label_12->setObjectName("label_12");

        General->addWidget(label_12, 13, 0, 1, 1);

        lineEditDescuento = new QLineEdit(Principal);
        lineEditDescuento->setObjectName("lineEditDescuento");

        General->addWidget(lineEditDescuento, 13, 1, 1, 1);

        label_13 = new QLabel(Principal);
        label_13->setObjectName("label_13");

        General->addWidget(label_13, 13, 2, 1, 1);

        dateEdit = new QDateEdit(Principal);
        dateEdit->setObjectName("dateEdit");

        General->addWidget(dateEdit, 13, 3, 1, 1);

        label_14 = new QLabel(Principal);
        label_14->setObjectName("label_14");

        General->addWidget(label_14, 14, 0, 1, 1);

        plainTextEdit = new QPlainTextEdit(Principal);
        plainTextEdit->setObjectName("plainTextEdit");

        General->addWidget(plainTextEdit, 15, 0, 1, 4);

        tabWidgetProductos->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        verticalLayout_Ventas = new QVBoxLayout(tab_4);
        verticalLayout_Ventas->setObjectName("verticalLayout_Ventas");
        groupBoxKPIs = new QGroupBox(tab_4);
        groupBoxKPIs->setObjectName("groupBoxKPIs");
        horizontalLayoutKPI = new QHBoxLayout(groupBoxKPIs);
        horizontalLayoutKPI->setObjectName("horizontalLayoutKPI");
        labelKpiGasto = new QLabel(groupBoxKPIs);
        labelKpiGasto->setObjectName("labelKpiGasto");
        QFont font1;
        font1.setBold(true);
        labelKpiGasto->setFont(font1);

        horizontalLayoutKPI->addWidget(labelKpiGasto);

        labelKpiTotalGasto = new QLabel(groupBoxKPIs);
        labelKpiTotalGasto->setObjectName("labelKpiTotalGasto");

        horizontalLayoutKPI->addWidget(labelKpiTotalGasto);

        labelKpiTickets = new QLabel(groupBoxKPIs);
        labelKpiTickets->setObjectName("labelKpiTickets");
        labelKpiTickets->setFont(font1);

        horizontalLayoutKPI->addWidget(labelKpiTickets);

        labelKpiTotalTickets = new QLabel(groupBoxKPIs);
        labelKpiTotalTickets->setObjectName("labelKpiTotalTickets");

        horizontalLayoutKPI->addWidget(labelKpiTotalTickets);

        labelKpiUltima = new QLabel(groupBoxKPIs);
        labelKpiUltima->setObjectName("labelKpiUltima");
        labelKpiUltima->setFont(font1);

        horizontalLayoutKPI->addWidget(labelKpiUltima);

        labelKpiFechaUltima = new QLabel(groupBoxKPIs);
        labelKpiFechaUltima->setObjectName("labelKpiFechaUltima");

        horizontalLayoutKPI->addWidget(labelKpiFechaUltima);

        labelAgrupacion = new QLabel(groupBoxKPIs);
        labelAgrupacion->setObjectName("labelAgrupacion");

        horizontalLayoutKPI->addWidget(labelAgrupacion);

        comboBoxAgrupacion = new QComboBox(groupBoxKPIs);
        comboBoxAgrupacion->addItem(QString());
        comboBoxAgrupacion->addItem(QString());
        comboBoxAgrupacion->addItem(QString());
        comboBoxAgrupacion->addItem(QString());
        comboBoxAgrupacion->setObjectName("comboBoxAgrupacion");

        horizontalLayoutKPI->addWidget(comboBoxAgrupacion);

        labelTipoGrafico = new QLabel(groupBoxKPIs);
        labelTipoGrafico->setObjectName("labelTipoGrafico");

        horizontalLayoutKPI->addWidget(labelTipoGrafico);

        comboBoxTipoGrafico = new QComboBox(groupBoxKPIs);
        comboBoxTipoGrafico->addItem(QString());
        comboBoxTipoGrafico->addItem(QString());
        comboBoxTipoGrafico->setObjectName("comboBoxTipoGrafico");

        horizontalLayoutKPI->addWidget(comboBoxTipoGrafico);

        horizontalSpacerKPI = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutKPI->addItem(horizontalSpacerKPI);


        verticalLayout_Ventas->addWidget(groupBoxKPIs);

        widgetGraficoVentas = new QWidget(tab_4);
        widgetGraficoVentas->setObjectName("widgetGraficoVentas");
        widgetGraficoVentas->setMinimumSize(QSize(0, 200));
        verticalLayoutGrafico = new QVBoxLayout(widgetGraficoVentas);
        verticalLayoutGrafico->setObjectName("verticalLayoutGrafico");

        verticalLayout_Ventas->addWidget(widgetGraficoVentas);

        splitterVentas = new QSplitter(tab_4);
        splitterVentas->setObjectName("splitterVentas");
        splitterVentas->setOrientation(Qt::Orientation::Horizontal);
        tableView2 = new QTableView(splitterVentas);
        tableView2->setObjectName("tableView2");
        tableView2->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableView2->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        splitterVentas->addWidget(tableView2);
        tableViewDetalleTicket = new QTableView(splitterVentas);
        tableViewDetalleTicket->setObjectName("tableViewDetalleTicket");
        tableViewDetalleTicket->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        splitterVentas->addWidget(tableViewDetalleTicket);

        verticalLayout_Ventas->addWidget(splitterVentas);

        tabWidgetProductos->addTab(tab_4, QString());
        Productos = new QWidget();
        Productos->setObjectName("Productos");
        gridLayout_3 = new QGridLayout(Productos);
        gridLayout_3->setObjectName("gridLayout_3");
        radioButtonCantidad = new QRadioButton(Productos);
        radioButtonCantidad->setObjectName("radioButtonCantidad");

        gridLayout_3->addWidget(radioButtonCantidad, 0, 0, 1, 1);

        radioButtonFecha = new QRadioButton(Productos);
        radioButtonFecha->setObjectName("radioButtonFecha");

        gridLayout_3->addWidget(radioButtonFecha, 0, 1, 1, 1);

        labelDesde_2 = new QLabel(Productos);
        labelDesde_2->setObjectName("labelDesde_2");

        gridLayout_3->addWidget(labelDesde_2, 0, 2, 1, 1);

        dateEditDesde_2 = new QDateEdit(Productos);
        dateEditDesde_2->setObjectName("dateEditDesde_2");
        dateEditDesde_2->setMinimumSize(QSize(110, 0));
        dateEditDesde_2->setCalendarPopup(true);

        gridLayout_3->addWidget(dateEditDesde_2, 0, 3, 1, 1);

        labelHasta_2 = new QLabel(Productos);
        labelHasta_2->setObjectName("labelHasta_2");

        gridLayout_3->addWidget(labelHasta_2, 0, 4, 1, 1);

        dateEditHasta_2 = new QDateEdit(Productos);
        dateEditHasta_2->setObjectName("dateEditHasta_2");
        dateEditHasta_2->setMinimumSize(QSize(110, 0));
        dateEditHasta_2->setCalendarPopup(true);

        gridLayout_3->addWidget(dateEditHasta_2, 0, 5, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 0, 6, 1, 1);

        labelBuscar = new QLabel(Productos);
        labelBuscar->setObjectName("labelBuscar");

        gridLayout_3->addWidget(labelBuscar, 1, 0, 1, 1);

        lineEditBuscarProducto = new QLineEdit(Productos);
        lineEditBuscarProducto->setObjectName("lineEditBuscarProducto");

        gridLayout_3->addWidget(lineEditBuscarProducto, 1, 1, 1, 6);

        tableViewProductos = new QTableView(Productos);
        tableViewProductos->setObjectName("tableViewProductos");
        tableViewProductos->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableViewProductos->setSortingEnabled(true);
        tableViewProductos->setCornerButtonEnabled(false);

        gridLayout_3->addWidget(tableViewProductos, 2, 0, 1, 7);

        tabWidgetProductos->addTab(Productos, QString());

        mainVerticalLayout->addWidget(tabWidgetProductos);

        horizontalLayoutButtons = new QHBoxLayout();
        horizontalLayoutButtons->setObjectName("horizontalLayoutButtons");
        pushButtonNuevo = new QPushButton(Clientes);
        pushButtonNuevo->setObjectName("pushButtonNuevo");
        pushButtonNuevo->setAutoDefault(false);

        horizontalLayoutButtons->addWidget(pushButtonNuevo);

        pushButtonBorrar = new QPushButton(Clientes);
        pushButtonBorrar->setObjectName("pushButtonBorrar");
        pushButtonBorrar->setEnabled(false);
        pushButtonBorrar->setAutoDefault(false);

        horizontalLayoutButtons->addWidget(pushButtonBorrar);

        pushButtonModificar = new QPushButton(Clientes);
        pushButtonModificar->setObjectName("pushButtonModificar");
        pushButtonModificar->setAutoDefault(false);

        horizontalLayoutButtons->addWidget(pushButtonModificar);

        pushButtonUnificar = new QPushButton(Clientes);
        pushButtonUnificar->setObjectName("pushButtonUnificar");
        pushButtonUnificar->setAutoDefault(false);

        horizontalLayoutButtons->addWidget(pushButtonUnificar);

        pushButtonSaneador = new QPushButton(Clientes);
        pushButtonSaneador->setObjectName("pushButtonSaneador");
        pushButtonSaneador->setAutoDefault(false);

        horizontalLayoutButtons->addWidget(pushButtonSaneador);

        horizontalSpacerButtons = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutButtons->addItem(horizontalSpacerButtons);

        pushButtonAnterior = new QPushButton(Clientes);
        pushButtonAnterior->setObjectName("pushButtonAnterior");
        pushButtonAnterior->setAutoDefault(false);

        horizontalLayoutButtons->addWidget(pushButtonAnterior);

        pushButtonSiguiente = new QPushButton(Clientes);
        pushButtonSiguiente->setObjectName("pushButtonSiguiente");
        pushButtonSiguiente->setAutoDefault(false);

        horizontalLayoutButtons->addWidget(pushButtonSiguiente);

        btn_encargos_cliente = new QPushButton(Clientes);
        btn_encargos_cliente->setObjectName("btn_encargos_cliente");
        btn_encargos_cliente->setAutoDefault(false);

        horizontalLayoutButtons->addWidget(btn_encargos_cliente);

        pushButtonCerrar = new QPushButton(Clientes);
        pushButtonCerrar->setObjectName("pushButtonCerrar");
        pushButtonCerrar->setAutoDefault(false);

        horizontalLayoutButtons->addWidget(pushButtonCerrar);


        mainVerticalLayout->addLayout(horizontalLayoutButtons);

        QWidget::setTabOrder(lineEditCod, lineEditNIF);
        QWidget::setTabOrder(lineEditNIF, lineEditNombre);
        QWidget::setTabOrder(lineEditNombre, lineEditApellidos);
        QWidget::setTabOrder(lineEditApellidos, lineEditDireccion);
        QWidget::setTabOrder(lineEditDireccion, lineEditCP);
        QWidget::setTabOrder(lineEditCP, lineEditLocalidad);
        QWidget::setTabOrder(lineEditLocalidad, lineEditProvincia);
        QWidget::setTabOrder(lineEditProvincia, lineEditTlfn1);
        QWidget::setTabOrder(lineEditTlfn1, lineEditTlfn2);
        QWidget::setTabOrder(lineEditTlfn2, lineEditMail);
        QWidget::setTabOrder(lineEditMail, lineEditDescuento);
        QWidget::setTabOrder(lineEditDescuento, dateEdit);

        retranslateUi(Clientes);
        QObject::connect(pushButtonCerrar, &QPushButton::clicked, Clientes, qOverload<>(&QDialog::close));

        tabWidgetProductos->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Clientes);
    } // setupUi

    void retranslateUi(QDialog *Clientes)
    {
        Clientes->setWindowTitle(QCoreApplication::translate("Clientes", "Gesti\303\263n de Clientes", nullptr));
        labelIcono->setText(QString());
        labelNombreCliente->setText(QCoreApplication::translate("Clientes", "Cliente", nullptr));
#if QT_CONFIG(tooltip)
        checkBoxTiendasConectadas->setToolTip(QCoreApplication::translate("Clientes", "Incluir datos de todas las tiendas conectadas en Ventas y Productos", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBoxTiendasConectadas->setText(QCoreApplication::translate("Clientes", "M\303\272ltiples tiendas", nullptr));
        groupBoxFiltros->setTitle(QCoreApplication::translate("Clientes", "Filtro de Fechas Global", nullptr));
        label_17->setText(QCoreApplication::translate("Clientes", "Desde:", nullptr));
        dateEditDesde->setDisplayFormat(QCoreApplication::translate("Clientes", "yyyy-MM-dd", nullptr));
        label_18->setText(QCoreApplication::translate("Clientes", "Hasta:", nullptr));
        dateEditHasta->setDisplayFormat(QCoreApplication::translate("Clientes", "yyyy-MM-dd", nullptr));
        pushButtonRefrescar->setText(QCoreApplication::translate("Clientes", "Refrescar", nullptr));
        label->setText(QCoreApplication::translate("Clientes", "C\303\263digo:", nullptr));
        label_2->setText(QCoreApplication::translate("Clientes", "N.I.F.:", nullptr));
        label_3->setText(QCoreApplication::translate("Clientes", "Nombre:", nullptr));
        label_4->setText(QCoreApplication::translate("Clientes", "Apellidos:", nullptr));
        label_5->setText(QCoreApplication::translate("Clientes", "Direcci\303\263n:", nullptr));
        label_6->setText(QCoreApplication::translate("Clientes", "C.P.:", nullptr));
        label_7->setText(QCoreApplication::translate("Clientes", "Localidad:", nullptr));
        label_8->setText(QCoreApplication::translate("Clientes", "Provincia:", nullptr));
        label_9->setText(QCoreApplication::translate("Clientes", "Tel\303\251fono 1:", nullptr));
        label_10->setText(QCoreApplication::translate("Clientes", "Tel\303\251fono 2:", nullptr));
        label_11->setText(QCoreApplication::translate("Clientes", "E-mail:", nullptr));
        label_12->setText(QCoreApplication::translate("Clientes", "Descuento:", nullptr));
        label_13->setText(QCoreApplication::translate("Clientes", "Fecha alta:", nullptr));
        dateEdit->setDisplayFormat(QCoreApplication::translate("Clientes", "yyyy-MM-dd", nullptr));
        label_14->setText(QCoreApplication::translate("Clientes", "Notas:", nullptr));
        tabWidgetProductos->setTabText(tabWidgetProductos->indexOf(tab_3), QCoreApplication::translate("Clientes", "\360\237\221\244 Datos Generales", nullptr));
        groupBoxKPIs->setTitle(QCoreApplication::translate("Clientes", "Panel de Resumen", nullptr));
        labelKpiGasto->setText(QCoreApplication::translate("Clientes", "Gasto Total:", nullptr));
        labelKpiTotalGasto->setText(QCoreApplication::translate("Clientes", "0.00 \342\202\254", nullptr));
        labelKpiTickets->setText(QCoreApplication::translate("Clientes", "  Tickets:", nullptr));
        labelKpiTotalTickets->setText(QCoreApplication::translate("Clientes", "0", nullptr));
        labelKpiUltima->setText(QCoreApplication::translate("Clientes", "  \303\232ltima Visita:", nullptr));
        labelKpiFechaUltima->setText(QCoreApplication::translate("Clientes", "-", nullptr));
        labelAgrupacion->setText(QCoreApplication::translate("Clientes", "Agrupar por:", nullptr));
        comboBoxAgrupacion->setItemText(0, QCoreApplication::translate("Clientes", "Meses", nullptr));
        comboBoxAgrupacion->setItemText(1, QCoreApplication::translate("Clientes", "A\303\261os", nullptr));
        comboBoxAgrupacion->setItemText(2, QCoreApplication::translate("Clientes", "Semanas", nullptr));
        comboBoxAgrupacion->setItemText(3, QCoreApplication::translate("Clientes", "D\303\255as", nullptr));

        labelTipoGrafico->setText(QCoreApplication::translate("Clientes", "Tipo gr\303\241fico:", nullptr));
        comboBoxTipoGrafico->setItemText(0, QCoreApplication::translate("Clientes", "Agrupado", nullptr));
        comboBoxTipoGrafico->setItemText(1, QCoreApplication::translate("Clientes", "Apilado", nullptr));

        tabWidgetProductos->setTabText(tabWidgetProductos->indexOf(tab_4), QCoreApplication::translate("Clientes", "\360\237\223\212 Historial de Ventas", nullptr));
        radioButtonCantidad->setText(QCoreApplication::translate("Clientes", "Cantidad", nullptr));
        radioButtonFecha->setText(QCoreApplication::translate("Clientes", "Fecha", nullptr));
        labelDesde_2->setText(QCoreApplication::translate("Clientes", "Desde:", nullptr));
        dateEditDesde_2->setDisplayFormat(QCoreApplication::translate("Clientes", "yyyy-MM-dd", nullptr));
        labelHasta_2->setText(QCoreApplication::translate("Clientes", "Hasta:", nullptr));
        dateEditHasta_2->setDisplayFormat(QCoreApplication::translate("Clientes", "yyyy-MM-dd", nullptr));
        labelBuscar->setText(QCoreApplication::translate("Clientes", "Buscar:", nullptr));
        lineEditBuscarProducto->setPlaceholderText(QCoreApplication::translate("Clientes", "Filtrar por c\303\263digo o descripci\303\263n...", nullptr));
        tabWidgetProductos->setTabText(tabWidgetProductos->indexOf(Productos), QCoreApplication::translate("Clientes", "\360\237\223\246 Productos Comprados", nullptr));
        pushButtonNuevo->setText(QCoreApplication::translate("Clientes", "Nuevo", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("Clientes", "Borrar", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("Clientes", "Modificar", nullptr));
        pushButtonUnificar->setText(QCoreApplication::translate("Clientes", "Unificar y Limpiar", nullptr));
        pushButtonSaneador->setText(QCoreApplication::translate("Clientes", "Saneador Global", nullptr));
        pushButtonAnterior->setText(QCoreApplication::translate("Clientes", "Anterior", nullptr));
        pushButtonSiguiente->setText(QCoreApplication::translate("Clientes", "Siguiente", nullptr));
        btn_encargos_cliente->setText(QCoreApplication::translate("Clientes", "Encargos", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("Clientes", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Clientes: public Ui_Clientes {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTES_H
