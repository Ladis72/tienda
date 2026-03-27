#include "configticket.h"
#include "ui_configticket.h"
#include <QFileDialog>
#include <QDir>
#include <QCoreApplication>
#include <QPixmap>
#include <QLabel>

/**
 * @brief Constructor del diálogo de configuración de tickets.
 *
 * Recupera la configuración actual de la base de datos y rellena
 * todos los campos del formulario, incluyendo las imágenes de cabecera
 * y promocional con su vista previa.
 */
ConfigTicket::ConfigTicket(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfigTicket)
{
    ui->setupUi(this);

    // Recuperar la configuración actual del ticket desde la base de datos
    QStringList configuracionTicket = base->recuperarConfigTicket(conf->getConexionLocal());

    // Configurar los campos de texto de cabecera y pie
    ui->plainTextEditCabecera->setPlainText(configuracionTicket.at(0));
    ui->plainTextEditPie->setPlainText(configuracionTicket.at(1));

    // Configurar el campo de imagen de cabecera (índice 2 = campo "promocion" reutilizado)
    // El campo "promocion" ahora almacena la ruta de la imagen de cabecera
    ui->lineEditImgCabecera->setText(configuracionTicket.at(2));
    actualizarPreview(configuracionTicket.at(2), ui->labelPreviewCabecera);

    // Configurar la ruta de la impresora y los códigos
    ui->lineEditCola->setText(configuracionTicket.at(3));
    ui->lineEditCodApertura->setText(configuracionTicket.at(4));
    ui->lineEditCodCorte->setText(configuracionTicket.at(5));

    // Configurar la imagen de ticket promocional (nuevo campo índice 6)
    if (configuracionTicket.size() > 6) {
        ui->lineEditImgPromo->setText(configuracionTicket.at(6));
        actualizarPreview(configuracionTicket.at(6), ui->labelPreviewPromo);
    }

    // Configurar el estado del checkbox de ticket promocional
    if (base->ticketPromo(conf->getConexionLocal())) {
        ui->checkBoxPromo->setChecked(true);
        ui->lineEditImgPromo->setEnabled(true);
        ui->toolButtonImgPromo->setEnabled(true);
    } else {
        ui->checkBoxPromo->setChecked(false);
        ui->lineEditImgPromo->setEnabled(false);
        ui->toolButtonImgPromo->setEnabled(false);
    }
}

ConfigTicket::~ConfigTicket()
{
    delete ui;
}

/**
 * @brief Slot del botón cancelar: cierra el diálogo sin guardar cambios.
 */
void ConfigTicket::on_pushButtonCancelar_clicked()
{
    close();
}

/**
 * @brief Slot del botón guardar: recopila todos los datos del formulario
 * y los graba en la base de datos.
 *
 * El orden de los campos en la lista es:
 * 0 - cabecera (texto)
 * 1 - pie (texto)
 * 2 - promocion (reutilizado: ruta imagen cabecera)
 * 3 - boolPromocion (1/0)
 * 4 - ruta impresora
 * 5 - código apertura cajón
 * 6 - código corte papel
 * 7 - ruta imagen promocional (nuevo campo)
 */
void ConfigTicket::on_pushButtonAceptar_clicked()
{
    QStringList configuracionTicket;

    // Texto de cabecera del ticket
    configuracionTicket.append(ui->plainTextEditCabecera->toPlainText());

    // Texto del pie del ticket
    configuracionTicket.append(ui->plainTextEditPie->toPlainText());

    // Ruta de la imagen de cabecera (almacenada en el campo "promocion")
    configuracionTicket.append(ui->lineEditImgCabecera->text());

    // Flag de ticket promocional activado/desactivado
    if (ui->checkBoxPromo->isChecked()) {
        configuracionTicket.append("1");
    } else {
        configuracionTicket.append("0");
    }

    // Ruta de la impresora de tickets
    configuracionTicket.append(ui->lineEditCola->text());

    // Código de apertura del cajón
    configuracionTicket.append(ui->lineEditCodApertura->text());

    // Código de corte del papel
    configuracionTicket.append(ui->lineEditCodCorte->text());

    // Ruta de la imagen promocional (nuevo campo)
    configuracionTicket.append(ui->lineEditImgPromo->text());

    base->grabarConfiguracionTicket(configuracionTicket);
    this->close();
}

