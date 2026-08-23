#include "dialogconocimientoia.h"
#include "ui_dialogconocimientoia.h"
#include "configuracion.h"
#include "syncmanager.h"
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QTextEdit>

extern Configuracion *conf;

// Lista de áreas y productos de salud por defecto
struct ReglaDefault {
    const char *categoria;
    const char *detonantes;
    const char *terminos_clave;
};

static const ReglaDefault REGLAS_POR_DEFECTO[] = {
    {
        "Articulaciones / Huesos / Dolor / Cartílagos",
        "articul, hueso, dolor, rodilla, espalda, lumbar, artrosis, artritis, cartilag, reuma, tendon, bursitis, tendinitis, fascitis, ciatica, esguince",
        "colamag, curcurina, artripol, cannalges, colagen, curcuma, harpagofit, glucosamin, msm, membrana, silici, magnesi"
    },
    {
        "Colesterol / Salud Cardiovascular / Tensión",
        "colesterol, triglicerid, lipido, cardio, corazon, arterial, tension",
        "colestia, lecidol, ometrix, nivelcol, cardiepa, monacolina, levadura, arroz rojo, bergamota, omega, fitosterol, ajo negro"
    },
    {
        "Digestión / Gases / Hígado / Tránsito / Detox",
        "digest, pesadez, gas, vientre, hinchazon, higado, hepatic, vesicula, reflujo, ardor, acidez, estomago, transito, estrenimient",
        "carbomag, despatic, drenadiet, alivia tus gases, como un reloj, vientre plano, alcachofa, cardo mariano, desmodium, boldo, hinojo, probiotico, carbon vegetal, plantago"
    },
    {
        "Sistema Inmune / Defensas / Garganta / Tos / Resfriados",
        "resfriad, gripe, tos, garganta, mucus, mucosidad, congestion, defensa, inmune, inmunidad, catarro, afon",
        "propol, echinacea, equinacea, tomillo, drosera, malvavisco, eucalipt, llanten, vitamina c, reishi, inmuno, bronpul"
    },
    {
        "Sueño / Insomnio / Nervios / Ansiedad / Estrés / Ánimo",
        "dormir, insomni, sueno, nervio, ansiedad, estres, relaj, depres, animo",
        "sedaner, nervaplant, melatonina, valeriana, pasiflora, amapola, ashwagandha, triptofano, gaba, melisa, hiperico"
    },
    {
        "Circulación / Piernas cansadas / Varices / Retención",
        "circulac, piernas cansadas, pesadez piernas, varices, hemorroid, retencion, celulit",
        "circular, flebodiet, ginkgo, castaño de indias, vid roja, rusco, hamamelis, centella, cola caballo, drenadiet"
    },
    {
        "Vías Urinarias / Cistitis / Próstata",
        "cistitis, orina, urinari, prostata, infeccion orina, arandano",
        "arandano rojo, cranberry, d-manosa, gayuba, sabal, epilobio, calabaza"
    },
    {
        "Control de Peso / Saciedad / Quemagrasas",
        "adelgazar, peso, grasa, quemagrasa, sacian, dieta, retencion liquidos",
        "drenadiet, controla tu peso, carnitina, glucomanano, garcinia, te verde, fucus, chitosan"
    },
    {
        "Memoria / Rendimiento / Cansancio / Vitalidad",
        "memoria, estudio, concentrac, cansanci, fatiga, agotamient, energia, vitalidad",
        "aktiner, jalea, ginseng, eleuterococo, bacopa, fosfatidilserina, coenzima q10, guarana, maca"
    },
    {
        "Menopausia / Salud Femenina / Sofocos",
        "menopaus, sofocos, regla, menstrua, ciclo, hormon",
        "isoflavonas, cimicifuga, onagra, salvia, sauzgatillo, probidiet intimo"
    }
};

/**
 * @brief Constructor del diálogo DialogConocimientoIA.
 */
DialogConocimientoIA::DialogConocimientoIA(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogConocimientoIA)
{
    ui->setupUi(this);
    asegurarTablaBaseDatos();
    configurarTabla();
    cargarDatos();
}

/**
 * @brief Destructor del diálogo.
 */
DialogConocimientoIA::~DialogConocimientoIA()
{
    delete ui;
}

/**
 * @brief Obtiene la conexión activa disponible.
 */
static QSqlDatabase obtenerBdConocimiento()
{
    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        return QSqlDatabase::database(SyncManager::CONEXION_NUBE);
    }
    QString connLocal = conf ? conf->getConexionLocal() : "DB";
    if (connLocal.isEmpty()) connLocal = "DB";
    if (QSqlDatabase::contains(connLocal)) {
        return QSqlDatabase::database(connLocal);
    }
    return QSqlDatabase::database();
}

