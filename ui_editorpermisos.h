/********************************************************************************
** Form generated from reading UI file 'editorpermisos.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITORPERMISOS_H
#define UI_EDITORPERMISOS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditorPermisos
{
public:
    QVBoxLayout *mainVerticalLayout;
    QFrame *headerFrame;
    QHBoxLayout *horizontalLayoutHeader;
    QLabel *labelIcono;
    QLabel *labelTitle;
    QSpacerItem *horizontalSpacerHeader;
    QHBoxLayout *layoutRol;
    QLabel *labelRol;
    QComboBox *comboRol;
    QSpacerItem *spacerRol;
    QPushButton *btnMarcarTodo;
    QPushButton *btnDesmarcarTodo;
    QScrollArea *scrollArea;
    QWidget *scrollAreaContents;
    QVBoxLayout *layoutPermisos;
    QHBoxLayout *horizontalLayoutButtons;
    QSpacerItem *horizontalSpacerButtons;
    QPushButton *pushButtonCerrar;
    QPushButton *pushButtonGuardar;

    void setupUi(QDialog *EditorPermisos)
    {
        if (EditorPermisos->objectName().isEmpty())
            EditorPermisos->setObjectName(QString::fromUtf8("EditorPermisos"));
        EditorPermisos->resize(800, 650);
        EditorPermisos->setStyleSheet(QString::fromUtf8("QDialog { background-color: #f8f9fa; }"));
        mainVerticalLayout = new QVBoxLayout(EditorPermisos);
        mainVerticalLayout->setSpacing(20);
        mainVerticalLayout->setObjectName(QString::fromUtf8("mainVerticalLayout"));
        mainVerticalLayout->setContentsMargins(20, 20, 20, 20);
        headerFrame = new QFrame(EditorPermisos);
        headerFrame->setObjectName(QString::fromUtf8("headerFrame"));
        headerFrame->setMinimumSize(QSize(0, 80));
        headerFrame->setStyleSheet(QString::fromUtf8("QFrame#headerFrame {\n"
"    background-color: #ede7f6;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #d1c4e9;\n"
"}"));
        horizontalLayoutHeader = new QHBoxLayout(headerFrame);
        horizontalLayoutHeader->setObjectName(QString::fromUtf8("horizontalLayoutHeader"));
        labelIcono = new QLabel(headerFrame);
        labelIcono->setObjectName(QString::fromUtf8("labelIcono"));
        labelIcono->setMaximumSize(QSize(48, 48));
        labelIcono->setPixmap(QPixmap(QString::fromUtf8("../iconos/seguridad.png")));
        labelIcono->setScaledContents(true);

        horizontalLayoutHeader->addWidget(labelIcono);

        labelTitle = new QLabel(headerFrame);
        labelTitle->setObjectName(QString::fromUtf8("labelTitle"));
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        labelTitle->setFont(font);
        labelTitle->setStyleSheet(QString::fromUtf8("color: #4527a0;"));

        horizontalLayoutHeader->addWidget(labelTitle);

        horizontalSpacerHeader = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutHeader->addItem(horizontalSpacerHeader);


        mainVerticalLayout->addWidget(headerFrame);

        layoutRol = new QHBoxLayout();
        layoutRol->setObjectName(QString::fromUtf8("layoutRol"));
        labelRol = new QLabel(EditorPermisos);
        labelRol->setObjectName(QString::fromUtf8("labelRol"));
        QFont font1;
        font1.setBold(true);
        labelRol->setFont(font1);

        layoutRol->addWidget(labelRol);

        comboRol = new QComboBox(EditorPermisos);
        comboRol->setObjectName(QString::fromUtf8("comboRol"));
        comboRol->setMinimumSize(QSize(250, 35));

        layoutRol->addWidget(comboRol);

        spacerRol = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layoutRol->addItem(spacerRol);

        btnMarcarTodo = new QPushButton(EditorPermisos);
        btnMarcarTodo->setObjectName(QString::fromUtf8("btnMarcarTodo"));

        layoutRol->addWidget(btnMarcarTodo);

        btnDesmarcarTodo = new QPushButton(EditorPermisos);
        btnDesmarcarTodo->setObjectName(QString::fromUtf8("btnDesmarcarTodo"));

        layoutRol->addWidget(btnDesmarcarTodo);


        mainVerticalLayout->addLayout(layoutRol);

        scrollArea = new QScrollArea(EditorPermisos);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollArea->setStyleSheet(QString::fromUtf8("QScrollArea { border: 1px solid #e0e0e0; border-radius: 5px; background: white; }"));
        scrollAreaContents = new QWidget();
        scrollAreaContents->setObjectName(QString::fromUtf8("scrollAreaContents"));
        layoutPermisos = new QVBoxLayout(scrollAreaContents);
        layoutPermisos->setSpacing(5);
        layoutPermisos->setObjectName(QString::fromUtf8("layoutPermisos"));
        scrollArea->setWidget(scrollAreaContents);

        mainVerticalLayout->addWidget(scrollArea);

        horizontalLayoutButtons = new QHBoxLayout();
        horizontalLayoutButtons->setObjectName(QString::fromUtf8("horizontalLayoutButtons"));
        horizontalSpacerButtons = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayoutButtons->addItem(horizontalSpacerButtons);

        pushButtonCerrar = new QPushButton(EditorPermisos);
        pushButtonCerrar->setObjectName(QString::fromUtf8("pushButtonCerrar"));
        pushButtonCerrar->setMinimumSize(QSize(120, 40));

        horizontalLayoutButtons->addWidget(pushButtonCerrar);

        pushButtonGuardar = new QPushButton(EditorPermisos);
        pushButtonGuardar->setObjectName(QString::fromUtf8("pushButtonGuardar"));
        pushButtonGuardar->setMinimumSize(QSize(120, 40));
        pushButtonGuardar->setStyleSheet(QString::fromUtf8("QPushButton { background-color: #673ab7; color: white; font-weight: bold; border-radius: 4px; }\n"
"QPushButton:hover { background-color: #512da8; }"));

        horizontalLayoutButtons->addWidget(pushButtonGuardar);


        mainVerticalLayout->addLayout(horizontalLayoutButtons);


        retranslateUi(EditorPermisos);

        QMetaObject::connectSlotsByName(EditorPermisos);
    } // setupUi

    void retranslateUi(QDialog *EditorPermisos)
    {
        EditorPermisos->setWindowTitle(QCoreApplication::translate("EditorPermisos", "Editor de Permisos", nullptr));
        labelTitle->setText(QCoreApplication::translate("EditorPermisos", "Control de Accesos", nullptr));
        labelRol->setText(QCoreApplication::translate("EditorPermisos", "Perfil / Rol:", nullptr));
        btnMarcarTodo->setText(QCoreApplication::translate("EditorPermisos", "Marcar Todo", nullptr));
        btnDesmarcarTodo->setText(QCoreApplication::translate("EditorPermisos", "Desmarcar Todo", nullptr));
        pushButtonCerrar->setText(QCoreApplication::translate("EditorPermisos", "Cerrar", nullptr));
        pushButtonGuardar->setText(QCoreApplication::translate("EditorPermisos", "Guardar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditorPermisos: public Ui_EditorPermisos {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITORPERMISOS_H
