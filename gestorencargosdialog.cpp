#include "gestorencargosdialog.h"
#include "ui_gestorencargosdialog.h"
#include "encargosdialog.h"
#include "imprimirticket.h"
#include <QMessageBox>
#include <QDate>
#include <QTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStyledItemDelegate>
#include <QPainter>
#include "tienda.h"
#include "tpv.h"
#include "dialogdevolucionanticipo.h"



// Delegado para colorear las filas según el estado del encargo
class EncargosDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        // El estado del encargo está en la columna 10 según la consulta SQL en ajustarFiltro()
        QString estado = index.model()->data(index.model()->index(index.row(), 10)).toString();

        QColor colorFondo = option.palette.base().color();
        QColor colorTexto = option.palette.text().color();

        if (estado == "Pendiente") {
            colorFondo = QColor(255, 205, 210); // Rojo suave
            colorTexto = QColor(183, 28, 28);   // Rojo oscuro
        } else if (estado == "Recibido") {
            colorFondo = QColor(255, 245, 157); // Amarillo suave
            colorTexto = QColor(130, 119, 23);  // Amarillo/marrón oscuro
        } else if (estado == "Entregado") {
            colorFondo = QColor(197, 225, 165); // Verde suave
            colorTexto = QColor(46, 125, 50);   // Verde oscuro
        } else if (estado == "Cancelado") {
            colorFondo = QColor(224, 224, 224); // Gris suave
            colorTexto = Qt::gray;
        }

        // Si la fila no está seleccionada, aplicar el color personalizado de fondo y texto
        if (!(option.state & QStyle::State_Selected)) {
            opt.backgroundBrush = QBrush(colorFondo);
            opt.palette.setColor(QPalette::Text, colorTexto);
            opt.palette.setColor(QPalette::WindowText, colorTexto);
        }

        QStyledItemDelegate::paint(painter, opt, index);
    }
};


GestorEncargosDialog::GestorEncargosDialog(QString autoFilterCliente, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GestorEncargosDialog),
    modelEncargos(nullptr)
{
    qDebug() << "GestorEncargosDialog: Iniciando constructor...";
    ui->setupUi(this);

    if (!conf) {
        qCritical() << "GestorEncargosDialog: ERROR - 'conf' es NULL";
        return;
    }

    // Configurar ComboBox de estados con la opción por defecto
    ui->comboBoxEstado->insertItem(0, "Pendientes (Todos)");
    ui->comboBoxEstado->setCurrentIndex(0);

    modelEncargos = new QSqlQueryModel(this);
    ui->tableViewEncargos->setModel(modelEncargos);
    
    // Aplicar el delegado de colores
    ui->tableViewEncargos->setItemDelegate(new EncargosDelegate(this));
    ui->tableViewEncargos->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    // Estilo mejorado para el botón de cobrar
    if (ui->btnCobrarTPV) {
        ui->btnCobrarTPV->setStyleSheet("QPushButton { "
                                        "background-color: #2e7d32; "
                                        "color: white; "
                                        "font-weight: bold; "
                                        "font-size: 14px; "
                                        "padding: 10px; "
                                        "border-radius: 5px; "
                                        "min-width: 150px; "
                                        "} "
                                        "QPushButton:hover { background-color: #1b5e20; }");
    }

    // Si se pasa un cliente desde la ficha, lo ponemos en el filtro
    if (!autoFilterCliente.isEmpty() && ui->lineEditFiltroCliente) {
        ui->lineEditFiltroCliente->setText(autoFilterCliente);
    }

    qDebug() << "GestorEncargosDialog: Realizando filtrado inicial...";
    ajustarFiltro();
    aplicarPermisos();
}

GestorEncargosDialog::~GestorEncargosDialog()
{
    delete ui;
}

/**
 * @brief Aplica permisos a las acciones del gestor de encargos.
 */
void GestorEncargosDialog::aplicarPermisos() {
    if (!conf || !conf->permisos())
        return;

    ui->btnNuevoEncargo->setEnabled(conf->permisos()->tiene("encargos.crear"));
    ui->btnMarcarRecibido->setEnabled(conf->permisos()->tiene("encargos.modificar"));
    ui->btnMarcarEntregado->setEnabled(conf->permisos()->tiene("encargos.modificar"));
    ui->btnBorrar->setEnabled(conf->permisos()->tiene("encargos.borrar"));
    ui->btnCobrarTPV->setEnabled(conf->permisos()->tiene("encargos.cobrar"));
    ui->btnImprimirTicket->setEnabled(conf->permisos()->tiene("encargos.imprimir"));
}

