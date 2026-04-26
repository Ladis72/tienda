/********************************************************************************
** Form generated from reading UI file 'salidas.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SALIDAS_H
#define UI_SALIDAS_H

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

class Ui_Salidas
{
public:
    QVBoxLayout *mainVerticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *labelTitle;
    QSpacerItem *horizontalSpacerHeader;
    QLabel *lbSalidas;
    QFrame *destinationFrame;
    QHBoxLayout *horizontalLayoutDest;
    QLabel *label_6;
    QComboBox *comboBoxDestino;
    QSpacerItem *horizontalSpacerDest;
    QTableView *tableView;
    QFrame *entryFrame;
    QGridLayout *gridLayout_Entry;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_3;
    QLineEdit *lineEditCod;
    QLineEdit *lineEditDesc;
    QLineEdit *lineEditCantidad;
    QLineEdit *lineEditPrecio;
    QDateEdit *dateEditFC;
    QHBoxLayout *horizontalLayoutActions;
    QPushButton *pushButtonAgregar;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonEnviar;
    QSpacerItem *horizontalSpacerActions;
    QPushButton *pushButtonCerrar;

    void setupUi(QDialog *Salidas)
    {
        if (Salidas->objectName().isEmpty())
            Salidas->setObjectName("Salidas");
        Salidas->resize(1100, 700);
        Salidas->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        mainVerticalLayout = new QVBoxLayout(Salidas);
        mainVerticalLayout->setSpacing(15);
        mainVerticalLayout->setObjectName("mainVerticalLayout");
        mainVerticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(Salidas);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame {\n"
"    background-color: #fff3e0;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #ffe0b2;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName("horizontalLayoutHeader");
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName("labelIcono");
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/salidas.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelTitle = new QLabel(headerFrame);
        labelTitle->setObjectName("labelTitle");
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelTitle->setFont(font);
        labelTitle->setStyleSheet(QString::fromUtf8("color: #e65100;"));

        horizontalLayoutHeader->addWidget(labelTitle);

        horizontalSpacerHeader = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);

        lbSalidas = new QLabel(headerFrame);
        lbSalidas->setObjectName("lbSalidas");
        QFont font1;
        font1.setPointSize(16);
        font1.setItalic(true);
        lbSalidas->setFont(font1);
        lbSalidas->setStyleSheet(QString::fromUtf8("color: #f57c00;"));

        horizontalLayoutHeader->addWidget(lbSalidas);


        mainVerticalLayout->addWidget(headerFrame);

        destinationFrame = new QFrame(Salidas);
        destinationFrame->setObjectName("destinationFrame");
        destinationFrame->setStyleSheet(QString::fromUtf8("QFrame#destinationFrame { background-color: white; border-radius: 5px; border: 1px solid #e0e0e0; }"));
        horizontalLayoutDest = new QHBoxLayout(destinationFrame);
        horizontalLayoutDest->setObjectName("horizontalLayoutDest");
        label_6 = new QLabel(destinationFrame);
        label_6->setObjectName("label_6");

        horizontalLayoutDest->addWidget(label_6);

        comboBoxDestino = new QComboBox(destinationFrame);
        comboBoxDestino->setObjectName("comboBoxDestino");
        comboBoxDestino->setMinimumSize(QSize(250, 0));

        horizontalLayoutDest->addWidget(comboBoxDestino);

        horizontalSpacerDest = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutDest->addItem(horizontalSpacerDest);


        mainVerticalLayout->addWidget(destinationFrame);

        tableView = new QTableView(Salidas);
        tableView->setObjectName("tableView");
        tableView->setAlternatingRowColors(true);
        tableView->setSortingEnabled(true);

        mainVerticalLayout->addWidget(tableView);

        entryFrame = new QFrame(Salidas);
        entryFrame->setObjectName("entryFrame");
        entryFrame->setStyleSheet(QString::fromUtf8("QFrame#entryFrame { background-color: #f5f5f5; border-radius: 5px; border: 1px solid #e0e0e0; }"));
        gridLayout_Entry = new QGridLayout(entryFrame);
        gridLayout_Entry->setObjectName("gridLayout_Entry");
        label = new QLabel(entryFrame);
        label->setObjectName("label");

        gridLayout_Entry->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(entryFrame);
        label_2->setObjectName("label_2");

        gridLayout_Entry->addWidget(label_2, 0, 1, 1, 1);

        label_4 = new QLabel(entryFrame);
        label_4->setObjectName("label_4");

        gridLayout_Entry->addWidget(label_4, 0, 2, 1, 1);

        label_5 = new QLabel(entryFrame);
        label_5->setObjectName("label_5");

        gridLayout_Entry->addWidget(label_5, 0, 3, 1, 1);

        label_3 = new QLabel(entryFrame);
        label_3->setObjectName("label_3");

        gridLayout_Entry->addWidget(label_3, 0, 4, 1, 1);

        lineEditCod = new QLineEdit(entryFrame);
        lineEditCod->setObjectName("lineEditCod");
        lineEditCod->setMaximumSize(QSize(150, 16777215));

        gridLayout_Entry->addWidget(lineEditCod, 1, 0, 1, 1);

        lineEditDesc = new QLineEdit(entryFrame);
        lineEditDesc->setObjectName("lineEditDesc");

        gridLayout_Entry->addWidget(lineEditDesc, 1, 1, 1, 1);

        lineEditCantidad = new QLineEdit(entryFrame);
        lineEditCantidad->setObjectName("lineEditCantidad");
        lineEditCantidad->setMaximumSize(QSize(80, 16777215));

        gridLayout_Entry->addWidget(lineEditCantidad, 1, 2, 1, 1);

        lineEditPrecio = new QLineEdit(entryFrame);
        lineEditPrecio->setObjectName("lineEditPrecio");
        lineEditPrecio->setMaximumSize(QSize(100, 16777215));

        gridLayout_Entry->addWidget(lineEditPrecio, 1, 3, 1, 1);

        dateEditFC = new QDateEdit(entryFrame);
        dateEditFC->setObjectName("dateEditFC");
        dateEditFC->setCalendarPopup(true);

        gridLayout_Entry->addWidget(dateEditFC, 1, 4, 1, 1);


        mainVerticalLayout->addWidget(entryFrame);

        horizontalLayoutActions = new QHBoxLayout();
        horizontalLayoutActions->setObjectName("horizontalLayoutActions");
        pushButtonAgregar = new QPushButton(Salidas);
        pushButtonAgregar->setObjectName("pushButtonAgregar");
        pushButtonAgregar->setMinimumSize(QSize(120, 40));
        pushButtonAgregar->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #ef6c00; color: white; font-weight: bold; }"));

        horizontalLayoutActions->addWidget(pushButtonAgregar);

        pushButtonBorrar = new QPushButton(Salidas);
        pushButtonBorrar->setObjectName("pushButtonBorrar");
        pushButtonBorrar->setMinimumSize(QSize(120, 40));

        horizontalLayoutActions->addWidget(pushButtonBorrar);

        pushButtonEnviar = new QPushButton(Salidas);
        pushButtonEnviar->setObjectName("pushButtonEnviar");
        pushButtonEnviar->setMinimumSize(QSize(150, 40));
        pushButtonEnviar->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #2e7d32; color: white; font-weight: bold; }"));

        horizontalLayoutActions->addWidget(pushButtonEnviar);

        horizontalSpacerActions = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutActions->addItem(horizontalSpacerActions);

        pushButtonCerrar = new QPushButton(Salidas);
        pushButtonCerrar->setObjectName("pushButtonCerrar");
        pushButtonCerrar->setMinimumSize(QSize(100, 40));

        horizontalLayoutActions->addWidget(pushButtonCerrar);


        mainVerticalLayout->addLayout(horizontalLayoutActions);


        retranslateUi(Salidas);

        QMetaObject::connectSlotsByName(Salidas);
    } // setupUi

    void retranslateUi(QDialog *Salidas)
    {
        Salidas->setWindowTitle(QCoreApplication::translate("Salidas", "Salidas de Mercanc\303\255a / Traspasos", nullptr));
        labelTitle->setText(QCoreApplication::translate("Salidas", "Salida de Mercanc\303\255a", nullptr));
        lbSalidas->setText(QCoreApplication::translate("Salidas", "Estado", nullptr));
        label_6->setText(QCoreApplication::translate("Salidas", "Destino / Tienda:", nullptr));
        label->setText(QCoreApplication::translate("Salidas", "C\303\263digo", nullptr));
        label_2->setText(QCoreApplication::translate("Salidas", "Descripci\303\263n", nullptr));
        label_4->setText(QCoreApplication::translate("Salidas", "Cantidad", nullptr));
        label_5->setText(QCoreApplication::translate("Salidas", "Precio Coste", nullptr));
        label_3->setText(QCoreApplication::translate("Salidas", "Fecha", nullptr));
        dateEditFC->setDisplayFormat(QCoreApplication::translate("Salidas", "yyyy-MM-dd", nullptr));
        pushButtonAgregar->setText(QCoreApplication::translate("Salidas", "A\303\261adir L\303\255nea", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("Salidas", "Borrar L\303\255nea", nullptr));
        pushButtonEnviar->setText(QCoreApplication::translate("Salidas", "Procesar Salida", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("Salidas", "Cerrar", nullptr));
#if QT_CONFIG(shortcut)
        pushButtonCerrar->setShortcut(QCoreApplication::translate("Salidas", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
    } // retranslateUi

};

namespace Ui {
    class Salidas: public Ui_Salidas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SALIDAS_H
