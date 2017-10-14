#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <thread>
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

  private:
    class Session;
    QUdpSocket *socket, *systemSocket;
    QByteArray message;
    QVector<std::shared_ptr<Session>> sessions;
  signals:
      void isReceived();
  public slots:
      void sendReceived();
      void read();
      void handshake();
};

#endif // UDPSERVER_H
