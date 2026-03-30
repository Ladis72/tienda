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
    ui(new Ui::GestorEncargosDialog)
{
    ui->setupUi(this);

    modelEncargos = new QSqlQueryModel(this);
    ui->tableViewEncargos->setModel(modelEncargos);
    
    // Aplicar el delegado de colores
    ui->tableViewEncargos->setItemDelegate(new EncargosDelegate(this));
    ui->tableViewEncargos->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    ajustarFiltro();
}

GestorEncargosDialog::~GestorEncargosDialog()
{
    delete ui;
}

void GestorEncargosDialog::ajustarFiltro()
{
    QString estado = ui->comboBoxEstado->currentText();
    QString cliente = ui->lineEditFiltroCliente->text();

    QString queryStr = "SELECT e.id_encargo, e.id_cliente, e.cod_articulo, a.descripcion, e.cantidad, e.fecha_encargo, e.notas, e.empleado, e.anticipo, e.estado "
                       "FROM encargos e LEFT JOIN articulos a ON e.cod_articulo = a.cod";
    
    QString where;
    if (estado != "Todos") {
        where = "e.estado = '" + estado + "'";
    }

    if (!cliente.isEmpty()) {
        if (!where.isEmpty()) where += " AND ";
        where += "(e.id_cliente LIKE '%" + cliente + "%' \
                  OR e.id_cliente IN (SELECT idCliente FROM clientes WHERE nombre LIKE '%" + cliente + "%' OR apellidos LIKE '%" + cliente + "%') \
                  OR e.cod_articulo LIKE '%" + cliente + "%' \
                  OR a.descripcion LIKE '%" + cliente + "%')";
    }

    if (!where.isEmpty()) {
        queryStr += " WHERE " + where;
    }
    
    queryStr += " ORDER BY e.fecha_encargo DESC";

    modelEncargos->setQuery(queryStr, QSqlDatabase::database(conf->getConexionLocal()));
    
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
    // Ignoramos arg1 porque leeremos todo desde ajustarFiltro()
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
    int idEncargo = modelEncargos->data(modelEncargos->index(row, 0)).toInt(); // assuming id is at column 0
    
    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    q.prepare("UPDATE encargos SET estado = 'Recibido' WHERE id_encargo = ?");
    q.bindValue(0, idEncargo);
    
    if (q.exec()) {
        ajustarFiltro(); // refresh
    } else {
        QMessageBox::critical(this, "Error", "No se pudo actualizar el estado:\n" + q.lastError().text());
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
    int idEncargo = modelEncargos->data(modelEncargos->index(row, 0)).toInt(); // assuming id is at column 0
    
    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    q.prepare("UPDATE encargos SET estado = 'Entregado' WHERE id_encargo = ?");
    q.bindValue(0, idEncargo);
    
    if (q.exec()) {
        ajustarFiltro();
    } else {
        QMessageBox::critical(this, "Error", "No se pudo actualizar el estado:\n" + q.lastError().text());
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
        int idEncargo = modelEncargos->data(modelEncargos->index(row, 0)).toInt(); // assuming id is at column 0
        
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
    // Mostrar Dialog para un nuevo encargo, con usuario de la app
    // Assuming the main window or configuracion has the user info, if not just "Sistema"
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
        // We need id_cliente, not just cod_cliente as text.
        // Or if id_cliente is a string/int. Let's assume cod_cliente maps to id_cliente.
        q.bindValue(0, enc.getCodCliente().toInt());
        q.bindValue(1, enc.getCodArticulo());
        q.bindValue(2, enc.getCantidad());
        q.bindValue(3, enc.getNotas());
        q.bindValue(4, usuario);
        q.bindValue(5, enc.getAnticipo());

        if (q.exec()) {
            QMessageBox::information(this, "Éxito", "Encargo creado y guardado.");
            ajustarFiltro();
            
            // Si hay anticipo, lo registramos en caja diaria si es necesario
            double anticipo = enc.getAnticipo();
            if (anticipo > 0) {
                QStringList datosES;
                datosES << QDate::currentDate().toString("yyyy-MM-dd");
                datosES << QTime::currentTime().toString("hh:mm:ss");
                datosES << QString::number(anticipo, 'f', 2);
                datosES << "Anticipo";
                datosES << "Anticipo Encargo Art: " + enc.getCodArticulo();
                base.insertarES(datosES, conf->getConexionLocal());
            }

            base.crearNota(conf->getConexionLocal(),
                           "Nuevo Encargo: " + enc.getCodArticulo() + " (" + enc.getDescArticulo() + ")",
                           "Cliente ID: " + enc.getCodCliente() + "\nCantidad: " + QString::number(enc.getCantidad()) + "\nNotas: " + enc.getNotas(),
                           usuario,
                           "",
                           "Alta");
        } else {
            QMessageBox::critical(this, "Error", "No se pudo guardar el encargo:\n" + q.lastError().text());
        }
    }
}
