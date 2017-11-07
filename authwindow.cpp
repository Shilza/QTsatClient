#include "authwindow.h"
#include "ui_authwindow.h"

AuthWindow::AuthWindow(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::AuthWindow)
{
    ui->setupUi(this);
    socket = new QUdpSocket(this);

    socket->bind(49002);
    host.setAddress(HOST_IP);

    timerWaitingAnswer = new QTimer(this);
    timerWaitingAnswer->setSingleShot(true);

    timerLabelSuccess = new QTimer(this);
    timerLabelSuccess->setSingleShot(true);

    timerErrorLabel = new QTimer(this);
    timerErrorLabel->setSingleShot(true);

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setStyleSheet("background: #C8A4E5;");
    this->setStyleSheet("background-image: url(:fon/fon.png);");

    QTime randTime(0,0,0);
    qsrand(randTime.secsTo(QTime::currentTime()));

    lineLog = new LineEdit(this);
    linePass = new LineEdit(this);
    lineConfirmPass = new LineEdit(this);
    lineEmail = new LineEdit(this);
    lineConfirmCode = new LineEdit(this);
    lineRecoveryPass = new LineEdit(this);
    lineRecoveryConfirmPass = new LineEdit(this);
    buttonSignIn = new QPushButton(this);
    buttonSignUp = new QPushButton(this);
    buttonOk = new QPushButton(this);
    buttonMinimize = new QPushButton(this);
    buttonClose = new QPushButton(this);
    buttonEye = new QPushButton(linePass);
    labelForgotPass = new ClickableLabel(this);
    labelSignUp = new ClickableLabel(this);
    labelSignIn = new ClickableLabel(this);
    labelSuccess = new ClickableLabel(this, false);
    labelUncorrectNickname = new QLabel(this);
    labelConnectionFailedBackground = new ClickableLabel(this);
    labelConnectionFailed = new ClickableLabel(labelConnectionFailedBackground, false);

    preloader = new QSvgWidget(this);
    opacity = new QGraphicsOpacityEffect;

    buttonClose->installEventFilter(this);
    buttonMinimize->installEventFilter(this);
    lineLog->installEventFilter(this);

    buttonEye->setCursor(Qt::ArrowCursor);
    buttonSignIn->setCursor(Qt::PointingHandCursor);
    buttonOk->setCursor(Qt::PointingHandCursor);
    buttonSignUp->setCursor(Qt::PointingHandCursor);

    labelSignUp->setCursor(Qt::PointingHandCursor);
    labelSignIn->setCursor(Qt::PointingHandCursor);
    labelForgotPass->setCursor(Qt::PointingHandCursor);

    lineEmail->setValidator(new QRegExpValidator(QRegExp("([^@])+(@?)([^@])+")));
    lineLog->setValidator(new QRegExpValidator(QRegExp("([^@])+(@?)([^@])+")));
    linePass->setValidator(new QRegExpValidator(QRegExp("^(\\d|\\w|[._]){6,32}$")));

    linePass->setEchoMode(QLineEdit::Password);
    lineConfirmPass->setEchoMode(QLineEdit::Password);

    lineLog->setPlaceholderText("Login or email");
    linePass->setPlaceholderText("Password");
    lineConfirmPass->setPlaceholderText("Confirm password");
    lineEmail->setPlaceholderText("Email");
    lineConfirmCode->setPlaceholderText("Confirmation code");
    lineRecoveryPass->setPlaceholderText("New password");
    lineRecoveryConfirmPass->setPlaceholderText("Confirm new password");
    buttonSignIn->setText("Sign in");
    buttonSignUp->setText("Sign up");
    buttonOk->setText("Ok");
    labelForgotPass->setText("Forgot password?");
    labelForgotPass->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    labelSignUp->setText("Sign up");
    labelSignUp->setAlignment(Qt::AlignRight | Qt::AlignTop);
    labelSignIn->setText("Sign in");
    labelUncorrectNickname->setText("Nickname already exists");
    labelUncorrectNickname->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    labelConnectionFailed->setText("Wrong login or password");
    labelConnectionFailed->setAlignment(Qt::AlignCenter|Qt::AlignTop);
    labelSuccess->setAlignment(Qt::AlignCenter);

    setMouseTracking(true);
    lineLog->setMouseTracking(true);
    preloader->load(QString(":fon/pre.svg"));

    resizeAll();

    QString strButtonStyle =  "QPushButton{"
                              "font-family: Century Gothic;"
                              "font-size: %1px;"
                              "border-radius: 6px;"
                              "border: 1px solid black;"
                              "background: rgba(172,230,168,245);"
                              "}"
                              "QPushButton:focus:pressed{"
                              "border: 2px solid #63E3E9;"
                              "background: rgba(172,230,168,210);"
                              "}"
                              "QPushButton:hover{"
                              "border-radius: 6px;"
                              "border: 1px solid #63E3E9;"
                              "background: rgba(172,230,168,220);"
                              "}";

    defaultFontSize = (width()/260)*11;
    buttonSignIn->setStyleSheet(strButtonStyle.arg(defaultFontSize));
    buttonSignUp->setStyleSheet(strButtonStyle.arg(defaultFontSize));
    buttonOk->setStyleSheet(strButtonStyle.arg(defaultFontSize));

    labelForgotPass->setStyleSheet(QString("font-family: Century Gothic;"
                                           "font-size: %1px;"
                                           "background: transparent;"
                                           "color: #B5EBEE;").arg(defaultFontSize));

    labelSignUp->setStyleSheet(QString("font-family: Century Gothic;"
                                       "font-size: %1px;"
                                       "background: transparent;"
                                       "color: #B5EBEE;").arg(defaultFontSize));

    labelSignIn->setStyleSheet(QString("font-family: Century Gothic;"
                                       "font-size: %1px;"
                                       "background: transparent;"
                                       "color: #B5EBEE;").arg(defaultFontSize));

    buttonEye->setStyleSheet("QPushButton{background: transparent;}"
                             "QPushButton:hover{background: rgba(123,55,65,50);}");

    buttonEye->setIcon(QIcon(":fon/eye.png"));

    lineEmail->setStyleSheet(QString("font-family: Century Gothic;"
                                     "font-size: %1px;"
                                     "background: transparent;"
                                     "color: #B5EBEE;").arg(defaultFontSize));

    lineLog->setStyleSheet(QString("font-family: Century Gothic;"
                                   "font-size: %1px;"
                                   "background: transparent;"
                                   "color: #B5EBEE;").arg(defaultFontSize));

    lineConfirmPass->setStyleSheet(QString("font-family: Century Gothic;"
                                           "font-size: %1px;"
                                           "background: transparent;"
                                           "color: #B5EBEE;").arg(defaultFontSize));
    linePass->setStyleSheet(QString("font-family: Century Gothic;"
                                    "font-size: %1px;"
                                    "background: transparent;"
                                    "color: #B5EBEE;").arg(defaultFontSize));
    lineConfirmCode->setStyleSheet(QString("font-family: Century Gothic;"
                                           "font-size: %1px;"
                                           "background: transparent;"
                                           "color: #B5EBEE;").arg(defaultFontSize));
    lineRecoveryPass->setStyleSheet(QString("font-family: Century Gothic;"
                                            "font-size: %1px;"
                                            "background: transparent;"
                                            "color: #B5EBEE;").arg(defaultFontSize));
    lineRecoveryConfirmPass->setStyleSheet(QString("font-family: Century Gothic;"
                                            "font-size: %1px;"
                                            "background: transparent;"
                                            "color: #B5EBEE;").arg(defaultFontSize));

    buttonClose->setStyleSheet("QPushButton {"
                               "qproperty-icon: url(:/fon/close1.png);"
                               "border: 0px;"
                               "background: transparent;"
                               "}"
                               "QPushButton:hover{"
                               "background: #fd3755;"
                               "border: 0px;"
                               "}");

    buttonMinimize->setStyleSheet("QPushButton {"
                                  "qproperty-icon: url(:/fon/min1.png);"
                                  "border: 0px;"
                                  "background: transparent;"
                                  "}"
                                  "QPushButton:hover{"
                                  "background: #939494;"
                                  "border: 0px;"
                                  "}");

    labelConnectionFailed->setStyleSheet(QString("font-family: Century Gothic;"
                                                 "font-size: %1px;"
                                                 "background: transparent;"
                                                 "color: red;").arg(defaultFontSize/11*13));
    labelSuccess->setStyleSheet(QString("font-family: Century Gothic;"
                                                       "font-size: %1px;"
                                                       "background: rgba(255,255,255,200);").arg(defaultFontSize/11*13));
    preloader->setStyleSheet("background:transparent;");

    labelConnectionFailedBackground->setStyleSheet("background:transparent;");
    labelConnectionFailedBackground->setPixmap(QPixmap(":fon/gradient.png").scaled(width(),labelConnectionFailedBackground->height()));

    QFont fontGothic("Century Gothic");
    fontGothic.setBold(true);
    fontGothic.setPointSize((defaultFontSize/11)*8);

    labelUncorrectNickname->setStyleSheet("background: transparent;"
                                          "color: red;");
    labelUncorrectNickname->setFont(fontGothic);

    QFontMetrics *tempFontSize = new QFontMetrics(labelUncorrectNickname->font());
    labelUncorrectNickname->resize(QSize(tempFontSize->width(labelUncorrectNickname->text()), tempFontSize->height()));
    delete tempFontSize;

    labelSignIn->close();
    buttonOk->close();
    buttonSignUp->close();
    lineConfirmPass->close();
    lineEmail->close();
    labelUncorrectNickname->close();
    preloader->close();
    labelConnectionFailedBackground->close();
    lineConfirmCode->close();
    labelSuccess->close();

    QPushButton *btn = new QPushButton(this);

    connect(btn, SIGNAL(released()), this, SLOT(test()));
    connect(this, SIGNAL(loadingWasStart()),this, SLOT(startPreloading()));
    connect(buttonSignIn, SIGNAL(released()), this, SLOT(signIn_released()));
    connect(buttonSignUp, SIGNAL(released()), this, SLOT(signUp_released()));
    connect(buttonOk, SIGNAL(released()), this, SLOT(buttonOk_released()));
    connect(buttonEye, SIGNAL(released()), this, SLOT(eye_released()));
    connect(labelSignUp, SIGNAL(released()),this, SLOT(signUpLabel_released()));
    connect(labelSignIn, SIGNAL(released()),this, SLOT(signInLabel_released()));
    connect(labelForgotPass, SIGNAL(released()), this,SLOT(forgotPassLabel_released()));
    connect(labelSuccess, SIGNAL(released()), this, SLOT(labelSuccessHide()));

    connect(lineEmail, SIGNAL(textChanged(QString)), this, SLOT(emailChange()));
    connect(lineEmail, SIGNAL(editingFinished()), this, SLOT(checkingEmail()));
    connect(lineLog, SIGNAL(textChanged(QString)), this, SLOT(logChange()));
    connect(lineLog, SIGNAL(editingFinished()), this, SLOT(checkingNickname()));
    connect(linePass, SIGNAL(textChanged(QString)), this, SLOT(passChange()));
    connect(lineConfirmPass, SIGNAL(textChanged(QString)),this, SLOT(checkingConfirming(QString)));
    connect(lineRecoveryPass, SIGNAL(textChanged(QString)), this, SLOT(passRecoveryChange()));
    connect(lineRecoveryConfirmPass, SIGNAL(textChanged(QString)),this, SLOT(checkingRecoveryConfirming(QString)));
    connect(lineConfirmCode, SIGNAL(textChanged(QString)), this, SLOT(codeChange()));

    connect(socket, SIGNAL(readyRead()),this,SLOT(socketReading()));
    connect(buttonClose, SIGNAL(released()), this, SLOT(close()));
    connect(buttonMinimize, SIGNAL(released()), this, SLOT(buttonMinimize_released()));
    connect(labelConnectionFailed, SIGNAL(released()),this,SLOT(errorHide()));
    connect(this, SIGNAL(closingErrorLabel()),this,SLOT(errorHide()));
    connect(this, SIGNAL(connectionFailed()), this, SLOT(cancelPreloading()));

    connect(timerWaitingAnswer, SIGNAL(timeout()), this, SLOT(waitingAnswer()));
    connect(timerLabelSuccess, SIGNAL(timeout()), this, SLOT(labelSuccessHide()));
    connect(timerErrorLabel, SIGNAL(timeout()), this, SLOT(errorHide()));

}