/**
 * @brief Asegura la existencia de la tabla ia_conocimiento y siembra los valores predeterminados.
 */
void DialogConocimientoIA::asegurarTablaBaseDatos()
{
    QSqlDatabase db = obtenerBdConocimiento();
    if (!db.isOpen()) return;

    QSqlQuery q(db);
    QString createSql =
        "CREATE TABLE IF NOT EXISTS `ia_conocimiento` ("
        "  `id` INT AUTO_INCREMENT PRIMARY KEY,"
        "  `categoria` VARCHAR(120) NOT NULL,"
        "  `detonantes` TEXT NOT NULL,"
        "  `terminos_clave` TEXT NOT NULL,"
        "  `activo` TINYINT DEFAULT 1,"
        "  `updated_at` DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;";

    // En SQLite (si no es MySQL):
    if (db.driverName().contains("SQLITE", Qt::CaseInsensitive)) {
        createSql =
            "CREATE TABLE IF NOT EXISTS ia_conocimiento ("
            "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  categoria TEXT NOT NULL,"
            "  detonantes TEXT NOT NULL,"
            "  terminos_clave TEXT NOT NULL,"
            "  activo INTEGER DEFAULT 1,"
            "  updated_at TEXT"
            ");";
    }

    q.exec(createSql);

    // Comprobar si tiene registros
    q.exec("SELECT COUNT(*) FROM ia_conocimiento");
    if (q.next() && q.value(0).toInt() == 0) {
        // Sembrar valores por defecto
        for (const ReglaDefault &regla : REGLAS_POR_DEFECTO) {
            QSqlQuery ins(db);
            ins.prepare("INSERT INTO ia_conocimiento (categoria, detonantes, terminos_clave, activo) "
                        "VALUES (:cat, :det, :term, 1)");
            ins.bindValue(":cat", QString::fromUtf8(regla.categoria));
            ins.bindValue(":det", QString::fromUtf8(regla.detonantes));
            ins.bindValue(":term", QString::fromUtf8(regla.terminos_clave));
            ins.exec();
        }
    }
}

/**
 * @brief Configura las columnas y aspecto de la tabla de reglas.
 */
void DialogConocimientoIA::configurarTabla()
{
    ui->tableWidgetReglas->setColumnCount(5);
    QStringList headers = {"ID", "Categoría / Dolencia", "Palabras Detonantes", "Principios Activos / Marcas", "Activo"};
    ui->tableWidgetReglas->setHorizontalHeaderLabels(headers);

    ui->tableWidgetReglas->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidgetReglas->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidgetReglas->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidgetReglas->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableWidgetReglas->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    ui->tableWidgetReglas->setColumnHidden(0, true); // Ocultar ID interno
}

/**
 * @brief Carga las reglas desde la base de datos a la tabla visual.
 */
void DialogConocimientoIA::cargarDatos()
{
    QSqlDatabase db = obtenerBdConocimiento();
    if (!db.isOpen()) return;

    ui->tableWidgetReglas->setRowCount(0);

    QSqlQuery q(db);
    q.exec("SELECT id, categoria, detonantes, terminos_clave, activo FROM ia_conocimiento ORDER BY id ASC");

    int row = 0;
    while (q.next()) {
        ui->tableWidgetReglas->insertRow(row);

        // ID
        QTableWidgetItem *itemId = new QTableWidgetItem(q.value("id").toString());
        itemId->setFlags(itemId->flags() & ~Qt::ItemIsEditable);
        ui->tableWidgetReglas->setItem(row, 0, itemId);

        // Categoría
        QTableWidgetItem *itemCat = new QTableWidgetItem(q.value("categoria").toString());
        ui->tableWidgetReglas->setItem(row, 1, itemCat);

        // Detonantes
        QTableWidgetItem *itemDet = new QTableWidgetItem(q.value("detonantes").toString());
        ui->tableWidgetReglas->setItem(row, 2, itemDet);

        // Términos clave
        QTableWidgetItem *itemTerm = new QTableWidgetItem(q.value("terminos_clave").toString());
        ui->tableWidgetReglas->setItem(row, 3, itemTerm);

        // Activo (Checkbox)
        QTableWidgetItem *itemActivo = new QTableWidgetItem();
        itemActivo->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        itemActivo->setCheckState(q.value("activo").toInt() == 1 ? Qt::Checked : Qt::Unchecked);
        itemActivo->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetReglas->setItem(row, 4, itemActivo);

        row++;
    }
}

/**
 * @brief Guarda todos los registros de la tabla visual en la base de datos.
 */
