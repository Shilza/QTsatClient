#include "udpclient.h"

MyUDP::MyUDP(QObject *parent) :
    QObject(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(1337);

    connect(socket, SIGNAL(readyRead()), this, SLOT(reading()));
}

void MyUDP::HelloUDP(QString msg)
{
    QByteArray Data;
    Data.append(msg);
    QHostAddress a;
    a.setAddress("31.131.27.154");
    socket->writeDatagram(Data, a, 1337);
}

void MyUDP::reading()
{
    buffer.resize(socket->pendingDatagramSize());
    socket->readDatagram(buffer.data(), buffer.size());
    emit updating();
}
