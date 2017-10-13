#include "udpserver.h"
#include <iostream>
#include <QTime>
UDPServer::UDPServer(QObject *parent) :
    QObject(parent)
{
    socket = new QUdpSocket(this);

    socket->bind(QHostAddress::Any, 49001);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(this, SIGNAL(forSay()),this,SLOT(SayHello()));
}

void UDPServer::SayHello()
{
    QByteArray Data;
    Data.append("Hello");
    socket->writeDatagram(Data, peer, 49000);
}

void UDPServer::readyRead()
{
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    quint16 senderPort;
    socket->readDatagram(buffer.data(), buffer.size(), &peer, &senderPort);
    qDebug() << buffer;
    emit forSay();
}
