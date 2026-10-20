#include "dialoginventario.h"
#include "ui_dialoginventario.h"
#include "configuracion.h"
#include "stock.h"
#include "syncmanager.h"

#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QPrinter>
#include <QProcess>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextDocument>
#include <QTreeWidgetItem>

extern Configuracion *conf;

/**
 * @brief Constructor del diálogo de inventario y desglose de existencias.
 * @param origenInicial Ámbito o tienda seleccionado inicialmente desde Estadísticas.
 * @param parent Widget padre.
 */
DialogInventario::DialogInventario(const InfoOrigenEstadisticas &origenInicial, QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogInventario), origenActual(origenInicial),
      modeloArticulos(nullptr), modeloLotes(nullptr), modeloFamilias(nullptr) {
  ui->setupUi(this);

  // Inicializar componentes y filtros
  inicializarFiltros();

  // Fecha del reporte de inventario
  ui->lblFechaInventario->setText("Generado: " + QDate::currentDate().toString("yyyy-MM-dd") +
                                  " " + QTime::currentTime().toString("hh:mm"));
}

/**
 * @brief Destructor de la clase DialogInventario.
 */
DialogInventario::~DialogInventario() {
  if (modeloArticulos) delete modeloArticulos;
  if (modeloLotes) delete modeloLotes;
  if (modeloFamilias) delete modeloFamilias;
  delete ui;
}

/**
 * @brief Evento showEvent para cargar los datos en primer plano.
 */
void DialogInventario::showEvent(QShowEvent *event) {
  QDialog::showEvent(event);
  cargarDatosDesdeBD();
}

/**
 * @brief Inicializa los selectores de filtros de tienda, familia y tipos de existencias.
 */
void DialogInventario::inicializarFiltros() {
  ui->comboTienda->blockSignals(true);
  ui->comboFamilia->blockSignals(true);
  ui->comboFiltroStock->blockSignals(true);

  // 1. Selector de tiendas / ámbitos
  ui->comboTienda->clear();

  int idLocal = conf ? conf->getIdTienda() : 1;
  QString localConn = conf ? conf->getConexionLocal() : "DB";
  QString tiendaLocalNombre = "Tienda Local";
  if (QSqlDatabase::database(localConn).isOpen()) {
    QSqlQuery qNom(QSqlDatabase::database(localConn));
    if (qNom.exec("SELECT nombre FROM tiendas WHERE local = 1 LIMIT 1") && qNom.next()) {
      tiendaLocalNombre = qNom.value(0).toString();
    }
  }

  // Opción tienda local
  QVariantMap dataLocal;
  dataLocal["tipo"] = static_cast<int>(TipoOrigenEstadisticas::Local);
  dataLocal["idTienda"] = idLocal;
  dataLocal["conn"] = localConn;
  dataLocal["nombre"] = tiendaLocalNombre;
  ui->comboTienda->addItem("🏠 " + tiendaLocalNombre + " [Local]", dataLocal);

  // Opción Nube central (si está disponible)
  bool nubeDisponible = QSqlDatabase::contains(SyncManager::CONEXION_NUBE) &&
                        QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen();

  if (nubeDisponible) {
    QVariantMap dataGlobal;
    dataGlobal["tipo"] = static_cast<int>(TipoOrigenEstadisticas::NubeGlobal);
    dataGlobal["idTienda"] = -1;
    dataGlobal["nombre"] = "Todas las tiendas [Nube Central]";
    ui->comboTienda->addItem("☁️ Todas las tiendas [Nube Central]", dataGlobal);

    // Tiendas individuales en la nube
    QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
    QSqlQuery qTiendas(dbNube);
    if (qTiendas.exec("SELECT id, nombre FROM tiendas ORDER BY id ASC")) {
      while (qTiendas.next()) {
        int idT = qTiendas.value(0).toInt();
        QString nomT = qTiendas.value(1).toString();
        QVariantMap dataRemota;
        dataRemota["tipo"] = static_cast<int>(TipoOrigenEstadisticas::NubeTienda);
        dataRemota["idTienda"] = idT;
        dataRemota["nombre"] = nomT;
        ui->comboTienda->addItem("☁️ " + nomT + " [Nube]", dataRemota);
      }
    }
  }

  // Preseleccionar la tienda según origenInicial
  int idxSeleccion = 0;
  for (int i = 0; i < ui->comboTienda->count(); ++i) {
    QVariantMap m = ui->comboTienda->itemData(i).toMap();
    if (m.value("tipo").toInt() == static_cast<int>(origenActual.tipo) &&
        m.value("idTienda").toInt() == origenActual.idTienda) {
      idxSeleccion = i;
      break;
    }
  }
  ui->comboTienda->setCurrentIndex(idxSeleccion);

  // 2. Selector de familias
  ui->comboFamilia->clear();
  ui->comboFamilia->addItem("📂 Todas las Familias", -1);

  if (nubeDisponible) {
    QSqlQuery qFam = base.listaFamiliasNube();
    while (qFam.next()) {
      ui->comboFamilia->addItem(qFam.value(1).toString(), qFam.value(0).toInt());
    }
  } else {
    QSqlQuery qFam(QSqlDatabase::database(conf ? conf->getConexionLocal() : "DB"));
    if (qFam.exec("SELECT id, descripcion FROM familias ORDER BY descripcion ASC")) {
      while (qFam.next()) {
        ui->comboFamilia->addItem(qFam.value(1).toString(), qFam.value(0).toInt());
      }
    }
  }

  // 3. Selector de filtro de existencias
  ui->comboFiltroStock->clear();
  ui->comboFiltroStock->addItem("Solo con Stock disponible (> 0)", 1);
  ui->comboFiltroStock->addItem("Catálogo Completo (con y sin stock)", 0);
  ui->comboFiltroStock->addItem("Sin Stock o Negativo (<= 0)", 2);
  ui->comboFiltroStock->setCurrentIndex(0);

  ui->comboTienda->blockSignals(false);
  ui->comboFamilia->blockSignals(false);
  ui->comboFiltroStock->blockSignals(false);
}

