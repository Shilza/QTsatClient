#include <QCoreApplication>
#include "udpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UDPServer serv;
    return a.exec();
}
