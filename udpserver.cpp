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
    connect(systemSocket, SIGNAL(readyRead()), this, SLOT(systemReading()));
    connect(this, SIGNAL(systemReceived(QByteArray)), this, SLOT(answersChecker(QByteArray)));
    connect(this, SIGNAL(systemReceived(QStringList, QHostAddress, quint16)), this, SLOT(handshake(QStringList,QHostAddress,quint16)));
    connect(this, SIGNAL(isReceived(QByteArray)), this, SLOT(sendReceived(QByteArray)));

    std::thread sessionsCheckerThread([&](){
        while(true){
            answers.clear();
            unsigned int time=QDateTime::currentDateTime().toTime_t();
            for(int i=0; i<sessions.size(); i++)
                if(time > sessions[i].get()->time+10){
                    systemSocket->writeDatagram(QByteArray::number(i), sessions[i].get()->IP, 49002);
                    qDebug() << QByteArray::number(i) << sessions[i].get()->IP;
                }
            std::this_thread::sleep_for(std::chrono::seconds(2));

            time=QDateTime::currentDateTime().toTime_t();
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
            qDebug() << "I FOUND";
            answers.erase(answers.begin()+i);
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

        QSqlQuery query;
        query.prepare("INSERT INTO messages (Sender, Text, Time) VALUES (:sender, :text, :time)");
        query.bindValue(":sender", nickname);
        query.bindValue(":text", finalMessage);
        query.bindValue(":time", QDateTime::currentDateTime().toTime_t());
        query.exec();

        finalMessage.push_front(nickname);

        for(int i=0; i<sessions.size(); i++)
            socket->writeDatagram(finalMessage.toUtf8(), sessions[i].get()->IP, 49000);
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

    if(id == "")
        return;

    sessions.push_back(shared_ptr<Session>(new Session(list.at(1), peer)));

    systemSocket->writeDatagram(sessions[sessions.size()-1].get()->sessionKey, peer, port);
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

    qDebug() << "Buffer: " << buffer;
    QStringList list = QString(buffer).split('|');
    if(list.at(0) == "handshake")
        emit systemReceived(list, peer, port);
    else
        emit systemReceived(buffer);
}
