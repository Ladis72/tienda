#include "pedidos.h"
#include "imprimirpedido.h"
#include "ui_pedidos.h"

#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QSqlRecord>
#include <QtMath>

extern Configuracion *conf;

/**
 * @brief Constructor de la clase pedidos.
 * @param idPed Identificador del pedido.
 * @param proveedor Nombre del proveedor.
 * @param ndoc Número de documento (albarán/pedido).
 * @param parent Widget padre.
 */
pedidos::pedidos(QString idPed, QString proveedor, QString ndoc,
                 QWidget *parent)
    : QDialog(parent), ui(new Ui::pedidos) {
  ui->setupUi(this);

  // Inicialización de datos
  proveedorNombre = proveedor;
  ui->labelProveedor->setText(proveedor);
  ui->labelDocumento->setText(ndoc);
  modeloPedido = new QSqlQueryModel(this);

  lineaSeleccionada = "";
  nDoc = ndoc;
  idPedido = idPed;
  idProveedor = base.idProveedor(proveedor, conf->getConexionLocal());
  
  // Desactivar autoDefault para evitar inserciones accidentales al pulsar ENTER
  ui->pushButtonAnadir->setAutoDefault(false);
  ui->pushButtonModificar->setAutoDefault(false);
  ui->pushButtonBorrar->setAutoDefault(false);
  ui->pushButtonImprimir->setAutoDefault(false);
  ui->pushButtonCerrar->setAutoDefault(false);

  // Cadena de navegación mediante la tecla ENTER (flujo rápido de datos)
  connect(ui->leUds, &QLineEdit::returnPressed, [this](){ ui->leBon->setFocus(); ui->leBon->selectAll(); });
  connect(ui->leBon, &QLineEdit::returnPressed, [this](){ ui->leLote->setFocus(); ui->leLote->selectAll(); });
  connect(ui->leLote, &QLineEdit::returnPressed, [this](){ ui->lePvt->setFocus(); ui->lePvt->selectAll(); });
  connect(ui->lePvt, &QLineEdit::returnPressed, [this](){ ui->leDescuento->setFocus(); ui->leDescuento->selectAll(); });
  connect(ui->leDescuento, &QLineEdit::returnPressed, [this](){ ui->leIva->setFocus(); ui->leIva->selectAll(); });
  connect(ui->leIva, &QLineEdit::returnPressed, this, &pedidos::on_pushButtonAnadir_clicked);
  
  // Llenar datos iniciales
  llenarTablaPedido(idPed);
  editando = false;
  
  // Establecer fecha actual por defecto
  ui->dateEdit->setDate(QDate::currentDate());

  // Cargar notas del pedido actual desde la tabla temporal
  QSqlQuery queryNotes(QSqlDatabase::database(conf->getConexionLocal()));
  queryNotes.prepare("SELECT notas FROM albaranes_tmp WHERE id = :id");
  queryNotes.bindValue(":id", idPedido);
  if (queryNotes.exec() && queryNotes.next()) {
      ui->leNotasPedido->setText(queryNotes.value("notas").toString());
  }

  // Guardar notas de forma reactiva al terminar de editar el campo
  connect(ui->leNotasPedido, &QLineEdit::editingFinished, this, [this]() {
      QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal()));
      query.prepare("UPDATE albaranes_tmp SET notas = :notas WHERE id = :id");
      query.bindValue(":notas", ui->leNotasPedido->text());
      query.bindValue(":id", idPedido);
      if (!query.exec()) {
          qDebug() << "Error al guardar notas en albaranes_tmp: " << query.lastError().text();
      }
  });
}

pedidos::~pedidos() {
  delete ui;
}

void pedidos::on_leCod_editingFinished() {
    // Slot vacío reservado para futuras implementaciones
}

/**
 * @brief Calcula el total de la línea actual basándose en PvT, Unidades e IVA.
 * @return String con el total calculado.
 */
