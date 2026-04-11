#include "listadoarqueos.h"
#include "qprinter.h"
#include "qprocess.h"
#include "qtextdocument.h"
#include "ui_listadoarqueos.h"

ListadoArqueos::ListadoArqueos(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ListadoArqueos)
{
    ui->setupUi(this);
    ui->dateEditDesde->setDate(QDate::currentDate());
    ui->dateEditHasta->setDate(QDate::currentDate());
}

ListadoArqueos::~ListadoArqueos()
{
    delete ui;
}

void ListadoArqueos::on_pushButtonConsultar_clicked()
{
    QString fechaI, fechaF;
    fechaI = ui->dateEditDesde->text();
    fechaF = ui->dateEditHasta->text();
    qDebug() << fechaF << "   " << fechaI;
    modeloTabla = new QSqlTableModel(this, QSqlDatabase::database(conf->getConexionLocal()));
    modeloTabla->setTable("arqueos");
    modeloTabla->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modeloTabla->setFilter("fecha >= '" + fechaI + "' AND fecha <= '" + fechaF + "'");
    modeloTabla->select();
    ui->tableView->setModel(modeloTabla);
}

void ListadoArqueos::on_pushButtonImprimir_clicked()
{
    QTextDocument documento;
    QString lineasHtml = "";
    QString html = R"(
<html>
<head>
  <meta charset='utf-8'>
  <style>
    body { font-family: Arial, sans-serif; font-size: 9pt; color: #333; }
    .header { text-align: center; padding: 10px; border-bottom: 2px solid #2c3e50; margin-bottom: 20px; }
    h2 { color: #2c3e50; margin-bottom: 5px; }
    table { width: 100%; border-collapse: collapse; margin-top: 10px; }
    th { background-color: #2c3e50; color: white; padding: 6px; font-size: 8pt; }
    td { border: 1px solid #ccc; padding: 5px; text-align: center; font-size: 8pt; }
    .rango { font-size: 10pt; color: #666; }
  </style>
</head>
<body>
  <div class='header'>
    <h2>Listado de Arqueos de Caja</h2>
    <div class='rango'>Desde: <strong>%DESDE%</strong> | Hasta: <strong>%HASTA%</strong></div>
  </div>

  <table>
    <thead>
      <tr>
        <th>FECHA</th>
        <th>HORA</th>
        <th>V. EFECT.</th>
        <th>V. TARJ.</th>
        <th>ENTR.</th>
        <th>EFECT.</th>
        <th>DESCUAD.</th>
        <th>CONTADO</th>
        <th>USUARIO</th>
      </tr>
    </thead>
    <tbody>
      %LINEAS%
    </tbody>
  </table>
</body>
</html>
)";
    for (int i = 0; i < modeloTabla->rowCount(); ++i) {
        QString fecha = modeloTabla->data(modeloTabla->index(i, 1)).toString();
        QString hora = modeloTabla->data(modeloTabla->index(i, 2)).toString();
        QString vefect = modeloTabla->data(modeloTabla->index(i, 3)).toString();
        QString vtarj = modeloTabla->data(modeloTabla->index(i, 4)).toString();
        QString entradas = modeloTabla->data(modeloTabla->index(i, 5)).toString();
        QString efectivo = modeloTabla->data(modeloTabla->index(i, 6)).toString();
        QString descuadre = modeloTabla->data(modeloTabla->index(i, 7)).toString();
        QString efectContado = modeloTabla->data(modeloTabla->index(i, 8)).toString();
        QString usuario = modeloTabla->data(modeloTabla->index(i, 9)).toString();

        lineasHtml += QString("<tr><td>%1</td><td>%2</td><td>%3 €</td><td>%4 €</td><td>%5 "
                              "€</td><td>%6 €</td><td>%7 €</td><td>%8 €</td><td>%9</td></tr>")
                          .arg(fecha, hora, vefect, vtarj, entradas, efectivo, descuadre,
                               efectContado, usuario);
    }
    html.replace("%DESDE%", ui->dateEditDesde->text());
    html.replace("%HASTA%", ui->dateEditHasta->text());
    html.replace("%LINEAS%", lineasHtml);
    documento.setHtml(html);

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(base->devolverDirectorio("documentos") + "/ListadoArqueos.pdf");
    documento.print(&printer);

    // Mostrarlo
    QProcess::startDetached("xdg-open",
                            QStringList()
                                << base->devolverDirectorio("documentos") + "/ListadoArqueos.pdf");
}
