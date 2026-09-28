#include "dialognueva_nota.h"
#include "ui_dialognueva_nota.h"
#include <QMessageBox>
#include <QPushButton>

/**
 * @brief Constructor del diálogo de notas.
 * Inicializa la interfaz gráfica desde el archivo .ui y configura los valores predeterminados.
 */
DialogNuevaNota::DialogNuevaNota(QWidget *parent, bool esEdicion)
    : QDialog(parent)
    , ui(new Ui::DialogNuevaNota)
{
    ui->setupUi(this);

    // Ajustar título de ventana según modo
    setWindowTitle(esEdicion ? tr("Editar nota") : tr("Nueva nota"));

    // Configurar textos de los botones del QDialogButtonBox
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Guardar"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancelar"));

    // Configurar fecha actual por defecto y formato obligatorio "yyyy-MM-dd"
    ui->dateEditFechaLimite->setDisplayFormat("yyyy-MM-dd");
    ui->dateEditFechaLimite->setDate(QDate::currentDate());
    ui->dateEditFechaLimite->setDisabled(true);

    // Conectar validación antes de aceptar
    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &DialogNuevaNota::onAceptar);
}

/**
 * @brief Destructor del diálogo. Libera la memoria de la interfaz generada por Qt.
 */
DialogNuevaNota::~DialogNuevaNota()
{
    delete ui;
}

/**
 * @brief Carga los datos previos en el formulario cuando se edita una nota.
 * Prioriza siempre el formato estándar "yyyy-MM-dd" respetando las reglas globales.
 */
void DialogNuevaNota::setDatos(const QString &titulo, const QString &descripcion,
                               const QString &fechaLimite, const QString &prioridad)
{
    ui->lineEditTitulo->setText(titulo);
    ui->textEditDescripcion->setPlainText(descripcion);

    if (!prioridad.isEmpty()) {
        ui->comboBoxPrioridad->setCurrentText(prioridad);
    }

    if (!fechaLimite.isEmpty()) {
        QDate d = QDate::fromString(fechaLimite, "yyyy-MM-dd");
        if (!d.isValid()) {
            d = QDate::fromString(fechaLimite, "dd/MM/yyyy"); // Fallback defensivo para datos antiguos
        }
        if (d.isValid()) {
            ui->dateEditFechaLimite->setDate(d);
            ui->checkBoxSinFecha->setChecked(false);
            ui->dateEditFechaLimite->setEnabled(true);
        } else {
            ui->checkBoxSinFecha->setChecked(true);
            ui->dateEditFechaLimite->setDisabled(true);
        }
    } else {
        ui->checkBoxSinFecha->setChecked(true);
        ui->dateEditFechaLimite->setDisabled(true);
    }
}

/**
 * @brief Devuelve el título introducido tras eliminar espacios residuales.
 */
QString DialogNuevaNota::titulo() const
{
    return ui->lineEditTitulo->text().trimmed();
}

/**
 * @brief Devuelve la descripción introducida tras eliminar espacios residuales.
 */
QString DialogNuevaNota::descripcion() const
{
    return ui->textEditDescripcion->toPlainText().trimmed();
}

/**
 * @brief Devuelve la fecha límite en formato estricto "yyyy-MM-dd" o cadena vacía si no aplica.
 */
QString DialogNuevaNota::fechaLimite() const
{
    if (ui->checkBoxSinFecha->isChecked()) {
        return QString();
    }
    return ui->dateEditFechaLimite->date().toString("yyyy-MM-dd");
}

/**
 * @brief Devuelve la prioridad seleccionada.
 */
QString DialogNuevaNota::prioridad() const
{
    return ui->comboBoxPrioridad->currentText();
}

/**
 * @brief Habilita o deshabilita el selector de fecha al alternar el checkbox "Sin fecha límite".
 */
void DialogNuevaNota::on_checkBoxSinFecha_toggled(bool checked)
{
    ui->dateEditFechaLimite->setDisabled(checked);
}

/**
 * @brief Valida que el campo obligatorio de título esté informado antes de aceptar el diálogo.
 */
void DialogNuevaNota::onAceptar()
{
    if (titulo().isEmpty()) {
        QMessageBox::warning(this, tr("Campo requerido"), tr("El título es obligatorio."));
        ui->lineEditTitulo->setFocus();
        return;
    }
    accept();
}
