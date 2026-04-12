#ifndef UNIFICARMAESTROS_H
#define UNIFICARMAESTROS_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QModelIndex>

namespace Ui {
class UnificarMaestros;
}

/**
 * @brief Configuración para el diálogo de unificación genérico.
 */
struct UnificarMaestrosConfig {
    QString titulo;             ///< Título de la ventana (ej: "Unificar Fabricantes")
    QString tablaMaestra;       ///< Tabla principal a unificar
    QString campoId;            ///< Clave primaria (ej: "id")
    QString campoNombre;        ///< Campo descriptivo (ej: "nombre" o "descripcion")
    
    struct Dependencia {
        QString tabla;          ///< Tabla hija que depende de la maestra
        QString campo;          ///< Columna en la tabla hija que referencia a la maestra
        QString campoMaestroOrigen; ///< Opcional. Si se indica, se usa este campo del maestro en lugar del Id.

        // Constructor explícito para evitar basura en memoria y avisos del compilador
        Dependencia(const QString &t = "", const QString &c = "", const QString &o = "")
            : tabla(t), campo(c), campoMaestroOrigen(o) {}
    };
    QList<Dependencia> dependencias;

    // Helper para inicialización rápida
    static UnificarMaestrosConfig paraFamilias() {
        return { "Unificar Familias", "familias", "id", "descripcion", {{"articulos", "familia"}} };
    }

    static UnificarMaestrosConfig paraFabricantes() {
        return { "Unificar Fabricantes", "fabricantes", "id", "nombre", {{"articulos", "fabricante"}} };
    }

    static UnificarMaestrosConfig paraMotivosEntrada() {
        return { "Unificar Motivos de Entrada", "motivosEntrada", "idtiposEntrada", "descripcion", {{"entradasSalidas", "idTiposRentrada", ""}} };
    }

    static UnificarMaestrosConfig paraProveedores() {
        return { "Unificar Proveedores", "proveedores", "idProveedor", "nombre", {
            {"facturas", "idProveedor", ""},
            {"albaranes", "idProveedor", ""},
            {"albaranes_tmp", "idProveedor", ""},
            {"pedidos", "idProveedor", ""},
            {"lineaspedido", "idProveedor", ""}
        }};
    }

    static UnificarMaestrosConfig paraClientes() {
        return { "Unificar Clientes", "clientes", "idCliente", "nombre", {
            {"tickets", "cliente", ""},
            {"vales", "idCliente", ""},
            {"encargos", "id_cliente", ""}
        }};
    }

    static UnificarMaestrosConfig paraFormasPago() {
        return { "Unificar Formas de Pago", "fpago", "id", "tipo", {
            {"tickets", "fpago", ""},
            {"proveedores", "formapago", ""}
        }};
    }

    static UnificarMaestrosConfig paraFormatos() {
        // En articulos, el formato se guarda por nombre (string), no por ID.
        return { "Unificar Formatos", "formatos", "id", "formato", {
            {"articulos", "formato", "formato"}
        }};
    }

    static UnificarMaestrosConfig paraUsuarios() {
        return { "Unificar Usuarios", "usuarios", "id", "usuario", {
            {"tickets", "usuario", ""}, // Por ID
            {"historico_stock", "usuario", "usuario"}, // Por login (campo 'usuario' en maestro)
            {"encargos", "empleado", "usuario"},       // Por login (campo 'usuario' en maestro)
            {"entradasSalidas", "usuario", "usuario"}  // Por login (campo 'usuario' en maestro)
        }};
    }

    static UnificarMaestrosConfig configParaTabla(const QString &tabla);
};

/**
 * @brief Diálogo genérico para la unificación interactiva de tablas maestras.
 * Permite fusionar múltiples registros y actualizar en cascada sus dependencias.
 */
class UnificarMaestros : public QDialog
{
    Q_OBJECT

public:
    explicit UnificarMaestros(const UnificarMaestrosConfig &config, QWidget *parent = nullptr);
    ~UnificarMaestros();

private slots:
    /// Recarga los datos de la tabla basándose en la configuración
    void refrescarTabla();

    /// Inicia el proceso de fusión
    void on_pushButtonFusionar_clicked();

    /// Permite renombrar el registro seleccionado
    void on_pushButtonRenombrar_clicked();

    /// Cierra el diálogo
    void on_pushButtonCerrar_clicked();

private:
    Ui::UnificarMaestros *ui;
    QSqlQueryModel *modelo;
    UnificarMaestrosConfig m_config;

    /// Ejecuta la fusión técnica en la base de datos (transaccional)
    bool ejecutarFusion(int idGanador, const QList<int> &idsPerdedores);
};

#endif // UNIFICARMAESTROS_H
