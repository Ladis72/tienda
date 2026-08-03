#ifndef VERIFICADORBASEDATOS_H
#define VERIFICADORBASEDATOS_H

#include <QDialog>
#include <QList>
#include <QStringList>
#include <QTableWidgetItem>
#include "base_datos.h"

namespace Ui {
class VerificadorBaseDatos;
}

/**
 * @brief Estructura que define una comprobación individual de integridad en la base de datos local.
 *
 * Contiene las consultas SQL para la verificación de anomalías, auto-corrección (si aplica)
 * y la obtención de detalles de los registros afectados.
 */
struct ComprobacionBD {
    QString id;             // Identificador único de la comprobación (ej. 'lineas_ticket_huerfanas')
    QString titulo;         // Título descriptivo visible en la interfaz de usuario
    QString consultaCheck;  // Sentencia SQL para contar la cantidad de anomalías
    QString consultaFix;    // Sentencia SQL para la auto-corrección automática de los errores
    QString consultaDetalle;// Sentencia SQL para obtener el listado descriptivo de IDs o códigos afectados
    bool esCorregible;      // Indica si el sistema puede corregir la anomalía de forma automatizada
    int totalErrores;       // Cantidad total de registros incoherentes detectados en el último análisis
    QStringList detalles;   // Mensajes o identificadores con los detalles de las filas afectadas
};

/**
 * @brief Diálogo para la verificación y reparación de la integridad de la base de datos local.
 *
 * Realiza un análisis exhaustivo ejecutando diversas consultas SQL para detectar
 * registros huérfanos, desajustes de stock, anomalías de clientes o fechas
 * fuera del estándar obligatorio ("yyyy-MM-dd").
 */
class VerificadorBaseDatos : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor de la ventana de verificación de base de datos.
     * @param parent Widget padre del diálogo (opcional).
     */
    explicit VerificadorBaseDatos(QWidget *parent = nullptr);

    /**
     * @brief Destructor de la ventana de verificación.
     */
    ~VerificadorBaseDatos();

private slots:
    /**
     * @brief Slot para el botón 'Analizar'. Inicia la comprobación completa de la base de datos.
     */
    void on_pushButtonAnalizar_clicked();

    /**
     * @brief Slot para el botón 'Corregir Errores'. Ejecuta las consultas de reparación en una transacción SQL.
     */
    void on_pushButtonCorregir_clicked();

    /**
     * @brief Slot para el botón 'Ver Detalles'. Muestra una ventana modal con los registros afectados de la comprobación seleccionada.
     */
    void on_pushButtonDetalles_clicked();

    /**
     * @brief Slot para marcar todas las casillas de comprobación corregibles.
     */
    void on_pushButtonMarcarTodos_clicked();

    /**
     * @brief Slot para desmarcar todas las casillas de comprobación.
     */
    void on_pushButtonDesmarcarTodos_clicked();

    /**
     * @brief Slot que se activa al hacer doble clic sobre una fila de la tabla.
     * @param item Celda seleccionada en la tabla.
     */
    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::VerificadorBaseDatos *ui;  // Puntero a los componentes gráficos generados desde el archivo .ui
    baseDatos m_base;               // Instancia de baseDatos para registro de logs
    QList<ComprobacionBD> m_comprobaciones; // Colección de verificaciones registradas

    /**
     * @brief Inicializa y registra todas las comprobaciones de integridad SQL del sistema.
     */
    void inicializarComprobaciones();
    
    /**
     * @brief Ejecuta las consultas SQL de análisis y actualiza el estado global de la interfaz.
     */
    void ejecutarAnalisis();
    
    /**
     * @brief Renderiza el estado de cada diagnóstico en el QTableWidget.
     */
    void actualizarTabla();
};

#endif // VERIFICADORBASEDATOS_H

