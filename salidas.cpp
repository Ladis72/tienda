#include "salidas.h"
#include <QMessageBox>
#include <QInputDialog>
#include "ui_salidas.h"
#include "conexion.h"
#include "dialogcomparartraspaso.h"
#include <QShortcut>

Salidas::Salidas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Salidas)
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

    mTablaSalidas = new QSqlTableModel(this, QSqlDatabase::database(conf->getConexionLocal()));
    ui->comboBoxDestino->blockSignals(true);
    llenarComboTiendas();
    ui->comboBoxDestino->blockSignals(false);

    mTablaSalidas->setTable("salidaGenero_tmp");
    ui->tableView->setModel(mTablaSalidas);
    ui->tableView->setSortingEnabled(true);
    mTablaSalidas->setSort(3, Qt::AscendingOrder); // Orden por descripción por defecto
    connect(mTablaSalidas, &QAbstractItemModel::dataChanged, this, &Salidas::actualizarTotales);

    // Modelo para la tabla remota
    mTablaRemota = new QSqlQueryModel(this);
    ui->tableViewRemota->setModel(mTablaRemota);
    ui->tableViewRemota->setSortingEnabled(true);

    actualizarTabla();

    // Desactivar autoDefault para evitar inserciones accidentales al pulsar ENTER
    ui->pushButtonAgregar->setAutoDefault(false);
    ui->pushButtonAgregar->setDefault(false);
    ui->pushButtonEnviar->setAutoDefault(false);
    ui->pushButtonEnviar->setDefault(false);
    ui->pushButtonBorrar->setAutoDefault(false);
    ui->pushButtonBorrar->setDefault(false);
    ui->pushButtonComparar->setAutoDefault(false);
    ui->pushButtonComparar->setDefault(false);
    ui->pushButtonAceptarAmbas->setAutoDefault(false);
    ui->pushButtonAceptarAmbas->setDefault(false);
    ui->pushButtonCerrar->setAutoDefault(false);
    ui->pushButtonCerrar->setDefault(false);

    // Cadena de enfoque mediante ENTER
    connect(ui->lineEditCantidad, &QLineEdit::returnPressed, [this](){
        ui->dateEditFC->setFocus();
    });
    connect(ui->lineEditPrecio, &QLineEdit::returnPressed, this, &Salidas::on_pushButtonAgregar_clicked);

    aplicarPermisos();
}

Salidas::~Salidas()
{
    delete ui;
}

/**
 * @brief Aplica permisos a las acciones del módulo de salidas de mercancía.
 */
void Salidas::aplicarPermisos() {
    if (!conf || !conf->permisos())
        return;

    ui->pushButtonAgregar->setEnabled(conf->permisos()->tiene("salidas.crear"));
    ui->pushButtonEnviar->setEnabled(conf->permisos()->tiene("salidas.crear"));
    ui->pushButtonBorrar->setEnabled(conf->permisos()->tiene("salidas.borrar"));

    // El botón para aceptar en ambas tiendas es exclusivo de administradores (Rol 0)
    bool esAdmin = (conf->getRol() == 0);
    ui->pushButtonAceptarAmbas->setEnabled(esAdmin);
    if (!esAdmin) {
        ui->pushButtonAceptarAmbas->setToolTip(tr("Solo disponible para administradores (Rol 0)"));
    }
}

void Salidas::on_lineEditCod_returnPressed()
{

    QString codigo = ui->lineEditCod->text().trimmed();
    if (codigo.isEmpty())
        return;

    QSqlRecord registro = base.consulta_producto(conf->getConexionLocal(), codigo);

    // 1. Buscar por código directo
    if (registro.isEmpty()) {
        // 2. Buscar por código auxiliar
        QString codAux = base.codigoDesdeAux(conf->getConexionLocal(), codigo);
        if (!codAux.isEmpty()) {
            registro = base.consulta_producto(conf->getConexionLocal(), codAux);
        }
    }

    // 3. Si hay producto válido
    if (!registro.isEmpty()) {
        ui->lineEditCod->setText(registro.value("cod").toString());
        ui->lineEditDesc->setText(registro.value("descripcion").toString());
        ui->lineEditPrecio->setText(registro.value("pvp").toString());
        ui->lineEditCantidad->setFocus();
        return;
    }

    // 4. No existe → preguntar creación
    QMessageBox msg(this);
    msg.setText("No se encuentra el producto");
    msg.setInformativeText("¿Desea crearlo?");
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Ok);

    if (msg.exec() == QMessageBox::Ok) {
        Articulos articulo(this);
        articulo.exec();
        articulo.borrarFormulario();
    } else {
        ui->lineEditCod->setFocus();
        ui->lineEditCod->selectAll();
    }
}

