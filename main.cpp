#include "mainwindow.h"
#include "udpclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.setMinimumSize(380,280);
    w.setFixedSize(360,260);
    w.setContentsMargins(0,0,0,-20);
    w.show();

    return a.exec();
}
