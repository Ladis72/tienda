/********************************************************************************
** Form generated from reading UI file 'tiposentradassalidas.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIPOSENTRADASSALIDAS_H
#define UI_TIPOSENTRADASSALIDAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_TiposEntradasSalidas
{
public:
    QGridLayout *gridLayout;
    QSplitter *splitter;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit;
    QLabel *label;
    QTableView *tableView;
    QPushButton *pushButton_3;

    void setupUi(QDialog *TiposEntradasSalidas)
    {
        if (TiposEntradasSalidas->objectName().isEmpty())
            TiposEntradasSalidas->setObjectName(QString::fromUtf8("TiposEntradasSalidas"));
        TiposEntradasSalidas->resize(704, 408);
        gridLayout = new QGridLayout(TiposEntradasSalidas);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter = new QSplitter(TiposEntradasSalidas);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Orientation::Horizontal);
        pushButton = new QPushButton(splitter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        splitter->addWidget(pushButton);
        pushButton_2 = new QPushButton(splitter);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        splitter->addWidget(pushButton_2);

        gridLayout->addWidget(splitter, 2, 0, 1, 1);

        lineEdit = new QLineEdit(TiposEntradasSalidas);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 1, 0, 1, 1);

        label = new QLabel(TiposEntradasSalidas);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 1, 1, 1);

        tableView = new QTableView(TiposEntradasSalidas);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout->addWidget(tableView, 0, 0, 1, 2);

        pushButton_3 = new QPushButton(TiposEntradasSalidas);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 2, 1, 1, 1);

        QWidget::setTabOrder(lineEdit, pushButton);
        QWidget::setTabOrder(pushButton, pushButton_2);
        QWidget::setTabOrder(pushButton_2, pushButton_3);
        QWidget::setTabOrder(pushButton_3, tableView);

        retranslateUi(TiposEntradasSalidas);
        QObject::connect(pushButton_3, SIGNAL(clicked()), TiposEntradasSalidas, SLOT(accept()));

        QMetaObject::connectSlotsByName(TiposEntradasSalidas);
    } // setupUi

    void retranslateUi(QDialog *TiposEntradasSalidas)
    {
        TiposEntradasSalidas->setWindowTitle(QCoreApplication::translate("TiposEntradasSalidas", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("TiposEntradasSalidas", "A\303\261adir", nullptr));
        pushButton_2->setText(QCoreApplication::translate("TiposEntradasSalidas", "Modificar", nullptr));
        label->setText(QCoreApplication::translate("TiposEntradasSalidas", "<---Filtro", nullptr));
        pushButton_3->setText(QCoreApplication::translate("TiposEntradasSalidas", "Cerrar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TiposEntradasSalidas: public Ui_TiposEntradasSalidas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIPOSENTRADASSALIDAS_H
