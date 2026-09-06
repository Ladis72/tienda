#include "entradamercancia.h"
#include "ui_entradamercancia.h"
#include "conexion.h"
#include "dialogcomparartraspaso.h"

#include <QMessageBox>
#include <QShortcut>

EntradaMercancia::EntradaMercancia(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EntradaMercancia)
{
    ui->setupUi(this);
    lineas = 0;
    productos = 0;
    m_idTiendaRemotaEnLocal = 0;
    m_idTiendaLocalEnRemota = 0;

    // El panel de la tienda remota se oculta por defecto
    ui->widgetRemoto->setVisible(false);

    // Atajo de teclado Control+S para alternar la visualización del panel remoto
    QShortcut *shortcutRemoto = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this);
    connect(shortcutRemoto, &QShortcut::activated, this, [this]() {
        bool visible = !ui->widgetRemoto->isVisible();
        ui->widgetRemoto->setVisible(visible);
        if (visible) {
            actualizarTablaRemota();
        }
    });

    mTablaEntradas = new QSqlTableModel(this, QSqlDatabase::database(conf->getConexionLocal()));
    mTablaEntradas->setTable("entradaGenero_tmp");
    ui->tableView->setModel(mTablaEntradas);
    ui->tableView->setSortingEnabled(true);
    mTablaEntradas->setSort(3, Qt::AscendingOrder); // Orden por descripción por defecto

    // Modelo para la tabla remota
    mTablaRemota = new QSqlQueryModel(this);
    ui->tableViewRemota->setModel(mTablaRemota);
    ui->tableViewRemota->setSortingEnabled(true);

    connect(mTablaEntradas, &QSqlTableModel::dataChanged, this, &EntradaMercancia::actualizarTotales);

    llenarComboTiendas();
    actualizarTabla();

    // Desactivar autoDefault para evitar inserciones accidentales al pulsar ENTER
    ui->pushButtonAgregarLinea->setAutoDefault(false);
    ui->pushButtonAgregarLinea->setDefault(false);
    ui->pushButtonAceptar->setAutoDefault(false);
    ui->pushButtonAceptar->setDefault(false);
    ui->pushButtonBorrar->setAutoDefault(false);
    ui->pushButtonBorrar->setDefault(false);
    ui->pushButtonComparar->setAutoDefault(false);
    ui->pushButtonComparar->setDefault(false);
    ui->pushButtonAceptarAmbas->setAutoDefault(false);
    ui->pushButtonAceptarAmbas->setDefault(false);
    ui->pushButtonSalir->setAutoDefault(false);
    ui->pushButtonSalir->setDefault(false);

    // Cadena de enfoque mediante ENTER
    connect(ui->lineEditUds, &QLineEdit::returnPressed, [this](){
        ui->dateEditCaducidad->setFocus();
    });
    connect(ui->lineEditPVP, &QLineEdit::returnPressed, this, &EntradaMercancia::on_pushButtonAgregarLinea_clicked);

    aplicarPermisos();
}

EntradaMercancia::~EntradaMercancia()
{
    delete ui;
}

/**
 * @brief Aplica permisos a las acciones del módulo de entrada de mercancía.
 */
void EntradaMercancia::aplicarPermisos() {
    if (!conf || !conf->permisos())
        return;

    ui->pushButtonAgregarLinea->setEnabled(conf->permisos()->tiene("entradas.crear"));
    ui->pushButtonAceptar->setEnabled(conf->permisos()->tiene("entradas.crear"));
    ui->pushButtonBorrar->setEnabled(conf->permisos()->tiene("entradas.borrar"));

    // El botón para aceptar en ambas tiendas es exclusivo de administradores (Rol 0)
    bool esAdmin = (conf->getRol() == 0);
    ui->pushButtonAceptarAmbas->setEnabled(esAdmin);
    if (!esAdmin) {
        ui->pushButtonAceptarAmbas->setToolTip(tr("Solo disponible para administradores (Rol 0)"));
    }
}