/**
 * @brief Carga todos los datos de artículos y lotes desde la base de datos correspondiente.
 */
void DialogInventario::cargarDatosDesdeBD() {
  articulosCargados.clear();

  // Actualizar origen actual según combo
  QVariantMap m = ui->comboTienda->currentData().toMap();
  origenActual.tipo = static_cast<TipoOrigenEstadisticas>(m.value("tipo").toInt());
  origenActual.idTienda = m.value("idTienda", -1).toInt();
  origenActual.nombreConexion = m.value("conn", conf ? conf->getConexionLocal() : "DB").toString();
  origenActual.nombreTienda = m.value("nombre", ui->comboTienda->currentText()).toString();

  // Etiqueta de cabecera con el origen consultado
  ui->lblInfoOrigen->setText("Fuente de datos: " + origenActual.nombreTienda);

  QMap<QString, ArticuloInventario> mapaArticulos;

  // CASO 1: Consulta a la Nube (stock_tiendas_nube)
  if (origenActual.tipo == TipoOrigenEstadisticas::NubeGlobal ||
      origenActual.tipo == TipoOrigenEstadisticas::NubeTienda) {
    QSqlDatabase dbNube = base.obtenerConexionNube();
    if (dbNube.isValid() && dbNube.isOpen()) {
      QString sql =
          "SELECT s.id_tienda, COALESCE(t.nombre, CONCAT('Tienda ', s.id_tienda)) AS tienda_nombre, "
          "s.cod, COALESCE(a.descripcion, 'Sin Descripción') AS descripcion, "
          "COALESCE(s.lote, '') AS lote, s.fecha AS caducidad, "
          "COALESCE(f.id, 0) AS id_familia, COALESCE(f.descripcion, 'Sin Familia') AS familia, "
          "COALESCE(a.pvp, 0.0) AS pvp, COALESCE(a.precio_compra, 0.0) AS coste, "
          "s.cantidad AS cantidad "
          "FROM stock_tiendas_nube s "
          "LEFT JOIN articulos a ON s.cod = a.cod "
          "LEFT JOIN familias f ON a.familia = f.id "
          "LEFT JOIN tiendas t ON s.id_tienda = t.id ";

      if (origenActual.tipo == TipoOrigenEstadisticas::NubeTienda) {
        if (origenActual.idTienda == 1) {
          sql += "WHERE s.id_tienda IN (0, 1) ";
        } else {
          sql += QString("WHERE s.id_tienda = %1 ").arg(origenActual.idTienda);
        }
      }

      sql += "ORDER BY f.descripcion ASC, a.descripcion ASC, s.fecha ASC";

      QSqlQuery q(dbNube);
      if (q.exec(sql)) {
        while (q.next()) {
          QString cod = q.value("cod").toString();
          if (cod.trimmed().isEmpty()) continue;

          if (!mapaArticulos.contains(cod)) {
            ArticuloInventario art;
            art.codigo = cod;
            art.descripcion = q.value("descripcion").toString();
            art.idFamilia = q.value("id_familia").toInt();
            art.familia = q.value("familia").toString();
            art.pvp = q.value("pvp").toDouble();
            art.coste = q.value("coste").toDouble();
            art.stockTotal = 0.0;
            mapaArticulos.insert(cod, art);
          }

          double cantidad = q.value("cantidad").toDouble();
          DetalleLoteInventario d;
          d.lote = q.value("lote").toString();
          d.fechaCaducidad = q.value("caducidad").toDate();
          d.cantidad = cantidad;
          d.idTienda = q.value("id_tienda").toInt();
          d.nombreTienda = q.value("tienda_nombre").toString();

          ArticuloInventario &refArt = mapaArticulos[cod];
          refArt.lotes.append(d);
          refArt.stockTotal += cantidad;

          if (d.fechaCaducidad.isValid() && d.fechaCaducidad > QDate(2000, 1, 1)) {
            if (!refArt.proximaCaducidad.isValid() || d.fechaCaducidad < refArt.proximaCaducidad) {
              refArt.proximaCaducidad = d.fechaCaducidad;
            }
          }
        }
      } else {
        qDebug() << "Error consultando stock_tiendas_nube:" << q.lastError().text();
      }

      // Si se desea el catálogo completo, cargar artículos que puedan tener 0 stock
      if (ui->comboFiltroStock->currentData().toInt() == 0) {
        QSqlQuery qArt(dbNube);
        if (qArt.exec("SELECT a.cod, COALESCE(a.descripcion, 'Sin Descripción') AS descripcion, "
                      "COALESCE(f.id, 0) AS id_familia, COALESCE(f.descripcion, 'Sin Familia') AS familia, "
                      "COALESCE(a.pvp, 0.0) AS pvp, COALESCE(a.precio_compra, 0.0) AS coste "
                      "FROM articulos a "
                      "LEFT JOIN familias f ON a.familia = f.id")) {
          while (qArt.next()) {
            QString cod = qArt.value("cod").toString();
            if (!mapaArticulos.contains(cod)) {
              ArticuloInventario art;
              art.codigo = cod;
              art.descripcion = qArt.value("descripcion").toString();
              art.idFamilia = qArt.value("id_familia").toInt();
              art.familia = qArt.value("familia").toString();
              art.pvp = qArt.value("pvp").toDouble();
              art.coste = qArt.value("coste").toDouble();
              art.stockTotal = 0.0;
              mapaArticulos.insert(cod, art);
            }
          }
        }
      }
    }
  } else {
    // CASO 2: Consulta a Base de Datos Local o Remota Directa
    QString connName = origenActual.nombreConexion;
    QSqlDatabase db = QSqlDatabase::database(connName);
    if (!db.isValid() || !db.isOpen()) {
      db = QSqlDatabase::database(conf ? conf->getConexionLocal() : "DB");
    }

    // 1. Cargar lotes físicos existentes
    QString sqlLotes =
        "SELECT l.ean AS cod, COALESCE(a.descripcion, 'Sin Descripción') AS descripcion, "
        "COALESCE(l.lote, '') AS lote, l.fecha AS caducidad, "
        "COALESCE(f.id, 0) AS id_familia, COALESCE(f.descripcion, 'Sin Familia') AS familia, "
        "COALESCE(a.pvp, 0.0) AS pvp, COALESCE(a.precio_compra, 0.0) AS coste, "
        "l.cantidad AS cantidad "
        "FROM lotes l "
        "JOIN articulos a ON l.ean = a.cod "
        "LEFT JOIN familias f ON a.familia = f.id "
        "WHERE l.cantidad != 0 "
        "ORDER BY f.descripcion ASC, a.descripcion ASC, l.fecha ASC";

    QSqlQuery q(db);
    if (q.exec(sqlLotes)) {
      while (q.next()) {
        QString cod = q.value("cod").toString();
        if (cod.trimmed().isEmpty()) continue;

        if (!mapaArticulos.contains(cod)) {
          ArticuloInventario art;
          art.codigo = cod;
          art.descripcion = q.value("descripcion").toString();
          art.idFamilia = q.value("id_familia").toInt();
          art.familia = q.value("familia").toString();
          art.pvp = q.value("pvp").toDouble();
          art.coste = q.value("coste").toDouble();
          art.stockTotal = 0.0;
          mapaArticulos.insert(cod, art);
        }

        double cantidad = q.value("cantidad").toDouble();
        DetalleLoteInventario d;
        d.lote = q.value("lote").toString();
        d.fechaCaducidad = q.value("caducidad").toDate();
        d.cantidad = cantidad;
        d.idTienda = conf ? conf->getIdTienda() : 1;
        d.nombreTienda = origenActual.nombreTienda;

        ArticuloInventario &refArt = mapaArticulos[cod];
        refArt.lotes.append(d);
        refArt.stockTotal += cantidad;

        if (d.fechaCaducidad.isValid() && d.fechaCaducidad > QDate(2000, 1, 1)) {
          if (!refArt.proximaCaducidad.isValid() || d.fechaCaducidad < refArt.proximaCaducidad) {
            refArt.proximaCaducidad = d.fechaCaducidad;
          }
        }
      }
    } else {
      qDebug() << "Error consultando lotes locales:" << q.lastError().text();
    }

    // 2. Si se selecciona catálogo completo o sin stock, cargar artículos sin lotes
    if (ui->comboFiltroStock->currentData().toInt() == 0 ||
        ui->comboFiltroStock->currentData().toInt() == 2) {
      QSqlQuery qArt(db);
      if (qArt.exec("SELECT a.cod, COALESCE(a.descripcion, 'Sin Descripción') AS descripcion, "
                    "COALESCE(f.id, 0) AS id_familia, COALESCE(f.descripcion, 'Sin Familia') AS familia, "
                    "COALESCE(a.pvp, 0.0) AS pvp, COALESCE(a.precio_compra, 0.0) AS coste "
                    "FROM articulos a "
                    "LEFT JOIN familias f ON a.familia = f.id")) {
        while (qArt.next()) {
          QString cod = qArt.value("cod").toString();
          if (!mapaArticulos.contains(cod)) {
            ArticuloInventario art;
            art.codigo = cod;
            art.descripcion = qArt.value("descripcion").toString();
            art.idFamilia = qArt.value("id_familia").toInt();
            art.familia = qArt.value("familia").toString();
            art.pvp = qArt.value("pvp").toDouble();
            art.coste = qArt.value("coste").toDouble();
            art.stockTotal = 0.0;
            mapaArticulos.insert(cod, art);
          }
        }
      }
    }
  }

  articulosCargados = mapaArticulos.values();

  // Aplicar filtros de búsqueda y familia, y refrescar vistas
  aplicarFiltrosYActualizarVistas();
}