QString pedidos::calcularTotalLinea()
{
    double pvt = ui->lePvt->text().toDouble();
    double uds = ui->leUds->text().toDouble();
    double desc = ui->leDescuento->text().toDouble();
    
    // Cálculo de la base imponible de la línea
    double total = pvt * uds * ((100.0 - desc) / 100.0);

    // Consulta de impuestos para el margen
    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    q.prepare("SELECT porcentaje, recargo FROM impuestos WHERE porcentaje = :porcentaje");
    q.bindValue(":porcentaje", ui->leIva->text());

    double pi = 0, pr = 0;
    if (q.exec() && q.first()) {
        pi = q.value(0).toDouble();
        pr = q.value(1).toDouble();
    } else {
        // Si no se encuentra el IVA, usamos el valor del campo como porcentaje por defecto
        pi = ui->leIva->text().toDouble();
        pr = 0;
    }

    // El recargo de equivalencia solo se aplica si la tienda lo tiene configurado
    bool esRE = (base.leerConfiguracion() == "1");
    if (!esRE) pr = 0;

    // Venta bruta estimada (Unidades + Bonificadas) * PVP
    double ventaTotal = (uds + ui->leBon->text().toDouble()) * ui->lePvp->text().toDouble();

    double margen = 0;
    if (esRE) {
        // En Régimen de Recargo de Equivalencia, los impuestos de compra se consideran coste
        double totalConImpuestos = total * (1.0 + (pi + pr) / 100.0);
        if (ventaTotal > 0)
            margen = (ventaTotal - totalConImpuestos) / ventaTotal * 100.0;
    } else {
        // En Régimen General, comparamos Base Imponible de Venta vs Base Imponible de Compra
        double ventaNeta = ventaTotal / (1.0 + pi / 100.0);
        if (ventaNeta > 0)
            margen = (ventaNeta - total) / ventaNeta * 100.0;
    }

    ui->leMargen->setText(QString::number(margen, 'f', 2));
    ui->leTotalLinea->setText(QString::number(total, 'f', 2));
    
    return QString::number(total, 'f', 2);
}

/**
 * @brief Llena la tabla de líneas del pedido y calcula los totales agrupados por IVA.
 * @param idPedido ID del pedido a mostrar.
 */
void pedidos::llenarTablaPedido(QString idPedido) {
  // Configuración del modelo principal
  QSqlQuery qPedido(QSqlDatabase::database(conf->getConexionLocal()));
  qPedido.prepare("SELECT * FROM lineaspedido_tmp WHERE idPedido = ?");
  qPedido.bindValue(0, idPedido);
  qPedido.exec();
  modeloPedido->setQuery(qPedido);
  ui->tableView->setModel(modeloPedido);
  
  // Ocultar columnas internas
  ui->tableView->hideColumn(0);  // ID
  ui->tableView->hideColumn(1);  // idPedido
  ui->tableView->hideColumn(10); // Base unitaria
  ui->tableView->hideColumn(13); // IVA total
  ui->tableView->hideColumn(14); // RE total
  ui->tableView->resizeColumnsToContents();

  // Consulta para agrupar por tipo de IVA y mostrar el resumen inferior
  QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal()));
  query.prepare("SELECT tipoIva, "
                "SUM(totalbase) AS totalBase, "
                "SUM(iva) AS totalIva, "
                "SUM(re) AS totalRe, "
                "(SUM(totalbase) + SUM(iva) + SUM(re)) AS totalGeneral "
                "FROM lineaspedido_tmp "
                "WHERE idPedido = ? "
                "GROUP BY tipoIva");
  query.bindValue(0, idPedido);
  if (!query.exec()) {
    QMessageBox::critical(this, "Error", "Error al consultar los totales por IVA");
    return;
  }

  // Modelo para la tabla de totales (desglose por IVA)
  QStandardItemModel *modelo = new QStandardItemModel(this);
  modelo->setHorizontalHeaderLabels(QStringList()
                                    << "Tipo IVA" << "Base Imponible"
                                    << "Total IVA" << "Total RE"
                                    << "Total General");

  double totalBase = 0, totalIva = 0, totalRe = 0, totalGeneral = 0;

  while (query.next()) {
    QList<QStandardItem *> fila;

    double tipoIva = query.value("tipoIva").toDouble();
    double baseVal = query.value("totalBase").toDouble();
    double ivaVal = query.value("totalIva").toDouble();
    double reVal = query.value("totalRe").toDouble();
    double totalVal = query.value("totalGeneral").toDouble();

    fila << new QStandardItem(QString::number(tipoIva) + "%")
         << new QStandardItem(QString::number(baseVal, 'f', 2))
         << new QStandardItem(QString::number(ivaVal, 'f', 2))
         << new QStandardItem(QString::number(reVal, 'f', 2))
         << new QStandardItem(QString::number(totalVal, 'f', 2));

    modelo->appendRow(fila);

    // Acumular para los campos de texto
    totalBase += baseVal;
    totalIva += ivaVal;
    totalRe += reVal;
    totalGeneral += totalVal;
  }

  // Actualizar widgets de totales
  ui->lineEditBase->setText(QString::number(totalBase, 'f', 2));
  ui->lineEditIVA->setText(QString::number(totalIva, 'f', 2));
  ui->lineEditRecargo->setText(QString::number(totalRe, 'f', 2));
  ui->lineEditTotal->setText(QString::number(totalGeneral, 'f', 2));
  
  contarArticulos();
  contarLineas();
  
  ui->tableViewTotales->setModel(modelo);
  ui->tableViewTotales->resizeColumnsToContents();
}

