#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QUdpSocket>
#include <QPushButton>
#include <QMouseEvent>
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

    QPoint mpos;

    void handshaking(QString log, QString pass);

public:
    explicit AuthWindow(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

    ~AuthWindow();
signals:
    void sessionKeyReceived();
    void sessionKeyReceived(QByteArray sessionKey);
private slots:
    void socketReading();
    void signIn_released();
};

#endif // AUTHWINDOW_H
