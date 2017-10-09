#include "udpclient.h"

MyUDP::MyUDP(QObject *parent) : QObject(parent){

    socketReceiver = new QUdpSocket(this);
    socketReceiver->bind(1337);

    socketSender = new QUdpSocket(this);
    socketSender->bind(1338);

    connect(socketReceiver, SIGNAL(readyRead()), this, SLOT(reading()));
    connect(socketSender, SIGNAL(readyRead()), this, SLOT(handshaking()));

    host.setAddress("31.131.27.154");
    socketSender->writeDatagram(" handshake|Shilza|0", host, 1338);

}

void MyUDP::HelloUDP(QString msg){
    QByteArray Data;
    socketSender->writeDatagram(Data.append(msg), host, 1338);
}

void MyUDP::reading(){
    buffer.resize(socketReceiver->pendingDatagramSize());
    socketReceiver->readDatagram(buffer.data(), buffer.size());
}

void MyUDP::handshaking(){
    sessionKey.resize(socketSender->pendingDatagramSize());
    socketSender->readDatagram(sessionKey.data(), sessionKey.size());
    emit updating();
}
