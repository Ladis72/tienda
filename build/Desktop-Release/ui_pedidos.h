/********************************************************************************
** Form generated from reading UI file 'pedidos.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
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
    QGridLayout *gridLayout;
    QLineEdit *leRe21;
    QLineEdit *leBase10;
    QLabel *label_3;
    QLineEdit *leTotalIva;
    QLabel *labelNArticulos;
    QLineEdit *leRe10;
    QLabel *label_6;
    QLabel *labelNLineas;
    QLineEdit *leBase21;
    QLineEdit *leTotal;
    QLabel *label_2;
    QLabel *label_22;
    QLabel *label_7;
    QFrame *line;
    QFrame *line_2;
    QLabel *label_8;
    QLabel *label;
    QLineEdit *leTotalBase;
    QLabel *label_4;
    QLineEdit *leTotal21;
    QFrame *line_4;
    QLabel *label_5;
    QLineEdit *leTotalRe;
    QLabel *label_9;
    QLineEdit *leIva10;
    QLabel *label_10;
    QLineEdit *leTotal10;
    QLineEdit *leIva21;
    QLabel *label_23;
    QLineEdit *leBase0;
    QLineEdit *leIva0;
    QLineEdit *leRe0;
    QLineEdit *leTotal0;
    QLineEdit *leBase4;
    QLineEdit *leIva4;
    QLineEdit *leRe4;
    QLineEdit *leTotal4;
    QLineEdit *leBase5;
    QLineEdit *leIva5;
    QLineEdit *leRe5;
    QLineEdit *leTotal5;
    QLabel *labelDocumento;
    QTableView *tableView;
    QSpacerItem *horizontalSpacer;
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
    QSpacerItem *horizontalSpacer_2;
    QLabel *labelProveedor;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonAnadir;
    QPushButton *pushButtonModificar;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonCerrar;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButtonImprimir;
    QFrame *line_3;

    void setupUi(QDialog *pedidos)
    {
        if (pedidos->objectName().isEmpty())
            pedidos->setObjectName(QString::fromUtf8("pedidos"));
        pedidos->resize(1182, 767);
        pedidos->setStyleSheet(QString::fromUtf8(""));
        gridLayout_3 = new QGridLayout(pedidos);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        leRe21 = new QLineEdit(pedidos);
        leRe21->setObjectName(QString::fromUtf8("leRe21"));
        leRe21->setEnabled(false);

        gridLayout->addWidget(leRe21, 1, 3, 1, 1);

        leBase10 = new QLineEdit(pedidos);
        leBase10->setObjectName(QString::fromUtf8("leBase10"));
        leBase10->setEnabled(false);

        gridLayout->addWidget(leBase10, 2, 1, 1, 1);

        label_3 = new QLabel(pedidos);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        leTotalIva = new QLineEdit(pedidos);
        leTotalIva->setObjectName(QString::fromUtf8("leTotalIva"));
        leTotalIva->setEnabled(false);

        gridLayout->addWidget(leTotalIva, 7, 2, 1, 1);

        labelNArticulos = new QLabel(pedidos);
        labelNArticulos->setObjectName(QString::fromUtf8("labelNArticulos"));
        QFont font;
        font.setPointSize(12);
        labelNArticulos->setFont(font);

        gridLayout->addWidget(labelNArticulos, 8, 5, 1, 1);

        leRe10 = new QLineEdit(pedidos);
        leRe10->setObjectName(QString::fromUtf8("leRe10"));
        leRe10->setEnabled(false);

        gridLayout->addWidget(leRe10, 2, 3, 1, 1);

        label_6 = new QLabel(pedidos);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 0, 1, 1, 1);

        labelNLineas = new QLabel(pedidos);
        labelNLineas->setObjectName(QString::fromUtf8("labelNLineas"));
        labelNLineas->setFont(font);

        gridLayout->addWidget(labelNLineas, 8, 2, 1, 1);

        leBase21 = new QLineEdit(pedidos);
        leBase21->setObjectName(QString::fromUtf8("leBase21"));
        leBase21->setEnabled(false);

        gridLayout->addWidget(leBase21, 1, 1, 1, 1);

        leTotal = new QLineEdit(pedidos);
        leTotal->setObjectName(QString::fromUtf8("leTotal"));
        leTotal->setEnabled(false);

        gridLayout->addWidget(leTotal, 7, 5, 1, 1);

        label_2 = new QLabel(pedidos);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 8, 3, 1, 1);

        label_22 = new QLabel(pedidos);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        gridLayout->addWidget(label_22, 4, 0, 1, 1);

        label_7 = new QLabel(pedidos);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 0, 2, 1, 1);

        line = new QFrame(pedidos);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 6, 0, 1, 6);

        line_2 = new QFrame(pedidos);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 1, 4, 3, 1);

        label_8 = new QLabel(pedidos);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 0, 3, 1, 1);

        label = new QLabel(pedidos);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 8, 1, 1, 1);

        leTotalBase = new QLineEdit(pedidos);
        leTotalBase->setObjectName(QString::fromUtf8("leTotalBase"));
        leTotalBase->setEnabled(false);

        gridLayout->addWidget(leTotalBase, 7, 1, 1, 1);

        label_4 = new QLabel(pedidos);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        leTotal21 = new QLineEdit(pedidos);
        leTotal21->setObjectName(QString::fromUtf8("leTotal21"));
        leTotal21->setEnabled(false);

        gridLayout->addWidget(leTotal21, 1, 5, 1, 1);

        line_4 = new QFrame(pedidos);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_4, 7, 4, 1, 1);

        label_5 = new QLabel(pedidos);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        leTotalRe = new QLineEdit(pedidos);
        leTotalRe->setObjectName(QString::fromUtf8("leTotalRe"));
        leTotalRe->setEnabled(false);

        gridLayout->addWidget(leTotalRe, 7, 3, 1, 1);

        label_9 = new QLabel(pedidos);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 0, 5, 1, 1);

        leIva10 = new QLineEdit(pedidos);
        leIva10->setObjectName(QString::fromUtf8("leIva10"));
        leIva10->setEnabled(false);

        gridLayout->addWidget(leIva10, 2, 2, 1, 1);

        label_10 = new QLabel(pedidos);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 7, 0, 1, 1);

        leTotal10 = new QLineEdit(pedidos);
        leTotal10->setObjectName(QString::fromUtf8("leTotal10"));
        leTotal10->setEnabled(false);

        gridLayout->addWidget(leTotal10, 2, 5, 1, 1);

        leIva21 = new QLineEdit(pedidos);
        leIva21->setObjectName(QString::fromUtf8("leIva21"));
        leIva21->setEnabled(false);

        gridLayout->addWidget(leIva21, 1, 2, 1, 1);

        label_23 = new QLabel(pedidos);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout->addWidget(label_23, 5, 0, 1, 1);

        leBase0 = new QLineEdit(pedidos);
        leBase0->setObjectName(QString::fromUtf8("leBase0"));
        leBase0->setEnabled(false);

        gridLayout->addWidget(leBase0, 5, 1, 1, 1);

        leIva0 = new QLineEdit(pedidos);
        leIva0->setObjectName(QString::fromUtf8("leIva0"));
        leIva0->setEnabled(false);

        gridLayout->addWidget(leIva0, 5, 2, 1, 1);

        leRe0 = new QLineEdit(pedidos);
        leRe0->setObjectName(QString::fromUtf8("leRe0"));
        leRe0->setEnabled(false);

        gridLayout->addWidget(leRe0, 5, 3, 1, 1);

        leTotal0 = new QLineEdit(pedidos);
        leTotal0->setObjectName(QString::fromUtf8("leTotal0"));
        leTotal0->setEnabled(false);

        gridLayout->addWidget(leTotal0, 5, 5, 1, 1);

        leBase4 = new QLineEdit(pedidos);
        leBase4->setObjectName(QString::fromUtf8("leBase4"));
        leBase4->setEnabled(false);

        gridLayout->addWidget(leBase4, 4, 1, 1, 1);

        leIva4 = new QLineEdit(pedidos);
        leIva4->setObjectName(QString::fromUtf8("leIva4"));
        leIva4->setEnabled(false);

        gridLayout->addWidget(leIva4, 4, 2, 1, 1);

        leRe4 = new QLineEdit(pedidos);
        leRe4->setObjectName(QString::fromUtf8("leRe4"));
        leRe4->setEnabled(false);

        gridLayout->addWidget(leRe4, 4, 3, 1, 1);

        leTotal4 = new QLineEdit(pedidos);
        leTotal4->setObjectName(QString::fromUtf8("leTotal4"));
        leTotal4->setEnabled(false);

        gridLayout->addWidget(leTotal4, 4, 5, 1, 1);

        leBase5 = new QLineEdit(pedidos);
        leBase5->setObjectName(QString::fromUtf8("leBase5"));
        leBase5->setEnabled(false);

        gridLayout->addWidget(leBase5, 3, 1, 1, 1);

        leIva5 = new QLineEdit(pedidos);
        leIva5->setObjectName(QString::fromUtf8("leIva5"));
        leIva5->setEnabled(false);

        gridLayout->addWidget(leIva5, 3, 2, 1, 1);

        leRe5 = new QLineEdit(pedidos);
        leRe5->setObjectName(QString::fromUtf8("leRe5"));
        leRe5->setEnabled(false);

        gridLayout->addWidget(leRe5, 3, 3, 1, 1);

        leTotal5 = new QLineEdit(pedidos);
        leTotal5->setObjectName(QString::fromUtf8("leTotal5"));
        leTotal5->setEnabled(false);

        gridLayout->addWidget(leTotal5, 3, 5, 1, 1);


        gridLayout_3->addLayout(gridLayout, 7, 6, 1, 1);

        labelDocumento = new QLabel(pedidos);
        labelDocumento->setObjectName(QString::fromUtf8("labelDocumento"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Fira Sans"));
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setItalic(true);
        labelDocumento->setFont(font1);

        gridLayout_3->addWidget(labelDocumento, 0, 7, 1, 1);

        tableView = new QTableView(pedidos);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

        gridLayout_3->addWidget(tableView, 2, 0, 1, 8);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 7, 4, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_21 = new QLabel(pedidos);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout_2->addWidget(label_21, 0, 12, 1, 1);

        label_18 = new QLabel(pedidos);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        gridLayout_2->addWidget(label_18, 0, 11, 1, 1);

        leCod = new QLineEdit(pedidos);
        leCod->setObjectName(QString::fromUtf8("leCod"));
        leCod->setMaximumSize(QSize(150, 16777215));
        leCod->setMaxLength(13);

        gridLayout_2->addWidget(leCod, 1, 0, 1, 1);

        leLote = new QLineEdit(pedidos);
        leLote->setObjectName(QString::fromUtf8("leLote"));
        leLote->setEnabled(false);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(5);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leLote->sizePolicy().hasHeightForWidth());
        leLote->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(leLote, 1, 4, 1, 1);

        leUds = new QLineEdit(pedidos);
        leUds->setObjectName(QString::fromUtf8("leUds"));
        leUds->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(leUds, 1, 2, 1, 1);

        label_12 = new QLabel(pedidos);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy1);
        label_12->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(label_12, 0, 4, 1, 1);

        label_16 = new QLabel(pedidos);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_2->addWidget(label_16, 0, 9, 1, 1);

        leIva = new QLineEdit(pedidos);
        leIva->setObjectName(QString::fromUtf8("leIva"));
        leIva->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(leIva, 1, 9, 1, 1);

        label_15 = new QLabel(pedidos);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_2->addWidget(label_15, 0, 8, 1, 1);

        leDescripcion = new QLineEdit(pedidos);
        leDescripcion->setObjectName(QString::fromUtf8("leDescripcion"));

        gridLayout_2->addWidget(leDescripcion, 1, 1, 1, 1);

        lePvp = new QLineEdit(pedidos);
        lePvp->setObjectName(QString::fromUtf8("lePvp"));
        lePvp->setMaximumSize(QSize(80, 16777215));

        gridLayout_2->addWidget(lePvp, 1, 10, 1, 1);

        lePvt = new QLineEdit(pedidos);
        lePvt->setObjectName(QString::fromUtf8("lePvt"));
        lePvt->setMaximumSize(QSize(80, 16777215));

        gridLayout_2->addWidget(lePvt, 1, 8, 1, 1);

        label_11 = new QLabel(pedidos);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_2->addWidget(label_11, 0, 0, 1, 1);

        label_17 = new QLabel(pedidos);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_2->addWidget(label_17, 0, 10, 1, 1);

        labelDescripcion = new QLabel(pedidos);
        labelDescripcion->setObjectName(QString::fromUtf8("labelDescripcion"));

        gridLayout_2->addWidget(labelDescripcion, 0, 1, 1, 1);

        leTotalLinea = new QLineEdit(pedidos);
        leTotalLinea->setObjectName(QString::fromUtf8("leTotalLinea"));
        leTotalLinea->setEnabled(false);
        leTotalLinea->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(leTotalLinea, 1, 13, 1, 1);

        dateEdit = new QDateEdit(pedidos);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));
        dateEdit->setCalendarPopup(true);

        gridLayout_2->addWidget(dateEdit, 1, 5, 1, 1);

        label_20 = new QLabel(pedidos);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout_2->addWidget(label_20, 0, 5, 1, 1);

        label_13 = new QLabel(pedidos);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_2->addWidget(label_13, 0, 2, 1, 1);

        leBon = new QLineEdit(pedidos);
        leBon->setObjectName(QString::fromUtf8("leBon"));
        leBon->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(leBon, 1, 3, 1, 1);

        label_19 = new QLabel(pedidos);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout_2->addWidget(label_19, 0, 13, 1, 1);

        label_14 = new QLabel(pedidos);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_2->addWidget(label_14, 0, 3, 1, 1);

        leDescuento = new QLineEdit(pedidos);
        leDescuento->setObjectName(QString::fromUtf8("leDescuento"));
        leDescuento->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(leDescuento, 1, 11, 1, 1);

        leMargen = new QLineEdit(pedidos);
        leMargen->setObjectName(QString::fromUtf8("leMargen"));
        leMargen->setEnabled(false);
        leMargen->setMaximumSize(QSize(54, 16777215));

        gridLayout_2->addWidget(leMargen, 1, 12, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 3, 0, 1, 8);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 7, 7, 1, 1);

        labelProveedor = new QLabel(pedidos);
        labelProveedor->setObjectName(QString::fromUtf8("labelProveedor"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Fira Sans"));
        font2.setPointSize(14);
        font2.setBold(true);
        font2.setItalic(true);
        labelProveedor->setFont(font2);

        gridLayout_3->addWidget(labelProveedor, 0, 6, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonAnadir = new QPushButton(pedidos);
        pushButtonAnadir->setObjectName(QString::fromUtf8("pushButtonAnadir"));
        pushButtonAnadir->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        pushButtonAnadir->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonAnadir);

        pushButtonModificar = new QPushButton(pedidos);
        pushButtonModificar->setObjectName(QString::fromUtf8("pushButtonModificar"));
        pushButtonModificar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonModificar);

        pushButtonBorrar = new QPushButton(pedidos);
        pushButtonBorrar->setObjectName(QString::fromUtf8("pushButtonBorrar"));
        pushButtonBorrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonBorrar);

        pushButtonCerrar = new QPushButton(pedidos);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));
        pushButtonCerrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonCerrar);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        pushButtonImprimir = new QPushButton(pedidos);
        pushButtonImprimir->setObjectName(QString::fromUtf8("pushButtonImprimir"));
        pushButtonImprimir->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonImprimir);


        gridLayout_3->addLayout(horizontalLayout, 4, 0, 1, 8);

        line_3 = new QFrame(pedidos);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(line_3, 5, 0, 1, 8);

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
        QWidget::setTabOrder(pushButtonBorrar, leTotalBase);
        QWidget::setTabOrder(leTotalBase, leTotal);
        QWidget::setTabOrder(leTotal, leTotalRe);
        QWidget::setTabOrder(leTotalRe, leTotalIva);
        QWidget::setTabOrder(leTotalIva, leBase10);
        QWidget::setTabOrder(leBase10, leBase21);
        QWidget::setTabOrder(leBase21, leIva10);
        QWidget::setTabOrder(leIva10, leRe10);
        QWidget::setTabOrder(leRe10, leTotal21);
        QWidget::setTabOrder(leTotal21, leRe21);
        QWidget::setTabOrder(leRe21, leTotal10);
        QWidget::setTabOrder(leTotal10, tableView);
        QWidget::setTabOrder(tableView, leIva21);
        QWidget::setTabOrder(leIva21, leTotalLinea);
        QWidget::setTabOrder(leTotalLinea, leMargen);

        retranslateUi(pedidos);
        QObject::connect(pushButtonCerrar, SIGNAL(clicked()), pedidos, SLOT(close()));

        QMetaObject::connectSlotsByName(pedidos);
    } // setupUi

    void retranslateUi(QDialog *pedidos)
    {
        pedidos->setWindowTitle(QCoreApplication::translate("pedidos", "Gest\303\263n pedidos", nullptr));
        label_3->setText(QCoreApplication::translate("pedidos", "21%", nullptr));
        labelNArticulos->setText(QCoreApplication::translate("pedidos", "TextLabel", nullptr));
        label_6->setText(QCoreApplication::translate("pedidos", "Base", nullptr));
        labelNLineas->setText(QCoreApplication::translate("pedidos", "TextLabel", nullptr));
        label_2->setText(QCoreApplication::translate("pedidos", "N\303\272mero de art\303\255culos", nullptr));
        label_22->setText(QCoreApplication::translate("pedidos", "4%", nullptr));
        label_7->setText(QCoreApplication::translate("pedidos", "I.V.A.", nullptr));
        label_8->setText(QCoreApplication::translate("pedidos", "R.E.", nullptr));
        label->setText(QCoreApplication::translate("pedidos", "N\303\272mero de lineas", nullptr));
        label_4->setText(QCoreApplication::translate("pedidos", "10%", nullptr));
        label_5->setText(QCoreApplication::translate("pedidos", "5%", nullptr));
        label_9->setText(QCoreApplication::translate("pedidos", "Total", nullptr));
        label_10->setText(QCoreApplication::translate("pedidos", "Total", nullptr));
        label_23->setText(QCoreApplication::translate("pedidos", "0%", nullptr));
        labelDocumento->setText(QCoreApplication::translate("pedidos", "N\303\272mero documento", nullptr));
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
        pushButtonAnadir->setText(QCoreApplication::translate("pedidos", "A\303\261adir", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("pedidos", "Modificar", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("pedidos", "Borrar", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("pedidos", "Cerrar", nullptr));
        pushButtonImprimir->setText(QCoreApplication::translate("pedidos", "Imprimir", nullptr));
    } // retranslateUi

};

namespace Ui {
    class pedidos: public Ui_pedidos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PEDIDOS_H
