/********************************************************************************
** Form generated from reading UI file 'clientes.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTES_H
#define UI_CLIENTES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Clientes
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonNuevo;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonModificar;
    QPushButton *pushButtonReffrescar;
    QPushButton *pushButtonAnterior;
    QPushButton *pushButtonSiguiente;
    QPushButton *pushButtonCerrar;
    QTabWidget *tabWidgetProductos;
    QWidget *tab_3;
    QWidget *Principal;
    QGridLayout *General;
    QLabel *label_2;
    QDateEdit *dateEdit;
    QLabel *label_14;
    QSpacerItem *verticalSpacer;
    QLineEdit *lineEditNombre;
    QLabel *label;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_5;
    QLabel *label_9;
    QLabel *label_11;
    QLineEdit *lineEditProvincia;
    QLabel *label_4;
    QLabel *label_8;
    QLineEdit *lineEditMail;
    QSpacerItem *verticalSpacer_5;
    QLabel *label_12;
    QLabel *label_13;
    QLineEdit *lineEditDescuento;
    QSpacerItem *verticalSpacer_4;
    QLineEdit *lineEditLocalidad;
    QLabel *label_7;
    QLineEdit *lineEditCod;
    QLineEdit *lineEditCP;
    QLineEdit *lineEditDireccion;
    QSpacerItem *verticalSpacer_6;
    QLineEdit *lineEditApellidos;
    QLabel *label_3;
    QSpacerItem *verticalSpacer_2;
    QPlainTextEdit *plainTextEdit;
    QLabel *label_10;
    QLabel *label_6;
    QLineEdit *lineEditNIF;
    QLineEdit *lineEditTlfn2;
    QLineEdit *lineEditTlfn1;
    QWidget *tab_4;
    QGridLayout *gridLayout_2;
    QTableView *tableView2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButtonMeses;
    QRadioButton *radioButtonAnos;
    QRadioButton *radioButtonFechas;
    QDateEdit *dateEditDesde;
    QDateEdit *dateEditHasta;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonVerProductos2;
    QPushButton *pushButtonVerProductos;
    QTableView *tableViewDetalleTicket;
    QTableView *tableView;
    QWidget *Productos;
    QGridLayout *gridLayout_3;
    QRadioButton *radioButtonCantidad;
    QRadioButton *radioButtonFecha;
    QTableView *tableViewProductos;
    QLabel *labelNombreCliente;

    void setupUi(QDialog *Clientes)
    {
        if (Clientes->objectName().isEmpty())
            Clientes->setObjectName(QString::fromUtf8("Clientes"));
        Clientes->resize(1061, 651);
        gridLayout = new QGridLayout(Clientes);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonNuevo = new QPushButton(Clientes);
        pushButtonNuevo->setObjectName(QString::fromUtf8("pushButtonNuevo"));
        pushButtonNuevo->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonNuevo);

        pushButtonBorrar = new QPushButton(Clientes);
        pushButtonBorrar->setObjectName(QString::fromUtf8("pushButtonBorrar"));
        pushButtonBorrar->setEnabled(false);
        pushButtonBorrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonBorrar);

        pushButtonModificar = new QPushButton(Clientes);
        pushButtonModificar->setObjectName(QString::fromUtf8("pushButtonModificar"));
        pushButtonModificar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonModificar);

        pushButtonReffrescar = new QPushButton(Clientes);
        pushButtonReffrescar->setObjectName(QString::fromUtf8("pushButtonReffrescar"));
        pushButtonReffrescar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonReffrescar);

        pushButtonAnterior = new QPushButton(Clientes);
        pushButtonAnterior->setObjectName(QString::fromUtf8("pushButtonAnterior"));
        pushButtonAnterior->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonAnterior);

        pushButtonSiguiente = new QPushButton(Clientes);
        pushButtonSiguiente->setObjectName(QString::fromUtf8("pushButtonSiguiente"));
        pushButtonSiguiente->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonSiguiente);

        pushButtonCerrar = new QPushButton(Clientes);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));
        pushButtonCerrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonCerrar);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);

        tabWidgetProductos = new QTabWidget(Clientes);
        tabWidgetProductos->setObjectName(QString::fromUtf8("tabWidgetProductos"));
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        Principal = new QWidget(tab_3);
        Principal->setObjectName(QString::fromUtf8("Principal"));
        Principal->setGeometry(QRect(0, 10, 1037, 449));
        General = new QGridLayout(Principal);
        General->setObjectName(QString::fromUtf8("General"));
        label_2 = new QLabel(Principal);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        General->addWidget(label_2, 1, 2, 1, 1);

        dateEdit = new QDateEdit(Principal);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));

        General->addWidget(dateEdit, 13, 3, 1, 1);

        label_14 = new QLabel(Principal);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        General->addWidget(label_14, 14, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        General->addItem(verticalSpacer, 2, 1, 1, 1);

        lineEditNombre = new QLineEdit(Principal);
        lineEditNombre->setObjectName(QString::fromUtf8("lineEditNombre"));

        General->addWidget(lineEditNombre, 3, 1, 1, 1);

        label = new QLabel(Principal);
        label->setObjectName(QString::fromUtf8("label"));

        General->addWidget(label, 1, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        General->addItem(verticalSpacer_3, 6, 1, 1, 1);

        label_5 = new QLabel(Principal);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        General->addWidget(label_5, 5, 0, 1, 1);

        label_9 = new QLabel(Principal);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        General->addWidget(label_9, 9, 0, 1, 1);

        label_11 = new QLabel(Principal);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        General->addWidget(label_11, 11, 0, 1, 1);

        lineEditProvincia = new QLineEdit(Principal);
        lineEditProvincia->setObjectName(QString::fromUtf8("lineEditProvincia"));

        General->addWidget(lineEditProvincia, 7, 3, 1, 1);

        label_4 = new QLabel(Principal);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        General->addWidget(label_4, 3, 2, 1, 1);

        label_8 = new QLabel(Principal);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        General->addWidget(label_8, 7, 2, 1, 1);

        lineEditMail = new QLineEdit(Principal);
        lineEditMail->setObjectName(QString::fromUtf8("lineEditMail"));

        General->addWidget(lineEditMail, 11, 1, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        General->addItem(verticalSpacer_5, 10, 1, 1, 1);

        label_12 = new QLabel(Principal);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        General->addWidget(label_12, 13, 0, 1, 1);

        label_13 = new QLabel(Principal);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        General->addWidget(label_13, 13, 2, 1, 1);

        lineEditDescuento = new QLineEdit(Principal);
        lineEditDescuento->setObjectName(QString::fromUtf8("lineEditDescuento"));

        General->addWidget(lineEditDescuento, 13, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        General->addItem(verticalSpacer_4, 8, 1, 1, 1);

        lineEditLocalidad = new QLineEdit(Principal);
        lineEditLocalidad->setObjectName(QString::fromUtf8("lineEditLocalidad"));

        General->addWidget(lineEditLocalidad, 7, 1, 1, 1);

        label_7 = new QLabel(Principal);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        General->addWidget(label_7, 7, 0, 1, 1);

        lineEditCod = new QLineEdit(Principal);
        lineEditCod->setObjectName(QString::fromUtf8("lineEditCod"));
        lineEditCod->setEnabled(true);

        General->addWidget(lineEditCod, 1, 1, 1, 1);

        lineEditCP = new QLineEdit(Principal);
        lineEditCP->setObjectName(QString::fromUtf8("lineEditCP"));

        General->addWidget(lineEditCP, 5, 3, 1, 1);

        lineEditDireccion = new QLineEdit(Principal);
        lineEditDireccion->setObjectName(QString::fromUtf8("lineEditDireccion"));

        General->addWidget(lineEditDireccion, 5, 1, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        General->addItem(verticalSpacer_6, 12, 1, 1, 1);

        lineEditApellidos = new QLineEdit(Principal);
        lineEditApellidos->setObjectName(QString::fromUtf8("lineEditApellidos"));

        General->addWidget(lineEditApellidos, 3, 3, 1, 1);

        label_3 = new QLabel(Principal);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        General->addWidget(label_3, 3, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        General->addItem(verticalSpacer_2, 4, 1, 1, 1);

        plainTextEdit = new QPlainTextEdit(Principal);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(plainTextEdit->sizePolicy().hasHeightForWidth());
        plainTextEdit->setSizePolicy(sizePolicy);

        General->addWidget(plainTextEdit, 15, 0, 1, 4);

        label_10 = new QLabel(Principal);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        General->addWidget(label_10, 9, 2, 1, 1);

        label_6 = new QLabel(Principal);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        General->addWidget(label_6, 5, 2, 1, 1);

        lineEditNIF = new QLineEdit(Principal);
        lineEditNIF->setObjectName(QString::fromUtf8("lineEditNIF"));

        General->addWidget(lineEditNIF, 1, 3, 1, 1);

        lineEditTlfn2 = new QLineEdit(Principal);
        lineEditTlfn2->setObjectName(QString::fromUtf8("lineEditTlfn2"));

        General->addWidget(lineEditTlfn2, 9, 3, 1, 1);

        lineEditTlfn1 = new QLineEdit(Principal);
        lineEditTlfn1->setObjectName(QString::fromUtf8("lineEditTlfn1"));

        General->addWidget(lineEditTlfn1, 9, 1, 1, 1);

        tabWidgetProductos->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        gridLayout_2 = new QGridLayout(tab_4);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        tableView2 = new QTableView(tab_4);
        tableView2->setObjectName(QString::fromUtf8("tableView2"));
        tableView2->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);

        gridLayout_2->addWidget(tableView2, 3, 1, 1, 1);

        groupBox = new QGroupBox(tab_4);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        radioButtonMeses = new QRadioButton(groupBox);
        radioButtonMeses->setObjectName(QString::fromUtf8("radioButtonMeses"));
        radioButtonMeses->setChecked(true);

        horizontalLayout_2->addWidget(radioButtonMeses);

        radioButtonAnos = new QRadioButton(groupBox);
        radioButtonAnos->setObjectName(QString::fromUtf8("radioButtonAnos"));

        horizontalLayout_2->addWidget(radioButtonAnos);

        radioButtonFechas = new QRadioButton(groupBox);
        radioButtonFechas->setObjectName(QString::fromUtf8("radioButtonFechas"));

        horizontalLayout_2->addWidget(radioButtonFechas);

        dateEditDesde = new QDateEdit(groupBox);
        dateEditDesde->setObjectName(QString::fromUtf8("dateEditDesde"));
        dateEditDesde->setEnabled(false);
        dateEditDesde->setCalendarPopup(true);

        horizontalLayout_2->addWidget(dateEditDesde);

        dateEditHasta = new QDateEdit(groupBox);
        dateEditHasta->setObjectName(QString::fromUtf8("dateEditHasta"));
        dateEditHasta->setEnabled(false);
        dateEditHasta->setCalendarPopup(true);

        horizontalLayout_2->addWidget(dateEditHasta);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButtonVerProductos2 = new QPushButton(groupBox);
        pushButtonVerProductos2->setObjectName(QString::fromUtf8("pushButtonVerProductos2"));

        horizontalLayout_2->addWidget(pushButtonVerProductos2);

        pushButtonVerProductos = new QPushButton(groupBox);
        pushButtonVerProductos->setObjectName(QString::fromUtf8("pushButtonVerProductos"));

        horizontalLayout_2->addWidget(pushButtonVerProductos);


        gridLayout_2->addWidget(groupBox, 1, 0, 1, 3);

        tableViewDetalleTicket = new QTableView(tab_4);
        tableViewDetalleTicket->setObjectName(QString::fromUtf8("tableViewDetalleTicket"));

        gridLayout_2->addWidget(tableViewDetalleTicket, 4, 1, 1, 1);

        tableView = new QTableView(tab_4);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout_2->addWidget(tableView, 3, 0, 2, 1);

        tabWidgetProductos->addTab(tab_4, QString());
        Productos = new QWidget();
        Productos->setObjectName(QString::fromUtf8("Productos"));
        gridLayout_3 = new QGridLayout(Productos);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        radioButtonCantidad = new QRadioButton(Productos);
        radioButtonCantidad->setObjectName(QString::fromUtf8("radioButtonCantidad"));

        gridLayout_3->addWidget(radioButtonCantidad, 0, 0, 1, 1);

        radioButtonFecha = new QRadioButton(Productos);
        radioButtonFecha->setObjectName(QString::fromUtf8("radioButtonFecha"));

        gridLayout_3->addWidget(radioButtonFecha, 0, 1, 1, 1);

        tableViewProductos = new QTableView(Productos);
        tableViewProductos->setObjectName(QString::fromUtf8("tableViewProductos"));
        tableViewProductos->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableViewProductos->setSortingEnabled(true);
        tableViewProductos->setCornerButtonEnabled(false);

        gridLayout_3->addWidget(tableViewProductos, 1, 0, 1, 2);

        tabWidgetProductos->addTab(Productos, QString());

        gridLayout->addWidget(tabWidgetProductos, 1, 0, 1, 1);

        labelNombreCliente = new QLabel(Clientes);
        labelNombreCliente->setObjectName(QString::fromUtf8("labelNombreCliente"));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        font.setPointSize(14);
        font.setBold(true);
        labelNombreCliente->setFont(font);

        gridLayout->addWidget(labelNombreCliente, 0, 0, 1, 1);

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
        QObject::connect(pushButtonCerrar, SIGNAL(clicked()), Clientes, SLOT(close()));
        QObject::connect(radioButtonFechas, SIGNAL(toggled(bool)), dateEditDesde, SLOT(setEnabled(bool)));
        QObject::connect(radioButtonFechas, SIGNAL(toggled(bool)), dateEditHasta, SLOT(setEnabled(bool)));

        tabWidgetProductos->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Clientes);
    } // setupUi

    void retranslateUi(QDialog *Clientes)
    {
        Clientes->setWindowTitle(QCoreApplication::translate("Clientes", "Clientes", nullptr));
        pushButtonNuevo->setText(QCoreApplication::translate("Clientes", "Nuevo", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("Clientes", "Borrar", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("Clientes", "Modificar", nullptr));
        pushButtonReffrescar->setText(QCoreApplication::translate("Clientes", "Refrescar", nullptr));
        pushButtonAnterior->setText(QCoreApplication::translate("Clientes", "Anterior", nullptr));
        pushButtonSiguiente->setText(QCoreApplication::translate("Clientes", "Siguiente", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("Clientes", "Cerrar", nullptr));
        label_2->setText(QCoreApplication::translate("Clientes", "N.I.F.:", nullptr));
        dateEdit->setDisplayFormat(QCoreApplication::translate("Clientes", "yyyy/MM/dd", nullptr));
        label_14->setText(QCoreApplication::translate("Clientes", "Notas:", nullptr));
        label->setText(QCoreApplication::translate("Clientes", "C\303\263digo:", nullptr));
        label_5->setText(QCoreApplication::translate("Clientes", "Direcci\303\263n:", nullptr));
        label_9->setText(QCoreApplication::translate("Clientes", "Tel\303\251fono 1:", nullptr));
        label_11->setText(QCoreApplication::translate("Clientes", "E-mail:", nullptr));
        label_4->setText(QCoreApplication::translate("Clientes", "Apellidos:", nullptr));
        label_8->setText(QCoreApplication::translate("Clientes", "Provincia:", nullptr));
        label_12->setText(QCoreApplication::translate("Clientes", "Descuento", nullptr));
        label_13->setText(QCoreApplication::translate("Clientes", "Fecha alta:", nullptr));
        label_7->setText(QCoreApplication::translate("Clientes", "Localidad:", nullptr));
        label_3->setText(QCoreApplication::translate("Clientes", "Nombre:", nullptr));
        label_10->setText(QCoreApplication::translate("Clientes", "Tel\303\251fono 2:", nullptr));
        label_6->setText(QCoreApplication::translate("Clientes", "C.P.:", nullptr));
        tabWidgetProductos->setTabText(tabWidgetProductos->indexOf(tab_3), QCoreApplication::translate("Clientes", "General", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Clientes", "Periodo", nullptr));
        radioButtonMeses->setText(QCoreApplication::translate("Clientes", "&Meses", nullptr));
        radioButtonAnos->setText(QCoreApplication::translate("Clientes", "A&\303\261os", nullptr));
        radioButtonFechas->setText(QCoreApplication::translate("Clientes", "E&ntre fechas", nullptr));
        dateEditDesde->setDisplayFormat(QCoreApplication::translate("Clientes", "dd/MM/yyyy", nullptr));
        dateEditHasta->setDisplayFormat(QCoreApplication::translate("Clientes", "dd/MM/yyyy", nullptr));
        pushButtonVerProductos2->setText(QCoreApplication::translate("Clientes", "Ver productos", nullptr));
        pushButtonVerProductos->setText(QCoreApplication::translate("Clientes", "Ver tickets", nullptr));
        tabWidgetProductos->setTabText(tabWidgetProductos->indexOf(tab_4), QCoreApplication::translate("Clientes", "Ventas", nullptr));
        radioButtonCantidad->setText(QCoreApplication::translate("Clientes", "Cantidad", nullptr));
        radioButtonFecha->setText(QCoreApplication::translate("Clientes", "Fecha", nullptr));
        tabWidgetProductos->setTabText(tabWidgetProductos->indexOf(Productos), QCoreApplication::translate("Clientes", "Productos", nullptr));
        labelNombreCliente->setText(QCoreApplication::translate("Clientes", "Nombre cliente", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Clientes: public Ui_Clientes {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTES_H
