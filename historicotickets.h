#ifndef HISTORICOTICKETS_H
#define HISTORICOTICKETS_H

#include <QDialog>
#include "base_datos.h"
namespace Ui {
class HistoricoTickets;
}

class HistoricoTickets : public QDialog
{
    Q_OBJECT

public:
    explicit HistoricoTickets(QWidget *parent = 0);
    ~HistoricoTickets();

private slots:
    void on_pushButtonConsultar_clicked();

    void on_tableViewTickets_activated(const QModelIndex &index);

private:
    Ui::HistoricoTickets *ui;
    QSqlQueryModel *listaTickets;
    QSqlQueryModel *ticket;
    baseDatos base;

    void mostrarTickets();
};

#endif // HISTORICOTICKETS_H