void DialogConocimientoIA::guardarDatos()
{
    QSqlDatabase db = obtenerBdConocimiento();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Error", "Base de datos no disponible para guardar cambios.");
        return;
    }

    QSqlQuery q(db);
    for (int r = 0; r < ui->tableWidgetReglas->rowCount(); ++r) {
        int id = ui->tableWidgetReglas->item(r, 0) ? ui->tableWidgetReglas->item(r, 0)->text().toInt() : 0;
        QString cat = ui->tableWidgetReglas->item(r, 1) ? ui->tableWidgetReglas->item(r, 1)->text().trimmed() : "";
        QString det = ui->tableWidgetReglas->item(r, 2) ? ui->tableWidgetReglas->item(r, 2)->text().trimmed() : "";
        QString term = ui->tableWidgetReglas->item(r, 3) ? ui->tableWidgetReglas->item(r, 3)->text().trimmed() : "";
        int activo = (ui->tableWidgetReglas->item(r, 4) && ui->tableWidgetReglas->item(r, 4)->checkState() == Qt::Checked) ? 1 : 0;

        if (cat.isEmpty()) continue;

        if (id > 0) {
            // Actualizar
            q.prepare("UPDATE ia_conocimiento SET categoria = :cat, detonantes = :det, "
                      "terminos_clave = :term, activo = :act WHERE id = :id");
            q.bindValue(":cat", cat);
            q.bindValue(":det", det);
            q.bindValue(":term", term);
            q.bindValue(":act", activo);
            q.bindValue(":id", id);
            q.exec();
        } else {
            // Insertar nuevo
            q.prepare("INSERT INTO ia_conocimiento (categoria, detonantes, terminos_clave, activo) "
                      "VALUES (:cat, :det, :term, :act)");
            q.bindValue(":cat", cat);
            q.bindValue(":det", det);
            q.bindValue(":term", term);
            q.bindValue(":act", activo);
            if (q.exec()) {
                int nuevoId = q.lastInsertId().toInt();
                if (ui->tableWidgetReglas->item(r, 0)) {
                    ui->tableWidgetReglas->item(r, 0)->setText(QString::number(nuevoId));
                }
            }
        }
    }

    emit conocimientoModificado();
}

/**
 * @brief Abre diálogo modal para crear una nueva regla de salud.
 */
void DialogConocimientoIA::on_pushButtonAnadir_clicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Añadir Nueva Regla de Salud");
    dlg.resize(550, 320);

    QFormLayout form(&dlg);
    QLineEdit editCat(&dlg);
    editCat.setPlaceholderText("Ej: Dolor de Cabeza / Migrañas");

    QTextEdit editDet(&dlg);
    editDet.setPlaceholderText("Palabras que dice el cliente (ej: cabeza, jaqueca, migrana, mareo, cefalea)");
    editDet.setMaximumHeight(80);

    QTextEdit editTerm(&dlg);
    editTerm.setPlaceholderText("Principios activos y productos recomendados (ej: ginkgo, matricaria, migrasin, magnesio, lavanda)");
    editTerm.setMaximumHeight(80);

    form.addRow("Categoría / Dolencia:", &editCat);
    form.addRow("Palabras Detonantes:", &editDet);
    form.addRow("Principios Activos y Marcas:", &editTerm);

    QDialogButtonBox bbox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form.addRow(&bbox);
    connect(&bbox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(&bbox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        QString cat = editCat.text().trimmed();
        QString det = editDet.toPlainText().trimmed();
        QString term = editTerm.toPlainText().trimmed();

        if (cat.isEmpty()) {
            QMessageBox::warning(this, "Aviso", "La categoría no puede estar vacía.");
            return;
        }

        int row = ui->tableWidgetReglas->rowCount();
        ui->tableWidgetReglas->insertRow(row);
        ui->tableWidgetReglas->setItem(row, 0, new QTableWidgetItem("0")); // ID pendiente
        ui->tableWidgetReglas->setItem(row, 1, new QTableWidgetItem(cat));
        ui->tableWidgetReglas->setItem(row, 2, new QTableWidgetItem(det));
        ui->tableWidgetReglas->setItem(row, 3, new QTableWidgetItem(term));

        QTableWidgetItem *itemActivo = new QTableWidgetItem();
        itemActivo->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        itemActivo->setCheckState(Qt::Checked);
        itemActivo->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetReglas->setItem(row, 4, itemActivo);

        guardarDatos();
    }
}

/**
 * @brief Abre diálogo modal para editar la regla seleccionada.
 */