void EntradaMercancia::on_pushButtonAceptar_clicked()
{
    int idTienda = base->idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()),
                                             ui->comboBoxProcedencia->currentText());

    for (int i = 0; i < mTablaEntradas->rowCount(); ++i) {
        procesarLineaEntrada(mTablaEntradas->record(i));
    }
    base->insertarLog(conf->getConexionLocal(), "Info", conf->getUsuario(), "Entrada genero ");

    guardarArticulo(idTienda);
    limpiarTabla(idTienda);
    actualizarTabla();
}

void EntradaMercancia::actualizarTabla()
{
    m_nombreTiendaRemota = ui->comboBoxProcedencia->currentText().trimmed();
    m_idTiendaRemotaEnLocal = base->idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()),
                                                        m_nombreTiendaRemota);

    mTablaEntradas->setFilter("idTienda = " + QString::number(m_idTiendaRemotaEnLocal));
    mTablaEntradas->select();

    // Forzar la carga de todos los registros para asegurar que rowCount() y la vista sean correctos
    while (mTablaEntradas->canFetchMore()) {
        mTablaEntradas->fetchMore();
    }

    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
    actualizarTotales();
}

void EntradaMercancia::llenarComboTiendas()
{
    ui->comboBoxProcedencia->blockSignals(true);
    ui->comboBoxProcedencia->clear();
    QSqlQuery listaCombo = base->tiendas(QSqlDatabase::database(conf->getConexionLocal()));
    while (listaCombo.next()) {
        ui->comboBoxProcedencia->addItem(listaCombo.value("nombre").toString());
    }
    ui->comboBoxProcedencia->blockSignals(false);
}

void EntradaMercancia::on_lineEditCod_returnPressed()
{
    QSqlRecord registro = base->consulta_producto(conf->getConexionLocal(), ui->lineEditCod->text());
    
    if (registro.isEmpty()) {
        QString cod = base->codigoDesdeAux(conf->getConexionLocal(), ui->lineEditCod->text());
        registro = base->consulta_producto(conf->getConexionLocal(), cod);
    }
    
    if (!registro.isEmpty()) {
        ui->lineEditCod->setText(registro.value("cod").toString());
        ui->lineEditDesc->setText(registro.value("descripcion").toString());
        ui->lineEditPVP->setText(registro.value("pvp").toString());
        ui->lineEditUds->setFocus();
    } else {
        QMessageBox msg(this);
        msg.setText("No se encuentra el producto");
        msg.setInformativeText("Desea crearlo?");
        msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Ok);
        int resp = msg.exec();
        if (resp == QMessageBox::Ok) {
            Articulos articulo;
            articulo.exec();
            articulo.borrarFormulario();

            qDebug() << "Crear producto";
        } else {
            ui->lineEditCod->setFocus();
            ui->lineEditCod->selectAll();
        }
    }
}

void EntradaMercancia::on_pushButtonAgregarLinea_clicked()
{
    if (ui->lineEditUds->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "La cantidad no puede estar vacía.");
        ui->lineEditUds->setFocus();
        return;
    }
    
    if (ui->lineEditPVP->text().trimmed().isEmpty()) {
        ui->lineEditPVP->setText("0");
    }
    
    QStringList datos;
    datos.clear();
    datos.append(ui->lineEditCod->text());
    datos.append(QDate::currentDate().toString("yyyy-MM-dd"));
    datos.append(ui->lineEditDesc->text());
    datos.append(ui->lineEditUds->text());
    datos.append(ui->dateEditCaducidad->text());
    datos.append(ui->lineEditPVP->text());
    datos.append(QString::number(base->idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()),
                                                           ui->comboBoxProcedencia->currentText())));
    base->insertarEnTabla(QSqlDatabase::database(conf->getConexionLocal()), "entradaGenero_tmp", datos);

    //mTablaEntradas->select();
    ui->lineEditCod->clear();
    ui->lineEditDesc->clear();
    ui->lineEditPVP->clear();
    ui->lineEditUds->clear();
    ui->lineEditCod->setFocus();
    actualizarTabla();
}

