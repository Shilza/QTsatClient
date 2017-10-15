#include "udpserver.h"

using std::shared_ptr;
class UDPServer::Session{
public:
    unsigned int time;
    QByteArray sessionKey;
    QString nickname;
    QHostAddress IP;

    Session(QString nickname, QHostAddress IP){
        this->nickname = nickname;
        this->IP = IP;
        time=QDateTime::currentDateTime().toTime_t();
        sessionKey = QCryptographicHash::hash(nickname.toUtf8() + time, QCryptographicHash::Md5).toHex();
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
    connect(this, SIGNAL(isReceived(QByteArray)), this, SLOT(sendReceived(QByteArray)));

}

QString UDPServer::check(QByteArray sessionKey){
    for(int i=0; i<sessions.size(); i++)
        if(sessions[i].get()->sessionKey == sessionKey)
            return sessions[i].get()->nickname;
    return "";
}

void UDPServer::sendReceived(QByteArray message){
    QStringList list = QString(message).split('|');
    QString nickname = check((list.at(0)).toUtf8());
    list.pop_front();

    if(nickname != ""){
        QString finalMessage;
        while(list.size()){
            finalMessage += list.front()+'|';
            list.pop_front();
        }

        QSqlQuery query;
        query.prepare("INSERT INTO messages (Sender, Text, Time) VALUES (:sender, :text, :time)");
        query.bindValue(":sender", nickname);
        query.bindValue(":text", finalMessage);
        query.bindValue(":time", QDateTime::currentDateTime().toTime_t());
        query.exec();

        finalMessage.push_front(nickname+'|');

        for(int i=0; i<sessions.size(); i++){
            socket->writeDatagram(finalMessage.toUtf8(), sessions[i].get()->IP, 49000);
            qDebug() << i;
        }


    }
    else
        return;
}

void UDPServer::read()
{
    QByteArray message;
    message.resize(socket->pendingDatagramSize());
    socket->readDatagram(message.data(), message.size());
    emit isReceived(message);
}

void UDPServer::handshake(){
    QByteArray buffer;
    quint16 port;
    QHostAddress peer;

    buffer.resize(systemSocket->pendingDatagramSize());
    systemSocket->readDatagram(buffer.data(), buffer.size(), &peer, &port);

    QStringList list = QString(buffer).split('|');

    if(list.at(0)!="handshake")
        return;

    QSqlQuery query;
    query.prepare("SELECT ID FROM users WHERE Nickname=? AND Password=?");
    query.bindValue(0, list.at(1));
    query.bindValue(1, list.at(2));
    query.exec();

    QString id="";
    while (query.next())
        id = query.value(0).toString();

    if(id == "")
        return;

    sessions.push_back(shared_ptr<Session>(new Session(list.at(1), peer)));
    systemSocket->writeDatagram(sessions[sessions.size()-1].get()->sessionKey, peer, port);
}




