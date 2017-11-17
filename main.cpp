#include "mainwindow.h"
#include "authwindow.h"
#include "udpclient.h"
#include <QObject>
#include <QApplication>

int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);

    MainWindow *mainWindow=new MainWindow;
//    AuthWindow *authWindow=new AuthWindow;
//    authWindow->show();

//    QObject::connect(authWindow, SIGNAL(sessionKeyReceived(QByteArray)), mainWindow, SLOT(start(QByteArray)));
//    QObject::connect(authWindow, SIGNAL(sessionKeyReceived(QByteArray)), authWindow, SLOT(close()));
    mainWindow->start("3jkfjsdksfjsidjgkjdfgj7fkg78tldp");

    return a.exec();
}