/**
 * @brief Slot que habilita/deshabilita los controles de imagen promocional
 * según el estado del checkbox.
 * @param checked true si el checkbox está marcado
 */
void ConfigTicket::on_checkBoxPromo_toggled(bool checked)
{
    ui->lineEditImgPromo->setEnabled(checked);
    ui->toolButtonImgPromo->setEnabled(checked);
}

/**
 * @brief Abre un diálogo para seleccionar el archivo de imagen de cabecera.
 *
 * Filtra por archivos de imagen (PNG, JPG, BMP). Al seleccionar una imagen,
 * actualiza el lineEdit con la ruta relativa y muestra una vista previa.
 */
void ConfigTicket::on_toolButtonImgCabecera_clicked()
{
    QString archivo = QFileDialog::getOpenFileName(
        this,
        "Seleccionar imagen de cabecera del ticket",
        ui->lineEditImgCabecera->text(),
        "Imágenes (*.png *.jpg *.jpeg *.bmp);;Todos los archivos (*)");

    if (!archivo.isEmpty()) {
        ui->lineEditImgCabecera->setText(rutaRelativa(archivo));
        actualizarPreview(rutaRelativa(archivo), ui->labelPreviewCabecera);
    }
}

/**
 * @brief Abre un diálogo para seleccionar el archivo de imagen promocional.
 *
 * Filtra por archivos de imagen (PNG, JPG, BMP). Al seleccionar una imagen,
 * actualiza el lineEdit con la ruta relativa y muestra una vista previa.
 */
void ConfigTicket::on_toolButtonImgPromo_clicked()
{
    QString archivo = QFileDialog::getOpenFileName(
        this,
        "Seleccionar imagen promocional del ticket",
        ui->lineEditImgPromo->text(),
        "Imágenes (*.png *.jpg *.jpeg *.bmp);;Todos los archivos (*)");

    if (!archivo.isEmpty()) {
        ui->lineEditImgPromo->setText(rutaRelativa(archivo));
        actualizarPreview(rutaRelativa(archivo), ui->labelPreviewPromo);
    }
}

/**
 * @brief Convierte una ruta absoluta a ruta relativa respecto al directorio del ejecutable.
 *
 * Igual que en directorios.cpp, genera una ruta relativa con prefijo "./"
 * para facilitar la portabilidad entre instalaciones.
 *
 * @param directorio Ruta absoluta del archivo o directorio
 * @return Ruta relativa con "./" como prefijo, o vacío si no se proporcionó ruta
 */
QString ConfigTicket::rutaRelativa(QString directorio)
{
    if (!directorio.isEmpty()) {
        QString rutaEjecutable = QCoreApplication::applicationDirPath();
        QString rutaRel = QDir(rutaEjecutable).relativeFilePath(directorio);

        if (!rutaRel.startsWith(".") && !rutaRel.startsWith("/"))
            rutaRel.prepend("./");
        return rutaRel;
    }
    return "";
}

/**
 * @brief Carga y muestra una vista previa de la imagen en el QLabel indicado.
 *
 * Si la ruta es válida y contiene una imagen legible, la escala al tamaño
 * del label manteniendo la proporción. Si no, muestra "Sin imagen".
 *
 * @param ruta Ruta (relativa o absoluta) del archivo de imagen
 * @param label QLabel donde mostrar la vista previa
 */
void ConfigTicket::actualizarPreview(const QString &ruta, QLabel *label)
{
    if (ruta.isEmpty()) {
        label->setText("Sin imagen");
        label->setPixmap(QPixmap());
        return;
    }

    // Resolver ruta relativa respecto al directorio del ejecutable
    QString rutaCompleta = ruta;
    if (!QDir::isAbsolutePath(ruta)) {
        rutaCompleta = QCoreApplication::applicationDirPath() + "/" + ruta;
    }

    QPixmap imagen(rutaCompleta);
    if (imagen.isNull()) {
        label->setText("Sin imagen (archivo no encontrado)");
        return;
    }

    // Escalar la imagen manteniendo la proporción al tamaño del label
    label->setPixmap(imagen.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    label->setText("");
}
