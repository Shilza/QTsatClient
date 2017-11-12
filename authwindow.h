#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QtSvg/QSvgWidget>
#include <QLineEdit>
#include <QUdpSocket>
#include <QPushButton>
#include <QMouseEvent>
#include <QRegExpValidator>
#include <QPropertyAnimation>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QFontMetrics>
#include <QGraphicsOpacityEffect>
#include <QDateTime>
#include <QTime>
#include <QTimer>
#include <QBrush>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "def.h"

#define DURATION 300
#define LOC_SIGNIN 1
#define LOC_REGISTRATION 2
#define LOC_REGISTRATION_CODE 3
#define LOC_RECOVERY_EMAIL 4
#define LOC_RECOVERY_CODE 5
#define LOC_RECOVERY_PASS 6

namespace Ui {
class AuthWindow;
}

class LineEdit : public QLineEdit{
    Q_OBJECT

public:
    LineEdit(QWidget *parent=0, bool isDefault = true);
    void setDisabledOverride();
    void setEnabledOverride();
private:
    int defaultFontSize;
    void keyPressEvent(QKeyEvent *event);
private slots:
    void showMenu(QPoint){}

public slots:
    void setDefaultStyleSheet();
    void setErrorStyleSheet();
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
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
};

class AuthWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::AuthWindow *ui;
    int defaultFontSize;
    bool nicknameExists = false;
    bool isPassEmpty = false;
    bool isRecoveryPassEmpty = false;

    quint8 location=LOC_SIGNIN;

    quint16 defaultY, defaultLineX, defaultButtonX;
    quint16 lineW, lineH;
    quint16 buttonW, buttonH;
    quint16 lineHWithSpace, buttonHWithSpace;

    LineEdit *lineEmail;
    LineEdit *lineLog;
    LineEdit *linePass;
    LineEdit *lineConfirmPass;
    LineEdit *lineConfirmCode;
    LineEdit *lineRecoveryPass;
    LineEdit *lineRecoveryConfirmPass;

    QPushButton *buttonSignIn;
    QPushButton *buttonSignUp;
    QPushButton *buttonOk;
    QPushButton *buttonClose;
    QPushButton *buttonEye;
    QPushButton *buttonRecoveryEye;
    QPushButton *buttonMinimize;

    QLabel *labelUncorrectNickname;
    QLabel *labelPass;
    QLabel *labelRecoveryPass;
    ClickableLabel *labelForgotPass;
    ClickableLabel *labelSignUp;
    ClickableLabel *labelSignIn;
    ClickableLabel *labelConnectionFailed;
    ClickableLabel *labelConnectionFailedBackground;
    ClickableLabel *labelSuccess;

    QUdpSocket *socket;
    QHostAddress host;

    QSvgWidget *preloader;
    QGraphicsOpacityEffect *opacity;
    QGraphicsOpacityEffect *opacityLabel;
    QPoint mpos;

    QTimer *timerWaitingAnswer;
    QTimer *timerLabelSuccess;
    QTimer *timerErrorLabel;

    bool isOnline();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void changeEvent(QEvent *e);
    void setPassEnabled();
    void resizeAll();

public:
    explicit AuthWindow(QMainWindow *parent = 0);
    bool eventFilter(QObject *watched, QEvent *event);
    ~AuthWindow();

signals:
    void sessionKeyReceived(QByteArray sessionKey);
    void loadingWasStart();
    void errorHasOccured();

private slots:
    void socketReading();

    void signIn_released();
    void authorizationSend();
    void signUp_released();
    void registrationSend();
    void registrationCodeSend();
    void buttonOk_released();
    void recoveryEmailSend();
    void recoveryCodeSend();
    void recoveryNewPassSend();

    void gotoSignInLoc();
    void gotoSignUpLoc();
    void gotoRecoveryLoc();

    void waitingAnswer();

    void checkingNickname();
    void checkingEmail();
    void checkingConfirming(QString);
    void checkingRecoveryConfirming(QString);

    void changingPassEchoMode();
    void changingRecoveryPassEchoMode();
    void changingPassBorder();
    void changingRecoveryPassBorder();

    void buttonMinimize_released();

    void startPreloading();
    void cancelPreloading();

    void errorHide();
    void labelSuccessHide();
    void test();
    void test2();
};

#endif // AUTHWINDOW_H
