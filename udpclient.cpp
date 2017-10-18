#include "udpclient.h"

MyUDP::MyUDP(QString log,QString pass, QObject *parent) : QObject(parent){

    socket = new QUdpSocket(this);
    systemSocket = new QUdpSocket(this);

    socket->bind(49000);
    systemSocket->bind(49002);
    host.setAddress(HOST_IP);
    handshaking(log,pass);

    connect(socket, SIGNAL(readyRead()), this, SLOT(reading()));
    connect(systemSocket, SIGNAL(readyRead()), this, SLOT(systemReading()));
}

void MyUDP::sendMessage(QString msg){
    QByteArray Data;
    socket->writeDatagram(Data.append(msg), host, 49001);
}

void MyUDP::reading(){
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

void MyUDP::systemReading(){
    if(systemSocket->pendingDatagramSize()>5){
        sessionKey.resize(systemSocket->pendingDatagramSize());
        systemSocket->readDatagram(sessionKey.data(),sessionKey.size());
    }
    else{
        index.resize(systemSocket->pendingDatagramSize());
        systemSocket->readDatagram(index.data(),index.size());
        systemSocket->writeDatagram(index, host, 49003);
    }
}

void MyUDP::handshaking(QString log, QString pass){
    QByteArray par;
    systemSocket->writeDatagram(par.append("handshake|"+log+"|"+pass), host, 49003);
}
