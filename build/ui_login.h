/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLineEdit *lineEditUser;
    QLineEdit *lineEditpwd;
    QLabel *label;
    QPushButton *pushButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->setWindowModality(Qt::WindowModality::NonModal);
        Login->resize(252, 245);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imagenes/Emeicjac logo.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        Login->setWindowIcon(icon);
        gridLayout = new QGridLayout(Login);
        gridLayout->setObjectName("gridLayout");
        label_2 = new QLabel(Login);
        label_2->setObjectName("label_2");
        QFont font;
        font.setPointSize(14);
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        lineEditUser = new QLineEdit(Login);
        lineEditUser->setObjectName("lineEditUser");

        gridLayout->addWidget(lineEditUser, 1, 0, 1, 1);

        lineEditpwd = new QLineEdit(Login);
        lineEditpwd->setObjectName("lineEditpwd");
        lineEditpwd->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout->addWidget(lineEditpwd, 3, 0, 1, 1);

        label = new QLabel(Login);
        label->setObjectName("label");
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        pushButton = new QPushButton(Login);
        pushButton->setObjectName("pushButton");

        gridLayout->addWidget(pushButton, 5, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer, 4, 0, 1, 1);

        QWidget::setTabOrder(lineEditUser, lineEditpwd);
        QWidget::setTabOrder(lineEditpwd, pushButton);

        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QDialog *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "Login", nullptr));
        label_2->setText(QCoreApplication::translate("Login", "Contrase\303\261a", nullptr));
        label->setText(QCoreApplication::translate("Login", "Ususario", nullptr));
        pushButton->setText(QCoreApplication::translate("Login", "Aceptar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
