/********************************************************************************
** Form generated from reading UI file 'visorimagenes.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VISORIMAGENES_H
#define UI_VISORIMAGENES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
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
            VisorImagenes->setObjectName("VisorImagenes");
        VisorImagenes->resize(821, 528);
        gridLayout = new QGridLayout(VisorImagenes);
        gridLayout->setObjectName("gridLayout");
        graphicsView = new QGraphicsView(VisorImagenes);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(VisorImagenes);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(VisorImagenes);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, VisorImagenes, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, VisorImagenes, qOverload<>(&QDialog::reject));

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
