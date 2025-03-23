#include "etiquetas.h"
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QUrl>
#include <QPrinter>
#include "ui_etiquetas.h"
#include <QFile>
#include <QDesktopServices>

Etiquetas::Etiquetas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Etiquetas)
{
    ui->setupUi(this);
    modelo = new QStandardItemModel;
    llenarModelo();
}

Etiquetas::~Etiquetas()
{
    delete ui;
}

void Etiquetas::on_pushButtonImprimir_clicked()
{
      QList<QList<QString>> datos;
    datos.clear();
      for (int i = 0; i < modelo->rowCount(); ++i) {
          QList<QString> datosFila;
          QString nombre = modelo->item(i,1)->text();
          if (nombre.length() > 25)  {
              nombre.insert(nombre.indexOf(" ", 12), "\n");
          }
          datosFila << nombre << modelo->item(i,2)->text();

          QString formato = modelo->item(i, 3)->text();
          QString formatoEtiqueta;
          if (formato == "Uds") {
              formatoEtiqueta = "Precio/ud: ";
              double pvp = classFormatear.redondear(modelo->item(i, 2)->text().toDouble()
                                                        / modelo->item(i, 4)->text().toDouble(),
                                                    3);
              formatoEtiqueta += QString::number(pvp);
          }
          if (formato == "Peso") {
              formatoEtiqueta = "Precio/Kg: ";
              double pvp = classFormatear.redondear(modelo->item(i, 2)->text().toDouble() * 1000
                                                        / modelo->item(i, 4)->text().toDouble(),
                                                    3);
              formatoEtiqueta += QString::number(pvp);
          }
          if (formato == "Volumen") {
              formatoEtiqueta = "Precio/L: ";
              double pvp = classFormatear.redondear(modelo->item(i, 2)->text().toDouble() * 1000
                                                        / modelo->item(i, 4)->text().toDouble(),
                                                    3);
              formatoEtiqueta += QString::number(pvp);
          }
          if (formato == "No definido") {
              formatoEtiqueta = "---/--- ";
          }
          datosFila << formatoEtiqueta;
          datos << datosFila;

      }
      generarPDF(datos);
      abriPDF(QUrl::fromLocalFile(base->cargarDirectorios(conf->getConexionLocal()).at(2)).toString());

}

void Etiquetas::on_lineEditCod_returnPressed()
{
    if (base->existeDatoEnTabla(QSqlDatabase::database("DB"),
                                "articulos",
                                "cod",
                                ui->lineEditCod->text())) {
        base->insertarEtiqueta(conf->getConexionLocal(), ui->lineEditCod->text());
        llenarModelo();
    }
}

void Etiquetas::llenarModelo()
{
    modelo->clear();
    QSqlQuery listaEtiquetas = base->ejecutarSentencia("SELECT * FROM etiquetas",
                                                       conf->getConexionLocal());
    listaEtiquetas.first();
    for (int i = 0; i < listaEtiquetas.numRowsAffected(); ++i) {
        qDebug() << "COD: " << listaEtiquetas.record().value(0).toString();
        QSqlQuery producto = base->consulta_producto("DB",
                                                     listaEtiquetas.record().value(0).toString());
        producto.first();
        double pvp = producto.value(2).toDouble();
        QStandardItem *cod = new QStandardItem(producto.value(0).toString());
        QStandardItem *nombre = new QStandardItem(producto.value(1).toString());
        QStandardItem *precio = new QStandardItem(QString::number(pvp, 'f', 2));
        QStandardItem *formato = new QStandardItem(producto.value("formato").toString());
        QStandardItem *cantidad = new QStandardItem(producto.value("cantformato").toString());
        modelo->setItem(i, 0, cod);
        modelo->setItem(i, 1, nombre);
        modelo->setItem(i, 2, precio);
        modelo->setItem(i, 3, formato);
        modelo->setItem(i, 4, cantidad);
        listaEtiquetas.next();
    }
    ui->tableView->setModel(modelo);
    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
    ui->lineEditCod->clear();
    ui->lineEditDesc->clear();
    ui->lineEditCod->setFocus();
}

void Etiquetas::on_lineEditDesc_returnPressed()
{
    QSqlQuery consulta = base->buscarProducto(QSqlDatabase::database("DB"),
                                              "articulos",
                                              ui->lineEditDesc->text());
    consulta.first();
    BuscarProducto *buscarProducto = new BuscarProducto(this, consulta);
    buscarProducto->exec();
    ui->lineEditCod->setText(buscarProducto->resultado);
    emit on_lineEditCod_returnPressed();
}