/**
 * @brief Filtra la lista en memoria según texto de búsqueda, familia y existencia de stock.
 */
QList<ArticuloInventario> DialogInventario::obtenerArticulosFiltrados() const {
  QList<ArticuloInventario> filtrados;
  QString textoBuscar = ui->txtBuscar->text().trimmed();
  int idFamiliaSel = ui->comboFamilia->currentData().toInt();
  int modoStock = ui->comboFiltroStock->currentData().toInt(); // 1: >0, 0: todo, 2: <=0

  for (const ArticuloInventario &art : articulosCargados) {
    // Filtro por stock
    if (modoStock == 1 && art.stockTotal <= 0) continue;
    if (modoStock == 2 && art.stockTotal > 0) continue;

    // Filtro por familia
    if (idFamiliaSel != -1 && art.idFamilia != idFamiliaSel) continue;

    // Filtro por texto de búsqueda
    if (!textoBuscar.isEmpty()) {
      bool coincide = art.codigo.contains(textoBuscar, Qt::CaseInsensitive) ||
                      art.descripcion.contains(textoBuscar, Qt::CaseInsensitive) ||
                      art.familia.contains(textoBuscar, Qt::CaseInsensitive);

      // Comprobar si coincide con algún número de lote
      if (!coincide) {
        for (const DetalleLoteInventario &l : art.lotes) {
          if (l.lote.contains(textoBuscar, Qt::CaseInsensitive)) {
            coincide = true;
            break;
          }
        }
      }
      if (!coincide) continue;
    }

    filtrados.append(art);
  }

  return filtrados;
}

/**
 * @brief Aplica los filtros vigentes y puebla las tablas, árbol y métricas.
 */
void DialogInventario::aplicarFiltrosYActualizarVistas() {
  QList<ArticuloInventario> filtrados = obtenerArticulosFiltrados();

  actualizarKPIs(filtrados);
  poblarTablaArticulos(filtrados);
  poblarTablaLotes(filtrados);
  poblarArbolJerarquico(filtrados);
  poblarTablaFamilias(filtrados);
}