void AuthWindow::test()
{
    timerWaitingAnswer->stop();
    if(location==LOC_RECOVERY_EMAIL){
        preloader->close();
        location = LOC_RECOVERY_CODE;
        QPropertyAnimation *animations[4];
        animations[0] = new QPropertyAnimation(lineConfirmCode, "pos");
        animations[1] = new QPropertyAnimation(buttonOk, "pos");
        animations[2] = new QPropertyAnimation(labelSignIn, "pos");
        animations[3] = new QPropertyAnimation(labelSignUp, "pos");

        lineConfirmCode->show();
        animations[0]->setDuration(DURATION);
        animations[0]->setStartValue(QPoint(defaultLineX, height()));
        animations[0]->setEndValue(QPoint(defaultLineX, defaultY+lineHWithSpace));
        animations[0]->start(QAbstractAnimation::DeleteWhenStopped);

        opacity->setOpacity(1.0);
        buttonOk->show();
        animations[1]->setDuration(DURATION);
        animations[1]->setStartValue(QPoint(buttonOk->x(), height()));
        animations[1]->setEndValue(QPoint(buttonOk->x(), defaultY+(2*lineHWithSpace)));
        animations[1]->start(QAbstractAnimation::DeleteWhenStopped);

        animations[2]->setDuration(DURATION);
        animations[2]->setEndValue(QPoint(labelSignIn->x(), defaultY+(2*lineHWithSpace)+buttonHWithSpace));
        animations[2]->start(QAbstractAnimation::DeleteWhenStopped);

        animations[3]->setDuration(DURATION);
        animations[3]->setEndValue(QPoint(labelSignUp->x(), defaultY+(2*lineHWithSpace)+buttonHWithSpace));
        animations[3]->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else if(location==LOC_RECOVERY_CODE){
        location = LOC_RECOVERY_PASS;
        preloader->close();
        QPropertyAnimation *animations[6];
        animations[0] = new QPropertyAnimation(lineConfirmCode, "pos");
        animations[1] = new QPropertyAnimation(lineRecoveryPass, "pos");
        animations[2] = new QPropertyAnimation(lineRecoveryConfirmPass, "pos");
        animations[3] = new QPropertyAnimation(opacity, "opacity");
        animations[4] = new QPropertyAnimation(labelSignIn, "pos");
        animations[5] = new QPropertyAnimation(labelSignUp, "pos");

        lineRecoveryPass->show();
        lineRecoveryConfirmPass->show();
        lineRecoveryPass->setEnabled(true);
        lineRecoveryConfirmPass->setEnabled(true);

        animations[0]->setEndValue(QPoint(width(), lineConfirmCode->y()));
        animations[1]->setStartValue(QPoint(defaultLineX, height()));
        animations[1]->setEndValue(QPoint(defaultLineX, defaultY+lineHWithSpace));
        animations[2]->setStartValue(QPoint(defaultLineX, height()+lineHWithSpace));
        animations[2]->setEndValue(QPoint(defaultLineX, defaultY+(2*lineHWithSpace)));
        animations[3]->setEndValue(1.0);
        animations[4]->setEndValue(QPoint(labelSignIn->x(), defaultY+(3*lineHWithSpace)+buttonHWithSpace));
        animations[5]->setEndValue(QPoint(labelSignUp->x(), defaultY+(3*lineHWithSpace)+buttonHWithSpace));

        buttonOk->move(defaultButtonX, defaultY+(3*lineHWithSpace));
        buttonOk->show();
        connect(animations[0], SIGNAL(finished()), lineConfirmCode, SLOT(clear()));
        for(int i=0; i<6; i++){
            animations[i]->setDuration(DURATION);
            animations[i]->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
    else if(location==LOC_RECOVERY_PASS){
        labelSuccess->setText("Your password has been changed succesfully");
        labelSuccess->setGraphicsEffect(opacity);
        location=LOC_SIGNIN;
        lineLog->setEnabled(true);
        preloader->close();
        labelSignIn->close();

        QPropertyAnimation *animations[5];
        animations[0] = new QPropertyAnimation(lineRecoveryConfirmPass, "pos");
        animations[1] = new QPropertyAnimation(buttonSignIn, "pos");
        animations[2] = new QPropertyAnimation(labelSignUp, "pos");
        animations[3] = new QPropertyAnimation(labelForgotPass, "pos");
        animations[4] = new QPropertyAnimation(opacity, "opacity");

        animations[0]->setEndValue(QPoint(width(), lineRecoveryConfirmPass->y()));
        animations[1]->setStartValue(QPoint(defaultButtonX, height()));
        animations[1]->setEndValue(QPoint(defaultButtonX, defaultY+(2*lineHWithSpace)));
        animations[2]->setStartValue(QPoint(defaultLineX+lineW-labelSignUp->width(), height()+buttonHWithSpace));
        animations[2]->setEndValue(QPoint(defaultLineX+lineW-labelSignUp->width(), defaultY+(2*lineHWithSpace)+buttonHWithSpace));
        animations[3]->setStartValue(QPoint(defaultLineX, height()+buttonHWithSpace));
        animations[3]->setEndValue(QPoint(defaultLineX, defaultY+(2*lineHWithSpace)+buttonHWithSpace));
        animations[4]->setStartValue(0.0);
        animations[4]->setEndValue(1.0);

        animations[0]->setDuration(DURATION);
        animations[1]->setDuration(DURATION);
        animations[2]->setDuration(DURATION);
        animations[3]->setDuration(DURATION);
        animations[4]->setDuration(DURATION);
        labelSuccess->show();

        buttonSignIn->show();
        labelSignUp->show();
        labelForgotPass->show();
        linePass->move(lineRecoveryPass->x(), lineRecoveryPass->y());
        lineRecoveryPass->close();
        lineRecoveryPass->clear();
        lineRecoveryConfirmPass->clear();
        linePass->show();
        connect(animations[0], SIGNAL(finished()), lineRecoveryConfirmPass, SLOT(close()));
        animations[0]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[1]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[2]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[3]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[4]->start(QAbstractAnimation::DeleteWhenStopped);

        timerLabelSuccess->start(5000);
    }
}

void AuthWindow::socketReading()
{
    timerWaitingAnswer->stop();

    QByteArray serverAnswer;
    serverAnswer.resize(socket->pendingDatagramSize());
    socket->readDatagram(serverAnswer.data(),serverAnswer.size());

    if(serverAnswer=="ERROR_AUTH"){
        //CHECK
        cancelPreloading();
        labelConnectionFailed->setText("Wrong login or password");
        emit connectionFailed();
    }
    else if(serverAnswer=="NICKEXIST"){
        //CHECK
        nicknameExists = true;
        lineLog->setStyleSheet(QString("font-family: Century Gothic;"
                                       "font-size: %1px;"
                                       "background: transparent;"
                                       "border: 1px solid red;"
                                       "color: #B5EBEE;").arg(defaultFontSize));
    }
    else if(serverAnswer=="NICKNEXIST"){
        nicknameExists = false;
        lineLog->setStyleSheet(QString("font-family: Century Gothic;"
                                       "font-size: %1px;"
                                       "background: transparent;"
                                       "color: #B5EBEE;").arg(defaultFontSize));
    }
    else if(serverAnswer=="EMAILEXIST"){
        //DO
    }
    else if(serverAnswer=="EMAILNEXIST"){
        //DO
    }
    else if(serverAnswer=="RECOVERYFOUND"){
        preloader->close();
        location = LOC_RECOVERY_CODE;
        QPropertyAnimation *animations[4];
        animations[0] = new QPropertyAnimation(lineConfirmCode, "pos");
        animations[1] = new QPropertyAnimation(buttonOk, "pos");
        animations[2] = new QPropertyAnimation(labelSignIn, "pos");
        animations[3] = new QPropertyAnimation(labelSignUp, "pos");

        lineConfirmCode->show();
        animations[0]->setDuration(DURATION);
        animations[0]->setEndValue(QPoint(lineConfirmCode->x(), defaultY+lineHWithSpace));
        animations[0]->start(QAbstractAnimation::DeleteWhenStopped);

        opacity->setOpacity(1.0);
        buttonOk->show();
        animations[1]->setDuration(DURATION);
        animations[1]->setStartValue(QPoint(buttonOk->x(), height()));
        animations[1]->setEndValue(QPoint(buttonOk->x(), defaultY+(2*lineHWithSpace)));
        animations[1]->start(QAbstractAnimation::DeleteWhenStopped);

        animations[2]->setDuration(DURATION);
        animations[2]->setEndValue(QPoint(labelSignIn->x(), defaultY+(2*lineHWithSpace)+buttonHWithSpace));
        animations[2]->start(QAbstractAnimation::DeleteWhenStopped);

        animations[3]->setDuration(DURATION);
        animations[3]->setEndValue(QPoint(labelSignUp->x(), defaultY+(2*lineHWithSpace)+buttonHWithSpace));
        animations[3]->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else if(serverAnswer=="RECOVERYNFOUND"){
        //DO
    }
    else if(serverAnswer=="RIGHT_CODE"){
        preloader->close();
        location = LOC_RECOVERY_PASS;
        QPropertyAnimation *animations[6];
        animations[0] = new QPropertyAnimation(lineConfirmCode, "pos");
        animations[1] = new QPropertyAnimation(lineRecoveryPass, "pos");
        animations[2] = new QPropertyAnimation(lineRecoveryConfirmPass, "pos");
        animations[3] = new QPropertyAnimation(opacity, "opacity");
        animations[4] = new QPropertyAnimation(labelSignIn, "pos");
        animations[5] = new QPropertyAnimation(labelSignUp, "pos");

        animations[0]->setEndValue(QPoint(width(), lineConfirmCode->y()));
        animations[1]->setStartValue(QPoint(defaultLineX, height()));
        animations[1]->setEndValue(QPoint(defaultLineX, defaultY+lineHWithSpace));
        animations[2]->setStartValue(QPoint(defaultLineX, height()+lineHWithSpace));
        animations[2]->setEndValue(QPoint(defaultLineX, defaultY+(2*lineHWithSpace)));
        animations[3]->setEndValue(1.0);
        animations[4]->setEndValue(QPoint(labelSignIn->x(), defaultY+(3*lineHWithSpace)+buttonHWithSpace));
        animations[5]->setEndValue(QPoint(labelSignUp->x(), defaultY+(3*lineHWithSpace)+buttonHWithSpace));

        buttonOk->show();
        for(int i=0; i<6; i++){
            animations[i]->setDuration(DURATION);
            animations[i]->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
    else if(serverAnswer=="NRIGHT_CODE"){
        //DO
    }
    else if(serverAnswer=="SUCCESS_RECOVERY"){
        //DO
    }
    else if(serverAnswer=="REGISTRATIONSUCCESSFUL"){
        labelSuccess->setText("Registration completed successfully");
        labelSuccess->setGraphicsEffect(opacity);
        QPropertyAnimation *animation = new QPropertyAnimation(opacity, "opacity");
        animation->setDuration(DURATION*2);
        animation->setStartValue(0);
        animation->setEndValue(1);
        labelSuccess->show();
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        timerLabelSuccess->start(5000);
    }
    else emit sessionKeyReceived(serverAnswer);
}


void AuthWindow::signIn_released(){
    QString log = lineLog->text();
    QString pass = linePass->text();
    bool isLineEmpty=false;

    if(log==""){
        lineLog->setStyleSheet(QString("font-family: Century Gothic;"
                                              "font-size: %1px;"
                                              "background: transparent;"
                                              "border: 1px solid red;"
                                              "color: #B5EBEE;").arg(defaultFontSize));
        isLineEmpty=true;
    }
    if(pass==""){
        linePass->setStyleSheet(QString("font-family: Century Gothic;"
                                              "font-size: %1px;"
                                              "background: transparent;"
                                              "border: 1px solid red;"
                                              "color: #B5EBEE;").arg(defaultFontSize));
        isLineEmpty=true;
    }

    if(!isLineEmpty){

        QNetworkConfigurationManager internetConnection;
        if(!internetConnection.isOnline()){
            labelConnectionFailed->setText("No Internet access");
            emit connectionFailed();
        }
        else{
            emit loadingWasStart();
            emit closingErrorLabel();

            timerWaitingAnswer->start(10000);
            QTimer::singleShot(600, this, SLOT(authorizationSend()));
        }
    }
}

void AuthWindow::authorizationSend(){
    socket->writeDatagram(QByteArray().append("handshake|"+lineLog->text()+"|"+linePass->text()), host, 49003);
}


void AuthWindow::signUp_released()
{
    QString email = this->lineEmail->text();
    QString log = this->lineLog->text();
    QString pass = this->linePass->text();
    QString confirmPass = this->lineConfirmPass->text();
    QString style = "font-family: Century Gothic;"
                            "font-size: %1px;"
                            "background: transparent;"
                            "border: 1px solid red;"
                            "color: #B5EBEE;";
    bool isLineEmpty=false;

    if(email==""){
        isLineEmpty=true;
        lineEmail->setStyleSheet(style.arg(defaultFontSize));
    }
    if(log==""){
        isLineEmpty=true;
        lineLog->setStyleSheet(style.arg(defaultFontSize));
    }
    if(pass==""){
        isLineEmpty=true;
        linePass->setStyleSheet(style.arg(defaultFontSize));
    }
    if(confirmPass==""){
        isLineEmpty=true;
        lineConfirmPass->setStyleSheet(style.arg(defaultFontSize));
    }
    if(!isLineEmpty && pass==confirmPass){

        QNetworkConfigurationManager internetConnection;
        if(!internetConnection.isOnline()){
            labelConnectionFailed->setText("No Internet access");
            emit connectionFailed();
        }
        else{
            emit loadingWasStart();
            emit closingErrorLabel();

            timerWaitingAnswer->start(10000);
            QTimer::singleShot(500, this, SLOT(registrationSend()));
        }
    }
}

void AuthWindow::registrationSend(){
    socket->writeDatagram(QByteArray().append("registration|"+lineEmail->text()+"|"+lineLog->text()+"|"+linePass->text()), host, 49003);
}

void AuthWindow::labelSuccessHide(){
    timerLabelSuccess->stop();
    QPropertyAnimation *animation = new QPropertyAnimation(opacity, "opacity");
    animation->setDuration(DURATION*2);
    animation->setEndValue(0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    connect(animation, SIGNAL(finished()), labelConnectionFailedBackground, SLOT(close()));

    if(location==LOC_SIGNUP){
        connect(animation, SIGNAL(finished()), this, SLOT(signInLabel_released()));
    }
}


void AuthWindow::buttonOk_released(){
    if(location == LOC_RECOVERY_EMAIL){
        if(lineLog->text()==""){
            lineLog->setStyleSheet(QString("font-family: Century Gothic;"
                                           "font-size: %1px;"
                                           "background: transparent;"
                                           "border: 1px solid red;"
                                           "color: #B5EBEE;").arg(defaultFontSize));
        }
        else{

            QNetworkConfigurationManager internetConnection;
            if(!internetConnection.isOnline()){
                labelConnectionFailed->setText("No Internet access");
                emit connectionFailed();
            }
            else{
                emit loadingWasStart();
                emit closingErrorLabel();

                timerWaitingAnswer->start(10000);
                QTimer::singleShot(500, this, SLOT(recoveryEmailSend()));
            }
        }
    }
    else if(location == LOC_RECOVERY_CODE){
        if(lineConfirmCode->text()==""){
            lineConfirmCode->setStyleSheet(QString("font-family: Century Gothic;"
                                                   "font-size: %1px;"
                                                   "background: transparent;"
                                                   "border: 1px solid red;"
                                                   "color: #B5EBEE;").arg(defaultFontSize));
        }
        else{
            QNetworkConfigurationManager internetConnection;
            if(!internetConnection.isOnline()){
                labelConnectionFailed->setText("No Internet access");
                emit connectionFailed();
            }
            else{
                emit loadingWasStart();
                emit closingErrorLabel();
                lineConfirmCode->setDisabled(true);

                timerWaitingAnswer->start(10000);
                QTimer::singleShot(500, this, SLOT(recoveryCodeSend()));
            }
        }
    }
    else if(location == LOC_RECOVERY_PASS){
        if(lineRecoveryPass->text()==""){
            lineRecoveryPass->setStyleSheet(QString("font-family: Century Gothic;"
                                                   "font-size: %1px;"
                                                   "background: transparent;"
                                                   "border: 1px solid red;"
                                                   "color: #B5EBEE;").arg(defaultFontSize));
        }
        else if(lineRecoveryConfirmPass->text()!=lineRecoveryPass->text()){
            lineRecoveryConfirmPass->setStyleSheet(QString("font-family: Century Gothic;"
                                                   "font-size: %1px;"
                                                   "background: transparent;"
                                                   "border: 1px solid red;"
                                                   "color: #B5EBEE;").arg(defaultFontSize));
        }
        else{
            QNetworkConfigurationManager internetConnection;
            if(!internetConnection.isOnline()){
                labelConnectionFailed->setText("No Internet access");
                emit connectionFailed();
            }
            else{
                emit loadingWasStart();
                emit closingErrorLabel();
                lineRecoveryConfirmPass->setDisabled(true);
                lineRecoveryPass->setDisabled(true);

                timerWaitingAnswer->start(10000);
                QTimer::singleShot(500, this, SLOT(recoveryNewPassSend()));
            }
        }
    }
}

void AuthWindow::recoveryEmailSend(){
    socket->writeDatagram(QByteArray().append("recovery|"+lineLog->text()), host, 49003);
}

void AuthWindow::recoveryCodeSend(){
    socket->writeDatagram(QByteArray().append("recoveryCode|"+lineLog->text()+'|'+lineConfirmCode->text()), host, 49003);
}

void AuthWindow::recoveryNewPassSend(){
    socket->writeDatagram(QByteArray().append("recoveryNewPass|"+lineRecoveryPass->text()), host, 49003);
}


void AuthWindow::startPreloading()
{
    if(location == LOC_SIGNIN){
        buttonSignIn->setGraphicsEffect(opacity);

        QPropertyAnimation *animations[3];

        animations[0] = new QPropertyAnimation(labelSignUp, "pos");
        animations[0]->setDuration(DURATION);
        animations[0]->setEndValue(QPoint(labelSignUp->x(),this->height()));

        animations[1] = new QPropertyAnimation(labelForgotPass, "pos");
        animations[1]->setDuration(DURATION);
        animations[1]->setEndValue(QPoint(labelForgotPass->x(),this->height()));

        animations[2] = new QPropertyAnimation(opacity, "opacity");
        animations[2]->setDuration(DURATION/2);
        animations[2]->setEndValue(0.0);

        animations[0]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[1]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[2]->start(QAbstractAnimation::DeleteWhenStopped);

        preloader->move((width()-buttonSignIn->height())/2,buttonSignIn->y());

        lineLog->setDisabled(true);
        linePass->setDisabled(true);

        connect(animations[2], SIGNAL(finished()), buttonSignIn, SLOT(close()));
    }
    else if(location==LOC_SIGNUP){
        buttonSignUp->setGraphicsEffect(opacity);

        QPropertyAnimation *animations[2];

        animations[0] = new QPropertyAnimation(labelSignIn, "pos");
        animations[0]->setDuration(DURATION);
        animations[0]->setEndValue(QPoint(labelSignIn->x(),this->height()));

        animations[1] = new QPropertyAnimation(opacity, "opacity");
        animations[1]->setDuration(DURATION/2);
        animations[1]->setEndValue(0.0);

        animations[0]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[1]->start(QAbstractAnimation::DeleteWhenStopped);

        preloader->move((width()-buttonSignUp->height())/2,buttonSignUp->y());
        lineEmail->setDisabled(true);
        lineLog->setDisabled(true);
        linePass->setDisabled(true);
        lineConfirmPass->setDisabled(true);

        connect(animations[1], SIGNAL(finished()), buttonSignUp, SLOT(close()));
    }
    else if(location==LOC_RECOVERY_EMAIL || location == LOC_RECOVERY_CODE || location == LOC_RECOVERY_PASS){
        buttonOk->setGraphicsEffect(opacity);
        QPropertyAnimation *animations[3];

        animations[0] = new QPropertyAnimation(labelSignUp, "pos");
        animations[0]->setDuration(DURATION);
        animations[0]->setEndValue(QPoint(labelSignUp->x(),this->height()));

        animations[1] = new QPropertyAnimation(labelSignIn, "pos");
        animations[1]->setDuration(DURATION);
        animations[1]->setEndValue(QPoint(labelSignIn->x(),this->height()));

        animations[2] = new QPropertyAnimation(opacity, "opacity");
        animations[2]->setDuration(DURATION/2);
        animations[2]->setEndValue(0.0);

        animations[0]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[1]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[2]->start(QAbstractAnimation::DeleteWhenStopped);

        preloader->move((width()-buttonOk->height())/2,buttonOk->y());
        lineLog->setDisabled(true);

        connect(animations[2], SIGNAL(finished()), buttonOk, SLOT(close()));
    }
    preloader->show();
}

void AuthWindow::cancelPreloading(){
        preloader->close();

        if(location == LOC_SIGNIN){
            buttonSignIn->show();
            buttonSignIn->setGraphicsEffect(opacity);

            QPropertyAnimation *animations[3];

            animations[0] = new QPropertyAnimation(labelSignUp, "pos");
            animations[0]->setDuration(DURATION);
            animations[0]->setEndValue(QPoint(labelSignUp->x(), defaultY+(2*lineHWithSpace)+buttonHWithSpace));

            animations[1] = new QPropertyAnimation(labelForgotPass, "pos");
            animations[1]->setDuration(DURATION);
            animations[1]->setEndValue(QPoint(labelForgotPass->x(), defaultY+(2*lineHWithSpace)+buttonHWithSpace));

            animations[2] = new QPropertyAnimation(opacity, "opacity");
            animations[2]->setDuration(DURATION);
            animations[2]->setEndValue(1);

            animations[0]->start(QAbstractAnimation::DeleteWhenStopped);
            animations[1]->start(QAbstractAnimation::DeleteWhenStopped);
            animations[2]->start(QAbstractAnimation::DeleteWhenStopped);

            lineLog->setEnabled(true);
            linePass->setEnabled(true);
        }
        else if(location==LOC_SIGNUP){
            buttonSignUp->show();
            buttonSignUp->setGraphicsEffect(opacity);

            QPropertyAnimation *animations[2];
            animations[0] = new QPropertyAnimation(labelSignIn, "pos");

            animations[0]->setDuration(DURATION);
            animations[0]->setEndValue(QPoint(labelSignIn->x(), defaultY+(3*lineHWithSpace)+buttonHWithSpace));
            animations[0]->start(QAbstractAnimation::DeleteWhenStopped);

            animations[1] = new QPropertyAnimation(opacity, "opacity");
            animations[1]->setDuration(DURATION);
            animations[1]->setEndValue(1);
            animations[1]->start(QAbstractAnimation::DeleteWhenStopped);

            lineEmail->setEnabled(true);
            lineLog->setEnabled(true);
            linePass->setEnabled(true);
            lineConfirmPass->setEnabled(true);
        }
        else if(location==LOC_RECOVERY_EMAIL){
            buttonOk->show();
            buttonOk->setGraphicsEffect(opacity);

            QPropertyAnimation *animations[3];

            animations[0] = new QPropertyAnimation(labelSignUp, "pos");
            animations[0]->setDuration(DURATION);
            animations[0]->setEndValue(QPoint(labelSignUp->x(), defaultY+lineHWithSpace+buttonHWithSpace));

            animations[1] = new QPropertyAnimation(labelSignIn, "pos");
            animations[1]->setDuration(DURATION);
            animations[1]->setEndValue(QPoint(labelSignIn->x(), defaultY+lineHWithSpace+buttonHWithSpace));

            animations[2] = new QPropertyAnimation(opacity, "opacity");
            animations[2]->setDuration(DURATION);
            animations[2]->setEndValue(1);

            animations[0]->start(QAbstractAnimation::DeleteWhenStopped);
            animations[1]->start(QAbstractAnimation::DeleteWhenStopped);
            animations[2]->start(QAbstractAnimation::DeleteWhenStopped);

            lineLog->setEnabled(true);
        }
        else if(location==LOC_RECOVERY_CODE){
            buttonOk->show();
            buttonOk->setGraphicsEffect(opacity);

            lineConfirmCode->setEnabled(true);
            QPropertyAnimation *animations[3];

            animations[0] = new QPropertyAnimation(labelSignUp, "pos");
            animations[0]->setDuration(DURATION);
            animations[0]->setEndValue(QPoint(labelSignUp->x(), defaultY+(2*lineHWithSpace)+buttonHWithSpace));

            animations[1] = new QPropertyAnimation(labelSignIn, "pos");
            animations[1]->setDuration(DURATION);
            animations[1]->setEndValue(QPoint(labelSignIn->x(), defaultY+(2*lineHWithSpace)+buttonHWithSpace));

            animations[2] = new QPropertyAnimation(opacity, "opacity");
            animations[2]->setDuration(DURATION);
            animations[2]->setEndValue(1);

            animations[0]->start(QAbstractAnimation::DeleteWhenStopped);
            animations[1]->start(QAbstractAnimation::DeleteWhenStopped);
            animations[2]->start(QAbstractAnimation::DeleteWhenStopped);

        }
        else if(location==LOC_RECOVERY_PASS){
            buttonOk->show();
            buttonOk->setGraphicsEffect(opacity);

            lineRecoveryPass->setEnabled(true);
            lineRecoveryConfirmPass->setEnabled(true);
            QPropertyAnimation *animations[3];

            animations[0] = new QPropertyAnimation(labelSignUp, "pos");
            animations[0]->setDuration(DURATION);
            animations[0]->setEndValue(QPoint(labelSignUp->x(), defaultY+(3*lineHWithSpace)+buttonHWithSpace));

            animations[1] = new QPropertyAnimation(labelSignIn, "pos");
            animations[1]->setDuration(DURATION);
            animations[1]->setEndValue(QPoint(labelSignIn->x(), defaultY+(3*lineHWithSpace)+buttonHWithSpace));

            animations[2] = new QPropertyAnimation(opacity, "opacity");
            animations[2]->setDuration(DURATION);
            animations[2]->setEndValue(1);

            animations[0]->start(QAbstractAnimation::DeleteWhenStopped);
            animations[1]->start(QAbstractAnimation::DeleteWhenStopped);
            animations[2]->start(QAbstractAnimation::DeleteWhenStopped);

        }

        labelConnectionFailedBackground->show();
        QPropertyAnimation *animation = new QPropertyAnimation(labelConnectionFailedBackground, "pos");
        animation->setDuration(DURATION*2);
        animation->setEndValue(QPoint(0,labelConnectionFailedBackground->y()));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        timerLabelSuccess->start(5000);
}


void AuthWindow::eye_released(){
    if(linePass->echoMode()==QLineEdit::Password){
        linePass->setEchoMode(QLineEdit::Normal);
        lineConfirmPass->setEchoMode(QLineEdit::Normal);
    }
    else{
        linePass->setEchoMode(QLineEdit::Password);
        lineConfirmPass->setEchoMode(QLineEdit::Password);
    }
}

void AuthWindow::forgotPassLabel_released(){
    lineConfirmCode->setEnabled(true);
    location=LOC_RECOVERY_EMAIL;

    lineLog->setStyleSheet(QString("font-family: Century Gothic;"
                                   "font-size: %1px;"
                                   "background: transparent;"
                                   "color: #B5EBEE;").arg(defaultFontSize));
    linePass->setStyleSheet(QString("font-family: Century Gothic;"
                                    "font-size: %1px;"
                                    "background: transparent;"
                                    "color: #B5EBEE;").arg(defaultFontSize));


    QPropertyAnimation *animations[5];

    animations[0] = new QPropertyAnimation(labelForgotPass, "pos");
    animations[1] = new QPropertyAnimation(buttonSignIn, "pos");
    animations[2] = new QPropertyAnimation(labelSignIn, "pos");
    animations[3] = new QPropertyAnimation(labelSignUp, "pos");
    animations[4] = new QPropertyAnimation(linePass, "pos");

    animations[0]->setEndValue(QPoint(-lineW, labelForgotPass->y()));
    animations[1]->setEndValue(QPoint(buttonSignIn->x(), defaultY+lineHWithSpace));
    animations[2]->setStartValue(QPoint(defaultLineX, height()));
    animations[2]->setEndValue(QPoint(defaultLineX, defaultY+lineHWithSpace+buttonHWithSpace));
    animations[3]->setEndValue(QPoint(labelSignUp->x(), defaultY+lineHWithSpace+buttonHWithSpace));
    animations[4]->setEndValue(QPoint(-lineW, linePass->y()));

    labelSignIn->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    labelSignIn->show();
    for(int i=0;i<5;i++){
        animations[i]->setDuration(DURATION);
        animations[i]->start(QAbstractAnimation::DeleteWhenStopped);
    }

    buttonOk->move(defaultButtonX, defaultY+lineHWithSpace);
    connect(animations[1], SIGNAL(finished()), buttonOk, SLOT(show()));
    connect(animations[1], SIGNAL(finished()), buttonSignIn, SLOT(close()));
}

void AuthWindow::signUpLabel_released(){
    lineLog->clear();
    linePass->clear();
    lineEmail->clear();
    lineConfirmPass->clear();
    lineLog->setEnabled(true);

    lineLog->setPlaceholderText("Nickname");
    lineLog->setValidator(new QRegExpValidator(QRegExp("^(\\d|\\w|[_]){2,12}$")));

    QPropertyAnimation *animations[4];

    animations[0] = new QPropertyAnimation(labelForgotPass, "pos");
    animations[1] = new QPropertyAnimation(lineConfirmPass, "pos");
    animations[2] = new QPropertyAnimation(lineEmail, "pos");
    animations[3] = new QPropertyAnimation(labelSignUp, "pos");

    animations[0]->setEndValue(QPoint(-labelForgotPass->width(), labelForgotPass->y()));
    animations[1]->setEndValue(QPoint(defaultLineX, lineConfirmPass->y()));
    animations[2]->setEndValue(QPoint(defaultLineX, lineEmail->y()));
    animations[3]->setEndValue(QPoint(labelSignUp->x(), defaultY+(3*lineHWithSpace)+buttonHWithSpace));
    labelSignIn->move(labelSignUp->x(), defaultY+(3*lineHWithSpace)+buttonHWithSpace);

    if(location==LOC_RECOVERY_EMAIL){
        labelSignIn->close();

        lineLog->setStyleSheet(QString("font-family: Century Gothic;"
                                       "font-size: %1px;"
                                       "background: transparent;"
                                       "color: #B5EBEE;").arg(defaultFontSize));
        lineConfirmCode->setStyleSheet(QString("font-family: Century Gothic;"
                                               "font-size: %1px;"
                                               "background: transparent;"
                                               "color: #B5EBEE;").arg(defaultFontSize));

        QPropertyAnimation *localAnimations[2];
        localAnimations[0] = new QPropertyAnimation(linePass, "pos");
        localAnimations[1] = new QPropertyAnimation(buttonOk, "pos");

        localAnimations[0]->setDuration(DURATION);
        localAnimations[0]->setEndValue(QPoint(defaultLineX, linePass->y()));
        localAnimations[0]->start(QAbstractAnimation::DeleteWhenStopped);

        localAnimations[1]->setDuration(DURATION);
        localAnimations[1]->setEndValue(QPoint(buttonOk->x(), defaultY+(3*lineHWithSpace)));
        localAnimations[1]->start(QAbstractAnimation::DeleteWhenStopped);

        buttonSignUp->move(buttonOk->x(), labelSignUp->y() + labelSignUp->height() + height()/26);
        buttonSignIn->move(width(), buttonSignIn->y());
        connect(localAnimations[1], SIGNAL(finished()), buttonOk, SLOT(close()));
        connect(localAnimations[1], SIGNAL(finished()), buttonSignUp, SLOT(show()));
    }
    else{
        lineLog->setStyleSheet(QString("font-family: Century Gothic;"
                                       "font-size: %1px;"
                                       "background: transparent;"
                                       "color: #B5EBEE;").arg(defaultFontSize));
        linePass->setStyleSheet(QString("font-family: Century Gothic;"
                                        "font-size: %1px;"
                                        "background: transparent;"
                                        "color: #B5EBEE;").arg(defaultFontSize));

        QPropertyAnimation *localAnimations[2];
        localAnimations[0] = new QPropertyAnimation(buttonSignUp, "pos");
        localAnimations[1] = new QPropertyAnimation(buttonSignIn, "pos");

        localAnimations[0]->setDuration(DURATION);
        localAnimations[0]->setStartValue(QPoint(width(), buttonSignUp->y()));
        localAnimations[0]->setEndValue(QPoint(defaultButtonX, buttonSignUp->y()));
        localAnimations[0]->start(QAbstractAnimation::DeleteWhenStopped);

        localAnimations[1]->setDuration(DURATION);
        localAnimations[1]->setEndValue(QPoint(width(), buttonSignIn->y()));
        localAnimations[1]->start();
        buttonSignUp->show();

    }
    lineConfirmPass->show();
    lineEmail->show();

    for(int i=0;i<4;i++){
        animations[i]->setDuration(DURATION);
        animations[i]->start(QAbstractAnimation::DeleteWhenStopped);           
    }

    location=LOC_SIGNUP;

    labelSignIn->setAlignment(Qt::AlignRight | Qt::AlignTop);
    connect(animations[3], SIGNAL(finished()), labelSignUp, SLOT(close()));
    connect(animations[3], SIGNAL(finished()), labelSignIn, SLOT(show()));
}

void AuthWindow::signInLabel_released(){
    lineLog->clear();
    linePass->clear();
    lineEmail->clear();
    lineConfirmPass->clear();
    lineLog->setEnabled(true);
    lineEmail->setStyleSheet(QString("font-family: Century Gothic;"
                                     "font-size: %1px;"
                                     "background: transparent;"
                                     "color: #B5EBEE;").arg(defaultFontSize));
    lineLog->setStyleSheet(QString("font-family: Century Gothic;"
                                   "font-size: %1px;"
                                   "background: transparent;"
                                   "color: #B5EBEE;").arg(defaultFontSize));
    lineConfirmPass->setStyleSheet(QString("font-family: Century Gothic;"
                                           "font-size: %1px;"
                                           "background: transparent;"
                                           "color: #B5EBEE;").arg(defaultFontSize));
    linePass->setStyleSheet(QString("font-family: Century Gothic;"
                                    "font-size: %1px;"
                                    "background: transparent;"
                                    "color: #B5EBEE;").arg(defaultFontSize));
    lineConfirmCode->setStyleSheet(QString("font-family: Century Gothic;"
                                           "font-size: %1px;"
                                           "background: transparent;"
                                           "color: #B5EBEE;").arg(defaultFontSize));
    lineLog->setValidator(new QRegExpValidator(QRegExp("([^@])+(@?)([^@])+")));
    buttonSignIn->show();

    if(location==LOC_RECOVERY_EMAIL){
        QPropertyAnimation *localAnimations[5];
        localAnimations[0] = new QPropertyAnimation(buttonOk,"pos");
        localAnimations[1] = new QPropertyAnimation(linePass,"pos");
        localAnimations[2] = new QPropertyAnimation(labelSignUp, "pos");
        localAnimations[3] = new QPropertyAnimation(buttonSignIn, "pos");

        localAnimations[0]->setDuration(DURATION);
        localAnimations[0]->setEndValue(QPoint(width(), buttonOk->y()));
        localAnimations[0]->start(QAbstractAnimation::DeleteWhenStopped);

        localAnimations[1]->setDuration(DURATION);
        localAnimations[1]->setEndValue(QPoint(defaultLineX,linePass->y()));
        localAnimations[1]->start(QAbstractAnimation::DeleteWhenStopped);

        localAnimations[2]->setDuration(DURATION);
        localAnimations[2]->setEndValue(QPoint(labelSignUp->x(), defaultY+(2*lineHWithSpace)+buttonHWithSpace));
        localAnimations[2]->start(QAbstractAnimation::DeleteWhenStopped);

        localAnimations[3]->setDuration(DURATION);
        localAnimations[3]->setStartValue(QPoint(-buttonW, defaultY+(2*lineHWithSpace)));
        localAnimations[3]->setEndValue(QPoint(defaultButtonX, defaultY+(2*lineHWithSpace)));
        localAnimations[3]->start(QAbstractAnimation::DeleteWhenStopped);

        localAnimations[4] = new QPropertyAnimation(labelForgotPass, "pos");
        localAnimations[4]->setEndValue(QPoint(defaultLineX, defaultY+(2*lineHWithSpace)+buttonHWithSpace));
        localAnimations[4]->start(QAbstractAnimation::DeleteWhenStopped);

        labelSignIn->close();
        connect(localAnimations[0], SIGNAL(finished()), buttonOk, SLOT(close()));

    }
    else if(location==LOC_SIGNUP){
        QPropertyAnimation *animations[6];

        animations[0] = new QPropertyAnimation(labelForgotPass, "pos");
        animations[1] = new QPropertyAnimation(buttonSignIn, "pos");
        animations[2] = new QPropertyAnimation(lineConfirmPass, "pos");
        animations[3] = new QPropertyAnimation(lineEmail, "pos");
        animations[4] = new QPropertyAnimation(buttonSignUp, "pos");
        animations[5] = new QPropertyAnimation(labelSignIn, "pos");

        animations[0]->setEndValue(QPoint(defaultLineX, defaultY+(2*lineHWithSpace)+buttonHWithSpace));
        animations[1]->setStartValue(QPoint(width(), defaultY+(2*lineHWithSpace)));
        animations[1]->setEndValue(QPoint(defaultButtonX, defaultY+(2*lineHWithSpace)));
        animations[2]->setEndValue(QPoint(-lineW, lineConfirmPass->y()));
        animations[3]->setEndValue(QPoint(-lineW, lineEmail->y()));
        animations[4]->setEndValue(QPoint(width(), buttonSignUp->y()));
        animations[5]->setEndValue(QPoint(labelSignIn->x(), defaultY+(2*lineHWithSpace)+buttonHWithSpace));

        nicknameExists=false;
        lineLog->setPlaceholderText("Login or email");
        labelSignUp->move(defaultLineX+lineW-labelSignUp->width(), defaultY+(2*lineHWithSpace)+buttonHWithSpace);
        connect(animations[5],SIGNAL(finished()), labelSignIn, SLOT(close()));
        connect(animations[5],SIGNAL(finished()), labelSignUp, SLOT(show()));
        connect(animations[4], SIGNAL(finished()), buttonSignUp, SLOT(close()));

        for(int i=0;i<6;i++){
            animations[i]->setDuration(DURATION);
            animations[i]->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
    else if(location==LOC_RECOVERY_CODE){
        QPropertyAnimation *animations[4];
        animations[0] = new QPropertyAnimation(lineConfirmCode, "pos");
        animations[1] = new QPropertyAnimation(linePass, "pos");
        animations[2] = new QPropertyAnimation(labelSignIn, "pos");
        animations[3] = new QPropertyAnimation(labelForgotPass, "pos");

        animations[0]->setDuration(DURATION);
        animations[0]->setEndValue(QPoint(width(), lineConfirmCode->y()));

        animations[1]->setDuration(DURATION);
        animations[1]->setStartValue(QPoint(-lineW, defaultY+lineHWithSpace));
        animations[1]->setEndValue(QPoint(defaultLineX, defaultY+lineHWithSpace));

        animations[2]->setDuration(DURATION);
        animations[2]->setEndValue(QPoint(-labelSignIn->width(), labelSignIn->y()));

        animations[3]->setDuration(DURATION);
        animations[3]->setStartValue(QPoint(-labelForgotPass->width(), defaultY+(2*lineHWithSpace)+buttonHWithSpace));
        animations[3]->setEndValue(QPoint(defaultLineX, defaultY+(2*lineHWithSpace)+buttonHWithSpace));

        connect(animations[2], SIGNAL(finished()), labelSignIn, SLOT(close()));
        animations[0]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[1]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[2]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[3]->start(QAbstractAnimation::DeleteWhenStopped);

        buttonSignIn->move(buttonOk->x(), buttonOk->y());
        buttonOk->close();
    }
    else if(location==LOC_RECOVERY_PASS){
        buttonSignIn->close();
        QPropertyAnimation *animations[5];
        animations[0] = new QPropertyAnimation(lineRecoveryConfirmPass, "pos");
        animations[1] = new QPropertyAnimation(buttonOk, "pos");
        animations[2] = new QPropertyAnimation(labelSignUp, "pos");
        animations[3] = new QPropertyAnimation(labelSignIn, "pos");
        animations[4] = new QPropertyAnimation(labelForgotPass, "pos");

        animations[0]->setDuration(DURATION);
        animations[0]->setEndValue(QPoint(width(), lineRecoveryConfirmPass->y()));

        animations[1]->setDuration(DURATION);
        animations[1]->setEndValue(QPoint(defaultButtonX, defaultY+(2*lineHWithSpace)));

        animations[2]->setDuration(DURATION);
        animations[2]->setEndValue(QPoint(labelSignUp->x(), defaultY+(2*lineHWithSpace)+buttonHWithSpace));

        animations[3]->setDuration(DURATION);
        animations[3]->setEndValue(QPoint(-labelSignIn->width(), labelSignIn->y()));

        animations[4]->setDuration(DURATION);
        animations[4]->setEndValue(QPoint(defaultLineX, defaultY+(2*lineHWithSpace)+buttonHWithSpace));

        buttonSignIn->move(defaultButtonX, defaultY+(2*lineHWithSpace));
        lineRecoveryPass->close();
        linePass->move(lineRecoveryPass->x(), lineRecoveryPass->y());
        linePass->show();
        connect(animations[1], SIGNAL(finished()), buttonOk, SLOT(close()));
        connect(animations[1], SIGNAL(finished()), buttonSignIn, SLOT(show()));
        connect(animations[3], SIGNAL(finished()), labelSignIn, SLOT(close()));
        animations[0]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[1]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[2]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[3]->start(QAbstractAnimation::DeleteWhenStopped);
        animations[4]->start(QAbstractAnimation::DeleteWhenStopped);
    }

    location = LOC_SIGNIN;
}


void AuthWindow::checkingNickname(){
    if(location==LOC_SIGNUP && lineLog->text()!=""){
        QByteArray par;
        socket->writeDatagram(par.append("DoesExNick|"+lineLog->text()), host, 49003);
    }
}

void AuthWindow::checkingEmail()
{
    if(lineLog->text()!=""){
        QByteArray par;
        socket->writeDatagram(par.append("DoesExEmail|"+lineEmail->text()), host, 49003);
    }
}

void AuthWindow::checkingConfirming(QString text){
    if(text=="")
        lineConfirmPass->setStyleSheet(QString("font-family: Century Gothic;"
                                               "font-size: %1px;"
                                               "background: transparent;"
                                               "color: #B5EBEE;").arg(defaultFontSize));
    else if(text!=linePass->text())
        lineConfirmPass->setStyleSheet(QString("font-family: Century Gothic;"
                                               "font-size: %1px;"
                                               "background: transparent;"
                                               "border: 1px solid red;"
                                               "color: #B5EBEE;").arg(defaultFontSize));
    else
        lineConfirmPass->setStyleSheet(QString("font-family: Century Gothic;"
                                               "font-size: %1px;"
                                               "background: transparent;"
                                               "color: #B5EBEE;").arg(defaultFontSize));
}

void AuthWindow::checkingRecoveryConfirming(QString text){
    if(text=="")
        lineRecoveryConfirmPass->setStyleSheet(QString("font-family: Century Gothic;"
                                       "font-size: %1px;"
                                       "background: transparent;"
                                       "color: #B5EBEE;").arg(defaultFontSize));
    else if(text!=lineRecoveryPass->text())
        lineRecoveryConfirmPass->setStyleSheet(QString("font-family: Century Gothic;"
                                       "font-size: %1px;"
                                       "background: transparent;"
                                       "border: 1px solid red;"
                                       "color: #B5EBEE;").arg(defaultFontSize));
    else
        lineRecoveryConfirmPass->setStyleSheet(QString("font-family: Century Gothic;"
                                       "font-size: %1px;"
                                       "background: transparent;"
                                       "color: #B5EBEE;").arg(defaultFontSize));
}


void AuthWindow::emailChange()
{
    lineEmail->setStyleSheet(QString("font-family: Century Gothic;"
                                  "font-size: %1px;"
                                  "background: transparent;"
                                   "color: #B5EBEE;").arg(defaultFontSize));
}

void AuthWindow::logChange(){
    lineLog->setStyleSheet(QString("font-family: Century Gothic;"
                                  "font-size: %1px;"
                                  "background: transparent;"
                                   "color: #B5EBEE;").arg(defaultFontSize));
}

void AuthWindow::passChange(){
    linePass->setStyleSheet(QString("font-family: Century Gothic;"
                                  "font-size: %1px;"
                                  "background: transparent;"
                                   "color: #B5EBEE;").arg(defaultFontSize));
}

void AuthWindow::passRecoveryChange(){
    lineRecoveryPass->setStyleSheet(QString("font-family: Century Gothic;"
                                  "font-size: %1px;"
                                  "background: transparent;"
                                            "color: #B5EBEE;").arg(defaultFontSize));
}

void AuthWindow::codeChange(){
    lineConfirmCode->setStyleSheet(QString("font-family: Century Gothic;"
                              "font-size: %1px;"
                              "background: transparent;"
                                        "color: #B5EBEE;").arg(defaultFontSize));
}


void AuthWindow::waitingAnswer(){
    //CHECK
    labelConnectionFailed->setText("Server is not available");

    emit connectionFailed();
}

void AuthWindow::errorHide(){
    timerLabelSuccess->stop();
    QPropertyAnimation *animation = new QPropertyAnimation(labelConnectionFailedBackground, "pos");
    animation->setDuration(DURATION*2);
    animation->setEndValue(QPoint(0-labelConnectionFailedBackground->width(), labelConnectionFailedBackground->y()));
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    connect(animation, SIGNAL(finished()), labelConnectionFailedBackground, SLOT(close()));
}

void AuthWindow::resizeAll(){
    //  quint16 windowSize = (QApplication::desktop()->width()/100)*25;

    quint16 windowSize = (1366/100)*25;

    if(QApplication::desktop()->width()<1366)
        windowSize=(1366/100)*25;
    else if(QApplication::desktop()->width()>7680)
        windowSize=(7680/100)*25;

    this->setFixedSize(windowSize, windowSize);

    lineW = (windowSize/13)*7;
    lineH = windowSize/13;
    lineHWithSpace = lineH + windowSize/26;
    buttonW = windowSize/52*15;
    buttonH = windowSize/52*5;
    buttonHWithSpace = buttonH + windowSize/26;
    defaultY = lineH * 4;
    defaultLineX = (windowSize - lineW)/2;
    defaultButtonX = (windowSize - buttonW)/2;

    quint16 buttonCloseW = windowSize/10;
    quint16 buttonCloseH = windowSize/16;
    quint16 buttonCloseX = windowSize - buttonCloseW;

    quint16 buttonMinimizeH = buttonCloseH;
    quint16 buttonMinimizeW = buttonCloseW;
    quint16 buttonMinimizeX = buttonCloseX - buttonMinimizeW;

    quint16 lineLogW = (windowSize/13)*7;
    quint16 lineLogH = windowSize/13;
    quint16 lineLogX = (windowSize - lineLogW)/2;
    quint16 lineLogY = lineLogH * 4;

    quint16 linePassW = lineLogW;
    quint16 linePassH = lineLogH;

    quint16 buttonSignInH = windowSize/52*5;

    quint16 labelSignUpW = (windowSize/26)*4;
    quint16 labelSignUpH = windowSize/13;

    quint16 labelForgotPassW = windowSize/26*9;
    quint16 labelForgotPassH = labelSignUpH;

    quint16 buttonEyeH = linePassH;
    quint16 buttonEyeW = buttonEyeH;
    quint16 buttonEyeX = linePassW - buttonEyeW;

    quint16 labelSignInW = labelSignUpW;
    quint16 labelSignInH = labelSignUpH;

    quint16 labelUncorrectNicknameX = lineLogX;
    quint16 labelUncorrectNicknameY = lineLogY - windowSize/26 - labelUncorrectNickname->height();

    quint16 preloaderH = buttonSignInH;
    quint16 preloaderW = preloaderH;

    quint16 labelConnectionFailedH = (windowSize/65)*6;
    quint16 labelConnectionFailedW = windowSize;

    quint16 labelConnectionFailedBackgroundH = (windowSize/65)*6;
    quint16 labelConnectionFailedBackgroundW = windowSize;
    qint16  labelConnectionFailedBackgroundX = 0-labelConnectionFailedBackgroundW;
    quint16 labelConnectionFailedBackgroundY = windowSize - labelConnectionFailedBackgroundH;

    quint16 labelRegistrationSuccessfulH = windowSize/3;
    quint16 labelRegistrationSuccessfulW = windowSize;
    quint16 labelRegistrationSuccessfulX = 0;
    quint16 labelRegistrationSuccessfulY = windowSize/3;

    quint16 lineConfirmCodeH = lineLogH;
    quint16 lineConfirmCodeW = lineLogW;
    quint16 lineConfirmCodeX = lineLogX;
    quint16 lineConfirmCodeY = windowSize;

    lineEmail->setGeometry(-lineW, defaultY-lineHWithSpace, lineW, lineH);
    lineLog->setGeometry(defaultLineX, defaultY, lineW, lineH);
    linePass->setGeometry(defaultLineX, defaultY+lineHWithSpace, lineW, lineH);
    lineConfirmPass->setGeometry(-lineW, (2*lineHWithSpace)+defaultY , lineW, lineH);
    lineRecoveryPass->setGeometry(height(), defaultLineX, lineW, lineH);
    lineRecoveryConfirmPass->setGeometry(height()+lineHWithSpace, defaultLineX, lineW, lineH);

    preloader->resize(preloaderW, preloaderH);

    buttonClose->setGeometry(buttonCloseX, 0, buttonCloseW, buttonCloseH);
    buttonMinimize->setGeometry(buttonMinimizeX, 0, buttonMinimizeW, buttonMinimizeH);

    buttonSignIn->setGeometry(defaultButtonX, (2*lineHWithSpace)+defaultY, buttonW, buttonH);
    buttonSignUp->setGeometry(defaultButtonX, defaultY+(3*lineHWithSpace), buttonW, buttonH);
    buttonOk->setGeometry(defaultButtonX, defaultY+lineHWithSpace, buttonW, buttonH);

    buttonEye->setGeometry(buttonEyeX, 0, buttonEyeW, buttonEyeH);
    buttonEye->setIconSize(QSize(buttonEyeW, buttonEyeH));

    labelForgotPass->setGeometry(defaultLineX, 2*lineHWithSpace + buttonHWithSpace + defaultY, labelForgotPassW, labelForgotPassH);
    labelSignUp->setGeometry(defaultLineX+lineW-labelSignUpW, 2*lineHWithSpace + buttonHWithSpace + defaultY, labelSignUpW, labelSignUpH);
    labelSignIn->setGeometry(defaultLineX+lineW-labelSignInW , defaultY + (3*lineHWithSpace) + buttonHWithSpace, labelSignInW, labelSignInH);

    labelUncorrectNickname->move(labelUncorrectNicknameX, labelUncorrectNicknameY);
    labelConnectionFailedBackground->setGeometry(labelConnectionFailedBackgroundX, labelConnectionFailedBackgroundY,labelConnectionFailedBackgroundW, labelConnectionFailedBackgroundH);
    labelConnectionFailed->setGeometry(0, 0, labelConnectionFailedW, labelConnectionFailedH);
    labelSuccess->setGeometry(labelRegistrationSuccessfulX,labelRegistrationSuccessfulY,labelRegistrationSuccessfulW, labelRegistrationSuccessfulH);
    lineConfirmCode->setGeometry(lineConfirmCodeX, lineConfirmCodeY, lineConfirmCodeW, lineConfirmCodeH);
}

void AuthWindow::mouseMoveEvent(QMouseEvent *event){
    if (mpos.x() >= 0 && event->buttons() && Qt::LeftButton){
        QPoint diff = event->pos() - mpos;
        QPoint newpos = this->pos() + diff;

        this->move(newpos);
    }
}

void AuthWindow::mousePressEvent(QMouseEvent *event){
    mpos = event->pos();
}

void AuthWindow::mouseReleaseEvent(QMouseEvent *){
    mpos = QPoint(-1, -1);
}


bool AuthWindow::eventFilter(QObject *target, QEvent *event){
    static bool inHover;

    if (target == buttonClose){
        if (event->type() == QEvent::HoverEnter)
            buttonClose->setIcon(QIcon(":fon/close2.png"));
        else if(event->type() == QEvent::HoverLeave)
            buttonClose->setIcon(QIcon(":fon/close1.png"));
    }
    else if(target == buttonMinimize){
        if (event->type() == QEvent::HoverEnter)
            buttonMinimize->setIcon(QIcon(":fon/min2.png"));
        else if(event->type() == QEvent::HoverLeave)
            buttonMinimize->setIcon(QIcon(":fon/min1.png"));
    }
    else if(target == lineLog){ 
        if(event->type() == QEvent::HoverEnter && location==LOC_SIGNUP && nicknameExists){
            labelUncorrectNickname->show();
            inHover = true;
        }
        else if(event->type() == QEvent::HoverLeave && location==LOC_SIGNUP && nicknameExists){
            labelUncorrectNickname->close();
            inHover = false;
        }
        else if(event->type() == QEvent::MouseMove && inHover && location==LOC_SIGNUP && nicknameExists){
            QPoint mousePos = QCursor::pos();

            if(labelUncorrectNickname->width()+mousePos.x()-x()+(width()/26)*2 >= this->width())
                labelUncorrectNickname->move(this->width()-labelUncorrectNickname->width()-width()/26, mousePos.y()-y()-labelUncorrectNickname->height()-this->height()/130);
            else
                labelUncorrectNickname->move(mousePos.x()-x()+width()/26, mousePos.y()-y()-labelUncorrectNickname->height()-this->height()/130);

        }
    }

    return QMainWindow::eventFilter(target, event);
}

void ClickableLabel::enterEvent(QEvent* event)
{
    if(isUnderlined){
        QFont f = font();
        f.setUnderline(true);
        setFont(f);
    }
}

void ClickableLabel::leaveEvent(QEvent* event)
{
    if(isUnderlined){
        QFont f = font();
        f.setUnderline(false);
        setFont(f);
    }
}

void LineEdit::init(){
    setAcceptDrops(false);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(showMenu(QPoint)));
}

void LineEdit::keyPressEvent(QKeyEvent *event){
    if(event->matches(QKeySequence::Copy) || event->matches(QKeySequence::Cut) || event->matches(QKeySequence::Paste))
        event->ignore();
    else return QLineEdit::keyPressEvent(event);
}


void AuthWindow::buttonMinimize_released(){
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setEndValue(0.0);
    animation->setDuration(200);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    connect(animation, SIGNAL(finished()), this, SLOT(showMinimized()));
}

void AuthWindow::changeEvent(QEvent* e){
    if( e->type() == QEvent::WindowStateChange ){
        QWindowStateChangeEvent* event = static_cast<QWindowStateChangeEvent*>(e);

        if(event->oldState() & Qt::WindowMinimized){
            QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
            animation->setDuration(150);
            animation->setEndValue(1.0);
            this->showNormal();
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
}


AuthWindow::~AuthWindow(){
    delete ui;

    delete lineLog;
    delete linePass;
    delete lineConfirmPass;
    delete lineEmail;
    delete buttonSignIn;
    delete buttonSignUp;
    delete buttonOk;
    delete labelUncorrectNickname;

    delete buttonClose;
    delete buttonEye;
    delete buttonMinimize;
    delete labelForgotPass;
    delete labelSignUp;
    delete labelSignIn;
    delete labelConnectionFailed;
    delete labelConnectionFailedBackground;
    delete labelSuccess;

    socket->close();
    delete socket;

    delete preloader;
    delete opacity;
}

ClickableLabel::ClickableLabel(QWidget* parent, bool isUnderlined) : QLabel(parent){
    this->isUnderlined = isUnderlined;
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *){
    emit released();
}

ClickableLabel::~ClickableLabel(){}
