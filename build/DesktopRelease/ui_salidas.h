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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_Salidas
{
public:
    QGridLayout *gridLayout;
    QLineEdit *lineEditCod;
    QLineEdit *lineEditDesc;
    QDateEdit *dateEditFC;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonAgregar;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonEnviar;
    QPushButton *pushButtonCerrar;
    QLabel *label_3;
    QTableView *tableView;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label;
    QLineEdit *lineEditCantidad;
    QLineEdit *lineEditPrecio;
    QLabel *label_5;
    QLabel *label_6;
    QComboBox *comboBoxDestino;
    QLabel *lbSalidas;

    void setupUi(QDialog *Salidas)
    {
        if (Salidas->objectName().isEmpty())
            Salidas->setObjectName("Salidas");
        Salidas->resize(854, 493);
        gridLayout = new QGridLayout(Salidas);
        gridLayout->setObjectName("gridLayout");
        lineEditCod = new QLineEdit(Salidas);
        lineEditCod->setObjectName("lineEditCod");
        lineEditCod->setMaximumSize(QSize(200, 16777215));
        lineEditCod->setMaxLength(13);

        gridLayout->addWidget(lineEditCod, 3, 0, 1, 1);

        lineEditDesc = new QLineEdit(Salidas);
        lineEditDesc->setObjectName("lineEditDesc");

        gridLayout->addWidget(lineEditDesc, 3, 1, 1, 1);

        dateEditFC = new QDateEdit(Salidas);
        dateEditFC->setObjectName("dateEditFC");
        dateEditFC->setCalendarPopup(true);

        gridLayout->addWidget(dateEditFC, 3, 4, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pushButtonAgregar = new QPushButton(Salidas);
        pushButtonAgregar->setObjectName("pushButtonAgregar");
        pushButtonAgregar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonAgregar);

        pushButtonBorrar = new QPushButton(Salidas);
        pushButtonBorrar->setObjectName("pushButtonBorrar");
        pushButtonBorrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonBorrar);

        pushButtonEnviar = new QPushButton(Salidas);
        pushButtonEnviar->setObjectName("pushButtonEnviar");
        pushButtonEnviar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonEnviar);

        pushButtonCerrar = new QPushButton(Salidas);
        pushButtonCerrar->setObjectName("pushButtonCerrar");
        pushButtonCerrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonCerrar);


        gridLayout->addLayout(horizontalLayout, 4, 0, 1, 5);

        label_3 = new QLabel(Salidas);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 2, 4, 1, 1);

        tableView = new QTableView(Salidas);
        tableView->setObjectName("tableView");
        tableView->setSortingEnabled(true);

        gridLayout->addWidget(tableView, 1, 0, 1, 5);

        label_2 = new QLabel(Salidas);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 2, 1, 1, 1);

        label_4 = new QLabel(Salidas);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 2, 2, 1, 1);

        label = new QLabel(Salidas);
        label->setObjectName("label");

        gridLayout->addWidget(label, 2, 0, 1, 1);

        lineEditCantidad = new QLineEdit(Salidas);
        lineEditCantidad->setObjectName("lineEditCantidad");
        lineEditCantidad->setMaximumSize(QSize(50, 16777215));

        gridLayout->addWidget(lineEditCantidad, 3, 2, 1, 1);

        lineEditPrecio = new QLineEdit(Salidas);
        lineEditPrecio->setObjectName("lineEditPrecio");
        lineEditPrecio->setMaximumSize(QSize(100, 16777215));

        gridLayout->addWidget(lineEditPrecio, 3, 3, 1, 1);

        label_5 = new QLabel(Salidas);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 2, 3, 1, 1);

        label_6 = new QLabel(Salidas);
        label_6->setObjectName("label_6");

        gridLayout->addWidget(label_6, 0, 0, 1, 1);

        comboBoxDestino = new QComboBox(Salidas);
        comboBoxDestino->setObjectName("comboBoxDestino");

        gridLayout->addWidget(comboBoxDestino, 0, 1, 1, 1);

        lbSalidas = new QLabel(Salidas);
        lbSalidas->setObjectName("lbSalidas");
        QFont font;
        font.setPointSize(14);
        lbSalidas->setFont(font);

        gridLayout->addWidget(lbSalidas, 0, 2, 1, 3);

        QWidget::setTabOrder(lineEditCod, lineEditDesc);
        QWidget::setTabOrder(lineEditDesc, lineEditCantidad);
        QWidget::setTabOrder(lineEditCantidad, lineEditPrecio);
        QWidget::setTabOrder(lineEditPrecio, dateEditFC);
        QWidget::setTabOrder(dateEditFC, pushButtonAgregar);
        QWidget::setTabOrder(pushButtonAgregar, pushButtonBorrar);
        QWidget::setTabOrder(pushButtonBorrar, tableView);
        QWidget::setTabOrder(tableView, pushButtonEnviar);
        QWidget::setTabOrder(pushButtonEnviar, pushButtonCerrar);

        retranslateUi(Salidas);
        QObject::connect(pushButtonCerrar, &QPushButton::clicked, Salidas, qOverload<>(&QDialog::close));

        QMetaObject::connectSlotsByName(Salidas);
    } // setupUi

    void retranslateUi(QDialog *Salidas)
    {
        Salidas->setWindowTitle(QCoreApplication::translate("Salidas", "Salidas de mercanc\303\255a", nullptr));
        dateEditFC->setDisplayFormat(QCoreApplication::translate("Salidas", "yyyy-MM-dd", nullptr));
        pushButtonAgregar->setText(QCoreApplication::translate("Salidas", "Agregar", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("Salidas", "Borrar", nullptr));
        pushButtonEnviar->setText(QCoreApplication::translate("Salidas", "Enviar", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("Salidas", "Cerrar", nullptr));
        label_3->setText(QCoreApplication::translate("Salidas", "Fecha", nullptr));
        label_2->setText(QCoreApplication::translate("Salidas", "Descripci\303\263n", nullptr));
        label_4->setText(QCoreApplication::translate("Salidas", "Cantidad", nullptr));
        label->setText(QCoreApplication::translate("Salidas", "Cod.", nullptr));
        label_5->setText(QCoreApplication::translate("Salidas", "Precio", nullptr));
        label_6->setText(QCoreApplication::translate("Salidas", "Destino :", nullptr));
        lbSalidas->setText(QCoreApplication::translate("Salidas", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Salidas: public Ui_Salidas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SALIDAS_H
