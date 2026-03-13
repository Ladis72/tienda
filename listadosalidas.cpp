#include "listadosalidas.h"
#include <QDebug>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include <QProcess>
#include <QTextDocument>
#include "ui_listadosalidas.h"

ListadoSalidas::ListadoSalidas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ListadoSalidas)
{
    ui->setupUi(this);
}

ListadoSalidas::~ListadoSalidas()
{
    delete ui;
}

void ListadoSalidas::on_pushButtonVer_clicked()
{
    QString fechaI, fechaF;
    fechaI = ui->dateEditDesde->text();
    fechaF = ui->dateEditHasta->text();
    modeloTabla = new QSqlQueryModel(this);
    modeloTabla->setQuery(
        base->listadoMovimientosEfectivo(conf->getConexionLocal(), fechaI, fechaF));
    modeloTabla->setHeaderData(0, Qt::Horizontal, "FECHA");
    modeloTabla->setHeaderData(1, Qt::Horizontal, "HORA");
    modeloTabla->setHeaderData(2, Qt::Horizontal, "CANTIDAD");
    modeloTabla->setHeaderData(3, Qt::Horizontal, "TIPO");
    //modeloTabla->setHeaderData(4,Qt::Horizontal,"DETALLE");
    ui->tableView->setModel(modeloTabla);
    ui->labelTotal->setText(QString::number(sumar(modeloTabla)));
}

double ListadoSalidas::sumar(QSqlQueryModel *modelo)
{
    double A = 0;
    for (int i = 0; i < modelo->rowCount(); ++i) {
        A += modelo->record(i).value("cantidad").toDouble();
    }
    return A;
}

void ListadoSalidas::on_pushButton_2_clicked()
{
    QString html;
    html += "<html><body>";
    html += "<h1>Informe de Movimientos</h1>";
    html += "<p>Desde: " + ui->dateEditDesde->text() + " Hasta: " + ui->dateEditHasta->text()
            + "</p>";
    html += "<table border='1'>";
    html
        += "<tr><th>Fecha</th><th>Hora</th><th>Cantidad</th><th>Tipo</th><th>Descripción</th></tr>";

    for (int row = 0; row < modeloTabla->rowCount(); ++row) {
        html += "<tr>";
        html += "<td>" + modeloTabla->record(row).value("fecha").toString() + "</td>";
        html += "<td>" + modeloTabla->record(row).value("hora").toString() + "</td>";
        html += "<td>" + modeloTabla->record(row).value("cantidad").toString() + "</td>";
        html += "<td>" + modeloTabla->record(row).value("descripcion").toString() + "</td>";
        html += "<td>" + modeloTabla->record(row).value(4).toString() + "</td>";
        html += "</tr>";
    }

    html += "</table>";
    html += "<p><b>Total: " + ui->labelTotal->text() + "</b></p>";
    html += "</body></html>";

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(base->devolverDirectorio("documentos") + "/Movimientos.pdf");
    document.print(&printer);
    QProcess::startDetached("xdg-open",
                            QStringList()
                                << base->devolverDirectorio("documentos") + "/Movimientos.pdf");
}
