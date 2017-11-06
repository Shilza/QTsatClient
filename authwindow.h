#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QtSvg/QSvgWidget>
#include <QLineEdit>
#include <QUdpSocket>
#include <QPushButton>
#include <QMouseEvent>
#include <QRegExp>
#include <QRegExpValidator>
#include <QPropertyAnimation>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QFontMetrics>
#include <QGraphicsOpacityEffect>
#include <QDateTime>
#include <QTime>
#include <QTimer>
#include <QNetworkConfigurationManager>
#include <QPicture>
#include <QPixmap>
#include "def.h"

#define sizeX 260
#define sizeY 260
#define DURATION 300
#define LOC_SIGNIN 1
#define LOC_SIGNUP 2
#define LOC_RECOVERY_EMAIL 3
#define LOC_RECOVERY_CODE 4
#define LOC_RECOVERY_PASS 5

namespace Ui {
class AuthWindow;
}

class LineEdit : public QLineEdit{
    Q_OBJECT
public:
    LineEdit(QWidget *parent=0) : QLineEdit(parent){ init(); }
    LineEdit(const QString &contents, QWidget *parent=0) : QLineEdit(contents,parent){ init(); }

private:
    void keyPressEvent(QKeyEvent *event);
    void init();

signals:
    void hoverEnter();
    void hoverLeave();


private slots:
    void showMenu(QPoint position){}
};

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget* parent=0, bool isUnderlined=true);
    ~ClickableLabel();
signals:
    void released();
private:
    bool isUnderlined;
    void mouseReleaseEvent(QMouseEvent* event);
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
};

class AuthWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::AuthWindow *ui;
    int defaultFontSize;
    bool nicknameExists=false;

    quint16 defaultY, defaultLineX, defaultButtonX;
    quint16 lineW, lineH;
    quint16 buttonW, buttonH;
    quint16 lineHWithSpace, buttonHWithSpace;

    quint8 location=LOC_SIGNIN;

    LineEdit *lineLog;
    LineEdit *linePass;
    LineEdit *lineConfirmPass;
    LineEdit *lineEmail;
    LineEdit *lineConfirmCode;
    LineEdit *lineRecoveryPass;
    LineEdit *lineRecoveryConfirmPass;
    QPushButton *buttonSignIn;
    QPushButton *buttonSignUp;
    QPushButton *buttonOk;
    QLabel *labelUncorrectNickname;

    QPushButton *buttonClose;
    QPushButton *buttonEye;
    QPushButton *buttonMinimize;
    ClickableLabel *labelForgotPass;
    ClickableLabel *labelSignUp;
    ClickableLabel *labelSignIn;
    ClickableLabel *labelConnectionFailed;
    ClickableLabel *labelConnectionFailedBackground;
    ClickableLabel *labelRegistrationSuccessful;

    QUdpSocket *socket;
    QHostAddress host;

    QSvgWidget *preloader;
    QGraphicsOpacityEffect *opacity;
    QPoint mpos;
    QTimer *waitingAnswerTimer;

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
    void sessionKeyReceived(QByteArray sessionKey);
    void loadingWasStart();
    void connectionFailed();
    void closingErrorLabel();
private slots:
    void socketReading();
    void signIn_released();
    void handshakeSend();
    void signUp_released();
    void registrationSend();
    void registrationSuccess();
    void buttonOk_released();
    void recoveryEmailSend();
    void recoveryCodeSend();
    void signInLabel_released();
    void signUpLabel_released();
    void forgotPassLabel_released();
    void waitingAnswer();
    void checkingNickname();
    void checkingEmail();
    void checkingConfirming(QString text);
    void emailChange();
    void logChange();
    void passChange();
    void eye_released();
    void buttonMinimize_released();
    void startPreloader();
    void cancelPreloading();
    void errorHide();
    void test();
};

#endif // AUTHWINDOW_H
