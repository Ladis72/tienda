#include "dialogreglasalud.h"
#include "ui_dialogreglasalud.h"
#include <QMessageBox>
#include <QPushButton>

/**
 * @brief Constructor de DialogReglaSalud.
 * Inicializa los componentes de la interfaz desde el archivo .ui y personaliza botones y título.
 */
DialogReglaSalud::DialogReglaSalud(QWidget *parent, bool esEdicion)
    : QDialog(parent)
    , ui(new Ui::DialogReglaSalud)
{
    ui->setupUi(this);

    // Ajustar título de ventana según la acción
    setWindowTitle(esEdicion ? tr("Editar Regla de Salud") : tr("Añadir Nueva Regla de Salud"));

    // Personalizar botones del cuadro de diálogo
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Guardar"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancelar"));

    // Interceptar aceptación para validación defensiva previa
    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &DialogReglaSalud::onAceptar);
}

/**
 * @brief Destructor de DialogReglaSalud. Libera los recursos de interfaz gráfica.
 */
DialogReglaSalud::~DialogReglaSalud()
{
    delete ui;
}

/**
 * @brief Inicializa los campos de texto con los valores de la regla a editar.
 */
void DialogReglaSalud::setDatos(const QString &categoria, const QString &detonantes, const QString &terminos)
{
    ui->lineEditCategoria->setText(categoria);
    ui->textEditDetonantes->setPlainText(detonantes);
    ui->textEditTerminos->setPlainText(terminos);
}

/**
 * @brief Devuelve la categoría o dolencia sin espacios residuales.
 */
QString DialogReglaSalud::categoria() const
{
    return ui->lineEditCategoria->text().trimmed();
}

/**
 * @brief Devuelve las palabras detonantes de la regla.
 */
QString DialogReglaSalud::detonantes() const
{
    return ui->textEditDetonantes->toPlainText().trimmed();
}

/**
 * @brief Devuelve los términos clave y marcas asociadas.
 */
QString DialogReglaSalud::terminos() const
{
    return ui->textEditTerminos->toPlainText().trimmed();
}

/**
 * @brief Valida que la categoría esté informada antes de confirmar el diálogo.
 */
void DialogReglaSalud::onAceptar()
{
    if (categoria().isEmpty()) {
        QMessageBox::warning(this, tr("Aviso"), tr("La categoría o dolencia no puede estar vacía."));
        ui->lineEditCategoria->setFocus();
        return;
    }
    accept();
}