void EntradaMercancia::on_lineEditDesc_returnPressed()
{
    QSqlQuery consulta = base->buscarProducto(QSqlDatabase::database(conf->getConexionLocal()),
                                              "articulos",
                                              ui->lineEditDesc->text());
    consulta.first();
    BuscarProducto *buscar = new BuscarProducto(this, std::move(consulta));
    buscar->exec();
    ui->lineEditCod->setText(buscar->resultado);
    emit on_lineEditCod_returnPressed();
}

void EntradaMercancia::on_pushButtonBorrar_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        return;
    }

    // Obtenemos el ID de la columna 0 de la fila seleccionada actualmente
    QString id = mTablaEntradas->data(mTablaEntradas->index(index.row(), 0)).toString();

    if (!id.isEmpty()) {
        // Se utiliza una consulta preparada para evitar inyecciones SQL al borrar una línea temporal de entrada de mercancía
        QSqlQuery tmp(QSqlDatabase::database(conf->getConexionLocal()));
        tmp.prepare("DELETE FROM entradaGenero_tmp WHERE id = ?");
        tmp.bindValue(0, id);
        tmp.exec();
        qDebug() << tmp.lastError();
    }
    actualizarTabla();
}

void EntradaMercancia::on_dateEditCaducidad_editingFinished()
{
    if (ui->dateEditCaducidad->date() <= QDate::currentDate()) {
        QMessageBox::information(this,
                                 "Error en la fecha de caducidad",
                                 "La fecha no puede ser anterior a la fecha de hoy");
        ui->dateEditCaducidad->setFocus();
        ui->dateEditCaducidad->setDate(QDate::currentDate());
    }
}

// void EntradaMercancia::on_comboBoxProcedencia_activated(const QString &arg1)
// {
//     actualizarTabla();
// }

void EntradaMercancia::actualizarTotales()
{
    lineas = mTablaEntradas->rowCount();
    productos = 0;
    for (int i = 0; i < mTablaEntradas->rowCount(); ++i) {
        QModelIndex idx = mTablaEntradas->index(i, 4);
        double valor = mTablaEntradas->data(idx).toDouble();
        productos += valor;
    }
    ui->lbProductos->setText("Lineas= " + QString::number(lineas)
                             + "  Productos=" + QString::number(productos));
    if (ui->lbTotalesLocal) {
        ui->lbTotalesLocal->setText(QString("%1 líneas | %2 uds").arg(lineas).arg(productos));
    }
}

void EntradaMercancia::procesarLineaEntrada(const QSqlRecord &registro)
{
    const QString cod = registro.value("cod").toString();
    const QString fechaCaducidad = registro.value("fechaCaducidad").toString();
    const int uds = registro.value("cantidad").toInt();
    const QString descripcion = registro.value("descripcion").toString();
    const QString precio = registro.value("pvp").toString();

    QString idLote = base->idLote(conf->getConexionLocal(), cod, "", "2000-01-01");

    if (idLote != "0") {
        int pendientes = base->unidadesLote(conf->getConexionLocal(), idLote);

        if (abs(pendientes) > uds) {
            base->aumentarLote(conf->getConexionLocal(), idLote, uds);
        } else {
            // Se parametriza la consulta de borrado de lote para garantizar la seguridad
            QSqlQuery deleteLoteQuery(QSqlDatabase::database(conf->getConexionLocal()));
            deleteLoteQuery.prepare("DELETE FROM lotes WHERE id = ?");
            deleteLoteQuery.bindValue(0, idLote);
            deleteLoteQuery.exec();
            int nuevasUnidades = uds + pendientes;

            idLote = base->idLote(conf->getConexionLocal(), cod, "", fechaCaducidad);
            if (idLote == "0") {
                base->crearLote(conf->getConexionLocal(),
                                cod,
                                "",
                                fechaCaducidad,
                                QString::number(nuevasUnidades));
            } else {
                base->aumentarLote(conf->getConexionLocal(), idLote, nuevasUnidades);
            }
        }
    } else {
        idLote = base->idLote(conf->getConexionLocal(), cod, "", fechaCaducidad);
        if (idLote == "0") {
            base->crearLote(conf->getConexionLocal(), cod, "", fechaCaducidad, QString::number(uds));
        } else {
            base->aumentarLote(conf->getConexionLocal(), idLote, uds);
        }
    }

    actualizarArticulo(cod, descripcion, precio);
}

