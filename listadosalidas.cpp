#include "listadosalidas.h"
#include <QDebug>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include <QProcess>
#include <QTextDocument>
#include <QDate>
#include <QSqlRecord>
#include "ui_listadosalidas.h"

ListadoSalidas::ListadoSalidas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ListadoSalidas)
{
    ui->setupUi(this);

    // Fechas por defecto: mes actual
    QDate hoy = QDate::currentDate();
    ui->dateEditDesde->setDate(QDate(hoy.year(), hoy.month(), 1));
    ui->dateEditHasta->setDate(hoy);

    cargarTipos();
}

ListadoSalidas::~ListadoSalidas()
{
    delete ui;
}

void ListadoSalidas::on_pushButtonVer_clicked()
{
    QString fechaI, fechaF;
    fechaI = ui->dateEditDesde->date().toString("yyyy-MM-dd");
    fechaF = ui->dateEditHasta->date().toString("yyyy-MM-dd");

    int idTipo = ui->comboBoxTipo->currentData().toInt();

    modeloTabla = new QSqlQueryModel(this);
    modeloTabla->setQuery(
        base->listadoMovimientosEfectivo(conf->getConexionLocal(), fechaI, fechaF, idTipo));
    modeloTabla->setHeaderData(0, Qt::Horizontal, "FECHA");
    modeloTabla->setHeaderData(1, Qt::Horizontal, "HORA");
    modeloTabla->setHeaderData(2, Qt::Horizontal, "CANTIDAD");
    modeloTabla->setHeaderData(3, Qt::Horizontal, "TIPO");
    modeloTabla->setHeaderData(4, Qt::Horizontal, "DESCRIPCIÓN");

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
    QString html = R"(
<html>
<head>
  <meta charset='utf-8'>
  <style>
    body { font-family: Arial, sans-serif; font-size: 10pt; color: #333; }
    .header { text-align: center; padding: 10px; border-bottom: 2px solid #2c3e50; margin-bottom: 20px; }
    h2 { color: #2c3e50; margin-bottom: 5px; }
    table { width: 100%; border-collapse: collapse; margin-top: 10px; }
    th { background-color: #2c3e50; color: white; padding: 8px; font-size: 9pt; }
    td { border: 1px solid #ccc; padding: 6px; text-align: center; font-size: 9pt; }
    .total-row { font-weight: bold; background-color: #f9f9f9; color: #e74c3c; font-size: 12pt; margin-top: 20px; text-align: right; }
  </style>
</head>
<body>
  <div class='header'>
    <h2>Informe de Movimientos de Efectivo</h2>
    <div>Desde: %DESDE% | Hasta: %HASTA% | Tipo: %TIPO%</div>
  </div>

  <table>
    <thead>
      <tr>
        <th>FECHA</th>
        <th>HORA</th>
        <th>CANTIDAD</th>
        <th>TIPO</th>
        <th>DESCRIPCIÓN</th>
      </tr>
    </thead>
    <tbody>
      %FILAS%
    </tbody>
  </table>

  <div class='total-row'>TOTAL ACUMULADO: %TOTAL% €</div>
</body>
</html>
)";

    html.replace("%DESDE%", ui->dateEditDesde->text());
    html.replace("%HASTA%", ui->dateEditHasta->text());
    html.replace("%TIPO%", ui->comboBoxTipo->currentText());
    html.replace("%TOTAL%", ui->labelTotal->text());

    QString filas = "";
    for (int row = 0; row < modeloTabla->rowCount(); ++row) {
        filas += "<tr>";
        filas += "<td>" + modeloTabla->record(row).value("fecha").toString() + "</td>";
        filas += "<td>" + modeloTabla->record(row).value("hora").toString() + "</td>";
        filas += "<td>" + modeloTabla->record(row).value("cantidad").toString() + " €</td>";
        filas += "<td>" + modeloTabla->record(row).value("descripcion").toString() + "</td>";
        filas += "<td>" + modeloTabla->record(row).value(4).toString() + "</td>";
        filas += "</tr>";
    }
    html.replace("%FILAS%", filas);

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

void ListadoSalidas::cargarTipos()
{
    ui->comboBoxTipo->clear();
    ui->comboBoxTipo->addItem("Todos los tipos", -1);

    QSqlQuery q = base->ejecutarSentencia("SELECT idtiposEntrada, descripcion FROM motivosEntrada ORDER BY descripcion", conf->getConexionLocal());
    while (q.next()) {
        ui->comboBoxTipo->addItem(q.value("descripcion").toString(), q.value("idtiposEntrada").toInt());
    }
}
