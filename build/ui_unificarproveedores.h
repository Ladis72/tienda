/********************************************************************************
** Form generated from reading UI file 'unificarproveedores.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNIFICARPROVEEDORES_H
#define UI_UNIFICARPROVEEDORES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UnificarProveedores
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tabSaneamiento;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QPushButton *pushButtonNormalizarNIF;
    QSpacerItem *verticalSpacer;
    QLabel *label_2;
    QPushButton *pushButtonDetectarNIF;
    QTextEdit *textEditLogSaneamiento;
    QWidget *tabUnificacion;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboBoxTiendas;
    QPushButton *pushButtonAnalizarIDs;
    QTableWidget *tableWidgetIDs;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButtonGenerarSQL;
    QPushButton *pushButtonEjecutarUnificacion;

    void setupUi(QDialog *UnificarProveedores)
    {
        if (UnificarProveedores->objectName().isEmpty())
            UnificarProveedores->setObjectName("UnificarProveedores");
        UnificarProveedores->resize(850, 600);
        verticalLayout = new QVBoxLayout(UnificarProveedores);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(UnificarProveedores);
        tabWidget->setObjectName("tabWidget");
        tabSaneamiento = new QWidget();
        tabSaneamiento->setObjectName("tabSaneamiento");
        verticalLayout_2 = new QVBoxLayout(tabSaneamiento);
        verticalLayout_2->setObjectName("verticalLayout_2");
        label = new QLabel(tabSaneamiento);
        label->setObjectName("label");
        label->setWordWrap(true);

        verticalLayout_2->addWidget(label);

        pushButtonNormalizarNIF = new QPushButton(tabSaneamiento);
        pushButtonNormalizarNIF->setObjectName("pushButtonNormalizarNIF");

        verticalLayout_2->addWidget(pushButtonNormalizarNIF);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        label_2 = new QLabel(tabSaneamiento);
        label_2->setObjectName("label_2");

        verticalLayout_2->addWidget(label_2);

        pushButtonDetectarNIF = new QPushButton(tabSaneamiento);
        pushButtonDetectarNIF->setObjectName("pushButtonDetectarNIF");

        verticalLayout_2->addWidget(pushButtonDetectarNIF);

        textEditLogSaneamiento = new QTextEdit(tabSaneamiento);
        textEditLogSaneamiento->setObjectName("textEditLogSaneamiento");
        textEditLogSaneamiento->setReadOnly(true);

        verticalLayout_2->addWidget(textEditLogSaneamiento);

        tabWidget->addTab(tabSaneamiento, QString());
        tabUnificacion = new QWidget();
        tabUnificacion->setObjectName("tabUnificacion");
        verticalLayout_3 = new QVBoxLayout(tabUnificacion);
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_3 = new QLabel(tabUnificacion);
        label_3->setObjectName("label_3");

        verticalLayout_3->addWidget(label_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        comboBoxTiendas = new QComboBox(tabUnificacion);
        comboBoxTiendas->setObjectName("comboBoxTiendas");

        horizontalLayout->addWidget(comboBoxTiendas);

        pushButtonAnalizarIDs = new QPushButton(tabUnificacion);
        pushButtonAnalizarIDs->setObjectName("pushButtonAnalizarIDs");

        horizontalLayout->addWidget(pushButtonAnalizarIDs);


        verticalLayout_3->addLayout(horizontalLayout);

        tableWidgetIDs = new QTableWidget(tabUnificacion);
        if (tableWidgetIDs->columnCount() < 5)
            tableWidgetIDs->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidgetIDs->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidgetIDs->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidgetIDs->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidgetIDs->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidgetIDs->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tableWidgetIDs->setObjectName("tableWidgetIDs");

        verticalLayout_3->addWidget(tableWidgetIDs);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        pushButtonGenerarSQL = new QPushButton(tabUnificacion);
        pushButtonGenerarSQL->setObjectName("pushButtonGenerarSQL");

        horizontalLayout_2->addWidget(pushButtonGenerarSQL);

        pushButtonEjecutarUnificacion = new QPushButton(tabUnificacion);
        pushButtonEjecutarUnificacion->setObjectName("pushButtonEjecutarUnificacion");
        pushButtonEjecutarUnificacion->setStyleSheet(QString::fromUtf8("background-color: #f44336; color: white;"));

        horizontalLayout_2->addWidget(pushButtonEjecutarUnificacion);


        verticalLayout_3->addLayout(horizontalLayout_2);

        tabWidget->addTab(tabUnificacion, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(UnificarProveedores);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(UnificarProveedores);
    } // setupUi

    void retranslateUi(QDialog *UnificarProveedores)
    {
        UnificarProveedores->setWindowTitle(QCoreApplication::translate("UnificarProveedores", "Asistente de Unificaci\303\263n de Maestros", nullptr));
        label->setText(QCoreApplication::translate("UnificarProveedores", "<html><head/><body><p><span style=\" font-weight:700;\">Paso 0: Normalizaci\303\263n de Formatos</span></p><p>Este bot\303\263n limpiar\303\241 guiones, espacios y pondr\303\241 en may\303\272sculas todos los NIF de todas las tiendas para facilitar el cruce de datos.</p></body></html>", nullptr));
        pushButtonNormalizarNIF->setText(QCoreApplication::translate("UnificarProveedores", "Normalizar todos los NIF (Global)", nullptr));
        label_2->setText(QCoreApplication::translate("UnificarProveedores", "<span style=\" font-weight:700;\">Paso 1: Propagaci\303\263n de NIFs</span><br>Busca proveedores con el mismo nombre en distintas tiendas donde en una falte el NIF y en otra est\303\251 presente, y rellena los huecos.", nullptr));
        pushButtonDetectarNIF->setText(QCoreApplication::translate("UnificarProveedores", "Detectar y Rellenar NIFs Faltantes", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabSaneamiento), QCoreApplication::translate("UnificarProveedores", "1. Saneamiento de NIFs", nullptr));
        label_3->setText(QCoreApplication::translate("UnificarProveedores", "<b>Generador de Scripts de Unificaci\303\263n</b><br>Selecciona una tienda para ver qu\303\251 IDs deben cambiarse para coincidir con la Tienda Local (Maestra).", nullptr));
        pushButtonAnalizarIDs->setText(QCoreApplication::translate("UnificarProveedores", "Analizar Discrepancias", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidgetIDs->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("UnificarProveedores", "Proveedor", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidgetIDs->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("UnificarProveedores", "NIF", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidgetIDs->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("UnificarProveedores", "ID Local", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidgetIDs->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("UnificarProveedores", "ID Remoto", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidgetIDs->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("UnificarProveedores", "Acci\303\263n", nullptr));
        pushButtonGenerarSQL->setText(QCoreApplication::translate("UnificarProveedores", "Generar Script SQL", nullptr));
        pushButtonEjecutarUnificacion->setText(QCoreApplication::translate("UnificarProveedores", "Ejecutar Unificaci\303\263n (Auto)", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabUnificacion), QCoreApplication::translate("UnificarProveedores", "2. Unificaci\303\263n de IDs", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UnificarProveedores: public Ui_UnificarProveedores {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNIFICARPROVEEDORES_H
