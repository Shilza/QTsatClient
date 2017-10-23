#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QUdpSocket>
#include <iostream>
#include <QPropertyAnimation>
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
    QLabel *errorLabel;

    QPushButton *closeButton;

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