/**
 * @brief Limpia los campos de entrada para agregar una nueva línea.
 */
void pedidos::borrarLineEdits() {
  ui->leCod->clear();
  ui->leDescripcion->clear();
  ui->leUds->setText("1");
  ui->leBon->setText("0");
  ui->lePvt->clear();
  ui->leDescuento->setText("0");
  ui->leIva->clear();
  ui->leIva->setText("21");
  ui->lePvp->clear();
  ui->leTotalLinea->clear();
  ui->leMargen->clear();
  ui->dateEdit->setDate(QDate::currentDate());
  ui->leLote->clear();
  editando = false;
  lineaSeleccionada = "";
}

/**
 * @brief Actualiza el contador de líneas visibles.
 */
void pedidos::contarLineas() {
  int lineas = modeloPedido->rowCount();
  ui->lineEditLineas->setText(QString::number(lineas));
}

/**
 * @brief Suma el total de unidades físicas (incluyendo bonificadas).
 */
void pedidos::contarArticulos() {
  float articulos = base.sumarColumna(conf->getConexionLocal(), "lineaspedido_tmp",
                         "cantidad", "idPedido", idPedido);
  float bonificacion = base.sumarColumna(conf->getConexionLocal(), "lineaspedido_tmp",
                         "bonificacion", "idPedido", idPedido);
  ui->lineEditUnidades->setText(QString::number(articulos + bonificacion));
}

// Slots de actualización reactiva
void pedidos::on_leUds_textChanged(const QString &) { calcularTotalLinea(); }
void pedidos::on_lePvt_textChanged(const QString &) { calcularTotalLinea(); }
void pedidos::on_leIva_textChanged(const QString &) { calcularTotalLinea(); }
void pedidos::on_leDescuento_textChanged(const QString &) { calcularTotalLinea(); }
void pedidos::on_leBon_textChanged(const QString &) { calcularTotalLinea(); }
void pedidos::on_lePvp_textChanged(const QString &) { calcularTotalLinea(); }

/**
 * @brief Guarda o modifica la línea actual en la base de datos temporal.
 */
