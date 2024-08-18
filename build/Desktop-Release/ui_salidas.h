/********************************************************************************
** Form generated from reading UI file 'salidas.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
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

    void setupUi(QDialog *Salidas)
    {
        if (Salidas->objectName().isEmpty())
            Salidas->setObjectName(QString::fromUtf8("Salidas"));
        Salidas->resize(854, 493);
        gridLayout = new QGridLayout(Salidas);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineEditCod = new QLineEdit(Salidas);
        lineEditCod->setObjectName(QString::fromUtf8("lineEditCod"));
        lineEditCod->setMaximumSize(QSize(200, 16777215));
        lineEditCod->setMaxLength(13);

        gridLayout->addWidget(lineEditCod, 3, 0, 1, 1);

        lineEditDesc = new QLineEdit(Salidas);
        lineEditDesc->setObjectName(QString::fromUtf8("lineEditDesc"));

        gridLayout->addWidget(lineEditDesc, 3, 1, 1, 1);

        dateEditFC = new QDateEdit(Salidas);
        dateEditFC->setObjectName(QString::fromUtf8("dateEditFC"));
        dateEditFC->setCalendarPopup(true);

        gridLayout->addWidget(dateEditFC, 3, 4, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonAgregar = new QPushButton(Salidas);
        pushButtonAgregar->setObjectName(QString::fromUtf8("pushButtonAgregar"));
        pushButtonAgregar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonAgregar);

        pushButtonBorrar = new QPushButton(Salidas);
        pushButtonBorrar->setObjectName(QString::fromUtf8("pushButtonBorrar"));
        pushButtonBorrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonBorrar);

        pushButtonEnviar = new QPushButton(Salidas);
        pushButtonEnviar->setObjectName(QString::fromUtf8("pushButtonEnviar"));
        pushButtonEnviar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonEnviar);

        pushButtonCerrar = new QPushButton(Salidas);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));
        pushButtonCerrar->setAutoDefault(false);

        horizontalLayout->addWidget(pushButtonCerrar);


        gridLayout->addLayout(horizontalLayout, 4, 0, 1, 5);

        label_3 = new QLabel(Salidas);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 4, 1, 1);

        tableView = new QTableView(Salidas);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout->addWidget(tableView, 1, 0, 1, 5);

        label_2 = new QLabel(Salidas);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 1, 1, 1);

        label_4 = new QLabel(Salidas);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 2, 1, 1);

        label = new QLabel(Salidas);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 2, 0, 1, 1);

        lineEditCantidad = new QLineEdit(Salidas);
        lineEditCantidad->setObjectName(QString::fromUtf8("lineEditCantidad"));
        lineEditCantidad->setMaximumSize(QSize(50, 16777215));

        gridLayout->addWidget(lineEditCantidad, 3, 2, 1, 1);

        lineEditPrecio = new QLineEdit(Salidas);
        lineEditPrecio->setObjectName(QString::fromUtf8("lineEditPrecio"));
        lineEditPrecio->setMaximumSize(QSize(100, 16777215));

        gridLayout->addWidget(lineEditPrecio, 3, 3, 1, 1);

        label_5 = new QLabel(Salidas);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 2, 3, 1, 1);

        label_6 = new QLabel(Salidas);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 0, 0, 1, 1);

        comboBoxDestino = new QComboBox(Salidas);
        comboBoxDestino->setObjectName(QString::fromUtf8("comboBoxDestino"));

        gridLayout->addWidget(comboBoxDestino, 0, 1, 1, 1);

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
        QObject::connect(pushButtonCerrar, SIGNAL(clicked()), Salidas, SLOT(close()));

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
    } // retranslateUi

};

namespace Ui {
    class Salidas: public Ui_Salidas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SALIDAS_H