/**
 * @brief Actualiza las tarjetas de resumen KPI con las cifras de existencias y valoración.
 */
void DialogInventario::actualizarKPIs(const QList<ArticuloInventario> &articulosFiltrados) {
  int totalRefs = articulosFiltrados.size();
  double totalUds = 0.0;
  double totalCoste = 0.0;
  double totalPvp = 0.0;

  for (const ArticuloInventario &art : articulosFiltrados) {
    if (art.stockTotal > 0) {
      totalUds += art.stockTotal;
      totalCoste += (art.stockTotal * art.coste);
      totalPvp += (art.stockTotal * art.pvp);
    }
  }

  double margenEuros = totalPvp - totalCoste;
  double margenPorc = (totalCoste > 0.0) ? ((margenEuros / totalCoste) * 100.0) : 0.0;

  ui->lblTotalReferencias->setText(QString::number(totalRefs));
  ui->lblTotalUnidades->setText(QString::number(totalUds, 'f', 0) + " uds");
  ui->lblValorCoste->setText(QString::number(totalCoste, 'f', 2) + " €");
  ui->lblValorPVP->setText(QString::number(totalPvp, 'f', 2) + " €");
  ui->lblMargenPotencial->setText(QString("%1 € (%2%)")
                                      .arg(QString::number(margenEuros, 'f', 2))
                                      .arg(QString::number(margenPorc, 'f', 1)));
}

/**
 * @brief Pueblan la tabla de vista resumida de artículos.
 */
void DialogInventario::poblarTablaArticulos(const QList<ArticuloInventario> &articulosFiltrados) {
  if (modeloArticulos) delete modeloArticulos;
  modeloArticulos = new QStandardItemModel(this);

  QStringList headers = {"Código", "Descripción", "Familia", "Stock Total", "Nº Lotes",
                         "Próx. Caducidad", "Coste Unit.", "PVP", "Valor Coste", "Valor PVP",
                         "Margen (€)", "Margen (%)"};
  modeloArticulos->setHorizontalHeaderLabels(headers);

  for (const ArticuloInventario &art : articulosFiltrados) {
    QList<QStandardItem *> fila;

    QStandardItem *iCod = new QStandardItem(art.codigo);
    iCod->setTextAlignment(Qt::AlignCenter);
    fila << iCod;

    QStandardItem *iDesc = new QStandardItem(art.descripcion);
    fila << iDesc;

    QStandardItem *iFam = new QStandardItem(art.familia);
    fila << iFam;

    NumericStandardItem *iStock = new NumericStandardItem(art.stockTotal, 0, "uds");
    if (art.stockTotal <= 0) iStock->setForeground(QBrush(QColor(180, 40, 40)));
    fila << iStock;

    NumericStandardItem *iNumLotes = new NumericStandardItem(art.lotes.size(), 0);
    iNumLotes->setTextAlignment(Qt::AlignCenter);
    fila << iNumLotes;

    QString cadStr = art.proximaCaducidad.isValid() ? art.proximaCaducidad.toString("yyyy-MM-dd") : "-";
    QStandardItem *iCad = new QStandardItem(cadStr);
    iCad->setTextAlignment(Qt::AlignCenter);
    if (art.proximaCaducidad.isValid() && art.proximaCaducidad <= QDate::currentDate().addDays(30)) {
      iCad->setForeground(QBrush(QColor(200, 50, 0)));
    }
    fila << iCad;

    NumericStandardItem *iCoste = new NumericStandardItem(art.coste, 2, "€");
    fila << iCoste;

    NumericStandardItem *iPvp = new NumericStandardItem(art.pvp, 2, "€");
    fila << iPvp;

    double vCoste = art.stockTotal * art.coste;
    NumericStandardItem *iValCoste = new NumericStandardItem(vCoste, 2, "€");
    fila << iValCoste;

    double vPvp = art.stockTotal * art.pvp;
    NumericStandardItem *iValPvp = new NumericStandardItem(vPvp, 2, "€");
    fila << iValPvp;

    double mEur = vPvp - vCoste;
    NumericStandardItem *iMargenEur = new NumericStandardItem(mEur, 2, "€");
    fila << iMargenEur;

    double mPorc = (art.coste > 0.0) ? (((art.pvp - art.coste) / art.coste) * 100.0) : 0.0;
    NumericStandardItem *iMargenPorc = new NumericStandardItem(mPorc, 1, "%");
    fila << iMargenPorc;

    modeloArticulos->appendRow(fila);
  }

  ui->tablaArticulos->setModel(modeloArticulos);
  ui->tablaArticulos->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->tablaArticulos->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  ui->tablaArticulos->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  ui->tablaArticulos->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaArticulos->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaArticulos->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaArticulos->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
  ui->tablaArticulos->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
  ui->tablaArticulos->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
  ui->tablaArticulos->horizontalHeader()->setSectionResizeMode(9, QHeaderView::ResizeToContents);
  ui->tablaArticulos->horizontalHeader()->setSectionResizeMode(10, QHeaderView::ResizeToContents);
  ui->tablaArticulos->horizontalHeader()->setSectionResizeMode(11, QHeaderView::ResizeToContents);
}

/**
 * @brief Pueblan la tabla con el desglose pormenorizado lote a lote.
 */
