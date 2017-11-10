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

#define DURATION 300
#define LOC_SIGNIN 1
#define LOC_REGISTRATION 2
#define LOC_RECOVERY_EMAIL 3
#define LOC_RECOVERY_CODE 4
#define LOC_RECOVERY_PASS 5
#define LOC_REGISTRATION_CODE 6

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
    void showMenu(QPoint){}
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
    QPushButton *buttonClose;
    QPushButton *buttonEye;
    QPushButton *buttonRecoveryEye;
    QPushButton *buttonMinimize;

    QLabel *labelUncorrectNickname;
    QLabel *labelPassword;
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

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void changeEvent(QEvent *e);
    void resizeAll();

public:
    explicit AuthWindow(QMainWindow *parent = 0);
    bool eventFilter(QObject *watched, QEvent *event);
    ~AuthWindow();

signals:
    void sessionKeyReceived(QByteArray sessionKey);
    void loadingWasStart();
    void error_signal();
    void closingErrorLabel();

private slots:
    void socketReading();
    void signIn_released();
    void authorizationSend();
    void signUp_released();
    void registrationSend();
    void registrationCodeSend();
    void labelSuccessHide();
    void buttonOk_released();
    void recoveryEmailSend();
    void recoveryCodeSend();
    void recoveryNewPassSend();
    void signInLabel_released();
    void signUpLabel_released();
    void forgotPassLabel_released();
    void waitingAnswer();
    void checkingNickname();
    void checkingEmail();
    void checkingConfirming(QString);
    void checkingRecoveryConfirming(QString);
    void emailChange();
    void logChange();
    void passChange();
    void passRecoveryChange();
    void codeChange();
    void eye_released();
    void eyeRecovery_released();
    void buttonMinimize_released();
    void startPreloading();
    void cancelPreloading();
    void errorHide();
    void test();
    void test2();
};

#endif // AUTHWINDOW_H
