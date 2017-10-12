#include "udpserver.h"
#include <iostream>

UDPServer::UDPServer(QObject *parent) :
    QObject(parent)
{
    socket = new QUdpSocket(this);

    socket->bind(QHostAddress::LocalHost, 1337);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(this, SIGNAL(forSay()),this,SLOT(SayHello()));
}

void UDPServer::SayHello()
{
    QByteArray Data;
    Data.append("Hello");
    socket->writeDatagram(Data, peer, 1338);
}

void UDPServer::readyRead()
{
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    quint16 senderPort;
    socket->readDatagram(buffer.data(), buffer.size(), &peer, &senderPort);

    qDebug() << "Message from: " << peer.toString();
    qDebug() << "Message: " << buffer;

    emit forSay();
}
