#include "gestorencargosdialog.h"
#include "ui_gestorencargosdialog.h"
#include "encargosdialog.h"
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



// Delegado para colorear las filas según el estado del encargo
class EncargosDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        // El estado está en la columna 9 según la consulta SQL en ajustarFiltro()
        QString estado = index.model()->data(index.model()->index(index.row(), 9)).toString();

        QColor colorFondo;
        QColor colorTexto = Qt::black;

        if (estado == "Pendiente") {
            colorFondo = QColor(255, 205, 210); // Rojo suave
        } else if (estado == "Recibido") {
            colorFondo = QColor(255, 245, 157); // Amarillo suave
        } else if (estado == "Entregado") {
            colorFondo = QColor(197, 225, 165); // Verde suave
        } else if (estado == "Cancelado") {
            colorFondo = QColor(224, 224, 224); // Gris suave
            colorTexto = Qt::gray;
        }

        painter->save();
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setPen(option.palette.highlightedText().color());
        } else {
            painter->fillRect(option.rect, colorFondo);
            painter->setPen(colorTexto);
        }
        
        // Dibujar el texto centrado verticalmente con un pequeño margen
        QString text = opt.text;
        QRect textRect = option.rect.adjusted(5, 0, -5, 0);
        painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);
        painter->restore();
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
}

GestorEncargosDialog::~GestorEncargosDialog()
{
    delete ui;
}

void GestorEncargosDialog::ajustarFiltro()
{
    if (!ui || !modelEncargos || !conf) {
        qWarning() << "GestorEncargosDialog::ajustarFiltro: Objetos no inicializados";
        return;
    }

    QString estado = ui->comboBoxEstado->currentText();
    QString cliente = ui->lineEditFiltroCliente->text();

    QString queryStr = "SELECT e.id_encargo, e.id_cliente, e.cod_articulo, a.descripcion, e.cantidad, e.fecha_encargo, e.notas, e.empleado, e.anticipo, e.estado "
                       "FROM encargos e LEFT JOIN articulos a ON e.cod_articulo = a.cod";
    
    QString where;
    if (estado == "Pendientes (Todos)") {
        where = "e.estado != 'Entregado'";
    } else if (estado != "Todos") {
        where = "e.estado = '" + estado + "'";
    }

    if (!cliente.isEmpty()) {
        if (!where.isEmpty()) where += " AND ";
        where += "(e.id_cliente LIKE '%" + cliente + "%' "
                  "OR e.id_cliente IN (SELECT idCliente FROM clientes WHERE nombre LIKE '%" + cliente + "%' OR apellidos LIKE '%" + cliente + "%') "
                  "OR e.cod_articulo LIKE '%" + cliente + "%' "
                  "OR a.descripcion LIKE '%" + cliente + "%')";
    }

    if (!where.isEmpty()) {
        queryStr += " WHERE " + where;
    }
    
    queryStr += " ORDER BY e.fecha_encargo DESC";

    QString connLocal = conf->getConexionLocal();
    if (connLocal.isEmpty()) {
        qWarning() << "GestorEncargosDialog::ajustarFiltro: Conexión local vacía";
        return;
    }

    modelEncargos->setQuery(queryStr, QSqlDatabase::database(connLocal));
    
    // Cabeceras
    modelEncargos->setHeaderData(0, Qt::Horizontal, tr("ID"));
    modelEncargos->setHeaderData(1, Qt::Horizontal, tr("ID Cliente"));
    modelEncargos->setHeaderData(2, Qt::Horizontal, tr("Código"));
    modelEncargos->setHeaderData(3, Qt::Horizontal, tr("Descripción"));
    modelEncargos->setHeaderData(4, Qt::Horizontal, tr("Cant."));
    modelEncargos->setHeaderData(5, Qt::Horizontal, tr("Fecha"));
    modelEncargos->setHeaderData(6, Qt::Horizontal, tr("Notas"));
    modelEncargos->setHeaderData(7, Qt::Horizontal, tr("Empleado"));
    modelEncargos->setHeaderData(8, Qt::Horizontal, tr("Anticipo"));
    modelEncargos->setHeaderData(9, Qt::Horizontal, tr("Estado"));

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
    
    if (QMessageBox::question(this, "Borrar", "¿Está seguro de que desea borrar este encargo? Esto no devolverá el anticipo a la caja automáticamente.", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        int row = index.row();
        int idEncargo = modelEncargos->data(modelEncargos->index(row, 0)).toInt();
        
        QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
        q.prepare("DELETE FROM encargos WHERE id_encargo = ?");
        q.bindValue(0, idEncargo);
        
        if (q.exec()) {
            ajustarFiltro();
        } else {
            QMessageBox::critical(this, "Error", "No se pudo borrar el encargo:\n" + q.lastError().text());
        }
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
        if (enc.getCantidad() <= 0 || enc.getCodArticulo().isEmpty() || enc.getCodCliente().isEmpty()) {
            QMessageBox::warning(this, "Aviso", "Datos incompletos para crear el encargo.");
            return;
        }

        baseDatos base;
        QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
        q.prepare("INSERT INTO encargos (id_cliente, cod_articulo, cantidad, notas, empleado, anticipo, estado) "
                  "VALUES (?, ?, ?, ?, ?, ?, 'Pendiente')");
        q.bindValue(0, enc.getCodCliente().toInt());
        q.bindValue(1, enc.getCodArticulo());
        q.bindValue(2, enc.getCantidad());
        q.bindValue(3, enc.getNotas());
        q.bindValue(4, usuario);
        q.bindValue(5, enc.getAnticipo());

        if (q.exec()) {
            ajustarFiltro();
            base.crearNota(conf->getConexionLocal(),
                           "Encargo: " + enc.getDescArticulo() + "(" + enc.getCodArticulo() +")",
                           "Cliente ID: " + enc.getCodCliente() + "\nCantidad: " + QString::number(enc.getCantidad()) + "\nNotas: " + enc.getNotas(),
                           usuario,
                           "",
                           "Alta");
        } else {
            QMessageBox::critical(this, "Error", "No se pudo guardar el encargo:\n" + q.lastError().text());
        }
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
    QString estado = modelEncargos->data(modelEncargos->index(row, 9)).toString();

    if (estado == "Entregado" || estado == "Cancelado") {
        QMessageBox::warning(this, "Aviso", "Este encargo ya ha sido procesado o cancelado.");
        return;
    }

    Tpv *tpv = nullptr;
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        tpv = qobject_cast<Tpv *>(widget);
        if (tpv) break;
    }

    if (!tpv) {
        QMessageBox::warning(this, "TPV no detectado", "Debe tener el TPV abierto para cobrar el encargo.");
        return;
    }

    int idEncargo = modelEncargos->data(modelEncargos->index(row, 0)).toInt();
    QString codArticulo = modelEncargos->data(modelEncargos->index(row, 2)).toString();
    int cantidad = modelEncargos->data(modelEncargos->index(row, 4)).toInt();
    double anticipo = modelEncargos->data(modelEncargos->index(row, 8)).toDouble();

    if (tpv->cargarEncargoConId(codArticulo, anticipo, cantidad, idEncargo)) {
        QMessageBox::information(this, "Éxito", "Encargo enviado al TPV.\n\nEl estado se actualizará a 'Entregado' automáticamente al cobrar el ticket.");
        ajustarFiltro();
    } else {
        QMessageBox::critical(this, "Error", "No se pudo cargar el artículo en el TPV. Verifique que el código existe.");
    }
}