void DialogInventario::poblarTablaLotes(const QList<ArticuloInventario> &articulosFiltrados) {
  if (modeloLotes) delete modeloLotes;
  modeloLotes = new QStandardItemModel(this);

  QStringList headers = {"Código", "Descripción", "Familia", "Nº Lote", "F. Caducidad",
                         "Stock Lote", "Coste Unit.", "PVP", "Valor Coste", "Valor PVP", "Tienda"};
  modeloLotes->setHorizontalHeaderLabels(headers);

  for (const ArticuloInventario &art : articulosFiltrados) {
    if (art.lotes.isEmpty()) {
      // Si el artículo no tiene lotes registrados pero se muestra en el filtro
      QList<QStandardItem *> fila;

      QStandardItem *iCod = new QStandardItem(art.codigo);
      iCod->setTextAlignment(Qt::AlignCenter);
      fila << iCod;

      QStandardItem *iDesc = new QStandardItem(art.descripcion);
      fila << iDesc;

      QStandardItem *iFam = new QStandardItem(art.familia);
      fila << iFam;

      QStandardItem *iLote = new QStandardItem("(Sin lotes)");
      iLote->setTextAlignment(Qt::AlignCenter);
      iLote->setForeground(QBrush(QColor(130, 130, 130)));
      fila << iLote;

      QStandardItem *iCad = new QStandardItem("-");
      iCad->setTextAlignment(Qt::AlignCenter);
      fila << iCad;

      NumericStandardItem *iStock = new NumericStandardItem(art.stockTotal, 0, "uds");
      fila << iStock;

      NumericStandardItem *iCoste = new NumericStandardItem(art.coste, 2, "€");
      fila << iCoste;

      NumericStandardItem *iPvp = new NumericStandardItem(art.pvp, 2, "€");
      fila << iPvp;

      NumericStandardItem *iValCoste = new NumericStandardItem(art.stockTotal * art.coste, 2, "€");
      fila << iValCoste;

      NumericStandardItem *iValPvp = new NumericStandardItem(art.stockTotal * art.pvp, 2, "€");
      fila << iValPvp;

      QStandardItem *iTienda = new QStandardItem(origenActual.nombreTienda);
      fila << iTienda;

      modeloLotes->appendRow(fila);
    } else {
      for (const DetalleLoteInventario &lote : art.lotes) {
        QList<QStandardItem *> fila;

        QStandardItem *iCod = new QStandardItem(art.codigo);
        iCod->setTextAlignment(Qt::AlignCenter);
        fila << iCod;

        QStandardItem *iDesc = new QStandardItem(art.descripcion);
        fila << iDesc;

        QStandardItem *iFam = new QStandardItem(art.familia);
        fila << iFam;

        QString textoLote = lote.lote.isEmpty() ? "General" : lote.lote;
        QStandardItem *iLote = new QStandardItem(textoLote);
        iLote->setTextAlignment(Qt::AlignCenter);
        fila << iLote;

        QString cadStr = lote.fechaCaducidad.isValid() ? lote.fechaCaducidad.toString("yyyy-MM-dd") : "-";
        QStandardItem *iCad = new QStandardItem(cadStr);
        iCad->setTextAlignment(Qt::AlignCenter);
        if (lote.fechaCaducidad.isValid() && lote.fechaCaducidad <= QDate::currentDate().addDays(30)) {
          iCad->setForeground(QBrush(QColor(200, 50, 0)));
        }
        fila << iCad;

        NumericStandardItem *iStock = new NumericStandardItem(lote.cantidad, 0, "uds");
        fila << iStock;

        NumericStandardItem *iCoste = new NumericStandardItem(art.coste, 2, "€");
        fila << iCoste;

        NumericStandardItem *iPvp = new NumericStandardItem(art.pvp, 2, "€");
        fila << iPvp;

        NumericStandardItem *iValCoste = new NumericStandardItem(lote.cantidad * art.coste, 2, "€");
        fila << iValCoste;

        NumericStandardItem *iValPvp = new NumericStandardItem(lote.cantidad * art.pvp, 2, "€");
        fila << iValPvp;

        QString tiendaNombre = lote.nombreTienda.isEmpty() ? origenActual.nombreTienda : lote.nombreTienda;
        QStandardItem *iTienda = new QStandardItem(tiendaNombre);
        fila << iTienda;

        modeloLotes->appendRow(fila);
      }
    }
  }

  ui->tablaLotes->setModel(modeloLotes);
  ui->tablaLotes->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->tablaLotes->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  ui->tablaLotes->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  ui->tablaLotes->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaLotes->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaLotes->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaLotes->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
  ui->tablaLotes->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
  ui->tablaLotes->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
  ui->tablaLotes->horizontalHeader()->setSectionResizeMode(9, QHeaderView::ResizeToContents);
  ui->tablaLotes->horizontalHeader()->setSectionResizeMode(10, QHeaderView::ResizeToContents);
}

/**
 * @brief Construye el árbol jerárquico interactivo (Familia ➔ Artículo ➔ Lote).
 */