void pedidos::on_pushButtonAnadir_clicked() {
  if (ui->leCod->text().isEmpty()) {
      QMessageBox::warning(this, "Atención", "Debe introducir un código de producto.");
      ui->leCod->setFocus();
      return;
  }

  QStringList datos;
  
  // Verificar existencia del producto
  bool existe = base.existeDatoEnTabla(QSqlDatabase::database(conf->getConexionLocal()),
                             "articulos", "cod", ui->leCod->text());
  if (existe) {
    double baseTotal = ui->leTotalLinea->text().toDouble();
    double iva = 0.0;
    double re = 0.0;
    QString tipoIvaStr = ui->leIva->text();

    // Obtener los porcentajes reales de impuestos de la tabla maestra
    QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal()));
    query.prepare("SELECT porcentaje, recargo FROM impuestos WHERE porcentaje = :porcentaje");
    query.bindValue(":porcentaje", tipoIvaStr);
    
    if (query.exec() && query.first()) {
        iva = baseTotal * query.value(0).toDouble() / 100.0;
        re = baseTotal * query.value(1).toDouble() / 100.0;
    } else {
        QMessageBox::warning(this, "IVA no válido", "Debe introducir un porcentaje de IVA válido de la configuración.");
        ui->leIva->setFocus();
        ui->leIva->selectAll();
        return;
    }

    double uds = ui->leUds->text().toDouble();
    if (uds == 0) uds = 1.0; 
    
    double baseProducto = baseTotal / uds;

    // Sanitizar valores numéricos para evitar errores SQL (cadena vacía -> "0")
    auto sanitizeNum = [](QString val) { 
        return (val.trimmed().isEmpty() ? "0" : val); 
    };

    // Preparar lista de datos para baseDatos (debe contener 15 elementos)
    datos.append(idPedido);                                // 1. idPedido
    datos.append(ui->leCod->text());                        // 2. cod
    datos.append(ui->leDescripcion->text());                // 3. descripcion
    datos.append(QString::number(uds));                     // 4. cantidad
    datos.append(sanitizeNum(ui->leBon->text()));           // 5. bonificacion
    datos.append(ui->leLote->text());                       // 6. lote
    datos.append(ui->dateEdit->date().toString("yyyy-MM-dd")); // 7. fc
    datos.append(sanitizeNum(ui->lePvt->text()));           // 8. costo
    datos.append(sanitizeNum(ui->leDescuento->text()));     // 9. descuento1
    datos.append(QString::number(baseProducto, 'f', 4));    // 10. base (base unitaria)
    datos.append(sanitizeNum(tipoIvaStr));                 // 11. tipoIva
    datos.append(QString::number(baseTotal, 'f', 2));       // 12. totalbase
    datos.append(QString::number(iva, 'f', 2));            // 13. iva
    datos.append(QString::number(re, 'f', 2));             // 14. re
    datos.append(sanitizeNum(ui->lePvp->text()));           // 15. pvp

    if (editando) {
      if (!lineaSeleccionada.isEmpty()) {
        datos.append(lineaSeleccionada);
        base.modificarLineaPedido(conf->getConexionLocal(), datos);
      }
    } else {
      base.grabarLineaPedido(conf->getConexionLocal(), datos);
    }

    llenarTablaPedido(idPedido);
    borrarLineEdits();
    ui->leCod->setFocus();
  } else {
    QMessageBox::information(this, "ERROR", "No hay un producto con ese código.");
  }
}

/**
 * @brief Busca el producto por código o código auxiliar al pulsar ENTER.
 */
