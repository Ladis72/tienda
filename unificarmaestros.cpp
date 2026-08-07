#include "unificarmaestros.h"
#include "ui_unificarmaestros.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include <QStyle>
#include "base_datos.h"

UnificarMaestros::UnificarMaestros(const UnificarMaestrosConfig &config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UnificarMaestros),
    m_config(config)
{
    qDebug() << "[UnificarMaestros] Iniciando constructor para tabla:" << m_config.tablaMaestra;
    ui->setupUi(this);
    setWindowTitle(m_config.titulo);
    ui->labelTitle->setText(m_config.titulo);

    modelo = new QSqlQueryModel(this);
    ui->tableView->setModel(modelo);

    // Ajustes visuales
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);

    // Iconos
    ui->pushButtonFusionar->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    ui->pushButtonRenombrar->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    ui->pushButtonCerrar->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));

    qDebug() << "[UnificarMaestros] Constructor finalizado. Llamando a refrescarTabla().";
    refrescarTabla();
}

UnificarMaestros::~UnificarMaestros()
{
    qDebug() << "[UnificarMaestros] Destructor llamado.";
    delete ui;
}

void UnificarMaestros::refrescarTabla()
{
    qDebug() << "[UnificarMaestros] Entrando en refrescarTabla()";
    
    QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());
    if (!db.isOpen()) {
        qDebug() << "[UnificarMaestros] ERROR: La conexión local no está abierta.";
        return;
    }

    // Construir la query dinámica. 
    // Mostramos el conteo de la primera dependencia si existe.
    QString subqueryArt = "0";
    QString labelDep = "N/A";
    if (!m_config.dependencias.isEmpty()) {
        const auto &dep = m_config.dependencias.first();
        subqueryArt = QString("(SELECT count(*) FROM %1 WHERE %2 = f.%3)").arg(dep.tabla, dep.campo, m_config.campoId);
        labelDep = "Uso"; // Genérico para "Artículos", "Tickets", etc.
        if (dep.tabla == "articulos") labelDep = "Artículos";
    }

    QString sql = QString("SELECT f.%1, f.%2 as 'Nombre', %3 as '%4' "
                          "FROM %5 f ORDER BY f.%2 ASC")
                  .arg(m_config.campoId, m_config.campoNombre, subqueryArt, labelDep, m_config.tablaMaestra);
    
    qDebug() << "[UnificarMaestros] Ejecutando SQL de refresco:" << sql;
    modelo->setQuery(sql, db);
    
    if (modelo->lastError().isValid()) {
        qDebug() << "[UnificarMaestros] Error refrescando maestros:" << modelo->lastError().text();
    }
    
    qDebug() << "[UnificarMaestros] Tabla refrescada con" << modelo->rowCount() << "filas.";
    
    ui->tableView->setColumnWidth(0, 60);  // ID
    ui->tableView->setColumnWidth(2, 90);  // Conteo
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