void DialogInventario::poblarArbolJerarquico(const QList<ArticuloInventario> &articulosFiltrados) {
  ui->treeInventario->clear();

  // Agrupar los artículos filtrados por familia
  QMap<QString, QList<ArticuloInventario>> mapaPorFamilia;
  for (const ArticuloInventario &art : articulosFiltrados) {
    QString famNombre = art.familia.trimmed().isEmpty() ? "Sin Familia" : art.familia;
    mapaPorFamilia[famNombre].append(art);
  }

  QStringList listaFamilias = mapaPorFamilia.keys();
  listaFamilias.sort();

  for (const QString &famNombre : listaFamilias) {
    const QList<ArticuloInventario> &listaArts = mapaPorFamilia[famNombre];

    double famUds = 0.0;
    double famCoste = 0.0;
    double famPvp = 0.0;

    for (const ArticuloInventario &a : listaArts) {
      famUds += a.stockTotal;
      famCoste += (a.stockTotal * a.coste);
      famPvp += (a.stockTotal * a.pvp);
    }

    // Nodo 1: Familia
    QTreeWidgetItem *nodoFam = new QTreeWidgetItem(ui->treeInventario);
    nodoFam->setText(0, "📂 " + famNombre + QString(" (%1 artículos)").arg(listaArts.size()));
    nodoFam->setText(1, famNombre);
    nodoFam->setText(3, QString::number(famUds, 'f', 0) + " uds");
    nodoFam->setText(6, QString::number(famCoste, 'f', 2) + " €");
    nodoFam->setText(7, QString::number(famPvp, 'f', 2) + " €");

    QFont fontFam = nodoFam->font(0);
    fontFam.setBold(true);
    nodoFam->setFont(0, fontFam);
    nodoFam->setFont(3, fontFam);
    nodoFam->setFont(6, fontFam);
    nodoFam->setFont(7, fontFam);

    // Fondo distintivo para la cabecera de familia
    QBrush brushFam(QColor(235, 242, 250));
    for (int col = 0; col < 8; ++col) {
      nodoFam->setBackground(col, brushFam);
    }

    // Nodos 2: Artículos dentro de la familia
    for (const ArticuloInventario &art : listaArts) {
      QTreeWidgetItem *nodoArt = new QTreeWidgetItem(nodoFam);
      nodoArt->setText(0, QString("📦 [%1] %2").arg(art.codigo, art.descripcion));
      nodoArt->setText(1, famNombre);
      if (art.proximaCaducidad.isValid()) {
        nodoArt->setText(2, art.proximaCaducidad.toString("yyyy-MM-dd"));
      } else {
        nodoArt->setText(2, "-");
      }
      nodoArt->setText(3, QString::number(art.stockTotal, 'f', 0) + " uds");
      nodoArt->setText(4, QString::number(art.coste, 'f', 2) + " €");
      nodoArt->setText(5, QString::number(art.pvp, 'f', 2) + " €");
      nodoArt->setText(6, QString::number(art.stockTotal * art.coste, 'f', 2) + " €");
      nodoArt->setText(7, QString::number(art.stockTotal * art.pvp, 'f', 2) + " €");
      nodoArt->setData(0, Qt::UserRole, art.codigo); // Para abrir ficha con doble clic

      // Nodos 3: Desglose de Lotes del artículo
      for (const DetalleLoteInventario &lote : art.lotes) {
        QTreeWidgetItem *nodoLote = new QTreeWidgetItem(nodoArt);
        QString textoL = lote.lote.isEmpty() ? "Lote General" : ("🏷️ " + lote.lote);
        if (!lote.nombreTienda.isEmpty()) {
          textoL += QString(" (%1)").arg(lote.nombreTienda);
        }
        nodoLote->setText(0, textoL);
        nodoLote->setText(1, famNombre);
        if (lote.fechaCaducidad.isValid()) {
          nodoLote->setText(2, lote.fechaCaducidad.toString("yyyy-MM-dd"));
        } else {
          nodoLote->setText(2, "-");
        }
        nodoLote->setText(3, QString::number(lote.cantidad, 'f', 0) + " uds");
        nodoLote->setText(4, QString::number(art.coste, 'f', 2) + " €");
        nodoLote->setText(5, QString::number(art.pvp, 'f', 2) + " €");
        nodoLote->setText(6, QString::number(lote.cantidad * art.coste, 'f', 2) + " €");
        nodoLote->setText(7, QString::number(lote.cantidad * art.pvp, 'f', 2) + " €");
        nodoLote->setData(0, Qt::UserRole, art.codigo);

        QFont fontLote = nodoLote->font(0);
        fontLote.setItalic(true);
        nodoLote->setFont(0, fontLote);
        nodoLote->setForeground(0, QBrush(QColor(80, 80, 80)));
      }
    }
  }

  // Ajustar anchos del árbol
  ui->treeInventario->header()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->treeInventario->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  ui->treeInventario->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  ui->treeInventario->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->treeInventario->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->treeInventario->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->treeInventario->header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
  ui->treeInventario->header()->setSectionResizeMode(7, QHeaderView::ResizeToContents);

  // Por defecto, desplegar las familias
  ui->treeInventario->expandToDepth(0);
}

/**
 * @brief Pueblan la tabla con el resumen agregado por familias.
 */
void DialogInventario::poblarTablaFamilias(const QList<ArticuloInventario> &articulosFiltrados) {
  if (modeloFamilias) delete modeloFamilias;
  modeloFamilias = new QStandardItemModel(this);

  QStringList headers = {"Familia / Categoría", "Nº Artículos", "Stock Total", "Valor Coste",
                         "Valor PVP", "Margen (€)", "Margen (%)", "% Peso Stock"};
  modeloFamilias->setHorizontalHeaderLabels(headers);

  // Calcular totales para los porcentajes
  double totalUdsGlobal = 0.0;
  QMap<QString, ResumenFamiliaInventario> mapaFamilias;

  for (const ArticuloInventario &art : articulosFiltrados) {
    QString fam = art.familia.trimmed().isEmpty() ? "Sin Familia" : art.familia;
    ResumenFamiliaInventario &rf = mapaFamilias[fam];
    rf.nombreFamilia = fam;
    rf.idFamilia = art.idFamilia;
    rf.articulosCount++;
    rf.unidadesTotal += art.stockTotal;
    rf.valorCosteTotal += (art.stockTotal * art.coste);
    rf.valorPvpTotal += (art.stockTotal * art.pvp);

    if (art.stockTotal > 0) {
      totalUdsGlobal += art.stockTotal;
    }
  }

  for (const ResumenFamiliaInventario &rf : mapaFamilias) {
    QList<QStandardItem *> fila;

    QStandardItem *iFam = new QStandardItem(rf.nombreFamilia);
    fila << iFam;

    NumericStandardItem *iRefs = new NumericStandardItem(rf.articulosCount, 0);
    iRefs->setTextAlignment(Qt::AlignCenter);
    fila << iRefs;

    NumericStandardItem *iUds = new NumericStandardItem(rf.unidadesTotal, 0, "uds");
    fila << iUds;

    NumericStandardItem *iCoste = new NumericStandardItem(rf.valorCosteTotal, 2, "€");
    fila << iCoste;

    NumericStandardItem *iPvp = new NumericStandardItem(rf.valorPvpTotal, 2, "€");
    fila << iPvp;

    double mEur = rf.valorPvpTotal - rf.valorCosteTotal;
    NumericStandardItem *iMargenEur = new NumericStandardItem(mEur, 2, "€");
    fila << iMargenEur;

    double mPorc = (rf.valorCosteTotal > 0.0) ? ((mEur / rf.valorCosteTotal) * 100.0) : 0.0;
    NumericStandardItem *iMargenPorc = new NumericStandardItem(mPorc, 1, "%");
    fila << iMargenPorc;

    double peso = (totalUdsGlobal > 0.0) ? ((rf.unidadesTotal / totalUdsGlobal) * 100.0) : 0.0;
    NumericStandardItem *iPeso = new NumericStandardItem(peso, 1, "%");
    fila << iPeso;

    modeloFamilias->appendRow(fila);
  }

  ui->tablaFamilias->setModel(modeloFamilias);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
}