void EntradaMercancia::actualizarArticulo(const QString &cod,
                                          const QString &descripcion,
                                          const QString &precio)
{
    QSqlRecord registro = base->consulta_producto(conf->getConexionLocal(), cod);
    if (!registro.isEmpty()) {
        QString descAnt = registro.value("descripcion").toString();
        QString pvpAnt = registro.value("pvp").toString();

        if (descripcion != descAnt || precio != pvpAnt) {
            QString precioValidado = precio.isEmpty() ? "0" : precio;
            // Se utiliza una consulta preparada para evitar inyecciones SQL al actualizar la descripción y precio del artículo
            QSqlQuery updateQuery(QSqlDatabase::database(conf->getConexionLocal()));
            updateQuery.prepare("UPDATE articulos SET descripcion = ?, pvp = ? WHERE cod = ?");
            updateQuery.bindValue(0, descripcion);
            updateQuery.bindValue(1, precioValidado.toDouble());
            updateQuery.bindValue(2, cod);
            updateQuery.exec();
        }
    }
}

void EntradaMercancia::guardarArticulo(int idTienda)
{
    // Se parametriza la inserción masiva desde la tabla temporal filtrando de forma segura por idTienda
    QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal()));
    query.prepare("INSERT INTO entradaGenero (cod, fechaEntrada, descripcion, cantidad, "
                  "fechaCaducidad, pvp, idTienda) "
                  "SELECT cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda "
                  "FROM entradaGenero_tmp WHERE idTienda = ?");
    query.bindValue(0, idTienda);
    query.exec();
}

void EntradaMercancia::limpiarTabla(int idTienda)
{
    // Se parametriza el borrado de la tabla temporal filtrando de forma segura por idTienda
    QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal()));
    query.prepare("DELETE FROM entradaGenero_tmp WHERE idTienda = ?");
    query.bindValue(0, idTienda);
    query.exec();
}

void EntradaMercancia::on_comboBoxProcedencia_currentIndexChanged(int)
{
    actualizarTabla();
    // Solo actualizar tabla remota si el panel remoto está visible
    if (ui->widgetRemoto->isVisible()) {
        actualizarTablaRemota();
    }
    qDebug() << "EntradaMercancia: Tienda cambiada a" << ui->comboBoxProcedencia->currentText();
}

/**
 * @brief Comprueba si la conexión con la base de datos de la tienda remota ya está activa.
 * No intenta conectar activamente por red: si ya está abierta se usa, si no se marca desconectada.
 * Se pasa open = false a QSqlDatabase::database para no bloquear con timeouts si está desconectada.
 * @param nombreTienda Nombre de la tienda según la tabla 'tiendas'.
 * @return true si la conexión está disponible y abierta.
 */
bool EntradaMercancia::asegurarConexionRemota(const QString &nombreTienda)
{
    if (nombreTienda.trimmed().isEmpty()) {
        m_connRemota.clear();
        return false;
    }

    m_connRemota = nombreTienda.trimmed();

    // open = false evita que Qt intente conectar por red si no está previamente abierta
    return (QSqlDatabase::contains(m_connRemota) && QSqlDatabase::database(m_connRemota, false).isOpen());
}

