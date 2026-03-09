#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
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
    query.first();
    qDebug() << "numRowsAffected:" << query.numRowsAffected();
    qDebug() << "size:" << query.size();
    
    QStringList listaOrdenadoresRemotos;
    for (int i = 0; i < query.numRowsAffected(); ++i) {
        listaOrdenadoresRemotos.append(query.value("nombre").toString());
        query.next();
    }
    qDebug() << "Result list:" << listaOrdenadoresRemotos;

    return 0;
}
