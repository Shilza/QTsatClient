#include <QCoreApplication>
#include "udpserver.h"
#include <QtSql/QSqlDatabase>

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName("localhost");
    db.setDatabaseName("Tsat");
    db.setUserName("shilza");
    db.setPassword("192.168.39.26");
    if(!db.open())
        return 1;

    UDPServer server;
    db.close();
    return application.exec();
}
