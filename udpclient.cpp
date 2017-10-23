#include "udpclient.h"

UDPClient::UDPClient(QObject *parent) : QObject(parent){

    socket = new QUdpSocket(this);
    systemSocket = new QUdpSocket(this);

    socket->bind(49000);
    systemSocket->bind(49002);
    host.setAddress(HOST_IP);

    connect(socket, SIGNAL(readyRead()), this, SLOT(reading()));
    connect(systemSocket, SIGNAL(readyRead()), this, SLOT(systemReading()));
}

void UDPClient::sendMessage(QString msg){
    QByteArray Data;
    socket->writeDatagram(Data.append(msg), host, 49001);
}

void UDPClient::reading(){
    buffer.resize(socket->pendingDatagramSize());
    socket->readDatagram(buffer.data(), buffer.size());

    QList<QByteArray> tempList = buffer.split('|');
    nickname = tempList.at(0);

    tempList.pop_front();
    buffer.clear();

    for(int i=0; i<tempList.size(); i++)
        buffer.push_back(tempList.at(i));

    emit updating();
}

void UDPClient::systemReading(){
        index.resize(systemSocket->pendingDatagramSize());
        systemSocket->readDatagram(index.data(),index.size());
        systemSocket->writeDatagram(index, host, 49003);
}

