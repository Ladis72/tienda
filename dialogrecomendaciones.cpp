#include "dialogrecomendaciones.h"
#include "ui_dialogrecomendaciones.h"
#include <QHeaderView>

DialogRecomendaciones::DialogRecomendaciones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRecomendaciones),
    m_modelo(new QStandardItemModel(this))
{
    ui->setupUi(this);

    m_modelo->setColumnCount(6);
    m_modelo->setHeaderData(0, Qt::Horizontal, "Cód / EAN");
    m_modelo->setHeaderData(1, Qt::Horizontal, "Descripción");
    m_modelo->setHeaderData(2, Qt::Horizontal, "Caducidad");
    m_modelo->setHeaderData(3, Qt::Horizontal, "Stock");
    m_modelo->setHeaderData(4, Qt::Horizontal, "Venta Local (Media Mes)");
    m_modelo->setHeaderData(5, Qt::Horizontal, "Sugerencia Inteligente");

    ui->tableViewRecomendaciones->setModel(m_modelo);
    ui->tableViewRecomendaciones->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableViewRecomendaciones->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
}

DialogRecomendaciones::~DialogRecomendaciones()
{
    delete ui;
}

void DialogRecomendaciones::cargarRecomendaciones(const QList<RecomendacionCaducidad> &recomendaciones)
{
    m_modelo->setRowCount(0);
    
    for (int i = 0; i < recomendaciones.size(); ++i) {
        const RecomendacionCaducidad &rec = recomendaciones.at(i);
        
        QStandardItem *itemEan = new QStandardItem(rec.ean);
        QStandardItem *itemDesc = new QStandardItem(rec.descripcion);
        QStandardItem *itemCad = new QStandardItem(rec.fechaCaducidad.toString("yyyy-MM-dd"));
        QStandardItem *itemStock = new QStandardItem(QString::number(rec.stockLote));
        QStandardItem *itemMedia = new QStandardItem(QString::number(rec.mediaVentaMensualLocal, 'f', 1));
        QStandardItem *itemSug = new QStandardItem(rec.sugerencia);
        
        // Colorear filas según nivel de alerta
        if (rec.nivelAlerta == 2) {
            itemCad->setBackground(QBrush(QColor(255, 200, 200))); // Rojo claro
            itemSug->setBackground(QBrush(QColor(255, 200, 200)));
        } else if (rec.nivelAlerta == 1) {
            itemCad->setBackground(QBrush(QColor(255, 255, 200))); // Amarillo claro
        }
        
        // Bloquear edición
        itemEan->setEditable(false);
        itemDesc->setEditable(false);
        itemCad->setEditable(false);
        itemStock->setEditable(false);
        itemMedia->setEditable(false);
        itemSug->setEditable(false);

        m_modelo->setItem(i, 0, itemEan);
        m_modelo->setItem(i, 1, itemDesc);
        m_modelo->setItem(i, 2, itemCad);
        m_modelo->setItem(i, 3, itemStock);
        m_modelo->setItem(i, 4, itemMedia);
        m_modelo->setItem(i, 5, itemSug);
    }
}
