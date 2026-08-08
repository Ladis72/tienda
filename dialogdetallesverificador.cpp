#include "dialogdetallesverificador.h"
#include "ui_dialogdetallesverificador.h"
#include <QStyle>

/**
 * @brief Constructor de la ventana modal de detalles del verificador de base de datos.
 * Configura los textos explicativos, los detalles del informe y los estilos estándar del programa.
 *
 * @param titulo Título de la comprobación analizada.
 * @param totalErrores Número total de incoherencias halladas.
 * @param detalles Listado detallado de registros o motivos del fallo.
 * @param parent Widget padre.
 */
DialogDetallesVerificador::DialogDetallesVerificador(const QString &titulo,
                                                     int totalErrores,
                                                     const QStringList &detalles,
                                                     QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogDetallesVerificador)
{
    ui->setupUi(this);

    // Asignar el icono estándar de cierre según el estilo global del programa
    ui->pushButtonCerrar->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));

    // Permitir que la ventana sea libremente redimensionable y maximizable por el usuario
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);

    // Establecer el título y los textos explicativos del diagnóstico
    ui->labelTitulo->setText(tr("Diagnóstico: %1").arg(titulo));
    ui->labelInfo->setText(tr("Se detectaron %1 registros incoherentes. A continuación se muestran los elementos afectados:")
                               .arg(totalErrores));

    // Cargar la información detallada en el editor de texto plano (de solo lectura)
    ui->plainTextEditDetalles->setPlainText(detalles.join("\n"));
}

/**
 * @brief Destructor del diálogo. Libera la memoria reservada para la interfaz de usuario.
 */
DialogDetallesVerificador::~DialogDetallesVerificador()
{
    delete ui;
}
