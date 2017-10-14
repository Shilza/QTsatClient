#include <QCoreApplication>
#include "udpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);

    UDPServer server;
    return application.exec();
}
