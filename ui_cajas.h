/********************************************************************************
** Form generated from reading UI file 'cajas.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAJAS_H
#define UI_CAJAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Cajas
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayout_header;
    QLabel *headerTitle;
    QSpacerItem *horizontalSpacer_h;
    QVBoxLayout *vbox_hu;
    QLabel *label_u;
    QLabel *labelFUArqueo;
    QHBoxLayout *mainContent;
    QVBoxLayout *leftPanel;
    QGroupBox *groupBox_efectivo;
    QGridLayout *gridLayout_conteo;
    QLabel *label_500;
    QSpinBox *spinBox500;
    QLabel *label500;
    QLabel *label_200_2;
    QSpinBox *spinBox200;
    QLabel *label200;
    QLabel *label_100_2;
    QSpinBox *spinBox100;
    QLabel *label100;
    QLabel *label_50_2;
    QSpinBox *spinBox50;
    QLabel *label50;
    QLabel *label_20_2;
    QSpinBox *spinBox20;
    QLabel *label20;
    QLabel *label_10_2;
    QSpinBox *spinBox10;
    QLabel *label10;
    QLabel *label_5_2;
    QSpinBox *spinBox5;
    QLabel *label5;
    QLabel *label_2_2;
    QSpinBox *spinBox2;
    QLabel *label2;
    QLabel *label_1_2;
    QSpinBox *spinBox1;
    QLabel *label1;
    QLabel *label_50c_2;
    QSpinBox *spinBox50c;
    QLabel *label50c;
    QLabel *label_20c_2;
    QSpinBox *spinBox20c;
    QLabel *label20c;
    QLabel *label_10c_2;
    QSpinBox *spinBox10c;
    QLabel *label10c;
    QLabel *label_5c_2;
    QSpinBox *spinBox5c;
    QLabel *label5c;
    QLabel *label_2c_2;
    QSpinBox *spinBox2c;
    QLabel *label2c;
    QLabel *label_1c_2;
    QSpinBox *spinBox1c;
    QLabel *label1c;
    QVBoxLayout *rightPanel;
    QGroupBox *groupBox_sistema;
    QGridLayout *gridLayout_sys;
    QLabel *label_ve;
    QLabel *label_ventasEfectivo;
    QLabel *label_1_t;
    QLabel *label_ventasB;
    QLabel *label_vt_t;
    QLabel *labelVentasTarjeta;
    QLabel *label_nt_t;
    QLabel *labelNumeroTarjetas;
    QLabel *label_sal_t;
    QLabel *labelEntradas;
    QGroupBox *groupBox_manual;
    QGridLayout *gridLayout_man;
    QLabel *label_ma_t;
    QLineEdit *lineEditVentasTarjeta;
    QLabel *label_ci_t;
    QDoubleSpinBox *doubleSpinBoxCambios;
    QLabel *label_cf_t;
    QDoubleSpinBox *doubleSpinBoxCajaF;
    QGroupBox *groupBox_resumen;
    QGridLayout *gridLayout_res;
    QLabel *label_tv_t;
    QLabel *label_ventasTotales;
    QLabel *label_te_t;
    QLabel *labelEfectivoTotal;
    QLabel *label_er_t;
    QLabel *labelEfectivoR;
    QLabel *label_1_1;
    QLabel *labelDescuadre;
    QLabel *label_21;
    QHBoxLayout *h_info;
    QLabel *label_sa_t;
    QLabel *labelSaldoAnterior;
    QSpacerItem *h_s;
    QHBoxLayout *h_buttons;
    QPushButton *pushButtonRetirarDinero;
    QSpacerItem *h_s2;
    QPushButton *pushButtonCerrar;
    QPushButton *pushButtonAceptar;

    void setupUi(QDialog *Cajas)
    {
        if (Cajas->objectName().isEmpty())
            Cajas->setObjectName(QString::fromUtf8("Cajas"));
        Cajas->resize(750, 850);
        Cajas->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f5f7fa; }\n"
"QFrame#headerFrame { background-color: #2e7d32; border-bottom-left-radius: 15px; border-bottom-right-radius: 15px; }\n"
"QLabel#headerTitle { color: white; font-size: 24px; font-weight: bold; }\n"
"QGroupBox { font-weight: bold; border: 2px solid #e0e0e0; border-radius: 10px; margin-top: 1.2em; padding: 15px; background-color: white; }\n"
"QGroupBox::title { subcontrol-origin: margin; left: 15px; padding: 0 5px; color: #2e7d32; }\n"
"QLineEdit, QDoubleSpinBox, QSpinBox { padding: 5px; border: 1px solid #d1d9e6; border-radius: 5px; background-color: #ffffff; }\n"
"QLineEdit:focus, QDoubleSpinBox:focus, QSpinBox:focus { border: 2px solid #2e7d32; }\n"
"QPushButton#pushButtonAceptar { background-color: #2e7d32; color: white; border-radius: 5px; padding: 10px 20px; font-weight: bold; }\n"
"QPushButton#pushButtonAceptar:hover { background-color: #1b5e20; }\n"
"QPushButton#pushButtonCerrar { background-color: #e0e0e0; color: #424242; border-radius: 5px; padding: 10px 20px; "
                        "}\n"
"QPushButton#pushButtonCerrar:hover { background-color: #bdbdbd; }\n"
"QLabel { color: #424242; }"));
        verticalLayout = new QVBoxLayout(Cajas);
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 15);
        headerFrame = new QFrame(Cajas);
        headerFrame->setObjectName(QString::fromUtf8("headerFrame"));
        headerFrame->setMinimumSize(QSize(0, 70));
        horizontalLayout_header = new QHBoxLayout(headerFrame);
        horizontalLayout_header->setObjectName(QString::fromUtf8("horizontalLayout_header"));
        horizontalLayout_header->setContentsMargins(30, -1, -1, -1);
        headerTitle = new QLabel(headerFrame);
        headerTitle->setObjectName(QString::fromUtf8("headerTitle"));

        horizontalLayout_header->addWidget(headerTitle);

        horizontalSpacer_h = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_header->addItem(horizontalSpacer_h);

        vbox_hu = new QVBoxLayout();
        vbox_hu->setObjectName(QString::fromUtf8("vbox_hu"));
        label_u = new QLabel(headerFrame);
        label_u->setObjectName(QString::fromUtf8("label_u"));

        vbox_hu->addWidget(label_u);

        labelFUArqueo = new QLabel(headerFrame);
        labelFUArqueo->setObjectName(QString::fromUtf8("labelFUArqueo"));

        vbox_hu->addWidget(labelFUArqueo);


        horizontalLayout_header->addLayout(vbox_hu);


        verticalLayout->addWidget(headerFrame);

        mainContent = new QHBoxLayout();
        mainContent->setObjectName(QString::fromUtf8("mainContent"));
        mainContent->setContentsMargins(15, -1, 15, -1);
        leftPanel = new QVBoxLayout();
        leftPanel->setObjectName(QString::fromUtf8("leftPanel"));
        groupBox_efectivo = new QGroupBox(Cajas);
        groupBox_efectivo->setObjectName(QString::fromUtf8("groupBox_efectivo"));
        gridLayout_conteo = new QGridLayout(groupBox_efectivo);
        gridLayout_conteo->setObjectName(QString::fromUtf8("gridLayout_conteo"));
        label_500 = new QLabel(groupBox_efectivo);
        label_500->setObjectName(QString::fromUtf8("label_500"));

        gridLayout_conteo->addWidget(label_500, 0, 0, 1, 1);

        spinBox500 = new QSpinBox(groupBox_efectivo);
        spinBox500->setObjectName(QString::fromUtf8("spinBox500"));

        gridLayout_conteo->addWidget(spinBox500, 0, 1, 1, 1);

        label500 = new QLabel(groupBox_efectivo);
        label500->setObjectName(QString::fromUtf8("label500"));

        gridLayout_conteo->addWidget(label500, 0, 2, 1, 1);

        label_200_2 = new QLabel(groupBox_efectivo);
        label_200_2->setObjectName(QString::fromUtf8("label_200_2"));

        gridLayout_conteo->addWidget(label_200_2, 1, 0, 1, 1);

        spinBox200 = new QSpinBox(groupBox_efectivo);
        spinBox200->setObjectName(QString::fromUtf8("spinBox200"));

        gridLayout_conteo->addWidget(spinBox200, 1, 1, 1, 1);

        label200 = new QLabel(groupBox_efectivo);
        label200->setObjectName(QString::fromUtf8("label200"));

        gridLayout_conteo->addWidget(label200, 1, 2, 1, 1);

        label_100_2 = new QLabel(groupBox_efectivo);
        label_100_2->setObjectName(QString::fromUtf8("label_100_2"));

        gridLayout_conteo->addWidget(label_100_2, 2, 0, 1, 1);

        spinBox100 = new QSpinBox(groupBox_efectivo);
        spinBox100->setObjectName(QString::fromUtf8("spinBox100"));

        gridLayout_conteo->addWidget(spinBox100, 2, 1, 1, 1);

        label100 = new QLabel(groupBox_efectivo);
        label100->setObjectName(QString::fromUtf8("label100"));

        gridLayout_conteo->addWidget(label100, 2, 2, 1, 1);

        label_50_2 = new QLabel(groupBox_efectivo);
        label_50_2->setObjectName(QString::fromUtf8("label_50_2"));

        gridLayout_conteo->addWidget(label_50_2, 3, 0, 1, 1);

        spinBox50 = new QSpinBox(groupBox_efectivo);
        spinBox50->setObjectName(QString::fromUtf8("spinBox50"));

        gridLayout_conteo->addWidget(spinBox50, 3, 1, 1, 1);

        label50 = new QLabel(groupBox_efectivo);
        label50->setObjectName(QString::fromUtf8("label50"));

        gridLayout_conteo->addWidget(label50, 3, 2, 1, 1);

        label_20_2 = new QLabel(groupBox_efectivo);
        label_20_2->setObjectName(QString::fromUtf8("label_20_2"));

        gridLayout_conteo->addWidget(label_20_2, 4, 0, 1, 1);

        spinBox20 = new QSpinBox(groupBox_efectivo);
        spinBox20->setObjectName(QString::fromUtf8("spinBox20"));

        gridLayout_conteo->addWidget(spinBox20, 4, 1, 1, 1);

        label20 = new QLabel(groupBox_efectivo);
        label20->setObjectName(QString::fromUtf8("label20"));

        gridLayout_conteo->addWidget(label20, 4, 2, 1, 1);

        label_10_2 = new QLabel(groupBox_efectivo);
        label_10_2->setObjectName(QString::fromUtf8("label_10_2"));

        gridLayout_conteo->addWidget(label_10_2, 5, 0, 1, 1);

        spinBox10 = new QSpinBox(groupBox_efectivo);
        spinBox10->setObjectName(QString::fromUtf8("spinBox10"));

        gridLayout_conteo->addWidget(spinBox10, 5, 1, 1, 1);

        label10 = new QLabel(groupBox_efectivo);
        label10->setObjectName(QString::fromUtf8("label10"));

        gridLayout_conteo->addWidget(label10, 5, 2, 1, 1);

        label_5_2 = new QLabel(groupBox_efectivo);
        label_5_2->setObjectName(QString::fromUtf8("label_5_2"));

        gridLayout_conteo->addWidget(label_5_2, 6, 0, 1, 1);

        spinBox5 = new QSpinBox(groupBox_efectivo);
        spinBox5->setObjectName(QString::fromUtf8("spinBox5"));

        gridLayout_conteo->addWidget(spinBox5, 6, 1, 1, 1);

        label5 = new QLabel(groupBox_efectivo);
        label5->setObjectName(QString::fromUtf8("label5"));

        gridLayout_conteo->addWidget(label5, 6, 2, 1, 1);

        label_2_2 = new QLabel(groupBox_efectivo);
        label_2_2->setObjectName(QString::fromUtf8("label_2_2"));

        gridLayout_conteo->addWidget(label_2_2, 7, 0, 1, 1);

        spinBox2 = new QSpinBox(groupBox_efectivo);
        spinBox2->setObjectName(QString::fromUtf8("spinBox2"));

        gridLayout_conteo->addWidget(spinBox2, 7, 1, 1, 1);

        label2 = new QLabel(groupBox_efectivo);
        label2->setObjectName(QString::fromUtf8("label2"));

        gridLayout_conteo->addWidget(label2, 7, 2, 1, 1);

        label_1_2 = new QLabel(groupBox_efectivo);
        label_1_2->setObjectName(QString::fromUtf8("label_1_2"));

        gridLayout_conteo->addWidget(label_1_2, 8, 0, 1, 1);

        spinBox1 = new QSpinBox(groupBox_efectivo);
        spinBox1->setObjectName(QString::fromUtf8("spinBox1"));

        gridLayout_conteo->addWidget(spinBox1, 8, 1, 1, 1);

        label1 = new QLabel(groupBox_efectivo);
        label1->setObjectName(QString::fromUtf8("label1"));

        gridLayout_conteo->addWidget(label1, 8, 2, 1, 1);

        label_50c_2 = new QLabel(groupBox_efectivo);
        label_50c_2->setObjectName(QString::fromUtf8("label_50c_2"));

        gridLayout_conteo->addWidget(label_50c_2, 9, 0, 1, 1);

        spinBox50c = new QSpinBox(groupBox_efectivo);
        spinBox50c->setObjectName(QString::fromUtf8("spinBox50c"));

        gridLayout_conteo->addWidget(spinBox50c, 9, 1, 1, 1);

        label50c = new QLabel(groupBox_efectivo);
        label50c->setObjectName(QString::fromUtf8("label50c"));

        gridLayout_conteo->addWidget(label50c, 9, 2, 1, 1);

        label_20c_2 = new QLabel(groupBox_efectivo);
        label_20c_2->setObjectName(QString::fromUtf8("label_20c_2"));

        gridLayout_conteo->addWidget(label_20c_2, 10, 0, 1, 1);

        spinBox20c = new QSpinBox(groupBox_efectivo);
        spinBox20c->setObjectName(QString::fromUtf8("spinBox20c"));

        gridLayout_conteo->addWidget(spinBox20c, 10, 1, 1, 1);

        label20c = new QLabel(groupBox_efectivo);
        label20c->setObjectName(QString::fromUtf8("label20c"));

        gridLayout_conteo->addWidget(label20c, 10, 2, 1, 1);

        label_10c_2 = new QLabel(groupBox_efectivo);
        label_10c_2->setObjectName(QString::fromUtf8("label_10c_2"));

        gridLayout_conteo->addWidget(label_10c_2, 11, 0, 1, 1);

        spinBox10c = new QSpinBox(groupBox_efectivo);
        spinBox10c->setObjectName(QString::fromUtf8("spinBox10c"));

        gridLayout_conteo->addWidget(spinBox10c, 11, 1, 1, 1);

        label10c = new QLabel(groupBox_efectivo);
        label10c->setObjectName(QString::fromUtf8("label10c"));

        gridLayout_conteo->addWidget(label10c, 11, 2, 1, 1);

        label_5c_2 = new QLabel(groupBox_efectivo);
        label_5c_2->setObjectName(QString::fromUtf8("label_5c_2"));

        gridLayout_conteo->addWidget(label_5c_2, 12, 0, 1, 1);

        spinBox5c = new QSpinBox(groupBox_efectivo);
        spinBox5c->setObjectName(QString::fromUtf8("spinBox5c"));

        gridLayout_conteo->addWidget(spinBox5c, 12, 1, 1, 1);

        label5c = new QLabel(groupBox_efectivo);
        label5c->setObjectName(QString::fromUtf8("label5c"));

        gridLayout_conteo->addWidget(label5c, 12, 2, 1, 1);

        label_2c_2 = new QLabel(groupBox_efectivo);
        label_2c_2->setObjectName(QString::fromUtf8("label_2c_2"));

        gridLayout_conteo->addWidget(label_2c_2, 13, 0, 1, 1);

        spinBox2c = new QSpinBox(groupBox_efectivo);
        spinBox2c->setObjectName(QString::fromUtf8("spinBox2c"));

        gridLayout_conteo->addWidget(spinBox2c, 13, 1, 1, 1);

        label2c = new QLabel(groupBox_efectivo);
        label2c->setObjectName(QString::fromUtf8("label2c"));

        gridLayout_conteo->addWidget(label2c, 13, 2, 1, 1);

        label_1c_2 = new QLabel(groupBox_efectivo);
        label_1c_2->setObjectName(QString::fromUtf8("label_1c_2"));

        gridLayout_conteo->addWidget(label_1c_2, 14, 0, 1, 1);

        spinBox1c = new QSpinBox(groupBox_efectivo);
        spinBox1c->setObjectName(QString::fromUtf8("spinBox1c"));

        gridLayout_conteo->addWidget(spinBox1c, 14, 1, 1, 1);

        label1c = new QLabel(groupBox_efectivo);
        label1c->setObjectName(QString::fromUtf8("label1c"));

        gridLayout_conteo->addWidget(label1c, 14, 2, 1, 1);


        leftPanel->addWidget(groupBox_efectivo);


        mainContent->addLayout(leftPanel);

        rightPanel = new QVBoxLayout();
        rightPanel->setObjectName(QString::fromUtf8("rightPanel"));
        groupBox_sistema = new QGroupBox(Cajas);
        groupBox_sistema->setObjectName(QString::fromUtf8("groupBox_sistema"));
        gridLayout_sys = new QGridLayout(groupBox_sistema);
        gridLayout_sys->setObjectName(QString::fromUtf8("gridLayout_sys"));
        label_ve = new QLabel(groupBox_sistema);
        label_ve->setObjectName(QString::fromUtf8("label_ve"));

        gridLayout_sys->addWidget(label_ve, 0, 0, 1, 1);

        label_ventasEfectivo = new QLabel(groupBox_sistema);
        label_ventasEfectivo->setObjectName(QString::fromUtf8("label_ventasEfectivo"));
        label_ventasEfectivo->setAlignment(Qt::AlignRight);

        gridLayout_sys->addWidget(label_ventasEfectivo, 0, 1, 1, 1);

        label_1_t = new QLabel(groupBox_sistema);
        label_1_t->setObjectName(QString::fromUtf8("label_1_t"));

        gridLayout_sys->addWidget(label_1_t, 1, 0, 1, 1);

        label_ventasB = new QLabel(groupBox_sistema);
        label_ventasB->setObjectName(QString::fromUtf8("label_ventasB"));
        label_ventasB->setAlignment(Qt::AlignRight);

        gridLayout_sys->addWidget(label_ventasB, 1, 1, 1, 1);

        label_vt_t = new QLabel(groupBox_sistema);
        label_vt_t->setObjectName(QString::fromUtf8("label_vt_t"));

        gridLayout_sys->addWidget(label_vt_t, 2, 0, 1, 1);

        labelVentasTarjeta = new QLabel(groupBox_sistema);
        labelVentasTarjeta->setObjectName(QString::fromUtf8("labelVentasTarjeta"));
        labelVentasTarjeta->setAlignment(Qt::AlignRight);

        gridLayout_sys->addWidget(labelVentasTarjeta, 2, 1, 1, 1);

        label_nt_t = new QLabel(groupBox_sistema);
        label_nt_t->setObjectName(QString::fromUtf8("label_nt_t"));

        gridLayout_sys->addWidget(label_nt_t, 3, 0, 1, 1);

        labelNumeroTarjetas = new QLabel(groupBox_sistema);
        labelNumeroTarjetas->setObjectName(QString::fromUtf8("labelNumeroTarjetas"));
        labelNumeroTarjetas->setAlignment(Qt::AlignRight);

        gridLayout_sys->addWidget(labelNumeroTarjetas, 3, 1, 1, 1);

        label_sal_t = new QLabel(groupBox_sistema);
        label_sal_t->setObjectName(QString::fromUtf8("label_sal_t"));

        gridLayout_sys->addWidget(label_sal_t, 4, 0, 1, 1);

        labelEntradas = new QLabel(groupBox_sistema);
        labelEntradas->setObjectName(QString::fromUtf8("labelEntradas"));
        labelEntradas->setAlignment(Qt::AlignRight);

        gridLayout_sys->addWidget(labelEntradas, 4, 1, 1, 1);


        rightPanel->addWidget(groupBox_sistema);

        groupBox_manual = new QGroupBox(Cajas);
        groupBox_manual->setObjectName(QString::fromUtf8("groupBox_manual"));
        gridLayout_man = new QGridLayout(groupBox_manual);
        gridLayout_man->setObjectName(QString::fromUtf8("gridLayout_man"));
        label_ma_t = new QLabel(groupBox_manual);
        label_ma_t->setObjectName(QString::fromUtf8("label_ma_t"));

        gridLayout_man->addWidget(label_ma_t, 0, 0, 1, 1);

        lineEditVentasTarjeta = new QLineEdit(groupBox_manual);
        lineEditVentasTarjeta->setObjectName(QString::fromUtf8("lineEditVentasTarjeta"));
        lineEditVentasTarjeta->setAlignment(Qt::AlignRight);

        gridLayout_man->addWidget(lineEditVentasTarjeta, 0, 1, 1, 1);

        label_ci_t = new QLabel(groupBox_manual);
        label_ci_t->setObjectName(QString::fromUtf8("label_ci_t"));

        gridLayout_man->addWidget(label_ci_t, 1, 0, 1, 1);

        doubleSpinBoxCambios = new QDoubleSpinBox(groupBox_manual);
        doubleSpinBoxCambios->setObjectName(QString::fromUtf8("doubleSpinBoxCambios"));
        doubleSpinBoxCambios->setAlignment(Qt::AlignRight);
        doubleSpinBoxCambios->setMaximum(9999.989999999999782);

        gridLayout_man->addWidget(doubleSpinBoxCambios, 1, 1, 1, 1);

        label_cf_t = new QLabel(groupBox_manual);
        label_cf_t->setObjectName(QString::fromUtf8("label_cf_t"));

        gridLayout_man->addWidget(label_cf_t, 2, 0, 1, 1);

        doubleSpinBoxCajaF = new QDoubleSpinBox(groupBox_manual);
        doubleSpinBoxCajaF->setObjectName(QString::fromUtf8("doubleSpinBoxCajaF"));
        doubleSpinBoxCajaF->setAlignment(Qt::AlignRight);
        doubleSpinBoxCajaF->setMaximum(9999.989999999999782);

        gridLayout_man->addWidget(doubleSpinBoxCajaF, 2, 1, 1, 1);


        rightPanel->addWidget(groupBox_manual);

        groupBox_resumen = new QGroupBox(Cajas);
        groupBox_resumen->setObjectName(QString::fromUtf8("groupBox_resumen"));
        gridLayout_res = new QGridLayout(groupBox_resumen);
        gridLayout_res->setObjectName(QString::fromUtf8("gridLayout_res"));
        label_tv_t = new QLabel(groupBox_resumen);
        label_tv_t->setObjectName(QString::fromUtf8("label_tv_t"));

        gridLayout_res->addWidget(label_tv_t, 0, 0, 1, 1);

        label_ventasTotales = new QLabel(groupBox_resumen);
        label_ventasTotales->setObjectName(QString::fromUtf8("label_ventasTotales"));
        label_ventasTotales->setAlignment(Qt::AlignRight);

        gridLayout_res->addWidget(label_ventasTotales, 0, 1, 1, 1);

        label_te_t = new QLabel(groupBox_resumen);
        label_te_t->setObjectName(QString::fromUtf8("label_te_t"));

        gridLayout_res->addWidget(label_te_t, 1, 0, 1, 1);

        labelEfectivoTotal = new QLabel(groupBox_resumen);
        labelEfectivoTotal->setObjectName(QString::fromUtf8("labelEfectivoTotal"));
        labelEfectivoTotal->setAlignment(Qt::AlignRight);

        gridLayout_res->addWidget(labelEfectivoTotal, 1, 1, 1, 1);

        label_er_t = new QLabel(groupBox_resumen);
        label_er_t->setObjectName(QString::fromUtf8("label_er_t"));

        gridLayout_res->addWidget(label_er_t, 2, 0, 1, 1);

        labelEfectivoR = new QLabel(groupBox_resumen);
        labelEfectivoR->setObjectName(QString::fromUtf8("labelEfectivoR"));
        labelEfectivoR->setAlignment(Qt::AlignRight);

        gridLayout_res->addWidget(labelEfectivoR, 2, 1, 1, 1);

        label_1_1 = new QLabel(groupBox_resumen);
        label_1_1->setObjectName(QString::fromUtf8("label_1_1"));

        gridLayout_res->addWidget(label_1_1, 3, 0, 1, 1);

        labelDescuadre = new QLabel(groupBox_resumen);
        labelDescuadre->setObjectName(QString::fromUtf8("labelDescuadre"));
        labelDescuadre->setAlignment(Qt::AlignRight);

        gridLayout_res->addWidget(labelDescuadre, 3, 1, 1, 1);

        label_21 = new QLabel(groupBox_resumen);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setVisible(false);

        gridLayout_res->addWidget(label_21, 4, 0, 1, 1);


        rightPanel->addWidget(groupBox_resumen);

        h_info = new QHBoxLayout();
        h_info->setObjectName(QString::fromUtf8("h_info"));
        label_sa_t = new QLabel(Cajas);
        label_sa_t->setObjectName(QString::fromUtf8("label_sa_t"));

        h_info->addWidget(label_sa_t);

        labelSaldoAnterior = new QLabel(Cajas);
        labelSaldoAnterior->setObjectName(QString::fromUtf8("labelSaldoAnterior"));

        h_info->addWidget(labelSaldoAnterior);

        h_s = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        h_info->addItem(h_s);


        rightPanel->addLayout(h_info);

        h_buttons = new QHBoxLayout();
        h_buttons->setObjectName(QString::fromUtf8("h_buttons"));
        pushButtonRetirarDinero = new QPushButton(Cajas);
        pushButtonRetirarDinero->setObjectName(QString::fromUtf8("pushButtonRetirarDinero"));

        h_buttons->addWidget(pushButtonRetirarDinero);

        h_s2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        h_buttons->addItem(h_s2);

        pushButtonCerrar = new QPushButton(Cajas);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));

        h_buttons->addWidget(pushButtonCerrar);

        pushButtonAceptar = new QPushButton(Cajas);
        pushButtonAceptar->setObjectName(QString::fromUtf8("pushButtonAceptar"));

        h_buttons->addWidget(pushButtonAceptar);


        rightPanel->addLayout(h_buttons);


        mainContent->addLayout(rightPanel);


        verticalLayout->addLayout(mainContent);


        retranslateUi(Cajas);

        QMetaObject::connectSlotsByName(Cajas);
    } // setupUi

    void retranslateUi(QDialog *Cajas)
    {
        Cajas->setWindowTitle(QCoreApplication::translate("Cajas", "Arqueo de Caja", nullptr));
        headerTitle->setText(QCoreApplication::translate("Cajas", "ARQUEO DE CAJA", nullptr));
        label_u->setText(QCoreApplication::translate("Cajas", "\303\232ltimo arqueo:", nullptr));
        label_u->setStyleSheet(QCoreApplication::translate("Cajas", "color: #e8f5e9;", nullptr));
        labelFUArqueo->setText(QCoreApplication::translate("Cajas", "-", nullptr));
        labelFUArqueo->setStyleSheet(QCoreApplication::translate("Cajas", "color: white; font-weight: bold;", nullptr));
        groupBox_efectivo->setTitle(QCoreApplication::translate("Cajas", "CONTEO DE EFECTIVO", nullptr));
        label_500->setText(QCoreApplication::translate("Cajas", "500 \342\202\254", nullptr));
        label500->setText(QCoreApplication::translate("Cajas", "0.00", nullptr));
        label_200_2->setText(QCoreApplication::translate("Cajas", "200 \342\202\254", nullptr));
        label200->setText(QCoreApplication::translate("Cajas", "0.00", nullptr));
        label_100_2->setText(QCoreApplication::translate("Cajas", "100 \342\202\254", nullptr));
        label100->setText(QCoreApplication::translate("Cajas", "0.00", nullptr));
        label_50_2->setText(QCoreApplication::translate("Cajas", "50 \342\202\254", nullptr));
        label50->setText(QCoreApplication::translate("Cajas", "0.00", nullptr));
        label_20_2->setText(QCoreApplication::translate("Cajas", "20 \342\202\254", nullptr));
        label20->setText(QCoreApplication::translate("Cajas", "0.00", nullptr));
        label_10_2->setText(QCoreApplication::translate("Cajas", "10 \342\202\254", nullptr));
        label10->setText(QCoreApplication::translate("Cajas", "0.00", nullptr));
        label_5_2->setText(QCoreApplication::translate("Cajas", "5 \342\202\254", nullptr));
        label5->setText(QCoreApplication::translate("Cajas", "0.00", nullptr));
        label_2_2->setText(QCoreApplication::translate("Cajas", "2 \342\202\254", nullptr));
        label2->setText(QCoreApplication::translate("Cajas", "0.00", nullptr));
        label_1_2->setText(QCoreApplication::translate("Cajas", "1 \342\202\254", nullptr));
        label1->setText(QCoreApplication::translate("Cajas", "0.00", nullptr));
        label_50c_2->setText(QCoreApplication::translate("Cajas", "0.50 \342\202\254", nullptr));
        label50c->setText(QCoreApplication::translate("Cajas", "0.00", nullptr));
        label_20c_2->setText(QCoreApplication::translate("Cajas", "0.20 \342\202\254", nullptr));
        label20c->setText(QCoreApplication::translate("Cajas", "0.00", nullptr));
        label_10c_2->setText(QCoreApplication::translate("Cajas", "0.10 \342\202\254", nullptr));
        label10c->setText(QCoreApplication::translate("Cajas", "0.00", nullptr));
        label_5c_2->setText(QCoreApplication::translate("Cajas", "0.05 \342\202\254", nullptr));
        label5c->setText(QCoreApplication::translate("Cajas", "0.00", nullptr));
        label_2c_2->setText(QCoreApplication::translate("Cajas", "0.02 \342\202\254", nullptr));
        label2c->setText(QCoreApplication::translate("Cajas", "0.00", nullptr));
        label_1c_2->setText(QCoreApplication::translate("Cajas", "0.01 \342\202\254", nullptr));
        label1c->setText(QCoreApplication::translate("Cajas", "0.00", nullptr));
        groupBox_sistema->setTitle(QCoreApplication::translate("Cajas", "DATOS DEL SISTEMA", nullptr));
        label_ve->setText(QCoreApplication::translate("Cajas", "Ventas Efectivo:", nullptr));
        label_ventasEfectivo->setStyleSheet(QCoreApplication::translate("Cajas", "font-weight: bold;", nullptr));
        label_1_t->setText(QCoreApplication::translate("Cajas", "Ventas B:", nullptr));
        label_ventasB->setStyleSheet(QCoreApplication::translate("Cajas", "font-weight: bold;", nullptr));
        label_vt_t->setText(QCoreApplication::translate("Cajas", "Tarjeta (Sistema):", nullptr));
        labelVentasTarjeta->setStyleSheet(QCoreApplication::translate("Cajas", "font-weight: bold;", nullptr));
        label_nt_t->setText(QCoreApplication::translate("Cajas", "N\302\272 Op. Tarjeta:", nullptr));
        labelNumeroTarjetas->setStyleSheet(QCoreApplication::translate("Cajas", "font-weight: bold;", nullptr));
        label_sal_t->setText(QCoreApplication::translate("Cajas", "Salidas / Gastos:", nullptr));
        labelEntradas->setStyleSheet(QCoreApplication::translate("Cajas", "font-weight: bold;", nullptr));
        groupBox_manual->setTitle(QCoreApplication::translate("Cajas", "ENTRADA MANUAL", nullptr));
        label_ma_t->setText(QCoreApplication::translate("Cajas", "Ventas Tarjeta (F\303\255sico):", nullptr));
        label_ci_t->setText(QCoreApplication::translate("Cajas", "Cambio Inicial:", nullptr));
        label_cf_t->setText(QCoreApplication::translate("Cajas", "Caja Fuerte:", nullptr));
        groupBox_resumen->setTitle(QCoreApplication::translate("Cajas", "RESUMEN ARQUEO", nullptr));
        label_tv_t->setText(QCoreApplication::translate("Cajas", "TOTAL VENTAS:", nullptr));
        label_ventasTotales->setStyleSheet(QCoreApplication::translate("Cajas", "font-size: 12pt; font-weight: bold;", nullptr));
        label_te_t->setText(QCoreApplication::translate("Cajas", "SALDO TE\303\223RICO CAJA:", nullptr));
        labelEfectivoTotal->setStyleSheet(QCoreApplication::translate("Cajas", "font-size: 12pt; font-weight: bold;", nullptr));
        label_er_t->setText(QCoreApplication::translate("Cajas", "EFECTIVO REAL:", nullptr));
        labelEfectivoR->setStyleSheet(QCoreApplication::translate("Cajas", "font-size: 14pt; font-weight: bold; color: #2e7d32;", nullptr));
        label_1_1->setText(QCoreApplication::translate("Cajas", "DESCUADRE:", nullptr));
        labelDescuadre->setStyleSheet(QCoreApplication::translate("Cajas", "font-size: 14pt; font-weight: bold;", nullptr));
        label_21->setText(QCoreApplication::translate("Cajas", "Indicador B:", nullptr));
        label_sa_t->setText(QCoreApplication::translate("Cajas", "Saldo Inicial:", nullptr));
        labelSaldoAnterior->setStyleSheet(QCoreApplication::translate("Cajas", "font-weight: bold;", nullptr));
        pushButtonRetirarDinero->setText(QCoreApplication::translate("Cajas", "Retirar Dinero", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("Cajas", "Cerrar", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("Cajas", "ACEPTAR", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Cajas: public Ui_Cajas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAJAS_H
