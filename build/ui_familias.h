/********************************************************************************
** Form generated from reading UI file 'familias.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FAMILIAS_H
#define UI_FAMILIAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Familias
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *labelNombreFamiliasHeader;
    QSpacerItem *horizontalSpacerHeader;
    QHBoxLayout *searchLayout;
    QLabel *labelBuscar;
    QLineEdit *lineEdit;
    QTableView *tableView;
    QHBoxLayout *buttonLayout;
    QPushButton *pushButtonSeleccionar;
    QPushButton *pushButtonAnadir;
    QPushButton *pushButtonBorrar;
    QPushButton *pushButtonUnificar;
    QPushButton *pushButtonSaneador;
    QSpacerItem *spacerButtons;
    QPushButton *pushButtonDeshacer;
    QPushButton *pushButtonGuardar;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Familias)
    {
        if (Familias->objectName().isEmpty())
            Familias->setObjectName("Familias");
        Familias->resize(600, 500);
        Familias->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        verticalLayout = new QVBoxLayout(Familias);
        verticalLayout->setSpacing(15);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(Familias);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame {\n"
"    background-color: #f3e5f5;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #e1bee7;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName("horizontalLayoutHeader");
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName("labelIcono");
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/familias.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelNombreFamiliasHeader = new QLabel(headerFrame);
        labelNombreFamiliasHeader->setObjectName("labelNombreFamiliasHeader");
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelNombreFamiliasHeader->setFont(font);
        labelNombreFamiliasHeader->setStyleSheet(QString::fromUtf8("color: #4a148c;"));

        horizontalLayoutHeader->addWidget(labelNombreFamiliasHeader);

        horizontalSpacerHeader = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);


        verticalLayout->addWidget(headerFrame);

        searchLayout = new QHBoxLayout();
        searchLayout->setObjectName("searchLayout");
        labelBuscar = new QLabel(Familias);
        labelBuscar->setObjectName("labelBuscar");

        searchLayout->addWidget(labelBuscar);

        lineEdit = new QLineEdit(Familias);
        lineEdit->setObjectName("lineEdit");

        searchLayout->addWidget(lineEdit);


        verticalLayout->addLayout(searchLayout);

        tableView = new QTableView(Familias);
        tableView->setObjectName("tableView");
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setSortingEnabled(true);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(tableView);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(10);
        buttonLayout->setObjectName("buttonLayout");
        pushButtonSeleccionar = new QPushButton(Familias);
        pushButtonSeleccionar->setObjectName("pushButtonSeleccionar");
        pushButtonSeleccionar->setAutoDefault(true);

        buttonLayout->addWidget(pushButtonSeleccionar);

        pushButtonAnadir = new QPushButton(Familias);
        pushButtonAnadir->setObjectName("pushButtonAnadir");

        buttonLayout->addWidget(pushButtonAnadir);

        pushButtonBorrar = new QPushButton(Familias);
        pushButtonBorrar->setObjectName("pushButtonBorrar");

        buttonLayout->addWidget(pushButtonBorrar);

        pushButtonUnificar = new QPushButton(Familias);
        pushButtonUnificar->setObjectName("pushButtonUnificar");

        buttonLayout->addWidget(pushButtonUnificar);

        pushButtonSaneador = new QPushButton(Familias);
        pushButtonSaneador->setObjectName("pushButtonSaneador");

        buttonLayout->addWidget(pushButtonSaneador);

        spacerButtons = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(spacerButtons);

        pushButtonDeshacer = new QPushButton(Familias);
        pushButtonDeshacer->setObjectName("pushButtonDeshacer");

        buttonLayout->addWidget(pushButtonDeshacer);

        pushButtonGuardar = new QPushButton(Familias);
        pushButtonGuardar->setObjectName("pushButtonGuardar");

        buttonLayout->addWidget(pushButtonGuardar);


        verticalLayout->addLayout(buttonLayout);

        buttonBox = new QDialogButtonBox(Familias);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        verticalLayout->addWidget(buttonBox);

        QWidget::setTabOrder(lineEdit, tableView);
        QWidget::setTabOrder(tableView, pushButtonAnadir);
        QWidget::setTabOrder(pushButtonAnadir, pushButtonBorrar);
        QWidget::setTabOrder(pushButtonBorrar, pushButtonUnificar);
        QWidget::setTabOrder(pushButtonUnificar, pushButtonDeshacer);
        QWidget::setTabOrder(pushButtonDeshacer, pushButtonGuardar);

        retranslateUi(Familias);
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, Familias, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(Familias);
    } // setupUi

    void retranslateUi(QDialog *Familias)
    {
        Familias->setWindowTitle(QCoreApplication::translate("Familias", "Gesti\303\263n de Familias", nullptr));
        labelNombreFamiliasHeader->setText(QCoreApplication::translate("Familias", "Gesti\303\263n de Familias", nullptr));
        labelBuscar->setText(QCoreApplication::translate("Familias", "Buscar:", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("Familias", "Escriba para filtrar por descripci\303\263n...", nullptr));
        pushButtonSeleccionar->setText(QCoreApplication::translate("Familias", " Seleccionar", nullptr));
        pushButtonAnadir->setText(QCoreApplication::translate("Familias", " A\303\261adir", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("Familias", " Borrar", nullptr));
        pushButtonUnificar->setText(QCoreApplication::translate("Familias", " Unificar y Limpiar", nullptr));
        pushButtonSaneador->setText(QCoreApplication::translate("Familias", " Saneador Global", nullptr));
        pushButtonDeshacer->setText(QCoreApplication::translate("Familias", " Deshacer", nullptr));
        pushButtonGuardar->setText(QCoreApplication::translate("Familias", " Guardar Cambios", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Familias: public Ui_Familias {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FAMILIAS_H
