#include "udpclient.h"

MyUDP::MyUDP(QString log,QString pass, QObject *parent) : QObject(parent){

    socket = new QUdpSocket(this);
    systemSocket = new QUdpSocket(this);

    socket->bind(49000);
    systemSocket->bind(49002);
    host.setAddress("31.131.27.154");
    handshaking(log,pass);

    connect(socket, SIGNAL(readyRead()), this, SLOT(reading()));
}

void MyUDP::HelloUDP(QString msg){
    QByteArray Data;
    socket->writeDatagram(Data.append(msg), host, 49001);
}

void MyUDP::reading(){
    buffer.resize(socket->pendingDatagramSize());
    socket->readDatagram(buffer.data(), buffer.size());
    emit updating();
}

void MyUDP::handshaking(QString log, QString pass){
    QByteArray par;
    sessionKey.resize(systemSocket->pendingDatagramSize());
    systemSocket->writeDatagram(par.append(" handshake|"+log+"|"+pass), host, 49002);
    systemSocket->readDatagram(sessionKey.data(), sessionKey.size());
}
