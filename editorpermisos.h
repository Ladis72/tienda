#ifndef EDITORPERMISOS_H
#define EDITORPERMISOS_H

#include "gestorpermisos.h"
#include <QCheckBox>
#include <QDialog>
#include <QMap>

namespace Ui {
class EditorPermisos;
}

/**
 * @brief Diálogo para editar los permisos de cada rol del sistema.
 *
 * Muestra un combo con los 5 roles y una lista de checkboxes con todas
 * las claves de permiso. Al cambiar de rol, se recargan los checks para
 * reflejar qué permisos tiene concedidos. Los cambios se guardan
 * directamente en la tabla `permisos` de MySQL.
 *
 * Se abre desde la ventana de Usuarios con el botón "Editar permisos".
 */
class EditorPermisos : public QDialog {
  Q_OBJECT

public:
  /// @param rolInicial Rol preseleccionado al abrir el diálogo.
  /// @param conexion Nombre de la conexión a BD.
  explicit EditorPermisos(int rolInicial, const QString &conexion,
                          QWidget *parent = nullptr);
  ~EditorPermisos();

private slots:
  /// @brief Recarga los checkboxes al cambiar de rol en el combo.
  void on_comboRol_currentIndexChanged(int index);

  /// @brief Guarda los permisos del rol seleccionado en la BD.
  void guardar();

  /// @brief Marca todos los checkboxes.
  void on_btnMarcarTodo_clicked();

  /// @brief Desmarca todos los checkboxes.
  void on_btnDesmarcarTodo_clicked();

  void on_pushButtonGuardar_clicked();
  void on_pushButtonCerrar_clicked();

private:
  Ui::EditorPermisos *ui;

  /// Conexión a BD utilizada para las consultas.
  QString m_conexion;

  /// Mapa de clave de permiso → checkbox asociado.
  /// Se crea dinámicamente al construir el diálogo.
  QMap<QString, QCheckBox *> m_checks;

  /// @brief Crea los checkboxes para todas las claves de permiso existentes.
  void construirCheckboxes();

  /// @brief Actualiza el estado de los checkboxes según el rol seleccionado.
  void actualizarChecks(int rol);
};

#endif // EDITORPERMISOS_H
