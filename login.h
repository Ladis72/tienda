#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QTimer>
#include "base_datos.h"
#include "configuracion.h"

extern Configuracion *conf;
namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_pushButton_clicked();
    void on_comboBoxTienda_currentIndexChanged(int index);

private:
    Ui::Login *ui;
    baseDatos *base;

    // SEC-05: Protección anti-fuerza bruta
    int m_intentosFallidos;           ///< Contador de intentos fallidos consecutivos
    static const int MAX_INTENTOS = 3; ///< Intentos antes de bloquear
    static const int BLOQUEO_MS = 30000; ///< Milisegundos de bloqueo (30s)
    QTimer m_timerBloqueo;            ///< Timer para desbloquear el botón
    int m_prevTiendaIndex;            ///< Guarda el índice anterior del combo de tienda
};

#endif // LOGIN_H