void UnificarMaestros::on_pushButtonFusionar_clicked()
{
    QModelIndexList seleccion = ui->tableView->selectionModel()->selectedRows();
    
    if (seleccion.count() < 2) {
        QMessageBox::warning(this, tr("Atención"), tr("Por favor, seleccione al menos dos elementos para fusionar."));
        return;
    }

    // 1. Identificar IDs y nombres
    QList<int> ids;
    QStringList nombres;
    for (const QModelIndex &idx : seleccion) {
        ids << modelo->data(modelo->index(idx.row(), 0)).toInt();
        nombres << modelo->data(modelo->index(idx.row(), 1)).toString();
    }

    // 2. Preguntar cuál sobrevive
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Seleccionar Destino"),
                                         tr("Seleccione el registro que desea conservar (ganador):"),
                                         nombres, 0, false, &ok);
    
    if (!ok) return;

    int indiceGanador = nombres.indexOf(item);
    int idGanador = ids.takeAt(indiceGanador);
    QList<int> idsPerdedores = ids;

    // 3. Confirmación final
    if (QMessageBox::question(this, tr("Confirmar Fusión"),
                             tr("Se van a fusionar %1 registros en '%2'.\n\n"
                                "Todos los datos dependientes pasarán al ganador y los registros originales serán BORRADOS localmente y en la nube.\n"
                                "¿Desea continuar?").arg(seleccion.count()).arg(item),
                             QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    // 4. Ejecutar
    if (ejecutarFusion(idGanador, idsPerdedores)) {
        QMessageBox::information(this, tr("Éxito"), tr("La fusión se ha realizado correctamente."));
        refrescarTabla();
    }
}

bool UnificarMaestros::ejecutarFusion(int idGanador, const QList<int> &idsPerdedores)
{
    QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());
    
    if (!db.transaction()) {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo iniciar la transacción: ") + db.lastError().text());
        return false;
    }

    QSqlQuery q(db);

    // A) Mover dependencias en cascada
    for (const auto &dep : m_config.dependencias) {
        // Los identificadores de tabla/campo provienen de la configuración
        // (constantes en código, whitelist). Todos los VALORES se pasan como
        // parámetros enlazados para evitar inyección SQL.
        QVariant valorGanador;
        QStringList valoresPerdedores;

        if (dep.campoMaestroOrigen.isEmpty()) {
            // Caso estándar: por ID numérico
            valorGanador = idGanador;
            for (int idPerdedor : idsPerdedores) valoresPerdedores << QString::number(idPerdedor);
        } else {
            // Caso especial: por otro campo (ej: login/usuario)
            // 1. Obtener valor del ganador
            q.prepare(QString("SELECT %1 FROM %2 WHERE %3 = ?")
                      .arg(dep.campoMaestroOrigen, m_config.tablaMaestra, m_config.campoId));
            q.addBindValue(idGanador);
            if (!q.exec() || !q.first()) {
                db.rollback();
                QMessageBox::critical(this, tr("Error"), tr("No se pudo obtener el valor de '%1' para el ganador.").arg(dep.campoMaestroOrigen));
                return false;
            }
            valorGanador = q.value(0);

            // 2. Obtener valores de los perdedores
            QStringList idsPerdedoresSql;
            for (int idPerdedor : idsPerdedores) idsPerdedoresSql << QString::number(idPerdedor);
            q.prepare(QString("SELECT %1 FROM %2 WHERE %3 IN (%4)")
                      .arg(dep.campoMaestroOrigen, m_config.tablaMaestra, m_config.campoId,
                           idsPerdedoresSql.join(",")));
            if (!q.exec()) {
                db.rollback();
                return false;
            }
            while (q.next()) valoresPerdedores << q.value(0).toString();
        }

        if (valoresPerdedores.isEmpty()) continue;

        // Construir placeholders para la lista de valores perdedores
        QStringList placeholders;
        for (int i = 0; i < valoresPerdedores.size(); ++i) placeholders << "?";
        QString sqlUpd = QString("UPDATE %1 SET %2 = ? WHERE %2 IN (%3)")
                         .arg(dep.tabla, dep.campo, placeholders.join(","));

        q.prepare(sqlUpd);
        q.addBindValue(valorGanador);
        for (const QString &vp : valoresPerdedores) q.addBindValue(vp);

        if (!q.exec()) {
            db.rollback();
            QMessageBox::critical(this, tr("Error SQL"), tr("Error actualizando dependencias en %1:\n").arg(dep.tabla) + q.lastError().text());
            return false;
        }
    }

    // B) Borrar registros perdedores
    QStringList placeholdersDel;
    for (int i = 0; i < idsPerdedores.size(); ++i) placeholdersDel << "?";
    QSqlQuery qDel(db);
    qDel.prepare(QString("DELETE FROM %1 WHERE %2 IN (%3)")
                 .arg(m_config.tablaMaestra, m_config.campoId, placeholdersDel.join(",")));
    for (int idPerdedor : idsPerdedores) qDel.addBindValue(idPerdedor);
    if (!qDel.exec()) {
        db.rollback();
        QMessageBox::critical(this, tr("Error SQL"), tr("Error eliminando registros obsoletos:\n") + qDel.lastError().text());
        return false;
    }

    // C) Registrar la unificación para propagar a otras tiendas. Se hace dentro
    // de la misma transacción: si falla el registro, la fusión se revierte y
    // nunca queda aplicada localmente sin estar propagada a la nube.
    QSqlQuery qSync(db);
    qSync.prepare("INSERT INTO sync_unificaciones (tabla, id_perdedor, id_ganador) VALUES (?, ?, ?)");
    qSync.addBindValue(m_config.tablaMaestra);
    for (int idPerdedor : idsPerdedores) {
        qSync.bindValue(1, QString::number(idPerdedor));
        qSync.bindValue(2, QString::number(idGanador));
        if (!qSync.exec()) {
            db.rollback();
            QMessageBox::critical(this, tr("Error SQL"),
                                  tr("No se pudo registrar la unificación en sync_unificaciones:\n") + qSync.lastError().text());
            return false;
        }
    }

    if (!db.commit()) {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo confirmar (commit) la transacción."));
        return false;
    }
    return true;
}

void UnificarMaestros::on_pushButtonRenombrar_clicked()
{
    QModelIndexList seleccion = ui->tableView->selectionModel()->selectedRows();
    if (seleccion.isEmpty()) {
        QMessageBox::warning(this, tr("Atención"), tr("Por favor, seleccione un elemento para renombrar."));
        return;
    }

    int id = modelo->data(modelo->index(seleccion.at(0).row(), 0)).toInt();
    QString nombreActual = modelo->data(modelo->index(seleccion.at(0).row(), 1)).toString();

    bool ok;
    QString nuevoNombre = QInputDialog::getText(this, tr("Renombrar"),
                                                tr("Nuevo nombre para '%1':").arg(nombreActual),
                                                QLineEdit::Normal, nombreActual, &ok);
    
    if (ok && !nuevoNombre.isEmpty() && nuevoNombre != nombreActual) {
        QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
        q.prepare(QString("UPDATE %1 SET %2 = :nombre WHERE %3 = :id")
                  .arg(m_config.tablaMaestra, m_config.campoNombre, m_config.campoId));
        q.bindValue(":nombre", nuevoNombre);
        q.bindValue(":id", id);
        
        if (q.exec()) {
            refrescarTabla();
        } else {
            QMessageBox::critical(this, tr("Error"), tr("No se pudo actualizar el nombre:\n") + q.lastError().text());
        }
    }
}

void UnificarMaestros::on_pushButtonCerrar_clicked()
{
    accept();
}

UnificarMaestrosConfig UnificarMaestrosConfig::configParaTabla(const QString &tabla)
{
    if (tabla == "proveedores") return paraProveedores();
    if (tabla == "clientes") return paraClientes();
    if (tabla == "fpago") return paraFormasPago();
    if (tabla == "formatos") return paraFormatos();
    if (tabla == "motivosEntrada") return paraMotivosEntrada();
    if (tabla == "usuarios") return paraUsuarios();
    
    // Default / fallback
    return { "", tabla, "id", "", {} };
}
