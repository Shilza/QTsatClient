#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QUdpSocket>
#include <QPushButton>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include "def.h"

namespace Ui {
class AuthWindow;
}

class ClickableLabel : public QLabel
{
Q_OBJECT
public:
    explicit ClickableLabel( QWidget* parent=0 );
    ~ClickableLabel();
signals:
    void released();
protected:
    void mouseReleaseEvent(QMouseEvent* event);
};

class AuthWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::AuthWindow *ui;

    const int sizeX=260;
    const int sizeY=260;

    QLineEdit *log;
    QLineEdit *pass;
    QPushButton *signIn;
    QLabel *errorLabel;

    QPushButton *closeButton;
    QPushButton *eye;
    ClickableLabel *forgotPass;
    ClickableLabel *signUp;

    QUdpSocket *socket;
    QHostAddress host;

    QPoint mpos;

    void handshaking(QString log, QString pass);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
public:
    explicit AuthWindow(QMainWindow *parent = 0);

    ~AuthWindow();
signals:
    void sessionKeyReceived();
    void sessionKeyReceived(QByteArray sessionKey);
private slots:
    void socketReading();
    void signIn_released();
    void forgotPass_released();
    void signUp_released();
    void eye_released();
};

#endif // AUTHWINDOW_H