void Salidas::actualizarTabla()
{
    qDebug() << "Salidas::actualizarTabla - Inicio";
    m_nombreTiendaRemota = ui->comboBoxDestino->currentText().trimmed();
    
    if (m_nombreTiendaRemota.isEmpty()) {
        m_idTiendaRemotaEnLocal = -1;
        mTablaSalidas->setFilter("idTienda = -1");
        qDebug() << "Salidas::actualizarTabla - Combo vacío, aplicando filtro idTienda = -1";
    } else {
        m_idTiendaRemotaEnLocal = base.idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()), m_nombreTiendaRemota);
        mTablaSalidas->setFilter("idTienda = " + QString::number(m_idTiendaRemotaEnLocal));
        qDebug() << "Salidas::actualizarTabla - Filtrando por idTienda:" << m_idTiendaRemotaEnLocal;
    }
    
    if (!mTablaSalidas->select()) {
        qDebug() << "Salidas::actualizarTabla - Error en select:" << mTablaSalidas->lastError().text();
    }

    // Forzar la carga de todos los registros para asegurar que rowCount() y la vista sean correctos
    int count = 0;
    while (mTablaSalidas->canFetchMore()) {
        mTablaSalidas->fetchMore();
        count++;
        if (count > 100) break; // Seguridad para evitar bucles infinitos en errores de driver
    }
    
    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
    actualizarTotales();
    qDebug() << "Salidas::actualizarTabla - Fin. Filas cargadas:" << mTablaSalidas->rowCount();
}

void Salidas::llenarComboTiendas()
{
    ui->comboBoxDestino->clear();
    QSqlQuery listaCombo = base.tiendas(QSqlDatabase::database(conf->getConexionLocal()));
    while (listaCombo.next()) {
        ui->comboBoxDestino->addItem(listaCombo.value("nombre").toString());
    }
}

void Salidas::on_pushButtonAgregar_clicked()
{
    if (ui->lineEditCantidad->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "La cantidad no puede estar vacía.");
        ui->lineEditCantidad->setFocus();
        return;
    }

    QString codProducto = ui->lineEditCod->text().trimmed();
    QString fechaFC = ui->dateEditFC->text();

    QString idLote = base.idLote(conf->getConexionLocal(),
                                  codProducto,
                                  "",
                                  fechaFC);
    if (idLote == "0") {
        // Consultar fechas/lotes existentes con stock para este producto
        QSqlQuery qFechas(QSqlDatabase::database(conf->getConexionLocal()));
        qFechas.prepare("SELECT DATE_FORMAT(fecha, '%Y-%m-%d') AS f, sum(cantidad) AS cant "
                        "FROM lotes WHERE ean = ? GROUP BY fecha HAVING cant > 0 ORDER BY f ASC");
        qFechas.bindValue(0, codProducto);
        QStringList opcionesFechas;
        if (qFechas.exec()) {
            while (qFechas.next()) {
                opcionesFechas << QString("%1 (Stock: %2)").arg(qFechas.value("f").toString()).arg(qFechas.value("cant").toString());
            }
        }

        if (!opcionesFechas.isEmpty()) {
            bool ok = false;
            QString seleccion = QInputDialog::getItem(
                this,
                "Seleccionar fecha de caducidad",
                "La fecha indicada no coincide con ninguna de las fechas registradas en almacén.\n"
                "Seleccione de qué fecha desea descontar los artículos:",
                opcionesFechas, 0, false, &ok);
            if (ok && !seleccion.isEmpty()) {
                fechaFC = seleccion.split(" ").first();
                ui->dateEditFC->setDate(QDate::fromString(fechaFC, "yyyy-MM-dd"));
                idLote = base.idLote(conf->getConexionLocal(), codProducto, "", fechaFC);
            } else {
                return; // Operación cancelada por el usuario
            }
        } else {
            QMessageBox msgBox;
            msgBox.setText("Parece que no hay en el almacén un producto con esos datos");
            msgBox.setInformativeText(
                "¿Desea continuar con la operación o cancelar y volver a comprobarlo?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            int ret = msgBox.exec();
            if (ret == QMessageBox::Cancel) {
                return;
            }
        }
    }
    // Se utiliza una consulta preparada para consultar las existencias del lote de forma segura
    QSqlQuery datosLote(QSqlDatabase::database(conf->getConexionLocal()));
    datosLote.prepare("SELECT cantidad FROM lotes WHERE id = ?");
    datosLote.bindValue(0, idLote);
    datosLote.exec();
    datosLote.first();
    int resto = datosLote.record().value("cantidad").toInt() - ui->lineEditCantidad->text().toInt();
    if (resto < 0) {
        QMessageBox msgBox;
        msgBox.setText("Parece que no hay en el almacen suficiente producto con esos datos");
        msgBox.setInformativeText(
            "¿Deseas continual con la operación o Cancelar y volver a comprobarlo?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Cancel:
            qDebug() << "cancelado";
            return;
            break;
        default:
            break;
        }
    }
    if (ui->lineEditPrecio->text().trimmed().isEmpty()) {
        ui->lineEditPrecio->setText("0");
    }

    QStringList datos;
    datos.clear();
    datos.append(ui->lineEditCod->text());
    datos.append(QDate::currentDate().toString("yyyy-MM-dd"));
    datos.append(ui->lineEditDesc->text());
    datos.append(ui->lineEditCantidad->text());
    datos.append(ui->dateEditFC->text());
    datos.append(ui->lineEditPrecio->text());
    datos.append(QString::number(base.idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()),
                                                           ui->comboBoxDestino->currentText())));
    base.insertarEnTabla(QSqlDatabase::database(conf->getConexionLocal()), "salidaGenero_tmp", datos);

    //    mTablaSalidas->select();
    ui->lineEditCantidad->clear();
    ui->lineEditCod->clear();
    ui->lineEditDesc->clear();
    ui->lineEditPrecio->clear();
    ui->lineEditCod->setFocus();
    //    ui->tableView->resizeColumnsToContents();
    actualizarTabla();
}

