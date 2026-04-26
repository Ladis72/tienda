/********************************************************************************
** Form generated from reading UI file 'fabricantes.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FABRICANTES_H
#define UI_FABRICANTES_H

#include <QtCore/QVariant>
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

class Ui_Fabricantes
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *labelNombreFabricantesHeader;
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
    QPushButton *pushButtonModificar;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Fabricantes)
    {
        if (Fabricantes->objectName().isEmpty())
            Fabricantes->setObjectName(QString::fromUtf8("Fabricantes"));
        Fabricantes->resize(600, 500);
        Fabricantes->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        verticalLayout = new QVBoxLayout(Fabricantes);
        verticalLayout->setSpacing(15);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(Fabricantes);
        headerFrame->setObjectName(QString::fromUtf8("headerFrame"));
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame {\n"
"    background-color: #e0f2f1;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #b2dfdb;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName(QString::fromUtf8("horizontalLayoutHeader"));
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName(QString::fromUtf8("labelIcono"));
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/fabricantes.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelNombreFabricantesHeader = new QLabel(headerFrame);
        labelNombreFabricantesHeader->setObjectName(QString::fromUtf8("labelNombreFabricantesHeader"));
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelNombreFabricantesHeader->setFont(font);
        labelNombreFabricantesHeader->setStyleSheet(QString::fromUtf8("color: #004d40;"));

        horizontalLayoutHeader->addWidget(labelNombreFabricantesHeader);

        horizontalSpacerHeader = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);


        verticalLayout->addWidget(headerFrame);

        searchLayout = new QHBoxLayout();
        searchLayout->setObjectName(QString::fromUtf8("searchLayout"));
        labelBuscar = new QLabel(Fabricantes);
        labelBuscar->setObjectName(QString::fromUtf8("labelBuscar"));

        searchLayout->addWidget(labelBuscar);

        lineEdit = new QLineEdit(Fabricantes);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        searchLayout->addWidget(lineEdit);


        verticalLayout->addLayout(searchLayout);

        tableView = new QTableView(Fabricantes);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setSortingEnabled(true);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(tableView);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(10);
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        pushButtonSeleccionar = new QPushButton(Fabricantes);
        pushButtonSeleccionar->setObjectName(QString::fromUtf8("pushButtonSeleccionar"));
        pushButtonSeleccionar->setAutoDefault(true);

        buttonLayout->addWidget(pushButtonSeleccionar);

        pushButtonAnadir = new QPushButton(Fabricantes);
        pushButtonAnadir->setObjectName(QString::fromUtf8("pushButtonAnadir"));

        buttonLayout->addWidget(pushButtonAnadir);

        pushButtonBorrar = new QPushButton(Fabricantes);
        pushButtonBorrar->setObjectName(QString::fromUtf8("pushButtonBorrar"));

        buttonLayout->addWidget(pushButtonBorrar);

        pushButtonUnificar = new QPushButton(Fabricantes);
        pushButtonUnificar->setObjectName(QString::fromUtf8("pushButtonUnificar"));

        buttonLayout->addWidget(pushButtonUnificar);

        pushButtonSaneador = new QPushButton(Fabricantes);
        pushButtonSaneador->setObjectName(QString::fromUtf8("pushButtonSaneador"));

        buttonLayout->addWidget(pushButtonSaneador);

        spacerButtons = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(spacerButtons);

        pushButtonDeshacer = new QPushButton(Fabricantes);
        pushButtonDeshacer->setObjectName(QString::fromUtf8("pushButtonDeshacer"));

        buttonLayout->addWidget(pushButtonDeshacer);

        pushButtonModificar = new QPushButton(Fabricantes);
        pushButtonModificar->setObjectName(QString::fromUtf8("pushButtonModificar"));

        buttonLayout->addWidget(pushButtonModificar);


        verticalLayout->addLayout(buttonLayout);

        buttonBox = new QDialogButtonBox(Fabricantes);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        verticalLayout->addWidget(buttonBox);

        QWidget::setTabOrder(lineEdit, tableView);
        QWidget::setTabOrder(tableView, pushButtonSeleccionar);
        QWidget::setTabOrder(pushButtonSeleccionar, pushButtonAnadir);
        QWidget::setTabOrder(pushButtonAnadir, pushButtonBorrar);
        QWidget::setTabOrder(pushButtonBorrar, pushButtonUnificar);
        QWidget::setTabOrder(pushButtonUnificar, pushButtonDeshacer);
        QWidget::setTabOrder(pushButtonDeshacer, pushButtonModificar);

        retranslateUi(Fabricantes);
        QObject::connect(buttonBox, SIGNAL(rejected()), Fabricantes, SLOT(reject()));

        QMetaObject::connectSlotsByName(Fabricantes);
    } // setupUi

    void retranslateUi(QDialog *Fabricantes)
    {
        Fabricantes->setWindowTitle(QCoreApplication::translate("Fabricantes", "Gesti\303\263n de Fabricantes", nullptr));
        labelNombreFabricantesHeader->setText(QCoreApplication::translate("Fabricantes", "Gesti\303\263n de Fabricantes", nullptr));
        labelBuscar->setText(QCoreApplication::translate("Fabricantes", "Buscar:", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("Fabricantes", "Escriba para filtrar por nombre...", nullptr));
        pushButtonSeleccionar->setText(QCoreApplication::translate("Fabricantes", " Seleccionar", nullptr));
        pushButtonAnadir->setText(QCoreApplication::translate("Fabricantes", " A\303\261adir", nullptr));
        pushButtonBorrar->setText(QCoreApplication::translate("Fabricantes", " Borrar", nullptr));
        pushButtonUnificar->setText(QCoreApplication::translate("Fabricantes", " Unificar y Limpiar", nullptr));
        pushButtonSaneador->setText(QCoreApplication::translate("Fabricantes", " Saneador Global", nullptr));
        pushButtonDeshacer->setText(QCoreApplication::translate("Fabricantes", " Deshacer", nullptr));
        pushButtonModificar->setText(QCoreApplication::translate("Fabricantes", " Guardar Cambios", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Fabricantes: public Ui_Fabricantes {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FABRICANTES_H
