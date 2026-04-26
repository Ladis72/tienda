/********************************************************************************
** Form generated from reading UI file 'entradamercancia.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENTRADAMERCANCIA_H
#define UI_ENTRADAMERCANCIA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
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
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EntradaMercancia
{
public:
    QVBoxLayout *mainVerticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *labelTitle;
    QSpacerItem *horizontalSpacerHeader;
    QLabel *lbProductos;
    QFrame *sourceFrame;
    QHBoxLayout *horizontalLayoutSrc;
    QLabel *label_6;
    QComboBox *comboBoxProcedencia;
    QSpacerItem *horizontalSpacerSrc;
    QTableView *tableView;
    QFrame *entryFrame;
    QGridLayout *gridLayout_Entry;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *lineEditCod;
    QLineEdit *lineEditDesc;
    QLineEdit *lineEditUds;
    QDateEdit *dateEditCaducidad;
    QLineEdit *lineEditPVP;
    QHBoxLayout *horizontalLayoutActions;
    QPushButton *pushButtonAgregarLinea;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonAceptar;
    QSpacerItem *horizontalSpacerActions;
    QPushButton *pushButtonSalir;

    void setupUi(QDialog *EntradaMercancia)
    {
        if (EntradaMercancia->objectName().isEmpty())
            EntradaMercancia->setObjectName("EntradaMercancia");
        EntradaMercancia->resize(1100, 750);
        EntradaMercancia->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        mainVerticalLayout = new QVBoxLayout(EntradaMercancia);
        mainVerticalLayout->setSpacing(15);
        mainVerticalLayout->setObjectName("mainVerticalLayout");
        mainVerticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(EntradaMercancia);
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
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/entradas.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelTitle = new QLabel(headerFrame);
        labelTitle->setObjectName("labelTitle");
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelTitle->setFont(font);
        labelTitle->setStyleSheet(QString::fromUtf8("color: #1b5e20;"));

        horizontalLayoutHeader->addWidget(labelTitle);

        horizontalSpacerHeader = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);

        lbProductos = new QLabel(headerFrame);
        lbProductos->setObjectName("lbProductos");
        QFont font1;
        font1.setPointSize(16);
        font1.setItalic(true);
        lbProductos->setFont(font1);
        lbProductos->setStyleSheet(QString::fromUtf8("color: #2e7d32;"));

        horizontalLayoutHeader->addWidget(lbProductos);


        mainVerticalLayout->addWidget(headerFrame);

        sourceFrame = new QFrame(EntradaMercancia);
        sourceFrame->setObjectName("sourceFrame");
        sourceFrame->setStyleSheet(QString::fromUtf8("QFrame#sourceFrame { background-color: white; border-radius: 5px; border: 1px solid #e0e0e0; }"));
        horizontalLayoutSrc = new QHBoxLayout(sourceFrame);
        horizontalLayoutSrc->setObjectName("horizontalLayoutSrc");
        label_6 = new QLabel(sourceFrame);
        label_6->setObjectName("label_6");

        horizontalLayoutSrc->addWidget(label_6);

        comboBoxProcedencia = new QComboBox(sourceFrame);
        comboBoxProcedencia->setObjectName("comboBoxProcedencia");
        comboBoxProcedencia->setMinimumSize(QSize(300, 0));

        horizontalLayoutSrc->addWidget(comboBoxProcedencia);

        horizontalSpacerSrc = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutSrc->addItem(horizontalSpacerSrc);


        mainVerticalLayout->addWidget(sourceFrame);

        tableView = new QTableView(EntradaMercancia);
        tableView->setObjectName("tableView");
        tableView->setAlternatingRowColors(true);
        tableView->setSortingEnabled(true);

        mainVerticalLayout->addWidget(tableView);

        entryFrame = new QFrame(EntradaMercancia);
        entryFrame->setObjectName("entryFrame");
        entryFrame->setStyleSheet(QString::fromUtf8("QFrame#entryFrame { background-color: #f1f8e9; border-radius: 5px; border: 1px solid #c8e6c9; }"));
        gridLayout_Entry = new QGridLayout(entryFrame);
        gridLayout_Entry->setObjectName("gridLayout_Entry");
        label = new QLabel(entryFrame);
        label->setObjectName("label");

        gridLayout_Entry->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(entryFrame);
        label_2->setObjectName("label_2");

        gridLayout_Entry->addWidget(label_2, 0, 1, 1, 1);

        label_3 = new QLabel(entryFrame);
        label_3->setObjectName("label_3");

        gridLayout_Entry->addWidget(label_3, 0, 2, 1, 1);

        label_4 = new QLabel(entryFrame);
        label_4->setObjectName("label_4");

        gridLayout_Entry->addWidget(label_4, 0, 3, 1, 1);

        label_5 = new QLabel(entryFrame);
        label_5->setObjectName("label_5");

        gridLayout_Entry->addWidget(label_5, 0, 4, 1, 1);

        lineEditCod = new QLineEdit(entryFrame);
        lineEditCod->setObjectName("lineEditCod");
        lineEditCod->setMaximumSize(QSize(150, 16777215));

        gridLayout_Entry->addWidget(lineEditCod, 1, 0, 1, 1);

        lineEditDesc = new QLineEdit(entryFrame);
        lineEditDesc->setObjectName("lineEditDesc");

        gridLayout_Entry->addWidget(lineEditDesc, 1, 1, 1, 1);

        lineEditUds = new QLineEdit(entryFrame);
        lineEditUds->setObjectName("lineEditUds");
        lineEditUds->setMaximumSize(QSize(60, 16777215));

        gridLayout_Entry->addWidget(lineEditUds, 1, 2, 1, 1);

        dateEditCaducidad = new QDateEdit(entryFrame);
        dateEditCaducidad->setObjectName("dateEditCaducidad");
        dateEditCaducidad->setCalendarPopup(true);

        gridLayout_Entry->addWidget(dateEditCaducidad, 1, 3, 1, 1);

        lineEditPVP = new QLineEdit(entryFrame);
        lineEditPVP->setObjectName("lineEditPVP");
        lineEditPVP->setMaximumSize(QSize(100, 16777215));

        gridLayout_Entry->addWidget(lineEditPVP, 1, 4, 1, 1);


        mainVerticalLayout->addWidget(entryFrame);

        horizontalLayoutActions = new QHBoxLayout();
        horizontalLayoutActions->setObjectName("horizontalLayoutActions");
        pushButtonAgregarLinea = new QPushButton(EntradaMercancia);
        pushButtonAgregarLinea->setObjectName("pushButtonAgregarLinea");
        pushButtonAgregarLinea->setMinimumSize(QSize(120, 40));
        pushButtonAgregarLinea->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #43a047; color: white; font-weight: bold; }"));

        horizontalLayoutActions->addWidget(pushButtonAgregarLinea);

        pushButtonBorrar = new QPushButton(EntradaMercancia);
        pushButtonBorrar->setObjectName("pushButtonBorrar");
        pushButtonBorrar->setMinimumSize(QSize(120, 40));

        horizontalLayoutActions->addWidget(pushButtonBorrar);

        pushButtonAceptar = new QPushButton(EntradaMercancia);
        pushButtonAceptar->setObjectName("pushButtonAceptar");
        pushButtonAceptar->setMinimumSize(QSize(180, 40));
        pushButtonAceptar->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #2e7d32; color: white; font-weight: bold; }"));

        horizontalLayoutActions->addWidget(pushButtonAceptar);

        horizontalSpacerActions = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutActions->addItem(horizontalSpacerActions);

        pushButtonSalir = new QPushButton(EntradaMercancia);
        pushButtonSalir->setObjectName("pushButtonSalir");
        pushButtonSalir->setMinimumSize(QSize(100, 40));

        horizontalLayoutActions->addWidget(pushButtonSalir);


        mainVerticalLayout->addLayout(horizontalLayoutActions);


        retranslateUi(EntradaMercancia);

        QMetaObject::connectSlotsByName(EntradaMercancia);
    } // setupUi

    void retranslateUi(QDialog *EntradaMercancia)
    {
        EntradaMercancia->setWindowTitle(QCoreApplication::translate("EntradaMercancia", "Entrada de Mercanc\303\255a / Almac\303\251n", nullptr));
        labelTitle->setText(QCoreApplication::translate("EntradaMercancia", "Entrada de Mercanc\303\255a", nullptr));
        lbProductos->setText(QCoreApplication::translate("EntradaMercancia", "Estado", nullptr));
        label_6->setText(QCoreApplication::translate("EntradaMercancia", "Procedencia / Proveedor:", nullptr));
        label->setText(QCoreApplication::translate("EntradaMercancia", "Cod.", nullptr));
        label_2->setText(QCoreApplication::translate("EntradaMercancia", "Descripci\303\263n", nullptr));
        label_3->setText(QCoreApplication::translate("EntradaMercancia", "Cant.", nullptr));
        label_4->setText(QCoreApplication::translate("EntradaMercancia", "Fecha Caducidad", nullptr));
        label_5->setText(QCoreApplication::translate("EntradaMercancia", "P.V.P.", nullptr));
        dateEditCaducidad->setDisplayFormat(QCoreApplication::translate("EntradaMercancia", "yyyy-MM-dd", nullptr));
        pushButtonAgregarLinea->setText(QCoreApplication::translate("EntradaMercancia", "A\303\261adir L\303\255nea", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("EntradaMercancia", "Borrar L\303\255nea", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("EntradaMercancia", "Aceptar Entradas", nullptr));
        pushButtonSalir->setText(QCoreApplication::translate("EntradaMercancia", "Salir", nullptr));
#if QT_CONFIG(shortcut)
        pushButtonSalir->setShortcut(QCoreApplication::translate("EntradaMercancia", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
    } // retranslateUi

};

namespace Ui {
    class EntradaMercancia: public Ui_EntradaMercancia {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENTRADAMERCANCIA_H