void Salidas::on_lineEditDesc_returnPressed()
{
    QSqlQuery consulta = base.buscarProducto(QSqlDatabase::database(conf->getConexionLocal()),
                                              "articulos",
                                              ui->lineEditDesc->text());
    consulta.first();
    BuscarProducto *buscar = new BuscarProducto(this, std::move(consulta));
    buscar->exec();
    ui->lineEditCod->setText(buscar->resultado);
    emit on_lineEditCod_returnPressed();
}

void Salidas::on_pushButtonEnviar_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Los datos serán prodesados");
    msgBox.setInformativeText("¿Deseas continual? \nNo se puede deshacer");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Cancel:
        qDebug() << "cancelado";
        return;
    default:
        break;
    }
    for (int i = 0; i < mTablaSalidas->rowCount(); ++i) {
        QString cod, descripcion, pvp, fechaCaducidad, idTienda;
        int uds;
        cod = mTablaSalidas->record(i).value(1).toString();
        descripcion = mTablaSalidas->record(i).value(3).toString();
        pvp = mTablaSalidas->record(i).value(6).toString();
        fechaCaducidad = mTablaSalidas->record(i).value(5).toString();
        uds = mTablaSalidas->record(i).value(4).toInt();
        idTienda = mTablaSalidas->record(i).value(7).toString();

        base.disminuirLote(cod, fechaCaducidad, uds);
        QString precioValidado = pvp.isEmpty() ? "0" : pvp;
        // Se parametriza la actualización del artículo para prevenir inyecciones SQL
        QSqlQuery tmp(QSqlDatabase::database(conf->getConexionLocal()));
        tmp.prepare("UPDATE articulos SET descripcion = ?, pvp = ? WHERE cod = ?");
        tmp.bindValue(0, descripcion);
        tmp.bindValue(1, precioValidado.toDouble());
        tmp.bindValue(2, cod);
        tmp.exec();
    }
    int idTienda = base.idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()), ui->comboBoxDestino->currentText());
    // Se parametriza el trasvase masivo de salida de género filtrando por idTienda
    QSqlQuery tmp(QSqlDatabase::database(conf->getConexionLocal()));
    tmp.prepare("INSERT INTO salidaGenero (cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda) "
                "SELECT cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda "
                "FROM salidaGenero_tmp WHERE idTienda = ?");
    tmp.bindValue(0, idTienda);
    tmp.exec();

    // Se parametriza el vaciado de la tabla temporal filtrando por idTienda
    QSqlQuery deleteTmpQuery(QSqlDatabase::database(conf->getConexionLocal()));
    deleteTmpQuery.prepare("DELETE FROM salidaGenero_tmp WHERE idTienda = ?");
    deleteTmpQuery.bindValue(0, idTienda);
    deleteTmpQuery.exec();
    base.insertarLog(conf->getConexionLocal(), "Info", conf->getUsuario(), "Salida genero ");

    actualizarTabla();
}

