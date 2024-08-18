/********************************************************************************
** Form generated from reading UI file 'visorimagenes.ui'
**
** Created by: Qt User Interface Compiler version 5.15.14
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VISORIMAGENES_H
#define UI_VISORIMAGENES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>

QT_BEGIN_NAMESPACE

class Ui_VisorImagenes
{
public:
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *VisorImagenes)
    {
        if (VisorImagenes->objectName().isEmpty())
            VisorImagenes->setObjectName(QString::fromUtf8("VisorImagenes"));
        VisorImagenes->resize(821, 528);
        gridLayout = new QGridLayout(VisorImagenes);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        graphicsView = new QGraphicsView(VisorImagenes);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(VisorImagenes);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(VisorImagenes);
        QObject::connect(buttonBox, SIGNAL(accepted()), VisorImagenes, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), VisorImagenes, SLOT(reject()));

        QMetaObject::connectSlotsByName(VisorImagenes);
    } // setupUi

    void retranslateUi(QDialog *VisorImagenes)
    {
        VisorImagenes->setWindowTitle(QCoreApplication::translate("VisorImagenes", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VisorImagenes: public Ui_VisorImagenes {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISORIMAGENES_H