/**
 * @brief Obtiene el ID numérico de la tienda local dentro de la base de datos de la tienda remota.
 * @param connRemota Nombre de la conexión abierta a la base de datos remota.
 * @return ID de la tienda local en la remota, o 0 si no se encuentra.
 */
int EntradaMercancia::obtenerIdLocalEnRemota(const QString &connRemota)
{
    QSqlDatabase dbRem = QSqlDatabase::database(connRemota, false);
    if (!dbRem.isOpen()) return 0;

    QString nombreLocal = base->nombreConexionLocal();
    QSqlQuery q(dbRem);
    q.prepare("SELECT id FROM tiendas WHERE nombre = ?");
    q.bindValue(0, nombreLocal);
    if (q.exec() && q.next()) {
        return q.value(0).toInt();
    }
    return 0;
}

/**
 * @brief Carga y visualiza las salidas pendientes preparadas en la tienda remota seleccionada.
 */
void EntradaMercancia::actualizarTablaRemota()
{
    m_nombreTiendaRemota = ui->comboBoxProcedencia->currentText().trimmed();
    if (m_nombreTiendaRemota.isEmpty()) {
        ui->lbEstadoRemota->setText("[Sin tienda]");
        ui->lbTotalesRemota->setText("0 líneas | 0 uds");
        mTablaRemota->clear();
        return;
    }

    ui->labelTituloRemota->setText(QString("🚚 Salidas en %1 (Pendientes)").arg(m_nombreTiendaRemota));

    bool online = asegurarConexionRemota(m_nombreTiendaRemota);
    if (!online) {
        ui->lbEstadoRemota->setText("[Desconectado]");
        ui->lbEstadoRemota->setStyleSheet("color: #c62828; font-weight: bold;");
        ui->lbTotalesRemota->setText("0 líneas | 0 uds");
        mTablaRemota->clear();
        return;
    }

    ui->lbEstadoRemota->setText("[En línea]");
    ui->lbEstadoRemota->setStyleSheet("color: #2e7d32; font-weight: bold;");

    m_idTiendaLocalEnRemota = obtenerIdLocalEnRemota(m_connRemota);

    QSqlDatabase dbRem = QSqlDatabase::database(m_connRemota, false);
    QSqlQuery q(dbRem);
    // Seleccionar salidas pendientes preparadas en la tienda remota con destino a nuestra tienda
    q.prepare("SELECT id, cod, DATE_FORMAT(fechaEntrada, '%Y-%m-%d') AS f_ent, descripcion, "
              "cantidad, DATE_FORMAT(fechaCaducidad, '%Y-%m-%d') AS f_cad, pvp "
              "FROM salidaGenero_tmp WHERE idTienda = ? ORDER BY descripcion ASC");
    q.bindValue(0, m_idTiendaLocalEnRemota);

    if (q.exec()) {
        mTablaRemota->setQuery(std::move(q));
        mTablaRemota->setHeaderData(0, Qt::Horizontal, tr("ID"));
        mTablaRemota->setHeaderData(1, Qt::Horizontal, tr("Código"));
        mTablaRemota->setHeaderData(2, Qt::Horizontal, tr("Fecha"));
        mTablaRemota->setHeaderData(3, Qt::Horizontal, tr("Descripción"));
        mTablaRemota->setHeaderData(4, Qt::Horizontal, tr("Cant."));
        mTablaRemota->setHeaderData(5, Qt::Horizontal, tr("Caducidad"));
        mTablaRemota->setHeaderData(6, Qt::Horizontal, tr("P.V.P."));

        ui->tableViewRemota->hideColumn(0);
        ui->tableViewRemota->resizeColumnsToContents();

        int filasRem = mTablaRemota->rowCount();
        double udsRem = 0;
        for (int i = 0; i < filasRem; ++i) {
            udsRem += mTablaRemota->record(i).value("cantidad").toDouble();
        }
        ui->lbTotalesRemota->setText(QString("%1 líneas | %2 uds").arg(filasRem).arg(udsRem));
    } else {
        qWarning() << "EntradaMercancia: Error al consultar salidaGenero_tmp remota:" << q.lastError().text();
        ui->lbTotalesRemota->setText("Error al consultar");
    }
}

