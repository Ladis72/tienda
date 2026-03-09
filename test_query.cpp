#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "DB");
    db.setHostName("localhost");
    db.setDatabaseName("tiendaNueva");
    db.setUserName("root");
    db.setPassword("meganizado");
    db.setPort(3306);
    if (!db.open()) {
        qDebug() << "Failed to connect:" << db.lastError().text();
        return 1;
    }
    QSqlQuery query(db);
    query.exec("SELECT * FROM tiendas WHERE local != '1'");
    while (query.next()) {
        qDebug() << query.value("nombre").toString();
    }
    return 0;
}
