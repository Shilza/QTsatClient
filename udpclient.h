#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include "def.h"

class MyUDP : public QObject
{
    Q_OBJECT
private:
    QUdpSocket *socket;
    QUdpSocket *systemSocket;
    QHostAddress host;
    void handshaking(QString log, QString pass);

public:
    explicit MyUDP(QString log,QString pass,QObject *parent = 0);
    //explicit MyUDP(QObject *parent = 0);
    void sendMessage(QString msg);
    QByteArray buffer;
    QString nickname;
    QByteArray sessionKey;
    QByteArray index;
signals:
    void updating();
private slots:
    void reading();
    void systemReading();
};

#endif // UDPCLIENT_H