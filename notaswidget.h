#ifndef NOTASWIDGET_H
#define NOTASWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QSqlQueryModel>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include "base_datos.h"

class NotasColorDelegate;

class NotasWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NotasWidget(QWidget *parent = nullptr);

    void refrescar();
    void setConexion(const QString &conn);

signals:
    void pendingCountChanged(int count);
    void hideRequested();

private slots:
    void onNuevaNota();
    void onFiltroChanged(int index);
    void onContextMenu(const QPoint &pos);
    void onDoubleClicked(const QModelIndex &index);
    void onMarcarCompletada();
    void onMarcarPendiente();
    void onEliminar();
    void onCambiarPrioridad(const QString &prioridad);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUi();
    void actualizarContador();
    int idNotaSeleccionada();
    bool dialogoNota(QString &titulo, QString &descripcion,
                     QString &fechaLimite, QString &prioridad,
                     int idEdicion = -1);

    QTableView          *tablaView;
    QSqlQueryModel      *modelo;
    QSortFilterProxyModel *proxyModel;
    QPushButton         *btnNueva;
    QComboBox           *cboFiltro;
    QLabel              *lblContador;
    NotasColorDelegate  *delegate;

    baseDatos            base;
    QString              conexionDB;
};

#endif // NOTASWIDGET_H