void DialogConocimientoIA::on_pushButtonEditar_clicked()
{
    int row = ui->tableWidgetReglas->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Aviso", "Selecciona una fila para editar.");
        return;
    }

    QString cat = ui->tableWidgetReglas->item(row, 1) ? ui->tableWidgetReglas->item(row, 1)->text() : "";
    QString det = ui->tableWidgetReglas->item(row, 2) ? ui->tableWidgetReglas->item(row, 2)->text() : "";
    QString term = ui->tableWidgetReglas->item(row, 3) ? ui->tableWidgetReglas->item(row, 3)->text() : "";

    QDialog dlg(this);
    dlg.setWindowTitle("Editar Regla de Salud");
    dlg.resize(550, 320);

    QFormLayout form(&dlg);
    QLineEdit editCat(&dlg);
    editCat.setText(cat);

    QTextEdit editDet(&dlg);
    editDet.setText(det);
    editDet.setMaximumHeight(80);

    QTextEdit editTerm(&dlg);
    editTerm.setText(term);
    editTerm.setMaximumHeight(80);

    form.addRow("Categoría / Dolencia:", &editCat);
    form.addRow("Palabras Detonantes:", &editDet);
    form.addRow("Principios Activos y Marcas:", &editTerm);

    QDialogButtonBox bbox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form.addRow(&bbox);
    connect(&bbox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(&bbox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        if (ui->tableWidgetReglas->item(row, 1)) ui->tableWidgetReglas->item(row, 1)->setText(editCat.text().trimmed());
        if (ui->tableWidgetReglas->item(row, 2)) ui->tableWidgetReglas->item(row, 2)->setText(editDet.toPlainText().trimmed());
        if (ui->tableWidgetReglas->item(row, 3)) ui->tableWidgetReglas->item(row, 3)->setText(editTerm.toPlainText().trimmed());
        guardarDatos();
    }
}

/**
 * @brief Permite editar al hacer doble clic en una fila.
 */
void DialogConocimientoIA::on_tableWidgetReglas_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    ui->tableWidgetReglas->selectRow(row);
    on_pushButtonEditar_clicked();
}

/**
 * @brief Elimina la regla seleccionada de la base de datos.
 */
void DialogConocimientoIA::on_pushButtonEliminar_clicked()
{
    int row = ui->tableWidgetReglas->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Aviso", "Selecciona una regla para eliminar.");
        return;
    }

    int id = ui->tableWidgetReglas->item(row, 0) ? ui->tableWidgetReglas->item(row, 0)->text().toInt() : 0;
    QString cat = ui->tableWidgetReglas->item(row, 1) ? ui->tableWidgetReglas->item(row, 1)->text() : "";

    if (QMessageBox::question(this, "Confirmar eliminación",
                              QString("¿Seguro que deseas eliminar la regla '%1'?").arg(cat),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        if (id > 0) {
            QSqlDatabase db = obtenerBdConocimiento();
            if (db.isOpen()) {
                QSqlQuery q(db);
                q.prepare("DELETE FROM ia_conocimiento WHERE id = :id");
                q.bindValue(":id", id);
                q.exec();
            }
        }
        ui->tableWidgetReglas->removeRow(row);
        emit conocimientoModificado();
    }
}

/**
 * @brief Restablece la base de datos a los 10 grupos clínicos predeterminados.
 */
void DialogConocimientoIA::on_pushButtonRestablecer_clicked()
{
    if (QMessageBox::question(this, "Restablecer reglas",
                              "¿Deseas restablecer todas las reglas a los valores de fitoterapia predeterminados?",
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    QSqlDatabase db = obtenerBdConocimiento();
    if (!db.isOpen()) return;

    QSqlQuery q(db);
    q.exec("DELETE FROM ia_conocimiento");

    for (const ReglaDefault &regla : REGLAS_POR_DEFECTO) {
        QSqlQuery ins(db);
        ins.prepare("INSERT INTO ia_conocimiento (categoria, detonantes, terminos_clave, activo) "
                    "VALUES (:cat, :det, :term, 1)");
        ins.bindValue(":cat", QString::fromUtf8(regla.categoria));
        ins.bindValue(":det", QString::fromUtf8(regla.detonantes));
        ins.bindValue(":term", QString::fromUtf8(regla.terminos_clave));
        ins.exec();
    }

    cargarDatos();
    emit conocimientoModificado();
    QMessageBox::information(this, "Completado", "Reglas restablecidas correctamente.");
}

/**
 * @brief Guarda los cambios y notifica al motor IA.
 */
void DialogConocimientoIA::on_pushButtonGuardar_clicked()
{
    guardarDatos();
    QMessageBox::information(this, "Guardado", "Base de conocimiento actualizada con éxito.");
}

/**
 * @brief Cierra el diálogo.
 */
void DialogConocimientoIA::on_pushButtonCerrar_clicked()
{
    accept();
}