void pedidos::on_leCod_returnPressed() {
  QString cod = ui->leCod->text();
  if (cod.isEmpty()) return;

  QSqlRecord registro = base.consulta_producto(conf->getConexionLocal(), cod);
  
  if (registro.isEmpty()) {
    // Intentar buscar por código auxiliar
    QString codReal = base.codigoDesdeAux(conf->getConexionLocal(), cod);
    if (!codReal.isEmpty()) {
        registro = base.consulta_producto(conf->getConexionLocal(), codReal);
    }
  }
  
  if (!registro.isEmpty()) {
    ui->leCod->setText(registro.value("cod").toString());
    ui->leDescripcion->setText(registro.value("descripcion").toString());
    ui->lePvt->setText(registro.value("precio_compra").toString().isEmpty() ? 
                      registro.value("precio_venta").toString() : registro.value("precio_compra").toString());
    QString ivaArt = registro.value("iva").toString();
    ui->leIva->setText(ivaArt.isEmpty() ? "21" : ivaArt);
    ui->lePvp->setText(registro.value("pvp").toString());
    ui->leDescuento->setText(base.descuentoProveedor(proveedorNombre));
    ui->leUds->setText("1");
    ui->leUds->setFocus();
    ui->leUds->selectAll();
  } else {
    if (QMessageBox::question(this, "Producto no encontrado", 
                             "No se encuentra el producto. ¿Desea crearlo ahora?",
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
      Articulos *articuloDialog = new Articulos(this);
      articuloDialog->exec();
      delete articuloDialog;
    } else {
      ui->leCod->selectAll();
      ui->leCod->setFocus();
    }
  }
}

/**
 * @brief Borra la línea seleccionada en la tabla.
 */
void pedidos::on_pushButtonBorrar_clicked() {
  if (lineaSeleccionada.isEmpty()) {
    QMessageBox::information(this, "ATENCIÓN", "Seleccione una línea para borrar.");
    return;
  }
  
  if (QMessageBox::question(this, "Confirmar borrado", 
                           "¿Está seguro de que desea eliminar la línea seleccionada?",
                           QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
      base.borrarLineaPedido(conf->getConexionLocal(), lineaSeleccionada);
      llenarTablaPedido(idPedido);
      borrarLineEdits();
  }
}

/**
 * @brief Carga los datos de una línea de la tabla al formulario para editarla.
 */
void pedidos::on_tableView_doubleClicked(const QModelIndex &index) {
  int fila = index.row();
  QModelIndex indexId = modeloPedido->index(fila, 0);
  lineaSeleccionada = modeloPedido->data(indexId, Qt::EditRole).toString();
  
  ui->leCod->setText(modeloPedido->data(modeloPedido->index(fila, 2)).toString());
  ui->leDescripcion->setText(modeloPedido->data(modeloPedido->index(fila, 3)).toString());
  ui->leUds->setText(modeloPedido->data(modeloPedido->index(fila, 4)).toString());
  ui->leBon->setText(modeloPedido->data(modeloPedido->index(fila, 5)).toString());
  ui->leLote->setText(modeloPedido->data(modeloPedido->index(fila, 6)).toString());
  ui->dateEdit->setDate(QDate::fromString(modeloPedido->data(modeloPedido->index(fila, 7)).toString(), "yyyy-MM-dd"));
  ui->lePvt->setText(modeloPedido->data(modeloPedido->index(fila, 8)).toString());
  ui->leDescuento->setText(modeloPedido->data(modeloPedido->index(fila, 9)).toString());
  ui->leIva->setText(modeloPedido->data(modeloPedido->index(fila, 11)).toString());
  ui->lePvp->setText(modeloPedido->data(modeloPedido->index(fila, 15)).toString());
  
  calcularTotalLinea();
  editando = true;
  ui->leUds->setFocus();
  ui->leUds->selectAll();
}

void pedidos::on_tableView_clicked(const QModelIndex &index) {
  QModelIndex indice = modeloPedido->index(index.row(), 0);
  lineaSeleccionada = modeloPedido->data(indice, Qt::EditRole).toString();
}

/**
 * @brief Busca productos por descripción al pulsar ENTER en el campo de descripción.
 */
void pedidos::on_leDescripcion_returnPressed() {
  QString busca = ui->leDescripcion->text();
  if (busca.isEmpty()) return;
  
  consulta = base.buscarProducto(QSqlDatabase::database(conf->getConexionLocal()),
                          "articulos", busca);
  
  BuscarProducto *buscarDialog = new BuscarProducto(this, consulta);
  if (buscarDialog->exec() == QDialog::Accepted) {
      ui->leCod->setText(buscarDialog->resultado);
      on_leCod_returnPressed();
  }
  delete buscarDialog;
}

void pedidos::on_pushButtonModificar_clicked() {
  on_pushButtonAnadir_clicked();
}

/**
 * @brief Valida que el IVA introducido exista en la tabla de impuestos.
 */
void pedidos::on_leIva_editingFinished() {
  if (ui->leIva->text().isEmpty()) return;
  
  QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal()));
  query.prepare("SELECT porcentaje FROM impuestos WHERE porcentaje = :por");
  query.bindValue(":por", ui->leIva->text());
  query.exec();

  if (!query.first()) {
    QMessageBox::warning(this, "IVA no válido", "El porcentaje de IVA ingresado no existe en la configuración del sistema.\nPor favor, introduzca un valor correcto (ej: 0, 4, 10, 21).");
    ui->leIva->setFocus();
    ui->leIva->selectAll();
  }
  calcularTotalLinea();
}

void pedidos::on_dateEdit_editingFinished() {
  // Validación opcional de fecha de caducidad
}

/**
 * @brief Imprime el pedido actual.
 */
void pedidos::on_pushButtonImprimir_clicked() {
  QString tienda = conf->getConexionLocal();
  QStringList cabecera;
  cabecera << nDoc
           << proveedorNombre
           << ui->lineEditBase->text()
           << ui->lineEditIVA->text()
           << ui->lineEditRecargo->text()
           << ui->lineEditTotal->text();
           
  imprimirPedido pedido(tienda, cabecera, modeloPedido);
}

void pedidos::on_pushButtonCerrar_clicked() {
  this->accept();
}
