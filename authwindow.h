#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QUdpSocket>
#include <QPushButton>
#include <QMouseEvent>
#include <QRegExp>
#include <QRegExpValidator>
#include <QPropertyAnimation>
#include <QDesktopWidget>
#include "def.h"


#define sizeX 260
#define sizeY 260
#define DURATION 300
#define LOC_SIGNIN 1
#define LOC_SIGNUP 2
#define LOC_RECOVERY 3

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

    quint8 location=LOC_SIGNIN;

    QLineEdit *lineLog;
    QLineEdit *linePass;
    QLineEdit *lineConfirmPass;
    QLineEdit *lineEmail;
    QPushButton *buttonSignIn;
    QPushButton *buttonSignUp;
    QPushButton *buttonOk;
    QLabel *labelError;

    QPushButton *buttonClose;
    QPushButton *buttonEye;
    QPushButton *buttonMinimize;
    ClickableLabel *labelForgotPass;
    ClickableLabel *labelSignUp;
    ClickableLabel *labelSignIn;

    QUdpSocket *socket;
    QHostAddress host;

    QPoint mpos;

    void handshaking(QString lineLog, QString linePass);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void changeEvent(QEvent* e);
    void resizeAll();
public:
    explicit AuthWindow(QMainWindow *parent = 0);
    bool eventFilter(QObject *watched, QEvent *event);
    ~AuthWindow();
signals:
    void sessionKeyReceived();
    void sessionKeyReceived(QByteArray sessionKey);
private slots:
    void socketReading();
    void signIn_released();
    void forgotPass_released();
    void signUpLabel_released();
    void signInLabel_released();
    void passRecovery_released();
    void checkingNickname();
    void checkingConfirming(QString text);
    void logChange(QString text);
    void eye_released();
    void buttonMinimize_released();
};

#endif // AUTHWINDOW_H
