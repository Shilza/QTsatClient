#include "udpserver.h"

UDPServer::UDPServer(QObject *parent) :
    QObject(parent)
{
    socket = new QUdpSocket(this);

    socket->bind(QHostAddress::Any, 49001);

    connect(socket, SIGNAL(readyRead()), this, SLOT(read()));
    connect(this, SIGNAL(isReceived()),this,SLOT(sendReceived()));
}

void UDPServer::sendReceived()
{
    QByteArray Data;
    Data.append("Hello");
    socket->writeDatagram(Data, peer, 49000);
}

void UDPServer::read()
{
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());

    quint16 senderPort;
    socket->readDatagram(buffer.data(), buffer.size(), &peer, &senderPort);
    qDebug() << buffer;
    emit isReceived();
}
