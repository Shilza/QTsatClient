#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <thread>
#include <chrono>
#include <memory>
#include <QtSql/QSql>
#include <QtSql/QSqlQuery>
#include <QVector>
#include <QDateTime>
#include <QUdpSocket>
#include <QStringList>
#include <QCryptographicHash>

class UDPServer : public QObject
{
  Q_OBJECT

  public:
      explicit UDPServer(QObject *parent = 0);
    void start();

  private:
    class Session;
    QUdpSocket *socket, *systemSocket;
    QVector<std::shared_ptr<Session>> sessions;
    QVector<short> answers;
    QString check(QByteArray sessionKey);
    bool findInAnswers(int i);
  signals:
      void isReceived(QByteArray message);
      void systemReceived(QStringList list, QHostAddress ip, quint16 port);
      void systemReceived(QByteArray index);
  public slots:
      void sendReceived(QByteArray message);
      void read();
      void handshake(QStringList list, QHostAddress peer, quint16 port);
      void answersChecker(QByteArray index);
      void systemReading();
};

#endif // UDPSERVER_H
