#include "udpserver.h"

using std::shared_ptr;
class UDPServer::Session{
public:
    unsigned int time;
    QByteArray sessionKey;
    QByteArray nickname;
    QHostAddress IP;

    Session(QByteArray nickname, QHostAddress IP){
        this->nickname = nickname;
        this->IP = IP;
        time=QDateTime::currentDateTime().toTime_t();
        sessionKey = QCryptographicHash::hash(time+nickname, QCryptographicHash::Md5).toHex();
    }
};


UDPServer::UDPServer(QObject *parent) :
    QObject(parent)
{
    sessions.clear();

    socket = new QUdpSocket(this);
    systemSocket = new QUdpSocket(this);

    socket->bind(QHostAddress::Any, 49001);
    systemSocket->bind(QHostAddress::Any, 49003);

    connect(socket, SIGNAL(readyRead()), this, SLOT(read()));
    connect(systemSocket, SIGNAL(readyRead()), this, SLOT(handshake()));
    connect(this, SIGNAL(isReceived()), this, SLOT(sendReceived()));
    handshake();
}

void UDPServer::sendReceived()
{
    QByteArray Data;
    Data.append("Hello");
   // socket->writeDatagram(Data, , 49000);
}

void UDPServer::read()
{
    message.resize(socket->pendingDatagramSize());
    socket->readDatagram(message.data(), message.size());
    emit isReceived();
}

void UDPServer::handshake(){
    QByteArray buffer;
    buffer.append("handshake|sdfsdf");
//    buffer.resize(socket->pendingDatagramSize());
    quint16 port;
    QHostAddress peer;
//    systemSocket->readDatagram(buffer.data(), buffer.size(), &peer, &port);
    QStringList list = QString(buffer).split('|');
    qDebug() << list.at(1);
//    sessions.push_back(shared_ptr<Session>(new Session("Shilza", peer)));
}




