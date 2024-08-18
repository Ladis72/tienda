/********************************************************************************
** Form generated from reading UI file 'tpv.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TPV_H
#define UI_TPV_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Tpv
{
public:
    QGridLayout *gridLayout_3;
    QTableView *tableView;
    QLabel *label_vendedor;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_anadir;
    QPushButton *btn_borrar;
    QPushButton *btn_modificar;
    QPushButton *pushButtonBorrarTodo;
    QPushButton *btn_cobrar;
    QPushButton *btn_preTicket;
    QPushButton *btn_cerrar;
    QTableView *tableViewTicketsPendientes;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_cliente;
    QGridLayout *gridLayout_2;
    QLabel *labelAcumulado;
    QLineEdit *lineEdit_cod_cliente;
    QLineEdit *lineEdit_nobre_cliente;
    QLabel *labelVale;
    QLabel *labelFoto;
    QTextBrowser *textInfo;
    QLCDNumber *lcdNumber_total;
    QComboBox *comboBox_vendedor;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QLineEdit *lineEdit_cod;
    QLabel *label;
    QLabel *label_5;
    QLineEdit *lineEdit_descuento;
    QLineEdit *lineEdit_desc;
    QLabel *label_2;
    QLineEdit *lineEdit_6;
    QLineEdit *lineEdit_Uds;
    QLineEdit *lineEdit_precio;
    QLabel *label_7;
    QLabel *label_3;
    QLCDNumber *lcdProductos;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QFormLayout *formLayout;
    QLabel *label_8;
    QLabel *labelStock;
    QLabel *label_9;
    QLabel *labelFecha;

    void setupUi(QWidget *Tpv)
    {
        if (Tpv->objectName().isEmpty())
            Tpv->setObjectName(QString::fromUtf8("Tpv"));
        Tpv->setWindowModality(Qt::WindowModality::ApplicationModal);
        Tpv->resize(1577, 992);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Tpv->sizePolicy().hasHeightForWidth());
        Tpv->setSizePolicy(sizePolicy);
        Tpv->setMinimumSize(QSize(200, 200));
        Tpv->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_3 = new QGridLayout(Tpv);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tableView = new QTableView(Tpv);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setMinimumSize(QSize(500, 0));
        QPalette palette;
        QBrush brush(QColor(209, 241, 177, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush);
        tableView->setPalette(palette);
        tableView->setStyleSheet(QString::fromUtf8(""));
        tableView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        gridLayout_3->addWidget(tableView, 3, 0, 6, 6);

        label_vendedor = new QLabel(Tpv);
        label_vendedor->setObjectName(QString::fromUtf8("label_vendedor"));
        label_vendedor->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        label_vendedor->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        gridLayout_3->addWidget(label_vendedor, 0, 3, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btn_anadir = new QPushButton(Tpv);
        btn_anadir->setObjectName(QString::fromUtf8("btn_anadir"));

        horizontalLayout->addWidget(btn_anadir);

        btn_borrar = new QPushButton(Tpv);
        btn_borrar->setObjectName(QString::fromUtf8("btn_borrar"));

        horizontalLayout->addWidget(btn_borrar);

        btn_modificar = new QPushButton(Tpv);
        btn_modificar->setObjectName(QString::fromUtf8("btn_modificar"));

        horizontalLayout->addWidget(btn_modificar);

        pushButtonBorrarTodo = new QPushButton(Tpv);
        pushButtonBorrarTodo->setObjectName(QString::fromUtf8("pushButtonBorrarTodo"));

        horizontalLayout->addWidget(pushButtonBorrarTodo);

        btn_cobrar = new QPushButton(Tpv);
        btn_cobrar->setObjectName(QString::fromUtf8("btn_cobrar"));

        horizontalLayout->addWidget(btn_cobrar);

        btn_preTicket = new QPushButton(Tpv);
        btn_preTicket->setObjectName(QString::fromUtf8("btn_preTicket"));

        horizontalLayout->addWidget(btn_preTicket);

        btn_cerrar = new QPushButton(Tpv);
        btn_cerrar->setObjectName(QString::fromUtf8("btn_cerrar"));

        horizontalLayout->addWidget(btn_cerrar);


        gridLayout_3->addLayout(horizontalLayout, 10, 0, 1, 6);

        tableViewTicketsPendientes = new QTableView(Tpv);
        tableViewTicketsPendientes->setObjectName(QString::fromUtf8("tableViewTicketsPendientes"));
        tableViewTicketsPendientes->setMaximumSize(QSize(250, 16777215));
        tableViewTicketsPendientes->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableViewTicketsPendientes->setAlternatingRowColors(true);
        tableViewTicketsPendientes->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableViewTicketsPendientes->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        gridLayout_3->addWidget(tableViewTicketsPendientes, 0, 0, 2, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 2, 1, 1);

        groupBox_cliente = new QGroupBox(Tpv);
        groupBox_cliente->setObjectName(QString::fromUtf8("groupBox_cliente"));
        groupBox_cliente->setMaximumSize(QSize(800, 16777215));
        gridLayout_2 = new QGridLayout(groupBox_cliente);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        labelAcumulado = new QLabel(groupBox_cliente);
        labelAcumulado->setObjectName(QString::fromUtf8("labelAcumulado"));

        gridLayout_2->addWidget(labelAcumulado, 0, 3, 1, 1);

        lineEdit_cod_cliente = new QLineEdit(groupBox_cliente);
        lineEdit_cod_cliente->setObjectName(QString::fromUtf8("lineEdit_cod_cliente"));
        lineEdit_cod_cliente->setMaximumSize(QSize(60, 16777215));
        lineEdit_cod_cliente->setMaxLength(9);

        gridLayout_2->addWidget(lineEdit_cod_cliente, 0, 0, 1, 1);

        lineEdit_nobre_cliente = new QLineEdit(groupBox_cliente);
        lineEdit_nobre_cliente->setObjectName(QString::fromUtf8("lineEdit_nobre_cliente"));

        gridLayout_2->addWidget(lineEdit_nobre_cliente, 0, 2, 1, 1);

        labelVale = new QLabel(groupBox_cliente);
        labelVale->setObjectName(QString::fromUtf8("labelVale"));

        gridLayout_2->addWidget(labelVale, 0, 4, 1, 1);


        gridLayout_3->addWidget(groupBox_cliente, 0, 1, 1, 1);

        labelFoto = new QLabel(Tpv);
        labelFoto->setObjectName(QString::fromUtf8("labelFoto"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(labelFoto->sizePolicy().hasHeightForWidth());
        labelFoto->setSizePolicy(sizePolicy1);
        labelFoto->setMinimumSize(QSize(200, 200));
        labelFoto->setMaximumSize(QSize(200, 200));

        gridLayout_3->addWidget(labelFoto, 1, 5, 1, 1);

        textInfo = new QTextBrowser(Tpv);
        textInfo->setObjectName(QString::fromUtf8("textInfo"));
        textInfo->setMinimumSize(QSize(2, 0));
        textInfo->setMaximumSize(QSize(16777215, 200));

        gridLayout_3->addWidget(textInfo, 1, 1, 1, 4);

        lcdNumber_total = new QLCDNumber(Tpv);
        lcdNumber_total->setObjectName(QString::fromUtf8("lcdNumber_total"));
        lcdNumber_total->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lcdNumber_total->sizePolicy().hasHeightForWidth());
        lcdNumber_total->setSizePolicy(sizePolicy2);
        lcdNumber_total->setMinimumSize(QSize(0, 0));
        lcdNumber_total->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setPointSize(16);
        font.setBold(false);
        lcdNumber_total->setFont(font);
        lcdNumber_total->setCursor(QCursor(Qt::ArrowCursor));
        lcdNumber_total->setAutoFillBackground(false);
        lcdNumber_total->setStyleSheet(QString::fromUtf8("color: rgb(255, 16, 16);"));
        lcdNumber_total->setInputMethodHints(Qt::InputMethodHint::ImhNone);
        lcdNumber_total->setFrameShape(QFrame::Shape::Box);
        lcdNumber_total->setLineWidth(1);
        lcdNumber_total->setMidLineWidth(0);
        lcdNumber_total->setSmallDecimalPoint(false);
        lcdNumber_total->setDigitCount(6);
        lcdNumber_total->setSegmentStyle(QLCDNumber::SegmentStyle::Filled);
        lcdNumber_total->setProperty("value", QVariant(0.000000000000000));
        lcdNumber_total->setProperty("intValue", QVariant(0));

        gridLayout_3->addWidget(lcdNumber_total, 2, 4, 1, 2);

        comboBox_vendedor = new QComboBox(Tpv);
        comboBox_vendedor->setObjectName(QString::fromUtf8("comboBox_vendedor"));

        gridLayout_3->addWidget(comboBox_vendedor, 0, 4, 1, 2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_4 = new QLabel(Tpv);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 4, 1, 1);

        lineEdit_cod = new QLineEdit(Tpv);
        lineEdit_cod->setObjectName(QString::fromUtf8("lineEdit_cod"));
        lineEdit_cod->setMaximumSize(QSize(150, 16777215));
        lineEdit_cod->setMaxLength(13);

        gridLayout->addWidget(lineEdit_cod, 2, 0, 1, 1);

        label = new QLabel(Tpv);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        label_5 = new QLabel(Tpv);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 1, 5, 1, 1);

        lineEdit_descuento = new QLineEdit(Tpv);
        lineEdit_descuento->setObjectName(QString::fromUtf8("lineEdit_descuento"));
        lineEdit_descuento->setMaximumSize(QSize(50, 16777215));
        lineEdit_descuento->setMaxLength(3);

        gridLayout->addWidget(lineEdit_descuento, 2, 4, 1, 1);

        lineEdit_desc = new QLineEdit(Tpv);
        lineEdit_desc->setObjectName(QString::fromUtf8("lineEdit_desc"));

        gridLayout->addWidget(lineEdit_desc, 2, 1, 1, 1);

        label_2 = new QLabel(Tpv);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 1, 1, 1);

        lineEdit_6 = new QLineEdit(Tpv);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setEnabled(false);
        lineEdit_6->setMaximumSize(QSize(80, 16777215));

        gridLayout->addWidget(lineEdit_6, 2, 5, 1, 1);

        lineEdit_Uds = new QLineEdit(Tpv);
        lineEdit_Uds->setObjectName(QString::fromUtf8("lineEdit_Uds"));
        lineEdit_Uds->setMaximumSize(QSize(50, 16777215));
        lineEdit_Uds->setInputMethodHints(Qt::InputMethodHint::ImhDigitsOnly);
        lineEdit_Uds->setMaxLength(3);

        gridLayout->addWidget(lineEdit_Uds, 2, 2, 1, 1);

        lineEdit_precio = new QLineEdit(Tpv);
        lineEdit_precio->setObjectName(QString::fromUtf8("lineEdit_precio"));
        lineEdit_precio->setMaximumSize(QSize(50, 16777215));
        lineEdit_precio->setMaxLength(6);

        gridLayout->addWidget(lineEdit_precio, 2, 3, 1, 1);

        label_7 = new QLabel(Tpv);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 1, 3, 1, 1);

        label_3 = new QLabel(Tpv);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 2, 1, 1);


        gridLayout_3->addLayout(gridLayout, 9, 0, 1, 6);

        lcdProductos = new QLCDNumber(Tpv);
        lcdProductos->setObjectName(QString::fromUtf8("lcdProductos"));
        QFont font1;
        font1.setBold(true);
        lcdProductos->setFont(font1);
        lcdProductos->setAutoFillBackground(false);
        lcdProductos->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        lcdProductos->setInputMethodHints(Qt::InputMethodHint::ImhNone);
        lcdProductos->setDigitCount(3);

        gridLayout_3->addWidget(lcdProductos, 2, 0, 1, 1);

        groupBox = new QGroupBox(Tpv);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy3);
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(true);
        groupBox->setFont(font2);
        groupBox->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        groupBox->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        groupBox->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        QFont font3;
        font3.setPointSize(10);
        font3.setBold(false);
        label_8->setFont(font3);

        formLayout->setWidget(0, QFormLayout::LabelRole, label_8);

        labelStock = new QLabel(groupBox);
        labelStock->setObjectName(QString::fromUtf8("labelStock"));
        QFont font4;
        font4.setPointSize(10);
        font4.setBold(true);
        labelStock->setFont(font4);

        formLayout->setWidget(0, QFormLayout::FieldRole, labelStock);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font3);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_9);

        labelFecha = new QLabel(groupBox);
        labelFecha->setObjectName(QString::fromUtf8("labelFecha"));
        labelFecha->setFont(font4);

        formLayout->setWidget(1, QFormLayout::FieldRole, labelFecha);


        horizontalLayout_2->addLayout(formLayout);


        gridLayout_3->addWidget(groupBox, 2, 1, 1, 3);

#if QT_CONFIG(shortcut)
        label_vendedor->setBuddy(comboBox_vendedor);
        label_4->setBuddy(lineEdit_descuento);
        label->setBuddy(lineEdit_cod);
        label_5->setBuddy(lineEdit_6);
        label_2->setBuddy(lineEdit_desc);
        label_7->setBuddy(lineEdit_precio);
        label_3->setBuddy(lineEdit_Uds);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(lineEdit_cod_cliente, lineEdit_nobre_cliente);
        QWidget::setTabOrder(lineEdit_nobre_cliente, lineEdit_cod);
        QWidget::setTabOrder(lineEdit_cod, lineEdit_desc);
        QWidget::setTabOrder(lineEdit_desc, lineEdit_Uds);
        QWidget::setTabOrder(lineEdit_Uds, lineEdit_precio);
        QWidget::setTabOrder(lineEdit_precio, lineEdit_descuento);
        QWidget::setTabOrder(lineEdit_descuento, btn_anadir);
        QWidget::setTabOrder(btn_anadir, btn_borrar);
        QWidget::setTabOrder(btn_borrar, btn_modificar);
        QWidget::setTabOrder(btn_modificar, pushButtonBorrarTodo);
        QWidget::setTabOrder(pushButtonBorrarTodo, btn_cobrar);
        QWidget::setTabOrder(btn_cobrar, btn_cerrar);
        QWidget::setTabOrder(btn_cerrar, lineEdit_6);
        QWidget::setTabOrder(lineEdit_6, tableView);
        QWidget::setTabOrder(tableView, btn_preTicket);

        retranslateUi(Tpv);
        QObject::connect(btn_cerrar, SIGNAL(clicked()), Tpv, SLOT(close()));
        QObject::connect(lineEdit_Uds, SIGNAL(returnPressed()), lineEdit_cod, SLOT(setFocus()));
        QObject::connect(lineEdit_Uds, SIGNAL(returnPressed()), lineEdit_cod, SLOT(selectAll()));
        QObject::connect(lineEdit_descuento, SIGNAL(returnPressed()), lineEdit_cod, SLOT(setFocus()));
        QObject::connect(lineEdit_descuento, SIGNAL(returnPressed()), lineEdit_cod, SLOT(selectAll()));
        QObject::connect(lineEdit_precio, SIGNAL(returnPressed()), lineEdit_cod, SLOT(setFocus()));
        QObject::connect(lineEdit_precio, SIGNAL(returnPressed()), lineEdit_cod, SLOT(selectAll()));

        QMetaObject::connectSlotsByName(Tpv);
    } // setupUi

    void retranslateUi(QWidget *Tpv)
    {
        Tpv->setWindowTitle(QCoreApplication::translate("Tpv", "TPV", nullptr));
        label_vendedor->setText(QCoreApplication::translate("Tpv", "&Vendedor", nullptr));
        btn_anadir->setText(QCoreApplication::translate("Tpv", "A\303\261adir", nullptr));
        btn_borrar->setText(QCoreApplication::translate("Tpv", "Borrar", nullptr));
#if QT_CONFIG(shortcut)
        btn_borrar->setShortcut(QCoreApplication::translate("Tpv", "Del", nullptr));
#endif // QT_CONFIG(shortcut)
#if QT_CONFIG(tooltip)
        btn_modificar->setToolTip(QCoreApplication::translate("Tpv", "Nuevo Ticket Ctrl+N", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_modificar->setText(QCoreApplication::translate("Tpv", "Nuevo Ticket", nullptr));
#if QT_CONFIG(shortcut)
        btn_modificar->setShortcut(QCoreApplication::translate("Tpv", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        pushButtonBorrarTodo->setText(QCoreApplication::translate("Tpv", "Borrar todo", nullptr));
#if QT_CONFIG(tooltip)
        btn_cobrar->setToolTip(QCoreApplication::translate("Tpv", "<html><head/><body><p>Cobrar (F8)</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_cobrar->setText(QCoreApplication::translate("Tpv", "Cobrar", nullptr));
#if QT_CONFIG(shortcut)
        btn_cobrar->setShortcut(QCoreApplication::translate("Tpv", "F8", nullptr));
#endif // QT_CONFIG(shortcut)
        btn_preTicket->setText(QCoreApplication::translate("Tpv", "Pre-ticket", nullptr));
#if QT_CONFIG(tooltip)
        btn_cerrar->setToolTip(QCoreApplication::translate("Tpv", "<html><head/><body><p>Cerrar Esc</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_cerrar->setText(QCoreApplication::translate("Tpv", "Cerrar", nullptr));
#if QT_CONFIG(shortcut)
        btn_cerrar->setShortcut(QCoreApplication::translate("Tpv", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        groupBox_cliente->setTitle(QCoreApplication::translate("Tpv", "Datos del cliente", nullptr));
        labelAcumulado->setText(QCoreApplication::translate("Tpv", "Acumulado", nullptr));
        labelVale->setText(QCoreApplication::translate("Tpv", "0", nullptr));
        labelFoto->setText(QCoreApplication::translate("Tpv", "Foto", nullptr));
        label_4->setText(QCoreApplication::translate("Tpv", "Desc&uento", nullptr));
        lineEdit_cod->setText(QString());
        label->setText(QCoreApplication::translate("Tpv", "Cod&igo", nullptr));
        label_5->setText(QCoreApplication::translate("Tpv", "Total &linea", nullptr));
        lineEdit_descuento->setText(QCoreApplication::translate("Tpv", "0", nullptr));
        label_2->setText(QCoreApplication::translate("Tpv", "Descripcion", nullptr));
        lineEdit_Uds->setText(QCoreApplication::translate("Tpv", "1", nullptr));
        label_7->setText(QCoreApplication::translate("Tpv", "Precio", nullptr));
        label_3->setText(QCoreApplication::translate("Tpv", "Ca&ntidad", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Tpv", "Datos del producto", nullptr));
        label_8->setText(QCoreApplication::translate("Tpv", "Stock", nullptr));
        labelStock->setText(QString());
        label_9->setText(QCoreApplication::translate("Tpv", "Fecha cad.", nullptr));
        labelFecha->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Tpv: public Ui_Tpv {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TPV_H