void GestorEncargosDialog::ajustarFiltro()
{
    if (!ui || !modelEncargos || !conf) {
        qWarning() << "GestorEncargosDialog::ajustarFiltro: Objetos no inicializados";
        return;
    }

    QString estado = ui->comboBoxEstado->currentText();
    QString cliente = ui->lineEditFiltroCliente->text();

    QString queryStr = "SELECT e.id_encargo, e.id_cliente, e.cod_articulo, a.descripcion, e.cantidad, e.fecha_encargo, e.notas, e.empleado, e.anticipo, e.forma_pago, e.estado "
                       "FROM encargos e LEFT JOIN articulos a ON e.cod_articulo = a.cod";
    
    QStringList where;
    QStringList binds;
    if (estado == "Pendientes (Todos)") {
        where << "e.estado != 'Entregado'";
    } else if (estado != "Todos") {
        where << "e.estado = ?";
        binds << estado;
    }

    if (!cliente.isEmpty()) {
        where << "(e.id_cliente LIKE ? "
                 "OR e.id_cliente IN (SELECT idCliente FROM clientes WHERE nombre LIKE ? OR apellidos LIKE ?) "
                 "OR e.cod_articulo LIKE ? "
                 "OR a.descripcion LIKE ?)";
        QString patron = "%" + cliente + "%";
        for (int i = 0; i < 5; ++i) binds << patron;
    }

    if (!where.isEmpty()) {
        queryStr += " WHERE " + where.join(" AND ");
    }
    
    queryStr += " ORDER BY e.fecha_encargo DESC";

    QString connLocal = conf->getConexionLocal();
    if (connLocal.isEmpty()) {
        qWarning() << "GestorEncargosDialog::ajustarFiltro: Conexión local vacía";
        return;
    }

    QSqlQuery q(QSqlDatabase::database(connLocal));
    q.prepare(queryStr);
    for (int i = 0; i < binds.size(); ++i) {
        q.bindValue(i, binds.at(i));
    }
    if (!q.exec()) {
        qWarning() << "GestorEncargosDialog::ajustarFiltro:" << q.lastError().text();
    }
    modelEncargos->setQuery(q);
    
    // Configuración de las cabeceras de la tabla
    modelEncargos->setHeaderData(0, Qt::Horizontal, tr("ID"));
    modelEncargos->setHeaderData(1, Qt::Horizontal, tr("ID Cliente"));
    modelEncargos->setHeaderData(2, Qt::Horizontal, tr("Código"));
    modelEncargos->setHeaderData(3, Qt::Horizontal, tr("Descripción"));
    modelEncargos->setHeaderData(4, Qt::Horizontal, tr("Cant."));
    modelEncargos->setHeaderData(5, Qt::Horizontal, tr("Fecha"));
    modelEncargos->setHeaderData(6, Qt::Horizontal, tr("Notas"));
    modelEncargos->setHeaderData(7, Qt::Horizontal, tr("Empleado"));
    modelEncargos->setHeaderData(8, Qt::Horizontal, tr("Anticipo"));
    modelEncargos->setHeaderData(9, Qt::Horizontal, tr("Forma Pago"));
    modelEncargos->setHeaderData(10, Qt::Horizontal, tr("Estado"));

    ui->tableViewEncargos->resizeColumnsToContents();
    ui->tableViewEncargos->hideColumn(0);
}

void GestorEncargosDialog::on_comboBoxEstado_currentTextChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ajustarFiltro();
}

void GestorEncargosDialog::on_lineEditFiltroCliente_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ajustarFiltro();
}

void GestorEncargosDialog::on_btnMarcarRecibido_clicked()
{
    QModelIndex index = ui->tableViewEncargos->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Aviso", "Seleccione un encargo primero.");
        return;
    }
    
    int row = index.row();
    int idEncargo = modelEncargos->data(modelEncargos->index(row, 0)).toInt();
    
    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    q.prepare("UPDATE encargos SET estado = 'Recibido' WHERE id_encargo = ?");
    q.bindValue(0, idEncargo);
    
    if (q.exec()) {
        ajustarFiltro();
    } else {
        QMessageBox::critical(this, "Error", "No se pudo actualizar el estado.");
    }
}

