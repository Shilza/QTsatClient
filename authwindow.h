#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QUdpSocket>
#include "def.h"

namespace Ui {
class AuthWindow;
}

class AuthWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::AuthWindow *ui;

    QLineEdit *log;
    QLineEdit *pass;
    QPushButton *signIn;

    QUdpSocket *socket;
    QHostAddress host;

    void handshaking(QString log, QString pass);
public:
    explicit AuthWindow(QWidget *parent = 0);
    ~AuthWindow();
signals:
    void sessionKeyReceived();
    void sessionKeyReceived(QByteArray sessionKey);
private slots:
    void socketReading();
    void signIn_released();
};

#endif // AUTHWINDOW_H
