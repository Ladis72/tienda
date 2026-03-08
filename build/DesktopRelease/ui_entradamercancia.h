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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_EntradaMercancia
{
public:
    QGridLayout *gridLayout_2;
    QLabel *label_6;
    QComboBox *comboBoxProcedencia;
    QTableView *tableView;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEditCod;
    QLineEdit *lineEditDesc;
    QLineEdit *lineEditUds;
    QDateEdit *dateEditCaducidad;
    QLabel *label_5;
    QLabel *label_4;
    QLineEdit *lineEditPVP;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonAgregarLinea;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonAceptar;
    QPushButton *pushButtonSalir;
    QLabel *lbProductos;

    void setupUi(QDialog *EntradaMercancia)
    {
        if (EntradaMercancia->objectName().isEmpty())
            EntradaMercancia->setObjectName("EntradaMercancia");
        EntradaMercancia->resize(1043, 525);
        gridLayout_2 = new QGridLayout(EntradaMercancia);
        gridLayout_2->setObjectName("gridLayout_2");
        label_6 = new QLabel(EntradaMercancia);
        label_6->setObjectName("label_6");

        gridLayout_2->addWidget(label_6, 0, 0, 1, 1);

        comboBoxProcedencia = new QComboBox(EntradaMercancia);
        comboBoxProcedencia->setObjectName("comboBoxProcedencia");

        gridLayout_2->addWidget(comboBoxProcedencia, 0, 1, 1, 1);

        tableView = new QTableView(EntradaMercancia);
        tableView->setObjectName("tableView");
        tableView->setSortingEnabled(true);

        gridLayout_2->addWidget(tableView, 1, 0, 1, 3);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(EntradaMercancia);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(EntradaMercancia);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        label_3 = new QLabel(EntradaMercancia);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        lineEditCod = new QLineEdit(EntradaMercancia);
        lineEditCod->setObjectName("lineEditCod");
        lineEditCod->setMaximumSize(QSize(150, 16777215));
        lineEditCod->setMaxLength(13);

        gridLayout->addWidget(lineEditCod, 1, 0, 1, 1);

        lineEditDesc = new QLineEdit(EntradaMercancia);
        lineEditDesc->setObjectName("lineEditDesc");

        gridLayout->addWidget(lineEditDesc, 1, 1, 1, 1);

        lineEditUds = new QLineEdit(EntradaMercancia);
        lineEditUds->setObjectName("lineEditUds");
        lineEditUds->setMaximumSize(QSize(50, 16777215));
        lineEditUds->setMaxLength(3);

        gridLayout->addWidget(lineEditUds, 1, 2, 1, 2);

        dateEditCaducidad = new QDateEdit(EntradaMercancia);
        dateEditCaducidad->setObjectName("dateEditCaducidad");
        dateEditCaducidad->setCalendarPopup(true);

        gridLayout->addWidget(dateEditCaducidad, 1, 4, 1, 2);

        label_5 = new QLabel(EntradaMercancia);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 0, 6, 1, 1);

        label_4 = new QLabel(EntradaMercancia);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 0, 4, 1, 2);

        lineEditPVP = new QLineEdit(EntradaMercancia);
        lineEditPVP->setObjectName("lineEditPVP");
        lineEditPVP->setMaximumSize(QSize(100, 16777215));
        lineEditPVP->setMaxLength(6);

        gridLayout->addWidget(lineEditPVP, 1, 6, 1, 1);


        gridLayout_2->addLayout(gridLayout, 2, 0, 1, 3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pushButtonAgregarLinea = new QPushButton(EntradaMercancia);
        pushButtonAgregarLinea->setObjectName("pushButtonAgregarLinea");
        pushButtonAgregarLinea->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonAgregarLinea);

        pushButtonBorrar = new QPushButton(EntradaMercancia);
        pushButtonBorrar->setObjectName("pushButtonBorrar");
        pushButtonBorrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonBorrar);

        pushButtonAceptar = new QPushButton(EntradaMercancia);
        pushButtonAceptar->setObjectName("pushButtonAceptar");
        pushButtonAceptar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonAceptar);

        pushButtonSalir = new QPushButton(EntradaMercancia);
        pushButtonSalir->setObjectName("pushButtonSalir");
        pushButtonSalir->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
        pushButtonSalir->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonSalir);


        gridLayout_2->addLayout(horizontalLayout, 3, 0, 1, 3);

        lbProductos = new QLabel(EntradaMercancia);
        lbProductos->setObjectName("lbProductos");
        QFont font;
        font.setPointSize(14);
        lbProductos->setFont(font);

        gridLayout_2->addWidget(lbProductos, 0, 2, 1, 1);

        QWidget::setTabOrder(comboBoxProcedencia, lineEditCod);
        QWidget::setTabOrder(lineEditCod, lineEditDesc);
        QWidget::setTabOrder(lineEditDesc, lineEditUds);
        QWidget::setTabOrder(lineEditUds, dateEditCaducidad);
        QWidget::setTabOrder(dateEditCaducidad, lineEditPVP);
        QWidget::setTabOrder(lineEditPVP, pushButtonAgregarLinea);
        QWidget::setTabOrder(pushButtonAgregarLinea, pushButtonBorrar);
        QWidget::setTabOrder(pushButtonBorrar, pushButtonAceptar);
        QWidget::setTabOrder(pushButtonAceptar, tableView);

        retranslateUi(EntradaMercancia);
        QObject::connect(pushButtonSalir, &QPushButton::clicked, EntradaMercancia, qOverload<>(&QDialog::close));

        QMetaObject::connectSlotsByName(EntradaMercancia);
    } // setupUi

    void retranslateUi(QDialog *EntradaMercancia)
    {
        EntradaMercancia->setWindowTitle(QCoreApplication::translate("EntradaMercancia", "Entrada de Mercanc\303\255a", nullptr));
        label_6->setText(QCoreApplication::translate("EntradaMercancia", "Procedencia :", nullptr));
        label->setText(QCoreApplication::translate("EntradaMercancia", "Cod.", nullptr));
        label_2->setText(QCoreApplication::translate("EntradaMercancia", "Descripci\303\263n", nullptr));
        label_3->setText(QCoreApplication::translate("EntradaMercancia", "Cantidad", nullptr));
        dateEditCaducidad->setDisplayFormat(QCoreApplication::translate("EntradaMercancia", "yyyy-MM-dd", nullptr));
        label_5->setText(QCoreApplication::translate("EntradaMercancia", "P.V.P.", nullptr));
        label_4->setText(QCoreApplication::translate("EntradaMercancia", "Fecha caducidad", nullptr));
        pushButtonAgregarLinea->setText(QCoreApplication::translate("EntradaMercancia", "Agregar", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("EntradaMercancia", "Borrar linea", nullptr));
        pushButtonAceptar->setText(QCoreApplication::translate("EntradaMercancia", "Aceptar entradas", nullptr));
        pushButtonSalir->setText(QCoreApplication::translate("EntradaMercancia", "Salir", nullptr));
        lbProductos->setText(QCoreApplication::translate("EntradaMercancia", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EntradaMercancia: public Ui_EntradaMercancia {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENTRADAMERCANCIA_H