void GestorEncargosDialog::on_btnMarcarEntregado_clicked()
{
    QModelIndex index = ui->tableViewEncargos->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Aviso", "Seleccione un encargo primero.");
        return;
    }
    
    int row = index.row();
    int idEncargo = modelEncargos->data(modelEncargos->index(row, 0)).toInt();
    
    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    q.prepare("UPDATE encargos SET estado = 'Entregado' WHERE id_encargo = ?");
    q.bindValue(0, idEncargo);
    
    if (q.exec()) {
        // Eliminar la nota del encargo entregado
        QSqlQuery qNota(QSqlDatabase::database(conf->getConexionLocal()));
        qNota.prepare("DELETE FROM notas WHERE titulo LIKE ? OR descripcion LIKE ?");
        QString patronNota = QString("%Encargo #%1%").arg(idEncargo);
        qNota.bindValue(0, patronNota);
        qNota.bindValue(1, patronNota);
        qNota.exec();

        ajustarFiltro();
    } else {
        QMessageBox::critical(this, "Error", "No se pudo actualizar el estado.");
    }
}

void GestorEncargosDialog::on_btnBorrar_clicked()
{
    QModelIndex index = ui->tableViewEncargos->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Aviso", "Seleccione un encargo primero.");
        return;
    }

    int row = index.row();
    int idEncargo = modelEncargos->data(modelEncargos->index(row, 0)).toInt();
    QString codCliente = modelEncargos->data(modelEncargos->index(row, 1)).toString();
    QString codArticulo = modelEncargos->data(modelEncargos->index(row, 2)).toString();
    double anticipo = modelEncargos->data(modelEncargos->index(row, 8)).toDouble();
    QString formaPagoOriginal = modelEncargos->data(modelEncargos->index(row, 9)).toString();
    QString estadoActual = modelEncargos->data(modelEncargos->index(row, 10)).toString();

    if (estadoActual == "Cancelado") {
        QMessageBox::information(this, "Aviso", "Este encargo ya se encuentra cancelado.");
        return;
    }

    if (QMessageBox::question(this, "Cancelar Encargo", QString("¿Está seguro de que desea cancelar el encargo #%1?").arg(idEncargo), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    baseDatos base;
    QString usuario = conf->getUsuario();
    if (usuario.isEmpty()) usuario = "Sistema";

    QString formaPagoDevolucion = "";
    double anticipoDevuelto = 0.0;
    bool registrarDevolucion = false;

    // Si el encargo tenía un anticipo cobrado previamente, preguntar la forma de devolución
    if (anticipo > 0) {
        DialogDevolucionAnticipo devDialog(anticipo, formaPagoOriginal, this);
        if (devDialog.exec() == QDialog::Accepted) {
            if (devDialog.getDevolverAnticipo()) {
                registrarDevolucion = true;
                formaPagoDevolucion = devDialog.getFormaPago();
                anticipoDevuelto = anticipo;
                bool esEfectivo = base.esFormaPagoEfectivo(formaPagoDevolucion, conf->getConexionLocal());

                if (esEfectivo) {
                    // Registrar salida de caja por la devolución en efectivo
                    QStringList datosES;
                    datosES.append(QDate::currentDate().toString("yyyy-MM-dd"));
                    datosES.append(QTime::currentTime().toString("hh:mm:ss"));
                    datosES.append(QString::number(-anticipo)); // Importe negativo para restar en salidas/cajón
                    datosES.append("2"); // Motivo salida de caja
                    datosES.append(QString("Devolución Anticipo Encargo #%1 (Efectivo) [Cliente %2]").arg(QString::number(idEncargo), codCliente));
                    
                    base.insertarES(datosES, conf->getConexionLocal(), usuario);
                    QMessageBox::information(this, "Devolución", QString("Se ha registrado la devolución de %1 € en efectivo en la caja.").arg(QString::number(anticipo, 'f', 2)));
                } else {
                    QMessageBox::information(this, "Devolución", QString("Se ha marcado la devolución del anticipo (%1 €) mediante %2.").arg(QString::number(anticipo, 'f', 2), formaPagoDevolucion));
                }
            }
        } else {
            // Cancelado por el usuario en el diálogo de devolución
            return;
        }
    }

    // Actualizar el estado del encargo a 'Cancelado' registrando la fecha y forma de devolución si la hubo
    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    if (registrarDevolucion) {
        q.prepare("UPDATE encargos SET estado = 'Cancelado', dev_fecha = CURRENT_TIMESTAMP, dev_forma_pago = ?, dev_anticipo = ? WHERE id_encargo = ?");
        q.bindValue(0, formaPagoDevolucion);
        q.bindValue(1, anticipoDevuelto);
        q.bindValue(2, idEncargo);
    } else {
        q.prepare("UPDATE encargos SET estado = 'Cancelado' WHERE id_encargo = ?");
        q.bindValue(0, idEncargo);
    }

    if (q.exec()) {
        base.crearNota(conf->getConexionLocal(),
                       QString("Encargo Cancelado #%1").arg(idEncargo),
                       QString("Encargo #%1 cancelado. Cliente: %2. Anticipo: %3 €").arg(QString::number(idEncargo), codCliente, QString::number(anticipo, 'f', 2)),
                       usuario,
                       "",
                       "Baja");
        ajustarFiltro();
    } else {
        QMessageBox::critical(this, "Error", "No se pudo cambiar el estado del encargo a Cancelado:\n" + q.lastError().text());
    }
}

void GestorEncargosDialog::on_btnCerrar_clicked()
{
    close();
}

void GestorEncargosDialog::on_btnNuevoEncargo_clicked()
{
    QString usuario = conf->getUsuario();
    if (usuario.isEmpty()) usuario = "Sistema";
    
    EncargosDialog enc("", "", "", "", usuario, this);
    if (enc.exec() == QDialog::Accepted) {
        QList<LineaEncargo> lineas = enc.getLineas();
        if (lineas.isEmpty() && enc.getCodArticulo().isEmpty()) {
            QMessageBox::warning(this, "Aviso", "Debe incluir al menos un producto para crear el encargo.");
            return;
        }

        if (enc.getCodCliente().isEmpty()) {
            QMessageBox::warning(this, "Aviso", "Falta seleccionar el cliente.");
            return;
        }

        baseDatos base;
        QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
        q.prepare("INSERT INTO encargos (id_cliente, cod_articulo, cantidad, notas, empleado, anticipo, forma_pago, estado) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, 'Pendiente')");
        q.bindValue(0, enc.getCodCliente().toInt());
        q.bindValue(1, enc.getCodArticulo());
        q.bindValue(2, enc.getCantidad());
        q.bindValue(3, enc.getNotas());
        q.bindValue(4, usuario);
        q.bindValue(5, enc.getAnticipo());
        q.bindValue(6, enc.getFormaPago());

        if (q.exec()) {
            int idNuevoEncargo = q.lastInsertId().toInt();

            // Insertar líneas de productos en encargos_lineas
            QSqlQuery qLin(QSqlDatabase::database(conf->getConexionLocal()));
            qLin.prepare("INSERT INTO encargos_lineas (id_encargo, cod_articulo, descripcion, cantidad, pvp) VALUES (?, ?, ?, ?, ?)");
            for (const LineaEncargo &l : lineas) {
                qLin.bindValue(0, idNuevoEncargo);
                qLin.bindValue(1, l.codArticulo);
                qLin.bindValue(2, l.descripcion);
                qLin.bindValue(3, l.cantidad);
                qLin.bindValue(4, l.pvp);
                qLin.exec();
            }

            ajustarFiltro();
            base.crearNota(conf->getConexionLocal(),
                           QString("Encargo #%1: %2").arg(QString::number(idNuevoEncargo), enc.getDescArticulo()),
                           QString("Encargo #%1\nCliente ID: %2\nProductos: %3\nForma Pago Anticipo: %4\nNotas: %5").arg(QString::number(idNuevoEncargo), enc.getCodCliente(), QString::number(lineas.size()), enc.getFormaPago(), enc.getNotas()),
                           usuario,
                           "",
                           "Alta");

            // Si se introdujo un anticipo mayor a 0, registrar el movimiento en el cajón de efectivo únicamente si es en EFECTIVO
            if (enc.getAnticipo() > 0) {
                bool esEfectivo = base.esFormaPagoEfectivo(enc.getFormaPago(), conf->getConexionLocal());
                if (esEfectivo) {
                    QStringList datosES;
                    datosES.append(QDate::currentDate().toString("yyyy-MM-dd"));
                    datosES.append(QTime::currentTime().toString("hh:mm:ss"));
                    datosES.append(QString::number(enc.getAnticipo()));
                    datosES.append("1");
                    datosES.append(QString("Anticipo Encargo (Efectivo) [Cliente %1] - %2").arg(enc.getCodCliente(), enc.getCodArticulo()));
                    base.insertarES(datosES, conf->getConexionLocal(), usuario);
                }
            }

            // Si el usuario marcó la opción de imprimir comprobante por la impresora de tickets
            if (enc.getImprimirTicket()) {
                ImprimirTicket::imprimirComprobanteEncargo(idNuevoEncargo,
                                                          enc.getCodCliente(),
                                                          enc.getCodArticulo(),
                                                          enc.getDescArticulo(),
                                                          enc.getCantidad(),
                                                          enc.getAnticipo(),
                                                          enc.getFormaPago(),
                                                          enc.getNotas(),
                                                          usuario);
            }
        } else {
            QMessageBox::critical(this, "Error", "No se pudo guardar el encargo:\n" + q.lastError().text());
        }
    }
}

/**
 * @brief Slot para reimprimir el comprobante del encargo seleccionado en la tabla.
 */
void GestorEncargosDialog::on_btnImprimirTicket_clicked()
{
    QModelIndex index = ui->tableViewEncargos->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Aviso", "Seleccione un encargo de la lista primero.");
        return;
    }

    int row = index.row();
    int idEncargo = modelEncargos->data(modelEncargos->index(row, 0)).toInt();
    QString codCliente = modelEncargos->data(modelEncargos->index(row, 1)).toString();
    QString codArticulo = modelEncargos->data(modelEncargos->index(row, 2)).toString();
    QString descArticulo = modelEncargos->data(modelEncargos->index(row, 3)).toString();
    int cantidad = modelEncargos->data(modelEncargos->index(row, 4)).toInt();
    QString fechaEncargo = modelEncargos->data(modelEncargos->index(row, 5)).toString();
    QString notas = modelEncargos->data(modelEncargos->index(row, 6)).toString();
    QString empleado = modelEncargos->data(modelEncargos->index(row, 7)).toString();
    double anticipo = modelEncargos->data(modelEncargos->index(row, 8)).toDouble();
    QString formaPago = modelEncargos->data(modelEncargos->index(row, 9)).toString();

    bool ok = ImprimirTicket::imprimirComprobanteEncargo(idEncargo,
                                                       codCliente,
                                                       codArticulo,
                                                       descArticulo,
                                                       cantidad,
                                                       anticipo,
                                                       formaPago,
                                                       notas,
                                                       empleado,
                                                       fechaEncargo);
    if (!ok) {
        QMessageBox::warning(this, "Error de Impresión", "No se pudo imprimir el comprobante del encargo. Verifique la conexión con la impresora.");
    }
}