/**
 * @brief Abre el diálogo de comparación para contrastar las tablas local y remota y solventar diferencias.
 */
void EntradaMercancia::on_pushButtonComparar_clicked()
{
    if (m_nombreTiendaRemota.isEmpty()) {
        QMessageBox::warning(this, tr("Aviso"), tr("Debe seleccionar una tienda de procedencia."));
        return;
    }

    if (!asegurarConexionRemota(m_nombreTiendaRemota)) {
        QMessageBox::warning(this, tr("Conexión Fallida"),
                             tr("No se puede conectar con la tienda remota '%1' para comparar las tablas.")
                                 .arg(m_nombreTiendaRemota));
        return;
    }

    DialogCompararTraspaso dlg(base->nombreConexionLocal(),
                               m_nombreTiendaRemota,
                               "entradaGenero_tmp",
                               "salidaGenero_tmp",
                               m_idTiendaRemotaEnLocal,
                               m_idTiendaLocalEnRemota,
                               m_connRemota,
                               this);

    connect(&dlg, &DialogCompararTraspaso::datosModificados, this, [this]() {
        actualizarTabla();
        actualizarTablaRemota();
    });

    dlg.exec();
}

/**
 * @brief Procesa el descuento de lotes y el archivo de salida en la tienda remota.
 */
bool EntradaMercancia::procesarSalidaRemota(const QString &connRemota, int idLocalEnRemota)
{
    QSqlDatabase dbRem = QSqlDatabase::database(connRemota, false);
    if (!dbRem.isOpen()) return false;

    // 1. Descontar lotes en la tienda remota
    QSqlQuery qSel(dbRem);
    qSel.prepare("SELECT cod, fechaCaducidad, cantidad, pvp, descripcion FROM salidaGenero_tmp WHERE idTienda = ?");
    qSel.bindValue(0, idLocalEnRemota);
    if (qSel.exec()) {
        while (qSel.next()) {
            QString cod = qSel.value("cod").toString();
            QString fechaCad = qSel.value("fechaCaducidad").toString();
            int uds = qSel.value("cantidad").toInt();

            // Intentar descontar lote por fecha exacta en la remota
            QSqlQuery qLote(dbRem);
            qLote.prepare("UPDATE lotes SET cantidad = cantidad - ? WHERE ean = ? AND fecha = ? LIMIT 1");
            qLote.bindValue(0, uds);
            qLote.bindValue(1, cod);
            qLote.bindValue(2, fechaCad);
            qLote.exec();

            if (qLote.numRowsAffected() < 1) {
                // Si no coincide la fecha exacta, descontar del lote más antiguo
                QSqlQuery qFallback(dbRem);
                qFallback.prepare("UPDATE lotes SET cantidad = cantidad - ? WHERE ean = ? ORDER BY fecha ASC LIMIT 1");
                qFallback.bindValue(0, uds);
                qFallback.bindValue(1, cod);
                qFallback.exec();
            }
        }
    }

    // 2. Insertar en salidaGenero remota desde salidaGenero_tmp
    QSqlQuery qInsert(dbRem);
    qInsert.prepare("INSERT INTO salidaGenero (cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda) "
                    "SELECT cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda "
                    "FROM salidaGenero_tmp WHERE idTienda = ?");
    qInsert.bindValue(0, idLocalEnRemota);
    bool insOk = qInsert.exec();

    // 3. Eliminar de salidaGenero_tmp remota
    QSqlQuery qDel(dbRem);
    qDel.prepare("DELETE FROM salidaGenero_tmp WHERE idTienda = ?");
    qDel.bindValue(0, idLocalEnRemota);
    bool delOk = qDel.exec();

    // 4. Registrar log de auditoría en la base remota
    QSqlQuery qLog(dbRem);
    qLog.prepare("INSERT INTO logs (tipo, usuario, accion, fecha, hora) VALUES ('Info', ?, ?, CURDATE(), CURTIME())");
    qLog.bindValue(0, conf ? conf->getUsuario() : "admin");
    qLog.bindValue(1, QString("Salida traspaso aceptada bilateralmente hacia %1").arg(base->nombreConexionLocal()));
    qLog.exec();

    return insOk && delOk;
}