void Salidas::on_pushButtonBorrar_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        return;
    }

    // Obtenemos el ID de la columna 0 de la fila seleccionada actualmente
    QString id = mTablaSalidas->data(mTablaSalidas->index(index.row(), 0)).toString();

    if (!id.isEmpty()) {
        // Se parametriza la consulta de borrado de la línea temporal
        QSqlQuery tmp(QSqlDatabase::database(conf->getConexionLocal()));
        tmp.prepare("DELETE FROM salidaGenero_tmp WHERE id = ?");
        tmp.bindValue(0, id);
        tmp.exec();
        qDebug() << tmp.lastError();
    }
    actualizarTabla();
}


void Salidas::actualizarTotales()
{
    lineas = mTablaSalidas->rowCount();
    productos = 0;
    for (int i = 0; i < mTablaSalidas->rowCount(); ++i) {
        QModelIndex idx = mTablaSalidas->index(i, 4);
        double valor = mTablaSalidas->data(idx).toDouble();
        productos += valor;
    }
    ui->lbSalidas->setText("Lineas= " + QString::number(lineas)
                           + "  Productos=" + QString::number(productos));
    if (ui->lbTotalesLocal) {
        ui->lbTotalesLocal->setText(QString("%1 líneas | %2 uds").arg(lineas).arg(productos));
    }
}

void Salidas::on_comboBoxDestino_currentIndexChanged(int)
{
    actualizarTabla();
    // Solo actualizar tabla remota si el panel remoto está visible
    if (ui->widgetRemoto->isVisible()) {
        actualizarTablaRemota();
    }
    qDebug() << "Salidas: Tienda destino cambiada a" << ui->comboBoxDestino->currentText();
}

/**
 * @brief Comprueba si la conexión con la base de datos de la tienda remota ya está activa.
 * No intenta conectar activamente por red: si ya está abierta se usa, si no se marca desconectada.
 * Se pasa open = false a QSqlDatabase::database para no bloquear con timeouts si está desconectada.
 * @param nombreTienda Nombre de la tienda según la tabla 'tiendas'.
 * @return true si la conexión está disponible y abierta.
 */
bool Salidas::asegurarConexionRemota(const QString &nombreTienda)
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
int Salidas::obtenerIdLocalEnRemota(const QString &connRemota)
{
    QSqlDatabase dbRem = QSqlDatabase::database(connRemota, false);
    if (!dbRem.isOpen()) return 0;

    QString nombreLocal = base.nombreConexionLocal();
    QSqlQuery q(dbRem);
    q.prepare("SELECT id FROM tiendas WHERE nombre = ?");
    q.bindValue(0, nombreLocal);
    if (q.exec() && q.next()) {
        return q.value(0).toInt();
    }
    return 0;
}

/**
 * @brief Carga y visualiza las entradas pendientes preparadas en la tienda remota de destino.
 */
void Salidas::actualizarTablaRemota()
{
    m_nombreTiendaRemota = ui->comboBoxDestino->currentText().trimmed();
    if (m_nombreTiendaRemota.isEmpty()) {
        ui->lbEstadoRemota->setText("[Sin tienda]");
        ui->lbTotalesRemota->setText("0 líneas | 0 uds");
        mTablaRemota->clear();
        return;
    }

    ui->labelTituloRemota->setText(QString("📥 Entradas en %1 (Pendientes)").arg(m_nombreTiendaRemota));

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
    // Seleccionar entradas pendientes en la tienda remota procedentes de nuestra tienda
    q.prepare("SELECT id, cod, DATE_FORMAT(fechaEntrada, '%Y-%m-%d') AS f_ent, descripcion, "
              "cantidad, DATE_FORMAT(fechaCaducidad, '%Y-%m-%d') AS f_cad, pvp "
              "FROM entradaGenero_tmp WHERE idTienda = ? ORDER BY descripcion ASC");
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
        qWarning() << "Salidas: Error al consultar entradaGenero_tmp remota:" << q.lastError().text();
        ui->lbTotalesRemota->setText("Error al consultar");
    }
}

