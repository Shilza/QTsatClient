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
        sessionKey = QCryptographicHash::hash(nickname.toUtf8() + QByteArray::number(time), QCryptographicHash::Md5).toHex();
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
    connect(systemSocket, SIGNAL(readyRead()), this, SLOT(systemReading()));
    connect(this, SIGNAL(systemReceived(QByteArray)), this, SLOT(answersChecker(QByteArray)));
    connect(this, SIGNAL(systemReceived(QStringList, QHostAddress, quint16)), this, SLOT(handshake(QStringList,QHostAddress,quint16)));
    connect(this, SIGNAL(isReceived(QByteArray)), this, SLOT(sendReceived(QByteArray)));
}

void UDPServer::start(){
    std::thread sessionsCheckerThread([&](){
        while(true){
            answers.clear();
            unsigned int time=QDateTime::currentDateTime().toTime_t();
            for(int i=0; i<sessions.size(); i++)
                if(time > sessions[i].get()->time+10){
                    systemSocket->writeDatagram(QByteArray::number(i), sessions[i].get()->IP, 49002);
                }
            std::this_thread::sleep_for(std::chrono::seconds(2));

            for(int i=0; i<sessions.size(); i++)
                if(!findInAnswers(i) && time > sessions[i].get()->time+10)
                    sessions.erase(sessions.begin()+i);
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    });
    sessionsCheckerThread.detach();
}

QString UDPServer::check(QByteArray sessionKey){
    for(int i=0; i<sessions.size(); i++)
        if(sessions[i].get()->sessionKey == sessionKey)
            return sessions[i].get()->nickname;
    return "";
}

bool UDPServer::findInAnswers(int i){
    for(int j=0; j<answers.size(); j++)
        if(answers[j] == i){
            answers.erase(answers.begin()+j);
            return true;
        }
    return false;
}

void UDPServer::sendReceived(QByteArray message){
    QStringList list = QString(message).split('|');
    QString nickname = check((list.at(0)).toUtf8());
    list.pop_front();

    if(nickname != ""){
        QString finalMessage;
        while(list.size()){
            finalMessage += '|' + list.front();
            list.pop_front();
        }

        for(int i=0; i<sessions.size(); i++)
            socket->writeDatagram(nickname.toUtf8() + finalMessage.toUtf8(), sessions[i].get()->IP, 49000);

        finalMessage.remove(0,1);
        QSqlQuery query;
        query.prepare("INSERT INTO messages (Sender, Text, Time) VALUES (:sender, :text, :time)");
        query.bindValue(":sender", nickname);
        query.bindValue(":text", finalMessage);
        query.bindValue(":time", QDateTime::currentDateTime().toTime_t());
        query.exec();
    }
}

void UDPServer::read()
{
    QByteArray message;
    message.resize(socket->pendingDatagramSize());
    socket->readDatagram(message.data(), message.size());
    emit isReceived(message);
}

void UDPServer::handshake(QStringList list, QHostAddress peer, quint16 port){
    for(int i=0; i<sessions.size(); i++)
        if(list.at(1) == sessions.at(i).get()->nickname){
            systemSocket->writeDatagram(sessions[i].get()->sessionKey, peer, port);
            return;
        }

    QSqlQuery query;
    query.prepare("SELECT ID FROM users WHERE Nickname=? AND Password=?");
    query.bindValue(0, list.at(1));
    query.bindValue(1, list.at(2));
    query.exec();

    QString id="";
    while (query.next())
        id = query.value(0).toString();

    if(id != ""){
        sessions.push_back(std::make_shared<Session>(Session(list.at(1), peer)));
        systemSocket->writeDatagram(sessions[sessions.size()-1].get()->sessionKey, peer, port);
    }
    else
        systemSocket->writeDatagram("ERROR_AUTH", peer, port);
}

void UDPServer::checkingNickname(QString nickname, QHostAddress peer, quint16 port){
    QSqlQuery query;
    query.prepare("SELECT ID FROM users WHERE Nickname=?");
    query.bindValue(0, nickname);

    query.exec();

    QString id="";
    while (query.next())
        id = query.value(0).toString();

    if(id != "")
        systemSocket->writeDatagram("EXIST", peer, port);
    else
        systemSocket->writeDatagram("NEXIST", peer, port);
}

void UDPServer::answersChecker(QByteArray index){
    answers.push_back(index.toShort());
}

void UDPServer::systemReading(){
    QByteArray buffer;
    quint16 port;
    QHostAddress peer;

    buffer.resize(systemSocket->pendingDatagramSize());
    systemSocket->readDatagram(buffer.data(), buffer.size(), &peer, &port);

    QStringList list = QString(buffer).split('|');
    if(list.at(0) == "handshake")
        emit systemReceived(list, peer, port);
    else if(list.at(0) == "DoesExNick")
        emit systemReceived(list.at(1), peer, port);
    else
        emit systemReceived(buffer);
}