// ======================== SLOTS DE EVENTOS ========================

void DialogInventario::on_btnActualizar_clicked() {
  cargarDatosDesdeBD();
}

void DialogInventario::on_comboTienda_currentIndexChanged(int /*index*/) {
  cargarDatosDesdeBD();
}

void DialogInventario::on_comboFamilia_currentIndexChanged(int /*index*/) {
  aplicarFiltrosYActualizarVistas();
}

void DialogInventario::on_comboFiltroStock_currentIndexChanged(int /*index*/) {
  cargarDatosDesdeBD();
}

void DialogInventario::on_txtBuscar_textChanged(const QString & /*texto*/) {
  aplicarFiltrosYActualizarVistas();
}

void DialogInventario::on_btnExpandirTodo_clicked() {
  ui->treeInventario->expandAll();
}

void DialogInventario::on_btnContraerTodo_clicked() {
  ui->treeInventario->collapseAll();
}

void DialogInventario::on_btnExpandirFamilias_clicked() {
  ui->treeInventario->collapseAll();
  ui->treeInventario->expandToDepth(0);
}

/**
 * @brief Abre la ventana de gestión de lotes/stock para un producto determinado.
 */
void DialogInventario::abrirFichaStock(const QString &codigoArticulo) {
  if (codigoArticulo.trimmed().isEmpty()) return;
  Stock dlg(codigoArticulo, &base, this);
  dlg.exec();
  // Recargar datos para reflejar posibles modificaciones de lotes realizadas en la ficha
  cargarDatosDesdeBD();
}

void DialogInventario::on_tablaArticulos_doubleClicked(const QModelIndex &index) {
  if (!index.isValid() || !modeloArticulos) return;
  QString cod = modeloArticulos->item(index.row(), 0)->text();
  abrirFichaStock(cod);
}

void DialogInventario::on_tablaLotes_doubleClicked(const QModelIndex &index) {
  if (!index.isValid() || !modeloLotes) return;
  QString cod = modeloLotes->item(index.row(), 0)->text();
  abrirFichaStock(cod);
}

void DialogInventario::on_treeInventario_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/) {
  if (!item) return;
  QString cod = item->data(0, Qt::UserRole).toString();
  if (!cod.isEmpty()) {
    abrirFichaStock(cod);
  }
}

/**
 * @brief Exporta los datos de la pestaña activa a un archivo CSV.
 */
void DialogInventario::on_btnExportarCSV_clicked() {
  int tabIdx = ui->tabWidgetInventario->currentIndex();
  QStandardItemModel *modeloActual = nullptr;
  QString nombrePorDefecto;

  switch (tabIdx) {
  case 0:
    modeloActual = modeloArticulos;
    nombrePorDefecto = "Inventario_Articulos_" + QDate::currentDate().toString("yyyy-MM-dd") + ".csv";
    break;
  case 1:
    modeloActual = modeloLotes;
    nombrePorDefecto = "Inventario_DesgloseLotes_" + QDate::currentDate().toString("yyyy-MM-dd") + ".csv";
    break;
  case 3:
    modeloActual = modeloFamilias;
    nombrePorDefecto = "Inventario_Familias_" + QDate::currentDate().toString("yyyy-MM-dd") + ".csv";
    break;
  default:
    modeloActual = modeloArticulos;
    nombrePorDefecto = "Inventario_" + QDate::currentDate().toString("yyyy-MM-dd") + ".csv";
    break;
  }

  if (!modeloActual || modeloActual->rowCount() == 0) {
    QMessageBox::information(this, "Exportar CSV", "No hay datos para exportar en la vista seleccionada.");
    return;
  }

  QString ruta = QFileDialog::getSaveFileName(this, "Guardar Inventario CSV",
                                              QDir::homePath() + "/" + nombrePorDefecto,
                                              "Archivos CSV (*.csv)");
  if (ruta.isEmpty()) return;

  QFile archivo(ruta);
  if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox::critical(this, "Error", "No se pudo abrir el archivo para escritura:\n" + archivo.errorString());
    return;
  }

  QTextStream out(&archivo);
  // Cabeceras
  for (int c = 0; c < modeloActual->columnCount(); ++c) {
    out << "\"" << modeloActual->headerData(c, Qt::Horizontal).toString() << "\"";
    if (c < modeloActual->columnCount() - 1) out << ";";
  }
  out << "\n";

  // Filas
  for (int r = 0; r < modeloActual->rowCount(); ++r) {
    for (int c = 0; c < modeloActual->columnCount(); ++c) {
      QStandardItem *it = modeloActual->item(r, c);
      out << "\"" << (it ? it->text() : "") << "\"";
      if (c < modeloActual->columnCount() - 1) out << ";";
    }
    out << "\n";
  }

  archivo.close();
  QMessageBox::information(this, "Exportación Completada",
                           "El inventario se ha exportado correctamente a:\n" + ruta);
}

/**
 * @brief Copia el contenido de la tabla activa al portapapeles.
 */