/**
 * @brief Acepta y consolida el traspaso en ambas tiendas simultáneamente. Exclusivo para administradores.
 */
void EntradaMercancia::on_pushButtonAceptarAmbas_clicked()
{
    if (!conf || conf->getRol() != 0) {
        QMessageBox::warning(this, tr("Acceso denegado"),
                             tr("Esta operación es exclusiva para administradores."));
        return;
    }

    if (m_nombreTiendaRemota.isEmpty()) {
        QMessageBox::warning(this, tr("Aviso"), tr("Debe seleccionar una tienda de procedencia."));
        return;
    }

    if (mTablaEntradas->rowCount() == 0 && mTablaRemota->rowCount() == 0) {
        QMessageBox::information(this, tr("Sin datos"),
                                 tr("No hay líneas pendientes de traspaso en ninguna de las dos tiendas."));
        return;
    }

    if (!asegurarConexionRemota(m_nombreTiendaRemota)) {
        QMessageBox::critical(this, tr("Error de Conexión"),
                              tr("No es posible conectar con la tienda remota '%1' para procesar su salida.")
                                  .arg(m_nombreTiendaRemota));
        return;
    }

    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("Aceptar Traspaso en Ambas Tiendas"));
    msgBox.setText(tr("¿Desea consolidar y aceptar el traspaso en AMBAS tiendas simultáneamente?"));
    msgBox.setInformativeText(tr("Acciones que se ejecutarán:\n"
                                 "1. Se registrarán las entradas e incrementará el stock en la tienda local (%1).\n"
                                 "2. Se registrarán las salidas y descontará el stock en la tienda remota (%2).\n\n"
                                 "Esta acción no se puede deshacer.")
                                  .arg(base->nombreConexionLocal(), m_nombreTiendaRemota));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);

    if (msgBox.exec() != QMessageBox::Yes) {
        return;
    }

    // 1. Procesar entrada en tienda local
    for (int i = 0; i < mTablaEntradas->rowCount(); ++i) {
        procesarLineaEntrada(mTablaEntradas->record(i));
    }
    base->insertarLog(conf->getConexionLocal(), "Info", conf->getUsuario(),
                      QString("Entrada traspaso aceptada bilateralmente desde %1").arg(m_nombreTiendaRemota));
    guardarArticulo(m_idTiendaRemotaEnLocal);
    limpiarTabla(m_idTiendaRemotaEnLocal);

    // 2. Procesar salida en la tienda remota
    bool okRemota = procesarSalidaRemota(m_connRemota, m_idTiendaLocalEnRemota);

    actualizarTabla();
    actualizarTablaRemota();

    if (okRemota) {
        QMessageBox::information(this, tr("Traspaso Completado"),
                                 tr("El traspaso ha sido aceptado y procesado con éxito en ambas tiendas."));
    } else {
        QMessageBox::warning(this, tr("Atención"),
                             tr("La entrada local se procesó correctamente, pero hubo incidencias al procesar la salida en la tienda remota. Compruebe los registros."));
    }
}

void EntradaMercancia::on_pushButtonSalir_clicked()
{
    close();
}