void Etiquetas::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice = modelo->index(index.row(), 0);

    QVariant dato = modelo->data(indice, Qt::EditRole);

    articuloSeleccionado = dato.toString();
}

void Etiquetas::on_pushButton_2_clicked()
{
    if (articuloSeleccionado.isEmpty()) {
        QMessageBox *aviso = new QMessageBox(this);
        aviso->setWindowTitle("Aviso");
        aviso->setText("No se ha seleccionado ningún artículo para borrar");
        aviso->exec();
    } else {
        base->ejecutarSentencia("DELETE FROM etiquetas WHERE cod = '" + articuloSeleccionado + "'",
                                conf->getConexionLocal());
        llenarModelo();
    }
}

void Etiquetas::on_pushButton_3_clicked()
{
    QMessageBox *pregunta = new QMessageBox(this);
    pregunta->setWindowTitle("Borrar");
    pregunta->setText("¿Está seguro que quiere borrar todas las etiquetas?");
    pregunta->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    pregunta->setDefaultButton(QMessageBox::Cancel);
    if (pregunta->exec() == QMessageBox::Yes) {
        qDebug() << "aceptado";
        base->ejecutarSentencia("TRUNCATE etiquetas", conf->getConexionLocal());
        llenarModelo();
    }
    return;
}

void Etiquetas::generarPDF(const QList<QList<QString> > datos)
{
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("etiquetas.pdf");
    qDebug() << base->cargarDirectorios(conf->getConexionLocal()).at(2);

    //printer.setPaperSize(QPrinter::A4);
    printer.setPageSize(QPageSize::A4);
    printer.setPageMargins(QMarginsF(5, 5, 5, 5));

    QPainter painter(&printer);

    // 📏 Tamaño de cada etiqueta en mm
    int anchoEtiqueta = 65;  // Ancho en mm
    int altoEtiqueta = 30;   // Alto en mm
    int margenX = 3;         // Espacio horizontal entre etiquetas
    int margenY = 4;        // Espacio vertical entre filas

    // 🔄 Convertir de mm a puntos (1 mm ≈ 2.83 puntos)
    anchoEtiqueta *= 2.83;
    altoEtiqueta *= 2.83;
    margenX *= 2.83;
    margenY *= 2.83;

    int columnas = 3;  // 3 etiquetas por fila
    int filasPorPagina = printer.pageLayout().paintRect().height() / (altoEtiqueta + margenY);

    int x = margenX, y = margenY;
    int etiquetasEnPagina = 0;



    for (int i = 0; i < datos.size(); i++) {
        QRect rect(x, y, anchoEtiqueta, altoEtiqueta);
        painter.drawRect(rect);

        // 📌 Dibujar el contenido de la etiqueta
        int textY = y + 3;

        // Nombre del producto
        QFont fuenteProducto("Arial", 12, QFont::Bold);
        painter.setFont(fuenteProducto);
        painter.drawText(QRect(x +5, textY, anchoEtiqueta -3, 27),Qt::AlignCenter, datos[i][0]);
        textY += 18;

        QFont fuentePrecio("Arial", 34, QFont::Bold);
        painter.setFont(fuentePrecio);
        painter.drawText(QRect(x+5,textY,anchoEtiqueta -3, 50),Qt::AlignCenter, datos[i][1]);
        textY += 45;

        QFont fuenteValor("Arial", 9);
        painter.setFont(fuenteValor);
        painter.drawText(QRect(x +5, textY, anchoEtiqueta -10, 12),Qt::AlignRight, datos[i][2]);

        // 📍 Moverse a la siguiente columna
        x += anchoEtiqueta + margenX;

        // 📌 Si llegamos a la 3ª etiqueta en una fila, saltamos a la siguiente
        if ((i + 1) % columnas == 0) {
            x = margenX;
            y += altoEtiqueta + margenY;
        }

        etiquetasEnPagina++;

        // 📝 Si llenamos la página, creamos una nueva
        if (etiquetasEnPagina >= (filasPorPagina * columnas)) {
            printer.newPage();
            x = margenX;
            y = margenY;
            etiquetasEnPagina = 0;
        }
    }

    painter.end();
    qDebug() << "Archivo etiquetas.pdf generado correctamente.";
}

void Etiquetas::abriPDF(const QString &ruta)
{
    QDesktopServices::openUrl(ruta);
}