void DialogInventario::on_btnCopiar_clicked() {
  int tabIdx = ui->tabWidgetInventario->currentIndex();
  QStandardItemModel *modeloActual = nullptr;

  switch (tabIdx) {
  case 0: modeloActual = modeloArticulos; break;
  case 1: modeloActual = modeloLotes; break;
  case 3: modeloActual = modeloFamilias; break;
  default: modeloActual = modeloArticulos; break;
  }

  if (!modeloActual || modeloActual->rowCount() == 0) {
    QMessageBox::information(this, "Copiar al Portapapeles", "No hay datos disponibles para copiar.");
    return;
  }

  QString buffer;
  // Cabeceras
  for (int c = 0; c < modeloActual->columnCount(); ++c) {
    buffer += modeloActual->headerData(c, Qt::Horizontal).toString();
    if (c < modeloActual->columnCount() - 1) buffer += "\t";
  }
  buffer += "\n";

  // Filas
  for (int r = 0; r < modeloActual->rowCount(); ++r) {
    for (int c = 0; c < modeloActual->columnCount(); ++c) {
      QStandardItem *it = modeloActual->item(r, c);
      buffer += (it ? it->text() : "");
      if (c < modeloActual->columnCount() - 1) buffer += "\t";
    }
    buffer += "\n";
  }

  QApplication::clipboard()->setText(buffer);
  QMessageBox::information(this, "Copiado al Portapapeles",
                           "Los datos de inventario se han copiado al portapapeles.\nPuede pegarlos directamente en Excel o LibreOffice Calc.");
}

/**
 * @brief Genera e imprime un informe PDF estructurado del inventario.
 */
void DialogInventario::on_btnImprimir_clicked() {
  int tabIdx = ui->tabWidgetInventario->currentIndex();
  QStandardItemModel *modeloActual = (tabIdx == 1) ? modeloLotes :
                                     ((tabIdx == 3) ? modeloFamilias : modeloArticulos);

  if (!modeloActual || modeloActual->rowCount() == 0) {
    QMessageBox::information(this, "Imprimir Inventario", "No hay datos para imprimir.");
    return;
  }

  QString fechaActual = QDate::currentDate().toString("yyyy-MM-dd");
  QString nombreFichero = QString("Inventario_%1_%2.pdf").arg(origenActual.nombreTienda.replace(" ", "_"), fechaActual);
  QString rutaDestino = QFileDialog::getSaveFileName(this, "Guardar Informe de Inventario en PDF",
                                                     QDir::homePath() + "/" + nombreFichero,
                                                     "Documentos PDF (*.pdf)");
  if (rutaDestino.isEmpty()) return;

  // Construir documento HTML estético y profesional
  QString html = "<html><head><meta charset='UTF-8'><style>"
                 "body { font-family: 'Helvetica', Arial, sans-serif; font-size: 10pt; color: #222; margin: 20px; }"
                 "h1 { color: #1565C0; margin-bottom: 4px; font-size: 18pt; }"
                 ".sub { color: #666; font-size: 10pt; margin-bottom: 20px; }"
                 ".kpi-box { background: #F5F7FA; border: 1px solid #DCE2E6; padding: 10px; border-radius: 6px; margin-bottom: 20px; }"
                 ".kpi-table { width: 100%; border-collapse: collapse; }"
                 ".kpi-table td { padding: 4px 8px; }"
                 "table.data { width: 100%; border-collapse: collapse; margin-top: 10px; font-size: 9pt; }"
                 "table.data th { background: #263238; color: #FFFFFF; padding: 6px 4px; text-align: left; }"
                 "table.data td { border-bottom: 1px solid #E0E0E0; padding: 5px 4px; }"
                 "table.data tr:nth-child(even) { background: #FAFAFA; }"
                 ".num { text-align: right; }"
                 ".center { text-align: center; }"
                 "</style></head><body>";

  html += QString("<h1>Informe de Inventario y Valoración de Stock</h1>");
  html += QString("<div class='sub'><b>Ámbito:</b> %1 | <b>Fecha:</b> %2 | <b>Filtro:</b> %3</div>")
              .arg(origenActual.nombreTienda, fechaActual, ui->comboFiltroStock->currentText());

  // Resumen KPIs
  html += "<div class='kpi-box'><table class='kpi-table'><tr>"
          "<td><b>Referencias:</b> " + ui->lblTotalReferencias->text() + "</td>"
          "<td><b>Total Unidades:</b> " + ui->lblTotalUnidades->text() + "</td>"
          "<td><b>Valor Coste:</b> " + ui->lblValorCoste->text() + "</td>"
          "<td><b>Valor PVP:</b> " + ui->lblValorPVP->text() + "</td>"
          "<td><b>Margen Potencial:</b> " + ui->lblMargenPotencial->text() + "</td>"
          "</tr></table></div>";

  // Tabla de datos
  html += "<table class='data'><thead><tr>";
  for (int c = 0; c < modeloActual->columnCount(); ++c) {
    html += "<th>" + modeloActual->headerData(c, Qt::Horizontal).toString() + "</th>";
  }
  html += "</tr></thead><tbody>";

  for (int r = 0; r < modeloActual->rowCount(); ++r) {
    html += "<tr>";
    for (int c = 0; c < modeloActual->columnCount(); ++c) {
      QStandardItem *it = modeloActual->item(r, c);
      QString val = it ? it->text() : "";
      if (val.contains("€") || val.contains("%") || val.contains("uds")) {
        html += "<td class='num'>" + val + "</td>";
      } else if (c == 0 || val == "-" || val.contains(QRegularExpression("^\\d{4}-\\d{2}-\\d{2}$"))) {
        html += "<td class='center'>" + val + "</td>";
      } else {
        html += "<td>" + val + "</td>";
      }
    }
    html += "</tr>";
  }
  html += "</tbody></table></body></html>";

  QTextDocument doc;
  doc.setHtml(html);

  QPrinter printer(QPrinter::HighResolution);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setOutputFileName(rutaDestino);
  doc.print(&printer);

  QMessageBox::information(this, "PDF Generado", "El informe se ha generado con éxito en:\n" + rutaDestino);
}
