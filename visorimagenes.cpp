#include "visorimagenes.h"
#include "ui_visorimagenes.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDir>

VisorImagenes::VisorImagenes(QString direccion ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VisorImagenes)
{
    ui->setupUi(this);

    QImage imagen(QDir::currentPath()+"/"+direccion);
    qDebug() << imagen;

    QGraphicsScene *escena = new QGraphicsScene;
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(imagen));

    escena->addItem(item);
    ui->graphicsView->setScene(escena);
    ui->graphicsView->show();
}

VisorImagenes::~VisorImagenes()
{
    delete ui;
}
