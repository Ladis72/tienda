/********************************************************************************
** Form generated from reading UI file 'articulos.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARTICULOS_H
#define UI_ARTICULOS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Articulos
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *labelNombrePrecio;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QWidget *General;
    QGridLayout *gridLayout;
    QDateEdit *dateEditUltimoPedido;
    QLineEdit *lineEditPvp;
    QLabel *label_12;
    QLabel *labelFoto;
    QLabel *label_11;
    QPushButton *pushButtonBorrarFoto;
    QLabel *label;
    QLineEdit *lineEditCod;
    QPushButton *pushButtonEtiqueta;
    QLabel *label_2;
    QLabel *label_4;
    QLineEdit *lineEditIva;
    QLabel *label_14;
    QLineEdit *lineEditCosto;
    QLabel *label_3;
    QLabel *label_5;
    QLineEdit *lineEditStock;
    QPushButton *pushButtonVer;
    QLabel *label_6;
    QLineEdit *lineEditMinimo;
    QLabel *label_7;
    QLineEdit *lineEditMaximo;
    QLabel *label_8;
    QLineEdit *lineEditPendientes;
    QLabel *label_10;
    QDateEdit *dateEditUltimaVenta;
    QLabel *label_15;
    QLineEdit *lineEditCodFamila;
    QLabel *labelFamilia;
    QLabel *label_16;
    QLineEdit *lineEditCodFabricante;
    QLabel *labelFabricante;
    QPlainTextEdit *plainTextEdit;
    QLineEdit *lineEditDesc;
    QPushButton *pushButtonPonerFoto;
    QPushButton *pushButton_2;
    QPushButton *pushButtonBuscarFamilia;
    QLabel *label_13;
    QLabel *label_9;
    QLineEdit *lineEditFoto;
    QLineEdit *lineEditEncargados;
    QLabel *label_17;
    QComboBox *comboBoxFormato;
    QLineEdit *lineEditCantidad;
    QTreeWidget *treeWidgetStockTiendas;
    QCheckBox *checkBoxRemoto;
    QWidget *tab_3;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QTableView *tableViewAux;
    QPushButton *pushButtonAnadir;
    QPushButton *pushButtonEliminar;
    QLineEdit *lineEditAux;
    QWidget *tabVentas;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButtonVentasDia;
    QRadioButton *radioButtonVentasMes;
    QRadioButton *radioButtonVentasAno;
    QTableView *tableViewVentas;
    QWidget *tab_2;
    QGridLayout *gridLayout_4;
    QTableView *tableViewCompras;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioButtonFacturas;
    QRadioButton *radioButtonMeses;
    QRadioButton *radioButtonAnos;
    QPushButton *pushButtonVerFactura;
    QLabel *labelProveedor;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonNuevo;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonModificar;
    QPushButton *pushButtonRefrescar;
    QPushButton *pushButtonAnterior;
    QPushButton *pushButtonSiguiente;
    QPushButton *pushButtonCaducados;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *Articulos)
    {
        if (Articulos->objectName().isEmpty())
            Articulos->setObjectName(QString::fromUtf8("Articulos"));
        Articulos->setWindowModality(Qt::WindowModality::WindowModal);
        Articulos->setEnabled(true);
        Articulos->resize(1039, 578);
        verticalLayout = new QVBoxLayout(Articulos);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelNombrePrecio = new QLabel(Articulos);
        labelNombrePrecio->setObjectName(QString::fromUtf8("labelNombrePrecio"));
        QFont font;
        font.setFamily(QString::fromUtf8("Utopia"));
        font.setPointSize(16);
        font.setBold(true);
        font.setItalic(true);
        labelNombrePrecio->setFont(font);
        labelNombrePrecio->setStyleSheet(QString::fromUtf8("\n"
"color: rgb(255, 25, 5);"));

        verticalLayout->addWidget(labelNombrePrecio);

        tabWidget = new QTabWidget(Articulos);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::TabPosition::North);
        tabWidget->setTabShape(QTabWidget::TabShape::Rounded);
        tabWidget->setElideMode(Qt::TextElideMode::ElideMiddle);
        tabWidget->setMovable(false);
        tabWidget->setTabBarAutoHide(false);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        General = new QWidget(tab);
        General->setObjectName(QString::fromUtf8("General"));
        gridLayout = new QGridLayout(General);
        gridLayout->setSpacing(4);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        gridLayout->setContentsMargins(4, 4, 4, 4);
        dateEditUltimoPedido = new QDateEdit(General);
        dateEditUltimoPedido->setObjectName(QString::fromUtf8("dateEditUltimoPedido"));
        dateEditUltimoPedido->setEnabled(false);
        dateEditUltimoPedido->setFocusPolicy(Qt::FocusPolicy::NoFocus);

        gridLayout->addWidget(dateEditUltimoPedido, 5, 3, 1, 1);

        lineEditPvp = new QLineEdit(General);
        lineEditPvp->setObjectName(QString::fromUtf8("lineEditPvp"));

        gridLayout->addWidget(lineEditPvp, 2, 6, 1, 1);

        label_12 = new QLabel(General);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout->addWidget(label_12, 7, 4, 1, 1);

        labelFoto = new QLabel(General);
        labelFoto->setObjectName(QString::fromUtf8("labelFoto"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelFoto->sizePolicy().hasHeightForWidth());
        labelFoto->setSizePolicy(sizePolicy);
        labelFoto->setScaledContents(false);
        labelFoto->setOpenExternalLinks(true);

        gridLayout->addWidget(labelFoto, 4, 5, 3, 1);

        label_11 = new QLabel(General);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 5, 2, 1, 1);

        pushButtonBorrarFoto = new QPushButton(General);
        pushButtonBorrarFoto->setObjectName(QString::fromUtf8("pushButtonBorrarFoto"));
        pushButtonBorrarFoto->setAutoDefault(false);

        gridLayout->addWidget(pushButtonBorrarFoto, 7, 7, 1, 1);

        label = new QLabel(General);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEditCod = new QLineEdit(General);
        lineEditCod->setObjectName(QString::fromUtf8("lineEditCod"));
        lineEditCod->setMaxLength(13);

        gridLayout->addWidget(lineEditCod, 0, 1, 1, 1);

        pushButtonEtiqueta = new QPushButton(General);
        pushButtonEtiqueta->setObjectName(QString::fromUtf8("pushButtonEtiqueta"));
        pushButtonEtiqueta->setAutoDefault(false);

        gridLayout->addWidget(pushButtonEtiqueta, 0, 2, 1, 1);

        label_2 = new QLabel(General);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_4 = new QLabel(General);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        lineEditIva = new QLineEdit(General);
        lineEditIva->setObjectName(QString::fromUtf8("lineEditIva"));

        gridLayout->addWidget(lineEditIva, 2, 1, 1, 1);

        label_14 = new QLabel(General);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout->addWidget(label_14, 2, 3, 1, 1);

        lineEditCosto = new QLineEdit(General);
        lineEditCosto->setObjectName(QString::fromUtf8("lineEditCosto"));
        lineEditCosto->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        lineEditCosto->setMaxLength(32767);
        lineEditCosto->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        gridLayout->addWidget(lineEditCosto, 2, 4, 1, 1);

        label_3 = new QLabel(General);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 5, 1, 1);

        label_5 = new QLabel(General);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        lineEditStock = new QLineEdit(General);
        lineEditStock->setObjectName(QString::fromUtf8("lineEditStock"));
        lineEditStock->setEnabled(false);
        lineEditStock->setFocusPolicy(Qt::FocusPolicy::NoFocus);

        gridLayout->addWidget(lineEditStock, 3, 1, 1, 1);

        pushButtonVer = new QPushButton(General);
        pushButtonVer->setObjectName(QString::fromUtf8("pushButtonVer"));
        pushButtonVer->setAutoDefault(false);

        gridLayout->addWidget(pushButtonVer, 3, 2, 1, 1);

        label_6 = new QLabel(General);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 3, 3, 1, 1);

        lineEditMinimo = new QLineEdit(General);
        lineEditMinimo->setObjectName(QString::fromUtf8("lineEditMinimo"));

        gridLayout->addWidget(lineEditMinimo, 3, 4, 1, 1);

        label_7 = new QLabel(General);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 3, 5, 1, 1);

        lineEditMaximo = new QLineEdit(General);
        lineEditMaximo->setObjectName(QString::fromUtf8("lineEditMaximo"));

        gridLayout->addWidget(lineEditMaximo, 3, 6, 1, 1);

        label_8 = new QLabel(General);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 4, 0, 1, 1);

        lineEditPendientes = new QLineEdit(General);
        lineEditPendientes->setObjectName(QString::fromUtf8("lineEditPendientes"));
        lineEditPendientes->setEnabled(false);

        gridLayout->addWidget(lineEditPendientes, 4, 1, 1, 1);

        label_10 = new QLabel(General);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 5, 0, 1, 1);

        dateEditUltimaVenta = new QDateEdit(General);
        dateEditUltimaVenta->setObjectName(QString::fromUtf8("dateEditUltimaVenta"));
        dateEditUltimaVenta->setEnabled(false);
        dateEditUltimaVenta->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        dateEditUltimaVenta->setCorrectionMode(QAbstractSpinBox::CorrectionMode::CorrectToNearestValue);

        gridLayout->addWidget(dateEditUltimaVenta, 5, 1, 1, 1);

        label_15 = new QLabel(General);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout->addWidget(label_15, 6, 0, 1, 1);

        lineEditCodFamila = new QLineEdit(General);
        lineEditCodFamila->setObjectName(QString::fromUtf8("lineEditCodFamila"));
        lineEditCodFamila->setMaximumSize(QSize(40, 16777215));

        gridLayout->addWidget(lineEditCodFamila, 6, 1, 1, 1);

        labelFamilia = new QLabel(General);
        labelFamilia->setObjectName(QString::fromUtf8("labelFamilia"));

        gridLayout->addWidget(labelFamilia, 6, 2, 1, 1);

        label_16 = new QLabel(General);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout->addWidget(label_16, 7, 0, 1, 1);

        lineEditCodFabricante = new QLineEdit(General);
        lineEditCodFabricante->setObjectName(QString::fromUtf8("lineEditCodFabricante"));
        lineEditCodFabricante->setMaximumSize(QSize(40, 16777215));

        gridLayout->addWidget(lineEditCodFabricante, 7, 1, 1, 1);

        labelFabricante = new QLabel(General);
        labelFabricante->setObjectName(QString::fromUtf8("labelFabricante"));

        gridLayout->addWidget(labelFabricante, 7, 2, 1, 1);

        plainTextEdit = new QPlainTextEdit(General);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        gridLayout->addWidget(plainTextEdit, 9, 0, 1, 10);

        lineEditDesc = new QLineEdit(General);
        lineEditDesc->setObjectName(QString::fromUtf8("lineEditDesc"));

        gridLayout->addWidget(lineEditDesc, 1, 1, 1, 6);

        pushButtonPonerFoto = new QPushButton(General);
        pushButtonPonerFoto->setObjectName(QString::fromUtf8("pushButtonPonerFoto"));
        pushButtonPonerFoto->setAutoDefault(false);

        gridLayout->addWidget(pushButtonPonerFoto, 7, 6, 1, 1);

        pushButton_2 = new QPushButton(General);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMaximumSize(QSize(40, 16777215));
        pushButton_2->setAutoDefault(false);

        gridLayout->addWidget(pushButton_2, 7, 3, 1, 1);

        pushButtonBuscarFamilia = new QPushButton(General);
        pushButtonBuscarFamilia->setObjectName(QString::fromUtf8("pushButtonBuscarFamilia"));
        pushButtonBuscarFamilia->setMaximumSize(QSize(40, 16777215));
        pushButtonBuscarFamilia->setAutoDefault(false);

        gridLayout->addWidget(pushButtonBuscarFamilia, 6, 3, 1, 1);

        label_13 = new QLabel(General);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout->addWidget(label_13, 8, 0, 1, 1);

        label_9 = new QLabel(General);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 4, 2, 1, 1);

        lineEditFoto = new QLineEdit(General);
        lineEditFoto->setObjectName(QString::fromUtf8("lineEditFoto"));
        lineEditFoto->setEnabled(false);
        lineEditFoto->setMinimumSize(QSize(0, 11));
        lineEditFoto->setFocusPolicy(Qt::FocusPolicy::NoFocus);

        gridLayout->addWidget(lineEditFoto, 7, 5, 1, 1);

        lineEditEncargados = new QLineEdit(General);
        lineEditEncargados->setObjectName(QString::fromUtf8("lineEditEncargados"));
        lineEditEncargados->setEnabled(false);

        gridLayout->addWidget(lineEditEncargados, 4, 3, 1, 1);

        label_17 = new QLabel(General);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout->addWidget(label_17, 8, 2, 1, 1);

        comboBoxFormato = new QComboBox(General);
        comboBoxFormato->setObjectName(QString::fromUtf8("comboBoxFormato"));

        gridLayout->addWidget(comboBoxFormato, 8, 1, 1, 1);

        lineEditCantidad = new QLineEdit(General);
        lineEditCantidad->setObjectName(QString::fromUtf8("lineEditCantidad"));

        gridLayout->addWidget(lineEditCantidad, 8, 3, 1, 1);

        treeWidgetStockTiendas = new QTreeWidget(General);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidgetStockTiendas->setHeaderItem(__qtreewidgetitem);
        treeWidgetStockTiendas->setObjectName(QString::fromUtf8("treeWidgetStockTiendas"));
        treeWidgetStockTiendas->setContextMenuPolicy(Qt::ContextMenuPolicy::DefaultContextMenu);

        gridLayout->addWidget(treeWidgetStockTiendas, 1, 7, 5, 1);

        checkBoxRemoto = new QCheckBox(General);
        checkBoxRemoto->setObjectName(QString::fromUtf8("checkBoxRemoto"));

        gridLayout->addWidget(checkBoxRemoto, 0, 7, 1, 1);


        verticalLayout_2->addWidget(General);

        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_6 = new QGridLayout(tab_3);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        tableViewAux = new QTableView(tab_3);
        tableViewAux->setObjectName(QString::fromUtf8("tableViewAux"));

        gridLayout_5->addWidget(tableViewAux, 0, 0, 1, 2);

        pushButtonAnadir = new QPushButton(tab_3);
        pushButtonAnadir->setObjectName(QString::fromUtf8("pushButtonAnadir"));

        gridLayout_5->addWidget(pushButtonAnadir, 2, 0, 1, 1);

        pushButtonEliminar = new QPushButton(tab_3);
        pushButtonEliminar->setObjectName(QString::fromUtf8("pushButtonEliminar"));

        gridLayout_5->addWidget(pushButtonEliminar, 2, 1, 1, 1);

        lineEditAux = new QLineEdit(tab_3);
        lineEditAux->setObjectName(QString::fromUtf8("lineEditAux"));
        lineEditAux->setMaxLength(13);

        gridLayout_5->addWidget(lineEditAux, 1, 0, 1, 2);


        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tabVentas = new QWidget();
        tabVentas->setObjectName(QString::fromUtf8("tabVentas"));
        gridLayout_3 = new QGridLayout(tabVentas);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox = new QGroupBox(tabVentas);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        radioButtonVentasDia = new QRadioButton(groupBox);
        radioButtonVentasDia->setObjectName(QString::fromUtf8("radioButtonVentasDia"));

        horizontalLayout_2->addWidget(radioButtonVentasDia);

        radioButtonVentasMes = new QRadioButton(groupBox);
        radioButtonVentasMes->setObjectName(QString::fromUtf8("radioButtonVentasMes"));
        radioButtonVentasMes->setChecked(true);

        horizontalLayout_2->addWidget(radioButtonVentasMes);

        radioButtonVentasAno = new QRadioButton(groupBox);
        radioButtonVentasAno->setObjectName(QString::fromUtf8("radioButtonVentasAno"));

        horizontalLayout_2->addWidget(radioButtonVentasAno);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 1);

        tableViewVentas = new QTableView(tabVentas);
        tableViewVentas->setObjectName(QString::fromUtf8("tableViewVentas"));

        gridLayout_3->addWidget(tableViewVentas, 1, 0, 1, 1);

        tabWidget->addTab(tabVentas, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_4 = new QGridLayout(tab_2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        tableViewCompras = new QTableView(tab_2);
        tableViewCompras->setObjectName(QString::fromUtf8("tableViewCompras"));
        tableViewCompras->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        gridLayout_4->addWidget(tableViewCompras, 1, 0, 1, 1);

        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_3 = new QHBoxLayout(groupBox_2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        radioButtonFacturas = new QRadioButton(groupBox_2);
        radioButtonFacturas->setObjectName(QString::fromUtf8("radioButtonFacturas"));
        radioButtonFacturas->setChecked(false);

        horizontalLayout_3->addWidget(radioButtonFacturas);

        radioButtonMeses = new QRadioButton(groupBox_2);
        radioButtonMeses->setObjectName(QString::fromUtf8("radioButtonMeses"));
        radioButtonMeses->setChecked(true);

        horizontalLayout_3->addWidget(radioButtonMeses);

        radioButtonAnos = new QRadioButton(groupBox_2);
        radioButtonAnos->setObjectName(QString::fromUtf8("radioButtonAnos"));

        horizontalLayout_3->addWidget(radioButtonAnos);


        gridLayout_4->addWidget(groupBox_2, 0, 0, 1, 1);

        pushButtonVerFactura = new QPushButton(tab_2);
        pushButtonVerFactura->setObjectName(QString::fromUtf8("pushButtonVerFactura"));
        pushButtonVerFactura->setEnabled(false);

        gridLayout_4->addWidget(pushButtonVerFactura, 3, 0, 1, 1);

        labelProveedor = new QLabel(tab_2);
        labelProveedor->setObjectName(QString::fromUtf8("labelProveedor"));

        gridLayout_4->addWidget(labelProveedor, 2, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonNuevo = new QPushButton(Articulos);
        pushButtonNuevo->setObjectName(QString::fromUtf8("pushButtonNuevo"));
        pushButtonNuevo->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonNuevo);

        pushButtonBorrar = new QPushButton(Articulos);
        pushButtonBorrar->setObjectName(QString::fromUtf8("pushButtonBorrar"));
        pushButtonBorrar->setEnabled(false);
        pushButtonBorrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonBorrar);

        pushButtonModificar = new QPushButton(Articulos);
        pushButtonModificar->setObjectName(QString::fromUtf8("pushButtonModificar"));
        pushButtonModificar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonModificar);

        pushButtonRefrescar = new QPushButton(Articulos);
        pushButtonRefrescar->setObjectName(QString::fromUtf8("pushButtonRefrescar"));
        pushButtonRefrescar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonRefrescar);

        pushButtonAnterior = new QPushButton(Articulos);
        pushButtonAnterior->setObjectName(QString::fromUtf8("pushButtonAnterior"));
        pushButtonAnterior->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonAnterior);

        pushButtonSiguiente = new QPushButton(Articulos);
        pushButtonSiguiente->setObjectName(QString::fromUtf8("pushButtonSiguiente"));
        pushButtonSiguiente->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonSiguiente);

        pushButtonCaducados = new QPushButton(Articulos);
        pushButtonCaducados->setObjectName(QString::fromUtf8("pushButtonCaducados"));
        pushButtonCaducados->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonCaducados);

        pushButtonCerrar = new QPushButton(Articulos);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));
        pushButtonCerrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonCerrar);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(lineEditCod, lineEditDesc);
        QWidget::setTabOrder(lineEditDesc, lineEditIva);
        QWidget::setTabOrder(lineEditIva, lineEditCosto);
        QWidget::setTabOrder(lineEditCosto, lineEditPvp);
        QWidget::setTabOrder(lineEditPvp, lineEditMinimo);
        QWidget::setTabOrder(lineEditMinimo, lineEditMaximo);
        QWidget::setTabOrder(lineEditMaximo, lineEditCodFamila);
        QWidget::setTabOrder(lineEditCodFamila, pushButtonBuscarFamilia);
        QWidget::setTabOrder(pushButtonBuscarFamilia, lineEditCodFabricante);
        QWidget::setTabOrder(lineEditCodFabricante, pushButton_2);
        QWidget::setTabOrder(pushButton_2, comboBoxFormato);
        QWidget::setTabOrder(comboBoxFormato, lineEditCantidad);
        QWidget::setTabOrder(lineEditCantidad, pushButtonEtiqueta);
        QWidget::setTabOrder(pushButtonEtiqueta, pushButtonNuevo);
        QWidget::setTabOrder(pushButtonNuevo, pushButtonBorrar);
        QWidget::setTabOrder(pushButtonBorrar, pushButtonModificar);
        QWidget::setTabOrder(pushButtonModificar, pushButtonRefrescar);
        QWidget::setTabOrder(pushButtonRefrescar, pushButtonAnterior);
        QWidget::setTabOrder(pushButtonAnterior, pushButtonSiguiente);
        QWidget::setTabOrder(pushButtonSiguiente, pushButtonCaducados);
        QWidget::setTabOrder(pushButtonCaducados, pushButtonCerrar);
        QWidget::setTabOrder(pushButtonCerrar, pushButtonVer);
        QWidget::setTabOrder(pushButtonVer, pushButtonPonerFoto);
        QWidget::setTabOrder(pushButtonPonerFoto, plainTextEdit);
        QWidget::setTabOrder(plainTextEdit, treeWidgetStockTiendas);
        QWidget::setTabOrder(treeWidgetStockTiendas, lineEditEncargados);
        QWidget::setTabOrder(lineEditEncargados, lineEditPendientes);
        QWidget::setTabOrder(lineEditPendientes, tabWidget);
        QWidget::setTabOrder(tabWidget, tableViewAux);
        QWidget::setTabOrder(tableViewAux, pushButtonAnadir);
        QWidget::setTabOrder(pushButtonAnadir, pushButtonEliminar);
        QWidget::setTabOrder(pushButtonEliminar, lineEditAux);
        QWidget::setTabOrder(lineEditAux, radioButtonVentasDia);
        QWidget::setTabOrder(radioButtonVentasDia, radioButtonVentasMes);
        QWidget::setTabOrder(radioButtonVentasMes, radioButtonVentasAno);
        QWidget::setTabOrder(radioButtonVentasAno, tableViewVentas);
        QWidget::setTabOrder(tableViewVentas, tableViewCompras);
        QWidget::setTabOrder(tableViewCompras, radioButtonFacturas);
        QWidget::setTabOrder(radioButtonFacturas, radioButtonMeses);
        QWidget::setTabOrder(radioButtonMeses, radioButtonAnos);
        QWidget::setTabOrder(radioButtonAnos, pushButtonVerFactura);
        QWidget::setTabOrder(pushButtonVerFactura, pushButtonBorrarFoto);

        retranslateUi(Articulos);
        QObject::connect(pushButtonCerrar, SIGNAL(clicked()), Articulos, SLOT(close()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Articulos);
    } // setupUi

    void retranslateUi(QDialog *Articulos)
    {
        Articulos->setWindowTitle(QCoreApplication::translate("Articulos", "Productos", nullptr));
        labelNombrePrecio->setText(QString());
        dateEditUltimoPedido->setDisplayFormat(QCoreApplication::translate("Articulos", "yyyy-MM-dd", nullptr));
        label_12->setText(QCoreApplication::translate("Articulos", "Foto", nullptr));
        labelFoto->setText(QCoreApplication::translate("Articulos", "Sin foto", nullptr));
        label_11->setText(QCoreApplication::translate("Articulos", "Ultimo pedido", nullptr));
        pushButtonBorrarFoto->setText(QCoreApplication::translate("Articulos", "Borrar foto", nullptr));
        label->setText(QCoreApplication::translate("Articulos", "Codigo", nullptr));
        pushButtonEtiqueta->setText(QCoreApplication::translate("Articulos", "Crear etiqueta", nullptr));
        label_2->setText(QCoreApplication::translate("Articulos", "Descripcion", nullptr));
        label_4->setText(QCoreApplication::translate("Articulos", "IVA", nullptr));
        label_14->setText(QCoreApplication::translate("Articulos", "Costo", nullptr));
        lineEditCosto->setInputMask(QString());
        label_3->setText(QCoreApplication::translate("Articulos", "P.V.P.", nullptr));
        label_5->setText(QCoreApplication::translate("Articulos", "Stock", nullptr));
        pushButtonVer->setText(QCoreApplication::translate("Articulos", "Ver/Actualizar", nullptr));
        label_6->setText(QCoreApplication::translate("Articulos", "Minimo", nullptr));
        label_7->setText(QCoreApplication::translate("Articulos", "Maximo", nullptr));
        label_8->setText(QCoreApplication::translate("Articulos", "Pendientes pedido", nullptr));
        lineEditPendientes->setText(QCoreApplication::translate("Articulos", "0", nullptr));
        label_10->setText(QCoreApplication::translate("Articulos", "Ultima venta", nullptr));
        dateEditUltimaVenta->setDisplayFormat(QCoreApplication::translate("Articulos", "yyyy-MM-dd", nullptr));
        label_15->setText(QCoreApplication::translate("Articulos", "Familia", nullptr));
        labelFamilia->setText(QCoreApplication::translate("Articulos", "Sin famila asignada", nullptr));
        label_16->setText(QCoreApplication::translate("Articulos", "Fabricante", nullptr));
        labelFabricante->setText(QCoreApplication::translate("Articulos", "Fabricante desconocido", nullptr));
        pushButtonPonerFoto->setText(QCoreApplication::translate("Articulos", "A\303\261adir Foto", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Articulos", "...", nullptr));
        pushButtonBuscarFamilia->setText(QCoreApplication::translate("Articulos", "...", nullptr));
        label_13->setText(QCoreApplication::translate("Articulos", "Presentaci\303\263n", nullptr));
        label_9->setText(QCoreApplication::translate("Articulos", "Encargados", nullptr));
        lineEditEncargados->setText(QCoreApplication::translate("Articulos", "0", nullptr));
        label_17->setText(QCoreApplication::translate("Articulos", "Cantidad", nullptr));
        checkBoxRemoto->setText(QCoreApplication::translate("Articulos", "Cargar remoto", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("Articulos", "General", nullptr));
        pushButtonAnadir->setText(QCoreApplication::translate("Articulos", "A\303\261adir", nullptr));
        pushButtonEliminar->setText(QCoreApplication::translate("Articulos", "Eliminar", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("Articulos", "C\303\263digos auxiliares", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Articulos", "Agrupar por:", nullptr));
        radioButtonVentasDia->setText(QCoreApplication::translate("Articulos", "Dia", nullptr));
        radioButtonVentasMes->setText(QCoreApplication::translate("Articulos", "Mes", nullptr));
        radioButtonVentasAno->setText(QCoreApplication::translate("Articulos", "A&\303\261o", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabVentas), QCoreApplication::translate("Articulos", "Ventas", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Articulos", "Ver por:", nullptr));
        radioButtonFacturas->setText(QCoreApplication::translate("Articulos", "Facturas", nullptr));
        radioButtonMeses->setText(QCoreApplication::translate("Articulos", "&Meses", nullptr));
        radioButtonAnos->setText(QCoreApplication::translate("Articulos", "A&\303\261os", nullptr));
        pushButtonVerFactura->setText(QCoreApplication::translate("Articulos", "Ver factura", nullptr));
        labelProveedor->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("Articulos", "Compras", nullptr));
        pushButtonNuevo->setText(QCoreApplication::translate("Articulos", "Nuevo", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("Articulos", "Borrar", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("Articulos", "Modificar", nullptr));
        pushButtonRefrescar->setText(QCoreApplication::translate("Articulos", "Refrescar", nullptr));
        pushButtonAnterior->setText(QCoreApplication::translate("Articulos", "Anterior", nullptr));
        pushButtonSiguiente->setText(QCoreApplication::translate("Articulos", "Siguiente", nullptr));
        pushButtonCaducados->setText(QCoreApplication::translate("Articulos", "Caducados", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("Articulos", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Articulos: public Ui_Articulos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARTICULOS_H
