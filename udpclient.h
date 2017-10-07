#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket>

class MyUDP : public QObject
{
    Q_OBJECT
private:
    QUdpSocket *socket;

public:
    explicit MyUDP(QObject *parent = 0);
    void HelloUDP(QString msg);
    QByteArray buffer;

signals:
     void updating();

private slots:
    void reading();

};

#endif // UDPCLIENT_H