void GestorEncargosDialog::on_btnCobrarTPV_clicked()
{
    QModelIndex index = ui->tableViewEncargos->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Aviso", "Seleccione un encargo primero.");
        return;
    }

    int row = index.row();
    // El campo 'estado' se encuentra ahora en el índice 10
    QString estado = modelEncargos->data(modelEncargos->index(row, 10)).toString();

    if (estado == "Entregado" || estado == "Cancelado") {
        QMessageBox::warning(this, "Aviso", "Este encargo ya ha sido procesado o cancelado.");
        return;
    }

    Tpv *tpv = nullptr;
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        Tpv *candidate = qobject_cast<Tpv *>(widget);
        if (candidate && candidate->isVisible()) {
            tpv = candidate;
            break;
        }
    }

    if (!tpv) {
        QMessageBox::warning(this, "TPV no detectado", "Debe tener el TPV abierto y visible para cobrar el encargo.");
        return;
    }

    int idEncargo = modelEncargos->data(modelEncargos->index(row, 0)).toInt();
    QString codArticulo = modelEncargos->data(modelEncargos->index(row, 2)).toString();
    int cantidad = modelEncargos->data(modelEncargos->index(row, 4)).toInt();
    double anticipo = modelEncargos->data(modelEncargos->index(row, 8)).toDouble();

    if (tpv->cargarEncargoConId(codArticulo, anticipo, cantidad, idEncargo)) {
        tpv->raise();
        tpv->activateWindow();
        QMessageBox::information(this, "Éxito", "Encargo enviado al TPV.\n\nLa parrilla de ventas se ha actualizado.");
        ajustarFiltro();
    } else {
        QMessageBox::critical(this, "Error", "No se pudo cargar el artículo en el TPV. Verifique que el código existe.");
    }
}
