#include "visorimagenes.h"
#include "base_datos.h"
#include <QDebug>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "ui_visorimagenes.h"

VisorImagenes::VisorImagenes(QString direccion, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VisorImagenes)
{
    ui->setupUi(this);

    baseDatos base;
    QImage imagen(base.resolverRutaImagen(direccion));
    qDebug() << "Visor: cargando imagen desde" << base.resolverRutaImagen(direccion);

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