/**
 * @brief Abre el diálogo de comparación para contrastar las tablas local y remota y solventar diferencias.
 */
void Salidas::on_pushButtonComparar_clicked()
{
    if (m_nombreTiendaRemota.isEmpty()) {
        QMessageBox::warning(this, tr("Aviso"), tr("Debe seleccionar una tienda de destino."));
        return;
    }

    if (!asegurarConexionRemota(m_nombreTiendaRemota)) {
        QMessageBox::warning(this, tr("Conexión Fallida"),
                             tr("No se puede conectar con la tienda remota '%1' para comparar las tablas.")
                                 .arg(m_nombreTiendaRemota));
        return;
    }

    DialogCompararTraspaso dlg(base.nombreConexionLocal(),
                               m_nombreTiendaRemota,
                               "salidaGenero_tmp",
                               "entradaGenero_tmp",
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
 * @brief Procesa el aumento de existencias en lotes y el registro de entrada en la tienda remota.
 */
bool Salidas::procesarEntradaRemota(const QString &connRemota, int idLocalEnRemota)
{
    QSqlDatabase dbRem = QSqlDatabase::database(connRemota, false);
    if (!dbRem.isOpen()) return false;

    // 1. Aumentar existencias en lotes en la tienda remota para cada línea
    QSqlQuery qSel(dbRem);
    qSel.prepare("SELECT cod, fechaCaducidad, cantidad, pvp, descripcion FROM entradaGenero_tmp WHERE idTienda = ?");
    qSel.bindValue(0, idLocalEnRemota);
    if (qSel.exec()) {
        while (qSel.next()) {
            QString cod = qSel.value("cod").toString();
            QString fechaCad = qSel.value("fechaCaducidad").toString();
            int uds = qSel.value("cantidad").toInt();

            // Buscar lote existente
            QSqlQuery qCheck(dbRem);
            qCheck.prepare("SELECT id FROM lotes WHERE ean = ? AND fecha = ? LIMIT 1");
            qCheck.bindValue(0, cod);
            qCheck.bindValue(1, fechaCad);
            if (qCheck.exec() && qCheck.next()) {
                QSqlQuery qUp(dbRem);
                qUp.prepare("UPDATE lotes SET cantidad = cantidad + ? WHERE id = ?");
                qUp.bindValue(0, uds);
                qUp.bindValue(1, qCheck.value(0));
                qUp.exec();
            } else {
                QSqlQuery qIns(dbRem);
                qIns.prepare("INSERT INTO lotes (ean, lote, fecha, cantidad) VALUES (?, '', ?, ?)");
                qIns.bindValue(0, cod);
                qIns.bindValue(1, fechaCad);
                qIns.bindValue(2, uds);
                qIns.exec();
            }
        }
    }

    // 2. Insertar en entradaGenero remota desde entradaGenero_tmp
    QSqlQuery qInsert(dbRem);
    qInsert.prepare("INSERT INTO entradaGenero (cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda) "
                    "SELECT cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda "
                    "FROM entradaGenero_tmp WHERE idTienda = ?");
    qInsert.bindValue(0, idLocalEnRemota);
    bool insOk = qInsert.exec();

    // 3. Eliminar de entradaGenero_tmp remota
    QSqlQuery qDel(dbRem);
    qDel.prepare("DELETE FROM entradaGenero_tmp WHERE idTienda = ?");
    qDel.bindValue(0, idLocalEnRemota);
    bool delOk = qDel.exec();

    // 4. Registrar log de auditoría en la remota
    QSqlQuery qLog(dbRem);
    qLog.prepare("INSERT INTO logs (tipo, usuario, accion, fecha, hora) VALUES ('Info', ?, ?, CURDATE(), CURTIME())");
    qLog.bindValue(0, conf ? conf->getUsuario() : "admin");
    qLog.bindValue(1, QString("Entrada traspaso aceptada bilateralmente procedente de %1").arg(base.nombreConexionLocal()));
    qLog.exec();

    return insOk && delOk;
}

/**
 * @brief Acepta y consolida el traspaso en ambas tiendas simultáneamente. Exclusivo para administradores.
 */
void Salidas::on_pushButtonAceptarAmbas_clicked()
{
    if (!conf || conf->getRol() != 0) {
        QMessageBox::warning(this, tr("Acceso denegado"),
                             tr("Esta operación es exclusiva para administradores."));
        return;
    }

    if (m_nombreTiendaRemota.isEmpty()) {
        QMessageBox::warning(this, tr("Aviso"), tr("Debe seleccionar una tienda de destino."));
        return;
    }

    if (mTablaSalidas->rowCount() == 0 && mTablaRemota->rowCount() == 0) {
        QMessageBox::information(this, tr("Sin datos"),
                                 tr("No hay líneas pendientes de traspaso en ninguna de las dos tiendas."));
        return;
    }

    if (!asegurarConexionRemota(m_nombreTiendaRemota)) {
        QMessageBox::critical(this, tr("Error de Conexión"),
                              tr("No es posible conectar con la tienda remota '%1' para procesar su entrada.")
                                  .arg(m_nombreTiendaRemota));
        return;
    }

    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("Aceptar Traspaso en Ambas Tiendas"));
    msgBox.setText(tr("¿Desea consolidar y aceptar el traspaso en AMBAS tiendas simultáneamente?"));
    msgBox.setInformativeText(tr("Acciones que se ejecutarán:\n"
                                 "1. Se registrarán las salidas y descontará el stock en la tienda local (%1).\n"
                                 "2. Se registrarán las entradas e incrementará el stock en la tienda remota (%2).\n\n"
                                 "Esta acción no se puede deshacer.")
                                  .arg(base.nombreConexionLocal(), m_nombreTiendaRemota));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);

    if (msgBox.exec() != QMessageBox::Yes) {
        return;
    }

    // 1. Procesar salida en la tienda local
    for (int i = 0; i < mTablaSalidas->rowCount(); ++i) {
        QString cod = mTablaSalidas->record(i).value("cod").toString();
        QString descripcion = mTablaSalidas->record(i).value("descripcion").toString();
        QString pvp = mTablaSalidas->record(i).value("pvp").toString();
        QString fechaCaducidad = mTablaSalidas->record(i).value("fechaCaducidad").toString();
        int uds = mTablaSalidas->record(i).value("cantidad").toInt();

        base.disminuirLote(cod, fechaCaducidad, uds);
        QString precioValidado = pvp.isEmpty() ? "0" : pvp;
        QSqlQuery tmp(QSqlDatabase::database(conf->getConexionLocal()));
        tmp.prepare("UPDATE articulos SET descripcion = ?, pvp = ? WHERE cod = ?");
        tmp.bindValue(0, descripcion);
        tmp.bindValue(1, precioValidado.toDouble());
        tmp.bindValue(2, cod);
        tmp.exec();
    }

    QSqlQuery tmpInsert(QSqlDatabase::database(conf->getConexionLocal()));
    tmpInsert.prepare("INSERT INTO salidaGenero (cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda) "
                      "SELECT cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda "
                      "FROM salidaGenero_tmp WHERE idTienda = ?");
    tmpInsert.bindValue(0, m_idTiendaRemotaEnLocal);
    tmpInsert.exec();

    QSqlQuery deleteTmpQuery(QSqlDatabase::database(conf->getConexionLocal()));
    deleteTmpQuery.prepare("DELETE FROM salidaGenero_tmp WHERE idTienda = ?");
    deleteTmpQuery.bindValue(0, m_idTiendaRemotaEnLocal);
    deleteTmpQuery.exec();

    base.insertarLog(conf->getConexionLocal(), "Info", conf->getUsuario(),
                     QString("Salida traspaso aceptada bilateralmente hacia %1").arg(m_nombreTiendaRemota));

    // 2. Procesar entrada en la tienda remota
    bool okRemota = procesarEntradaRemota(m_connRemota, m_idTiendaLocalEnRemota);

    actualizarTabla();
    actualizarTablaRemota();

    if (okRemota) {
        QMessageBox::information(this, tr("Traspaso Completado"),
                                 tr("El traspaso ha sido aceptado y procesado con éxito en ambas tiendas."));
    } else {
        QMessageBox::warning(this, tr("Atención"),
                             tr("La salida local se procesó correctamente, pero hubo incidencias al registrar la entrada en la tienda remota. Compruebe los registros."));
    }
}

void Salidas::on_pushButtonCerrar_clicked()
{
    close();
}

