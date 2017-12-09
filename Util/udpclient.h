#ifndef UDPCLIENT_H
#define UDPCLIENT_H
#include <QObject>
#include <QUdpSocket>
#include "Config/def.h"

class UDPClient : public QObject
{
    Q_OBJECT
private:
    QUdpSocket *socket;
    QUdpSocket *systemSocket;
    QHostAddress host;

public:
    explicit UDPClient(QObject *parent = 0);
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
