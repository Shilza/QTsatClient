#include "mainwindow.h"
#include "authwindow.h"
#include "udpclient.h"
#include <QObject>
#include <QApplication>

int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);
    AuthWindow *authWindow=new AuthWindow;

    authWindow->setAttribute(Qt::WA_TranslucentBackground);
    authWindow->setWindowFlags(Qt::FramelessWindowHint);
    authWindow->setGeometry(500,310,260,260);
    authWindow->setStyleSheet("background: #C8A4E5;");
    authWindow->show();

    MainWindow *w=new MainWindow;
    //w.setMinimumSize(380,280);
    w->setFixedSize(360,260);
    w->setContentsMargins(0,0,0,-20);
    QObject::connect(authWindow, SIGNAL(sessionKeyReceived(QByteArray)), w,SLOT(start(QByteArray)));
    QObject::connect(authWindow, SIGNAL(sessionKeyReceived()), authWindow,SLOT(close()));

    return a.exec();
}
