/********************************************************************************
** Form generated from reading UI file 'cajas.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_Cajas
{
public:
    QGridLayout *gridLayout_2;
    QLabel *label_31;
    QLabel *labelFUArqueo;
    QFrame *frame;
    QGridLayout *gridLayout;
    QDoubleSpinBox *doubleSpinBoxCambios;
    QLabel *label_23;
    QLabel *label_22;
    QLabel *label_24;
    QDoubleSpinBox *doubleSpinBoxTarjetas;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_5;
    QLabel *label_4;
    QSpinBox *spinBox500;
    QLabel *label_5;
    QSpinBox *spinBox20;
    QLabel *label_7;
    QSpinBox *spinBox100;
    QLabel *label_6;
    QSpinBox *spinBox200;
    QSpinBox *spinBox50;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_3;
    QSpinBox *spinBox5;
    QSpinBox *spinBox10;
    QLabel *label500;
    QLabel *label200;
    QLabel *label100;
    QLabel *label50;
    QLabel *label20;
    QLabel *label10;
    QLabel *label5;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_7;
    QSpinBox *spinBox5c;
    QSpinBox *spinBox20c;
    QSpinBox *spinBox1c;
    QLabel *label_10;
    QLabel *label_16;
    QLabel *label_14;
    QSpinBox *spinBox2;
    QLabel *label_15;
    QSpinBox *spinBox50c;
    QLabel *label_11;
    QLabel *label_17;
    QSpinBox *spinBox2c;
    QLabel *label_13;
    QLabel *label_12;
    QSpinBox *spinBox1;
    QSpinBox *spinBox10c;
    QLabel *label2;
    QLabel *label1;
    QLabel *label50c;
    QLabel *label20c;
    QLabel *label10c;
    QLabel *label5c;
    QLabel *label2c;
    QLabel *label1c;
    QDoubleSpinBox *doubleSpinBoxCajaF;
    QFrame *frame_2;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_6;
    QLabel *label_2;
    QLabel *label;
    QLabel *labelNumeroTarjetas;
    QLabel *label_29;
    QLabel *labelSaldoAnterior;
    QLabel *labelEfectivoTotal;
    QLabel *label_ventasTarjeta;
    QLabel *label_20;
    QLabel *label_19;
    QLabel *label_27;
    QLabel *labelEntradas;
    QLabel *label_28;
    QSpacerItem *verticalSpacer;
    QLabel *labelEfectivoR;
    QLabel *labelDescuadre;
    QLabel *label_18;
    QLabel *label_ventasEfectivo;
    QLabel *label_21;
    QLabel *label_ventasB;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_4;
    QLabel *label_ventasTotales;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonCerrar;
    QPushButton *pushButtonRetirarDinero;
    QPushButton *pushButtonAceptar;

    void setupUi(QDialog *Cajas)
    {
        if (Cajas->objectName().isEmpty())
            Cajas->setObjectName(QString::fromUtf8("Cajas"));
        Cajas->resize(665, 506);
        gridLayout_2 = new QGridLayout(Cajas);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_31 = new QLabel(Cajas);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        gridLayout_2->addWidget(label_31, 0, 0, 1, 1);

        labelFUArqueo = new QLabel(Cajas);
        labelFUArqueo->setObjectName(QString::fromUtf8("labelFUArqueo"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        labelFUArqueo->setFont(font);
        labelFUArqueo->setStyleSheet(QString::fromUtf8("color: rgb(30, 90, 255);"));

        gridLayout_2->addWidget(labelFUArqueo, 0, 1, 1, 1);

        frame = new QFrame(Cajas);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        doubleSpinBoxCambios = new QDoubleSpinBox(frame);
        doubleSpinBoxCambios->setObjectName(QString::fromUtf8("doubleSpinBoxCambios"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Fira Sans"));
        doubleSpinBoxCambios->setFont(font1);
        doubleSpinBoxCambios->setCorrectionMode(QAbstractSpinBox::CorrectionMode::CorrectToPreviousValue);
        doubleSpinBoxCambios->setProperty("showGroupSeparator", QVariant(false));
        doubleSpinBoxCambios->setMaximum(9999.989999999999782);

        gridLayout->addWidget(doubleSpinBoxCambios, 1, 3, 1, 1);

        label_23 = new QLabel(frame);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Fira Sans"));
        font2.setBold(true);
        label_23->setFont(font2);

        gridLayout->addWidget(label_23, 3, 0, 1, 1);

        label_22 = new QLabel(frame);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setFont(font2);

        gridLayout->addWidget(label_22, 1, 0, 1, 1);

        label_24 = new QLabel(frame);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setFont(font2);

        gridLayout->addWidget(label_24, 2, 0, 1, 1);

        doubleSpinBoxTarjetas = new QDoubleSpinBox(frame);
        doubleSpinBoxTarjetas->setObjectName(QString::fromUtf8("doubleSpinBoxTarjetas"));
        doubleSpinBoxTarjetas->setFont(font1);
        doubleSpinBoxTarjetas->setMaximum(9999.989999999999782);

        gridLayout->addWidget(doubleSpinBoxTarjetas, 2, 3, 1, 1);

        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFont(font1);
        gridLayout_5 = new QGridLayout(groupBox);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font2);

        gridLayout_5->addWidget(label_4, 2, 0, 1, 1);

        spinBox500 = new QSpinBox(groupBox);
        spinBox500->setObjectName(QString::fromUtf8("spinBox500"));

        gridLayout_5->addWidget(spinBox500, 0, 1, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font2);

        gridLayout_5->addWidget(label_5, 3, 0, 1, 1);

        spinBox20 = new QSpinBox(groupBox);
        spinBox20->setObjectName(QString::fromUtf8("spinBox20"));

        gridLayout_5->addWidget(spinBox20, 6, 1, 1, 1);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font2);

        gridLayout_5->addWidget(label_7, 6, 0, 1, 1);

        spinBox100 = new QSpinBox(groupBox);
        spinBox100->setObjectName(QString::fromUtf8("spinBox100"));

        gridLayout_5->addWidget(spinBox100, 3, 1, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font2);

        gridLayout_5->addWidget(label_6, 5, 0, 1, 1);

        spinBox200 = new QSpinBox(groupBox);
        spinBox200->setObjectName(QString::fromUtf8("spinBox200"));

        gridLayout_5->addWidget(spinBox200, 2, 1, 1, 1);

        spinBox50 = new QSpinBox(groupBox);
        spinBox50->setObjectName(QString::fromUtf8("spinBox50"));

        gridLayout_5->addWidget(spinBox50, 5, 1, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font2);

        gridLayout_5->addWidget(label_8, 7, 0, 1, 1);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font2);

        gridLayout_5->addWidget(label_9, 8, 0, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font2);

        gridLayout_5->addWidget(label_3, 0, 0, 1, 1);

        spinBox5 = new QSpinBox(groupBox);
        spinBox5->setObjectName(QString::fromUtf8("spinBox5"));

        gridLayout_5->addWidget(spinBox5, 8, 1, 1, 1);

        spinBox10 = new QSpinBox(groupBox);
        spinBox10->setObjectName(QString::fromUtf8("spinBox10"));

        gridLayout_5->addWidget(spinBox10, 7, 1, 1, 1);

        label500 = new QLabel(groupBox);
        label500->setObjectName(QString::fromUtf8("label500"));

        gridLayout_5->addWidget(label500, 0, 2, 1, 1);

        label200 = new QLabel(groupBox);
        label200->setObjectName(QString::fromUtf8("label200"));

        gridLayout_5->addWidget(label200, 2, 2, 1, 1);

        label100 = new QLabel(groupBox);
        label100->setObjectName(QString::fromUtf8("label100"));

        gridLayout_5->addWidget(label100, 3, 2, 1, 1);

        label50 = new QLabel(groupBox);
        label50->setObjectName(QString::fromUtf8("label50"));

        gridLayout_5->addWidget(label50, 5, 2, 1, 1);

        label20 = new QLabel(groupBox);
        label20->setObjectName(QString::fromUtf8("label20"));

        gridLayout_5->addWidget(label20, 6, 2, 1, 1);

        label10 = new QLabel(groupBox);
        label10->setObjectName(QString::fromUtf8("label10"));

        gridLayout_5->addWidget(label10, 7, 2, 1, 1);

        label5 = new QLabel(groupBox);
        label5->setObjectName(QString::fromUtf8("label5"));

        gridLayout_5->addWidget(label5, 8, 2, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(frame);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setFont(font1);
        gridLayout_7 = new QGridLayout(groupBox_2);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        spinBox5c = new QSpinBox(groupBox_2);
        spinBox5c->setObjectName(QString::fromUtf8("spinBox5c"));

        gridLayout_7->addWidget(spinBox5c, 7, 1, 1, 1);

        spinBox20c = new QSpinBox(groupBox_2);
        spinBox20c->setObjectName(QString::fromUtf8("spinBox20c"));

        gridLayout_7->addWidget(spinBox20c, 5, 1, 1, 1);

        spinBox1c = new QSpinBox(groupBox_2);
        spinBox1c->setObjectName(QString::fromUtf8("spinBox1c"));

        gridLayout_7->addWidget(spinBox1c, 9, 1, 1, 1);

        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setFont(font2);

        gridLayout_7->addWidget(label_10, 4, 0, 1, 1);

        label_16 = new QLabel(groupBox_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setFont(font2);

        gridLayout_7->addWidget(label_16, 6, 0, 1, 1);

        label_14 = new QLabel(groupBox_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setFont(font2);

        gridLayout_7->addWidget(label_14, 0, 0, 1, 1);

        spinBox2 = new QSpinBox(groupBox_2);
        spinBox2->setObjectName(QString::fromUtf8("spinBox2"));

        gridLayout_7->addWidget(spinBox2, 0, 1, 1, 1);

        label_15 = new QLabel(groupBox_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setFont(font2);

        gridLayout_7->addWidget(label_15, 7, 0, 1, 1);

        spinBox50c = new QSpinBox(groupBox_2);
        spinBox50c->setObjectName(QString::fromUtf8("spinBox50c"));

        gridLayout_7->addWidget(spinBox50c, 4, 1, 1, 1);

        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font2);

        gridLayout_7->addWidget(label_11, 8, 0, 1, 1);

        label_17 = new QLabel(groupBox_2);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setFont(font2);

        gridLayout_7->addWidget(label_17, 9, 0, 1, 1);

        spinBox2c = new QSpinBox(groupBox_2);
        spinBox2c->setObjectName(QString::fromUtf8("spinBox2c"));

        gridLayout_7->addWidget(spinBox2c, 8, 1, 1, 1);

        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setFont(font2);

        gridLayout_7->addWidget(label_13, 3, 0, 1, 1);

        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setFont(font2);

        gridLayout_7->addWidget(label_12, 5, 0, 1, 1);

        spinBox1 = new QSpinBox(groupBox_2);
        spinBox1->setObjectName(QString::fromUtf8("spinBox1"));

        gridLayout_7->addWidget(spinBox1, 3, 1, 1, 1);

        spinBox10c = new QSpinBox(groupBox_2);
        spinBox10c->setObjectName(QString::fromUtf8("spinBox10c"));

        gridLayout_7->addWidget(spinBox10c, 6, 1, 1, 1);

        label2 = new QLabel(groupBox_2);
        label2->setObjectName(QString::fromUtf8("label2"));

        gridLayout_7->addWidget(label2, 0, 2, 1, 1);

        label1 = new QLabel(groupBox_2);
        label1->setObjectName(QString::fromUtf8("label1"));

        gridLayout_7->addWidget(label1, 3, 2, 1, 1);

        label50c = new QLabel(groupBox_2);
        label50c->setObjectName(QString::fromUtf8("label50c"));

        gridLayout_7->addWidget(label50c, 4, 2, 1, 1);

        label20c = new QLabel(groupBox_2);
        label20c->setObjectName(QString::fromUtf8("label20c"));

        gridLayout_7->addWidget(label20c, 5, 2, 1, 1);

        label10c = new QLabel(groupBox_2);
        label10c->setObjectName(QString::fromUtf8("label10c"));

        gridLayout_7->addWidget(label10c, 6, 2, 1, 1);

        label5c = new QLabel(groupBox_2);
        label5c->setObjectName(QString::fromUtf8("label5c"));

        gridLayout_7->addWidget(label5c, 7, 2, 1, 1);

        label2c = new QLabel(groupBox_2);
        label2c->setObjectName(QString::fromUtf8("label2c"));

        gridLayout_7->addWidget(label2c, 8, 2, 1, 1);

        label1c = new QLabel(groupBox_2);
        label1c->setObjectName(QString::fromUtf8("label1c"));

        gridLayout_7->addWidget(label1c, 9, 2, 1, 1);


        gridLayout->addWidget(groupBox_2, 0, 3, 1, 1);

        doubleSpinBoxCajaF = new QDoubleSpinBox(frame);
        doubleSpinBoxCajaF->setObjectName(QString::fromUtf8("doubleSpinBoxCajaF"));
        doubleSpinBoxCajaF->setFont(font1);
        doubleSpinBoxCajaF->setMaximum(9999.989999999999782);

        gridLayout->addWidget(doubleSpinBoxCajaF, 3, 3, 1, 1);

        groupBox->raise();
        groupBox_2->raise();
        label_22->raise();
        label_23->raise();
        doubleSpinBoxCambios->raise();
        doubleSpinBoxCajaF->raise();
        label_24->raise();
        doubleSpinBoxTarjetas->raise();

        gridLayout_2->addWidget(frame, 1, 0, 2, 1);

        frame_2 = new QFrame(Cajas);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        gridLayout_3 = new QGridLayout(frame_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox_3 = new QGroupBox(frame_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setFont(font1);
        gridLayout_6 = new QGridLayout(groupBox_3);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Fira Sans"));
        font3.setPointSize(12);
        font3.setBold(false);
        label_2->setFont(font3);

        gridLayout_6->addWidget(label_2, 3, 0, 1, 1);

        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font3);

        gridLayout_6->addWidget(label, 1, 0, 1, 1);

        labelNumeroTarjetas = new QLabel(groupBox_3);
        labelNumeroTarjetas->setObjectName(QString::fromUtf8("labelNumeroTarjetas"));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Fira Sans"));
        font4.setPointSize(12);
        font4.setBold(true);
        labelNumeroTarjetas->setFont(font4);

        gridLayout_6->addWidget(labelNumeroTarjetas, 2, 1, 1, 1);

        label_29 = new QLabel(groupBox_3);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setFont(font3);

        gridLayout_6->addWidget(label_29, 7, 0, 1, 1);

        labelSaldoAnterior = new QLabel(groupBox_3);
        labelSaldoAnterior->setObjectName(QString::fromUtf8("labelSaldoAnterior"));
        labelSaldoAnterior->setFont(font4);

        gridLayout_6->addWidget(labelSaldoAnterior, 4, 1, 1, 1);

        labelEfectivoTotal = new QLabel(groupBox_3);
        labelEfectivoTotal->setObjectName(QString::fromUtf8("labelEfectivoTotal"));
        labelEfectivoTotal->setFont(font4);
        labelEfectivoTotal->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));

        gridLayout_6->addWidget(labelEfectivoTotal, 7, 1, 1, 1);

        label_ventasTarjeta = new QLabel(groupBox_3);
        label_ventasTarjeta->setObjectName(QString::fromUtf8("label_ventasTarjeta"));
        label_ventasTarjeta->setFont(font4);
        label_ventasTarjeta->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));

        gridLayout_6->addWidget(label_ventasTarjeta, 3, 1, 1, 1);

        label_20 = new QLabel(groupBox_3);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setFont(font3);

        gridLayout_6->addWidget(label_20, 5, 0, 1, 1);

        label_19 = new QLabel(groupBox_3);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setFont(font3);

        gridLayout_6->addWidget(label_19, 4, 0, 1, 1);

        label_27 = new QLabel(groupBox_3);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setFont(font3);

        gridLayout_6->addWidget(label_27, 9, 0, 1, 1);

        labelEntradas = new QLabel(groupBox_3);
        labelEntradas->setObjectName(QString::fromUtf8("labelEntradas"));
        labelEntradas->setFont(font4);
        labelEntradas->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));

        gridLayout_6->addWidget(labelEntradas, 5, 1, 1, 1);

        label_28 = new QLabel(groupBox_3);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setFont(font3);

        gridLayout_6->addWidget(label_28, 8, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(verticalSpacer, 6, 0, 1, 1);

        labelEfectivoR = new QLabel(groupBox_3);
        labelEfectivoR->setObjectName(QString::fromUtf8("labelEfectivoR"));
        labelEfectivoR->setFont(font4);
        labelEfectivoR->setStyleSheet(QString::fromUtf8("color: rgb(0, 170, 0);"));

        gridLayout_6->addWidget(labelEfectivoR, 8, 1, 1, 1);

        labelDescuadre = new QLabel(groupBox_3);
        labelDescuadre->setObjectName(QString::fromUtf8("labelDescuadre"));
        labelDescuadre->setFont(font4);

        gridLayout_6->addWidget(labelDescuadre, 9, 1, 1, 1);

        label_18 = new QLabel(groupBox_3);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setFont(font3);

        gridLayout_6->addWidget(label_18, 2, 0, 1, 1);

        label_ventasEfectivo = new QLabel(groupBox_3);
        label_ventasEfectivo->setObjectName(QString::fromUtf8("label_ventasEfectivo"));
        label_ventasEfectivo->setFont(font4);
        label_ventasEfectivo->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        label_ventasEfectivo->setFrameShape(QFrame::Shape::NoFrame);
        label_ventasEfectivo->setTextFormat(Qt::TextFormat::AutoText);

        gridLayout_6->addWidget(label_ventasEfectivo, 1, 1, 1, 1);

        label_21 = new QLabel(groupBox_3);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setEnabled(true);
        label_21->setFont(font3);

        gridLayout_6->addWidget(label_21, 0, 0, 1, 1);

        label_ventasB = new QLabel(groupBox_3);
        label_ventasB->setObjectName(QString::fromUtf8("label_ventasB"));
        label_ventasB->setFont(font4);

        gridLayout_6->addWidget(label_ventasB, 0, 1, 1, 1);

        groupBox_4 = new QGroupBox(groupBox_3);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_4 = new QGridLayout(groupBox_4);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_ventasTotales = new QLabel(groupBox_4);
        label_ventasTotales->setObjectName(QString::fromUtf8("label_ventasTotales"));
        label_ventasTotales->setFont(font4);
        label_ventasTotales->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 127)"));

        gridLayout_4->addWidget(label_ventasTotales, 0, 0, 1, 1);


        gridLayout_6->addWidget(groupBox_4, 0, 2, 2, 1);


        gridLayout_3->addWidget(groupBox_3, 1, 0, 1, 1);


        gridLayout_2->addWidget(frame_2, 1, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonCerrar = new QPushButton(Cajas);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));
        pushButtonCerrar->setFont(font1);

        horizontalLayout->addWidget(pushButtonCerrar);

        pushButtonRetirarDinero = new QPushButton(Cajas);
        pushButtonRetirarDinero->setObjectName(QString::fromUtf8("pushButtonRetirarDinero"));
        pushButtonRetirarDinero->setFont(font1);

        horizontalLayout->addWidget(pushButtonRetirarDinero);

        pushButtonAceptar = new QPushButton(Cajas);
        pushButtonAceptar->setObjectName(QString::fromUtf8("pushButtonAceptar"));
        pushButtonAceptar->setFont(font1);

        horizontalLayout->addWidget(pushButtonAceptar);


        gridLayout_2->addLayout(horizontalLayout, 2, 1, 1, 1);

#if QT_CONFIG(shortcut)
        label_23->setBuddy(doubleSpinBoxCajaF);
        label_22->setBuddy(doubleSpinBoxCambios);
        label_24->setBuddy(doubleSpinBoxTarjetas);
        label_4->setBuddy(spinBox200);
        label_5->setBuddy(spinBox100);
        label_7->setBuddy(spinBox20);
        label_6->setBuddy(spinBox50);
        label_8->setBuddy(spinBox10);
        label_9->setBuddy(spinBox5);
        label_3->setBuddy(spinBox500);
        label_10->setBuddy(spinBox50c);
        label_16->setBuddy(spinBox10c);
        label_14->setBuddy(spinBox2);
        label_15->setBuddy(spinBox5c);
        label_11->setBuddy(spinBox2c);
        label_17->setBuddy(spinBox1c);
        label_13->setBuddy(spinBox1);
        label_12->setBuddy(spinBox20c);
#endif // QT_CONFIG(shortcut)
        QWidget::setTabOrder(doubleSpinBoxCambios, doubleSpinBoxTarjetas);
        QWidget::setTabOrder(doubleSpinBoxTarjetas, doubleSpinBoxCajaF);
        QWidget::setTabOrder(doubleSpinBoxCajaF, spinBox500);
        QWidget::setTabOrder(spinBox500, spinBox200);
        QWidget::setTabOrder(spinBox200, spinBox100);
        QWidget::setTabOrder(spinBox100, spinBox50);
        QWidget::setTabOrder(spinBox50, spinBox20);
        QWidget::setTabOrder(spinBox20, spinBox10);
        QWidget::setTabOrder(spinBox10, spinBox5);
        QWidget::setTabOrder(spinBox5, spinBox2);
        QWidget::setTabOrder(spinBox2, spinBox1);
        QWidget::setTabOrder(spinBox1, spinBox50c);
        QWidget::setTabOrder(spinBox50c, spinBox20c);
        QWidget::setTabOrder(spinBox20c, spinBox10c);
        QWidget::setTabOrder(spinBox10c, spinBox5c);
        QWidget::setTabOrder(spinBox5c, spinBox2c);
        QWidget::setTabOrder(spinBox2c, spinBox1c);
        QWidget::setTabOrder(spinBox1c, pushButtonCerrar);
        QWidget::setTabOrder(pushButtonCerrar, pushButtonRetirarDinero);
        QWidget::setTabOrder(pushButtonRetirarDinero, pushButtonAceptar);

        retranslateUi(Cajas);
        QObject::connect(pushButtonCerrar, SIGNAL(clicked()), Cajas, SLOT(close()));

        QMetaObject::connectSlotsByName(Cajas);
    } // setupUi

    void retranslateUi(QDialog *Cajas)
    {
        Cajas->setWindowTitle(QCoreApplication::translate("Cajas", "Arqueo de caja", nullptr));
        label_31->setText(QCoreApplication::translate("Cajas", "Fecha \303\272ltimo arqueo", nullptr));
        labelFUArqueo->setText(QCoreApplication::translate("Cajas", "TextLabel", nullptr));
        label_23->setText(QCoreApplication::translate("Cajas", "Caja F.", nullptr));
        label_22->setText(QCoreApplication::translate("Cajas", "Ca&mbios", nullptr));
        label_24->setText(QCoreApplication::translate("Cajas", "Tar&jetas", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Cajas", "Billetes", nullptr));
        label_4->setText(QCoreApplication::translate("Cajas", "&200\342\202\254", nullptr));
        label_5->setText(QCoreApplication::translate("Cajas", "&100\342\202\254", nullptr));
        label_7->setText(QCoreApplication::translate("Cajas", "20\342\202\254", nullptr));
        label_6->setText(QCoreApplication::translate("Cajas", "5&0\342\202\254", nullptr));
        label_8->setText(QCoreApplication::translate("Cajas", "10\342\202\254", nullptr));
        label_9->setText(QCoreApplication::translate("Cajas", "5\342\202\254", nullptr));
        label_3->setText(QCoreApplication::translate("Cajas", "&500\342\202\254", nullptr));
        label500->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label200->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label100->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label50->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label20->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label10->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label5->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Cajas", "Monedas", nullptr));
        label_10->setText(QCoreApplication::translate("Cajas", "50c", nullptr));
        label_16->setText(QCoreApplication::translate("Cajas", "10c", nullptr));
        label_14->setText(QCoreApplication::translate("Cajas", "2\342\202\254", nullptr));
        label_15->setText(QCoreApplication::translate("Cajas", "5c", nullptr));
        label_11->setText(QCoreApplication::translate("Cajas", "2c", nullptr));
        label_17->setText(QCoreApplication::translate("Cajas", "1c", nullptr));
        label_13->setText(QCoreApplication::translate("Cajas", "1\342\202\254", nullptr));
        label_12->setText(QCoreApplication::translate("Cajas", "20c", nullptr));
        label2->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label1->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label50c->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label20c->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label10c->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label5c->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label2c->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label1c->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("Cajas", "Movimientos", nullptr));
        label_2->setText(QCoreApplication::translate("Cajas", "Ventas tarjeta", nullptr));
        label->setText(QCoreApplication::translate("Cajas", "Ventas efectivo", nullptr));
        labelNumeroTarjetas->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label_29->setText(QCoreApplication::translate("Cajas", "Efectivo total", nullptr));
        labelSaldoAnterior->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        labelEfectivoTotal->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label_ventasTarjeta->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label_20->setText(QCoreApplication::translate("Cajas", "Entradas", nullptr));
        label_19->setText(QCoreApplication::translate("Cajas", "Saldo anterior", nullptr));
        label_27->setText(QCoreApplication::translate("Cajas", "Descuadre", nullptr));
        labelEntradas->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label_28->setText(QCoreApplication::translate("Cajas", "Efectivo real", nullptr));
        labelEfectivoR->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        labelDescuadre->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label_18->setText(QCoreApplication::translate("Cajas", "N\303\272mero de tarjetas", nullptr));
        label_ventasEfectivo->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        label_21->setText(QCoreApplication::translate("Cajas", "Ventas B", nullptr));
        label_ventasB->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("Cajas", "Total ventas", nullptr));
        label_ventasTotales->setText(QCoreApplication::translate("Cajas", "0", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("Cajas", "Cerrar", nullptr));
        pushButtonRetirarDinero->setText(QCoreApplication::translate("Cajas", "Retirar dinero", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("Cajas", "Aceptar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Cajas: public Ui_Cajas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAJAS_H
