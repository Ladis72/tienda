#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QStringList>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1"); // Attempting standard if local
    db.setUserName("root"); // try typical or empty
    db.setPassword("");
    db.setDatabaseName("tienda");
    if (!db.open()) {
        qDebug() << "Cannot connect:" << db.lastError().text();
        return 1;
    }
    
    QStringList tables = {"pedidos", "albaranes", "facturas", "lineaspedido"};
    for (const QString& table : tables) {
        qDebug() << "---" << table << "---";
        QSqlQuery q("DESCRIBE " + table, db);
        while(q.next()) {
            qDebug() << q.value(0).toString() << q.value(1).toString();
        }
    }
    return 0;
}
