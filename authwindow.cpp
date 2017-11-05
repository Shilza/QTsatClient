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
    buttonSignIn = new QPushButton(this);
    buttonSignUp = new QPushButton(this);
    buttonOk = new QPushButton(this);
    buttonMinimize = new QPushButton(this);
    buttonClose = new QPushButton(this);
    buttonEye = new QPushButton(linePass);
    labelForgotPass = new ClickableLabel(this);
    labelSignUp = new ClickableLabel(this);
    labelSignIn = new ClickableLabel(this);
    labelRegistrationSuccessful = new ClickableLabel(this, false);
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
    buttonSignIn->setText("Sign in");
    buttonSignUp->setText("Sign Up");
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
    labelRegistrationSuccessful->setText("Registration completed successfully");
    labelRegistrationSuccessful->setAlignment(Qt::AlignCenter);

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
    labelRegistrationSuccessful->setStyleSheet(QString("font-family: Century Gothic;"
                                                       "font-size: %1px;"
                                                       "background: rgba(255,255,255,128);").arg(defaultFontSize/11*13));

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
    labelRegistrationSuccessful->close();

    QPushButton *btn = new QPushButton(this);

    connect(btn, SIGNAL(released()),this, SLOT(test()));
    connect(this, SIGNAL(loadingWasStart()),this, SLOT(startPreloader()));
    connect(buttonSignIn, SIGNAL(released()), this, SLOT(signIn_released()));
    connect(buttonSignUp, SIGNAL(released()), this, SLOT(signUp_released()));
    connect(buttonOk, SIGNAL(released()), this, SLOT(passRecovery_released()));
    connect(buttonEye, SIGNAL(released()), this, SLOT(eye_released()));
    connect(labelSignUp, SIGNAL(released()),this, SLOT(signUpLabel_released()));
    connect(labelSignIn, SIGNAL(released()),this, SLOT(signInLabel_released()));
    connect(labelForgotPass, SIGNAL(released()), this,SLOT(forgotPassLabel_released()));
    connect(labelRegistrationSuccessful, SIGNAL(released()), this, SLOT(registrationSuccess()));


    connect(lineEmail, SIGNAL(textChanged(QString)), this, SLOT(emailChange()));
    connect(lineEmail, SIGNAL(editingFinished()), this, SLOT(checkingEmail()));
    connect(lineLog, SIGNAL(textChanged(QString)), this, SLOT(logChange()));
    connect(lineLog, SIGNAL(editingFinished()), this, SLOT(checkingNickname()));
    connect(linePass, SIGNAL(textChanged(QString)), this, SLOT(passChange()));
    connect(lineConfirmPass, SIGNAL(textChanged(QString)),this, SLOT(checkingConfirming(QString)));

    connect(socket, SIGNAL(readyRead()),this,SLOT(socketReading()));
    connect(buttonClose, SIGNAL(released()), this, SLOT(close()));
    connect(buttonMinimize, SIGNAL(released()), this, SLOT(buttonMinimize_released()));
    connect(labelConnectionFailed, SIGNAL(released()),this,SLOT(errorHide()));
    connect(this, SIGNAL(closingErrorLabel()),this,SLOT(errorHide()));
    connect(this, SIGNAL(connectionFailed()), this, SLOT(cancelPreloading()));
}

void AuthWindow::resizeAll()
{
    //  quint16 windowSize = (QApplication::desktop()->width()/100)*25;

    quint16 windowSize = (1366/100)*25;

    if(QApplication::desktop()->width()<1366)
        windowSize=(1366/100)*25;
    else if(QApplication::desktop()->width()>7680)
        windowSize=(7680/100)*25;

    this->setFixedSize(windowSize, windowSize);

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
    quint16 linePassX = lineLogX;
    quint16 linePassY = lineLogY + lineLogH + windowSize/26;

    quint16 buttonSignInW = windowSize/52*15;
    quint16 buttonSignInH = windowSize/52*5;
    quint16 buttonSignInX = (windowSize - buttonSignInW)/2;
    quint16 buttonSignInY = linePassY + linePassH + windowSize/26;

    quint16 labelSignUpW = (windowSize/26)*4;
    quint16 labelSignUpH = windowSize/13;
    quint16 labelSignUpX_inSIGNIN = linePassX + linePassW - labelSignUpW;
    quint16 labelSignUpY_inSIGNIN = buttonSignInY + buttonSignInH + windowSize/26;

    quint16 labelForgotPassW = windowSize/26*9;
    quint16 labelForgotPassH = labelSignUpH;
    quint16 labelForgotPassX = linePassX;
    quint16 labelForgotPassY = labelSignUpY_inSIGNIN;

    quint16 lineConfirmPassW = lineLogW;
    quint16 lineConfirmPassH = lineLogH;

    quint16 lineEmailW = lineLogW;
    quint16 lineEmailH = lineLogH;

    quint16 buttonSignUpW = buttonSignInW;
    quint16 buttonSignUpH = buttonSignInH;
    quint16 buttonSignUpX = buttonSignInX;
    quint16 buttonSignUpY = linePassY + linePassH + lineConfirmPassH + windowSize/13;

    quint16 buttonOkW = buttonSignInW;
    quint16 buttonOkH = buttonSignInH;
    quint16 buttonOkX = buttonSignInX;
    quint16 buttonOkY = linePassY;

    quint16 buttonEyeH = linePassH;
    quint16 buttonEyeW = buttonEyeH;
    quint16 buttonEyeX = linePassW - buttonEyeW;

    quint16 labelSignInW = labelSignUpW;
    quint16 labelSignInH = labelSignUpH;
    quint16 labelSignInX_inSIGNUP = labelSignUpX_inSIGNIN;
    quint16 labelSignInY_inSIGNUP = buttonSignUpY + buttonSignUpH + windowSize/26;

    quint16 labelUncorrectNicknameX = lineLogX;
    quint16 labelUncorrectNicknameY = lineLogY - windowSize/26 - labelUncorrectNickname->height();

    quint16 preloaderH = buttonSignInH;
    quint16 preloaderW = preloaderH;

    quint16 labelConnectionFailedH = (windowSize/65)*6;
    quint16 labelConnectionFailedW = windowSize;

    quint16 labelConnectionFailedBackgroundH = (windowSize/65)*6;
    quint16 labelConnectionFailedBackgroundW = windowSize;
    qint16 labelConnectionFailedBackgroundX = 0-labelConnectionFailedBackgroundW;
    quint16 labelConnectionFailedBackgroundY = windowSize - labelConnectionFailedBackgroundH;

    quint16 labelRegistrationSuccessfulH = windowSize/3;
    quint16 labelRegistrationSuccessfulW = windowSize;
    quint16 labelRegistrationSuccessfulX = 0;
    quint16 labelRegistrationSuccessfulY = windowSize/3;

    lineLog->setGeometry(lineLogX, lineLogY, lineLogW, lineLogH);
    linePass->setGeometry(linePassX, linePassY, linePassW, linePassH);
    lineConfirmPass->resize(lineConfirmPassW, lineConfirmPassH);
    lineEmail->resize(lineEmailW, lineEmailH);
    preloader->resize(preloaderW, preloaderH);

    buttonSignIn->setGeometry(buttonSignInX, buttonSignInY, buttonSignInW,buttonSignInH);
    buttonSignUp->setGeometry(buttonSignUpX, buttonSignUpY, buttonSignUpW, buttonSignUpH);
    buttonOk->setGeometry(buttonOkX, buttonOkY, buttonOkW, buttonOkH);
    buttonClose->setGeometry(buttonCloseX, 0, buttonCloseW, buttonCloseH);
    buttonMinimize->setGeometry(buttonMinimizeX, 0, buttonMinimizeW, buttonMinimizeH);
    buttonEye->setGeometry(buttonEyeX, 0, buttonEyeW, buttonEyeH);
    buttonEye->setIconSize(QSize(buttonEyeW, buttonEyeH));

    labelForgotPass->setGeometry(labelForgotPassX, labelForgotPassY, labelForgotPassW, labelForgotPassH);
    labelSignUp->setGeometry(labelSignUpX_inSIGNIN, labelSignUpY_inSIGNIN, labelSignUpW,labelSignUpH);
    labelSignIn->setGeometry(labelSignInX_inSIGNUP, labelSignInY_inSIGNUP, labelSignInW, labelSignInH);
    labelUncorrectNickname->move(labelUncorrectNicknameX, labelUncorrectNicknameY);
    labelConnectionFailedBackground->setGeometry(labelConnectionFailedBackgroundX, labelConnectionFailedBackgroundY,labelConnectionFailedBackgroundW, labelConnectionFailedBackgroundH);
    labelConnectionFailed->setGeometry(0, 0, labelConnectionFailedW, labelConnectionFailedH);
    labelRegistrationSuccessful->setGeometry(labelRegistrationSuccessfulX,labelRegistrationSuccessfulY,labelRegistrationSuccessfulW, labelRegistrationSuccessfulH);
}

void AuthWindow::waitingAnswer(){
    //CHECK
    if(answerState==SERVER_RESPONDED)
        return;

    labelConnectionFailed->setText("Server is not available");

    emit connectionFailed();
}

void AuthWindow::socketReading()
{
    answerState = SERVER_RESPONDED;
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
        //DO
    }
    else if(serverAnswer=="RECOVERYNFOUND"){
        //DO
    }
    else if(serverAnswer=="REGISTRATIONSUCCESSFUL"){
        labelRegistrationSuccessful->setGraphicsEffect(opacity);
        QPropertyAnimation *animation = new QPropertyAnimation(opacity, "opacity");
        animation->setDuration(DURATION*2);
        animation->setStartValue(0);
        animation->setEndValue(1);
        labelRegistrationSuccessful->show();
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        QTimer::singleShot(5000, this, SLOT(registrationSuccess()));
    }
    else
        emit sessionKeyReceived(serverAnswer);
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

            QTimer::singleShot(10000, this, SLOT(waitingAnswer()));
            QTimer::singleShot(600, this, SLOT(handshakeSend()));
        }
    }
}

void AuthWindow::handshakeSend(){
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

            QTimer::singleShot(10000, this, SLOT(waitingAnswer()));
            QTimer::singleShot(500, this, SLOT(registrationSend()));
        }
    }
}

void AuthWindow::registrationSend(){
    socket->writeDatagram(QByteArray().append("registration|"+lineEmail->text()+"|"+lineLog->text()+"|"+linePass->text()), host, 49003);
}

void AuthWindow::registrationSuccess(){
    QPropertyAnimation *animation = new QPropertyAnimation(opacity, "opacity");
    animation->setDuration(DURATION*2);
    animation->setEndValue(0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    connect(animation, SIGNAL(finished()), labelConnectionFailedBackground, SLOT(close()));
}

void AuthWindow::passRecovery_released(){
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

            QTimer::singleShot(10000, this, SLOT(waitingAnswer()));
            QTimer::singleShot(500, this, SLOT(recoverySend()));
        }

    }
}

void AuthWindow::recoverySend(){
    socket->writeDatagram(QByteArray().append("recovery|"+lineLog->text()), host, 49003);
    qDebug() << QByteArray().append("recovery|"+lineLog->text());
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
    location=LOC_RECOVERY;

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

    animations[0]->setEndValue(QPoint(0-labelForgotPass->width(), labelForgotPass->y()));
    animations[1]->setEndValue(QPoint(buttonSignIn->x(), linePass->y()));
    animations[2]->setStartValue(QPoint(lineLog->x(), height()));
    animations[2]->setEndValue(QPoint(lineLog->x(), buttonSignIn->y()));
    animations[3]->setEndValue(QPoint(labelSignUp->x(),buttonSignIn->y()));
    animations[4]->setEndValue(QPoint(0-linePass->width(), linePass->y()));

    for(int i=0;i<5;i++){
        animations[i]->setDuration(DURATION);
        animations[i]->start(QAbstractAnimation::DeleteWhenStopped);
        if(i==2){
            labelSignIn->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            labelSignIn->show();
        }
    }

    buttonOk->setProperty("pos", QPoint(buttonSignIn->x(),linePass->y()));
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

    QPropertyAnimation *animations[5];

    animations[0] = new QPropertyAnimation(labelForgotPass, "pos");
    animations[1] = new QPropertyAnimation(buttonSignIn, "pos");
    animations[2] = new QPropertyAnimation(lineConfirmPass, "pos");
    animations[3] = new QPropertyAnimation(lineEmail, "pos");
    animations[4] = new QPropertyAnimation(labelSignUp, "pos");

    animations[0]->setEndValue(QPoint(0-labelForgotPass->width(), labelForgotPass->y()));
    animations[2]->setStartValue(QPoint(0-lineConfirmPass->width(), linePass->y() + lineConfirmPass->height() + height()/26));
    animations[2]->setEndValue(QPoint(lineLog->x(), linePass->y() + lineConfirmPass->height() + height()/26));
    animations[3]->setStartValue(QPoint(0-lineEmail->width(), lineLog->y() - lineConfirmPass->height() - height()/26));
    animations[3]->setEndValue(QPoint(lineLog->x(), lineLog->y() - lineConfirmPass->height() - height()/26));
    animations[4]->setEndValue(QPoint(labelSignUp->x(), buttonSignUp->y() + buttonSignUp->height() + height()/26));
    labelSignIn->move(labelSignUp->x(), buttonSignUp->y() + buttonSignUp->height() + height()/26);

    if(location==LOC_RECOVERY){
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
        localAnimations[0]->setEndValue(QPoint(lineLog->x(), linePass->y()));
        localAnimations[0]->start(QAbstractAnimation::DeleteWhenStopped);

        localAnimations[1]->setDuration(DURATION);
        localAnimations[1]->setEndValue(QPoint(buttonOk->x(), labelSignUp->y() + labelSignUp->height() + height()/26));
        localAnimations[1]->start(QAbstractAnimation::DeleteWhenStopped);

        animations[1]->setEndValue(QPoint(width(), lineConfirmPass->y()));

        buttonSignUp->move(buttonOk->x(), labelSignUp->y() + labelSignUp->height() + height()/26);
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

        animations[1]->setEndValue(QPoint(width(),buttonSignIn->y()));
        QPropertyAnimation *animation = new QPropertyAnimation(buttonSignUp, "pos");
        animation->setDuration(DURATION);
        animation->setStartValue(QPoint(width(), buttonSignUp->y()));
        animation->setEndValue(QPoint(buttonSignIn->x(), buttonSignUp->y()));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        buttonSignUp->show();
    }

    for(int i=0;i<5;i++){
        animations[i]->setDuration(DURATION);
        animations[i]->start(QAbstractAnimation::DeleteWhenStopped);
        if(i==2)
            lineConfirmPass->show();
        else if(i == 3)
            lineEmail->show();
    }

    location=LOC_SIGNUP;

    labelSignIn->setAlignment(Qt::AlignRight | Qt::AlignTop);
    connect(animations[4],SIGNAL(finished()), labelSignUp, SLOT(close()));
    connect(animations[4],SIGNAL(finished()), labelSignIn, SLOT(show()));
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

    QPropertyAnimation *animations[6];

    animations[0] = new QPropertyAnimation(labelForgotPass, "pos");
    animations[1] = new QPropertyAnimation(buttonSignIn, "pos");
    animations[2] = new QPropertyAnimation(lineConfirmPass, "pos");
    animations[3] = new QPropertyAnimation(lineEmail, "pos");
    animations[4] = new QPropertyAnimation(buttonSignUp, "pos");
    animations[5] = new QPropertyAnimation(labelSignIn, "pos");

    animations[0]->setEndValue(QPoint(lineLog->x(), buttonSignUp->y()));
    animations[1]->setStartValue(QPoint(width(), linePass->y() + linePass->height() + height()/26));
    animations[1]->setEndValue(QPoint((width() - buttonSignUp->width())/2, linePass->y() + linePass->height() + height()/26));
    animations[2]->setEndValue(QPoint(0-lineConfirmPass->width(), lineConfirmPass->y()));
    animations[3]->setEndValue(QPoint(0-lineEmail->width(), lineEmail->y()));
    animations[4]->setEndValue(QPoint(width(), buttonSignUp->y()));
    animations[5]->setEndValue(QPoint(labelSignIn->x(), buttonSignUp->y()));

    if(location==LOC_RECOVERY){
        animations[1]->setStartValue(QPoint(0-buttonSignIn->width(), linePass->y() + linePass->height() + height()/26));

        QPropertyAnimation *localAnimations[3];
        localAnimations[0] = new QPropertyAnimation(buttonOk,"pos");
        localAnimations[1] = new QPropertyAnimation(linePass,"pos");
        localAnimations[2] = new QPropertyAnimation(labelSignUp, "pos");

        localAnimations[0]->setDuration(DURATION);
        localAnimations[0]->setEndValue(QPoint(width(), buttonOk->y()));
        localAnimations[0]->start(QAbstractAnimation::DeleteWhenStopped);

        localAnimations[1]->setDuration(DURATION);
        localAnimations[1]->setEndValue(QPoint(lineLog->x(),linePass->y()));
        localAnimations[1]->start(QAbstractAnimation::DeleteWhenStopped);

        localAnimations[2]->setDuration(DURATION);
        localAnimations[2]->setEndValue(QPoint(labelSignUp->x(), labelSignUp->y() + labelSignUp->height() + height()/26));
        localAnimations[2]->start(QAbstractAnimation::DeleteWhenStopped);

        labelSignIn->close();
        connect(localAnimations[0], SIGNAL(finished()), buttonOk, SLOT(close()));
    }
    else{
        nicknameExists=false;
        lineLog->setPlaceholderText("Login or email");
        labelSignUp->setProperty("pos",QPoint(labelSignIn->x(), buttonSignUp->y()));
        connect(animations[5],SIGNAL(finished()), labelSignIn, SLOT(close()));
        connect(animations[5],SIGNAL(finished()), labelSignUp, SLOT(show()));
        connect(animations[4], SIGNAL(finished()), buttonSignUp, SLOT(close()));
    }

    for(int i=0;i<6;i++){
        animations[i]->setDuration(DURATION);
        animations[i]->start(QAbstractAnimation::DeleteWhenStopped);
    }
    location = LOC_SIGNIN;
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

void AuthWindow::passChange()
{
    linePass->setStyleSheet(QString("font-family: Century Gothic;"
                                  "font-size: %1px;"
                                  "background: transparent;"
                                   "color: #B5EBEE;").arg(defaultFontSize));
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

void AuthWindow::buttonMinimize_released(){
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setEndValue(0.0);
    animation->setDuration(200);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    connect(animation, SIGNAL(finished()), this, SLOT(showMinimized()));
}

void AuthWindow::startPreloader()
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
    else if(location==LOC_RECOVERY){
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

void AuthWindow::changeEvent(QEvent* e){
    if( e->type() == QEvent::WindowStateChange ){
        QWindowStateChangeEvent* event = static_cast< QWindowStateChangeEvent* >( e );

        if(event->oldState() & Qt::WindowMinimized){
            QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
            animation->setDuration(150);
            animation->setEndValue(1.0);
            this->showNormal();
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
}

void AuthWindow::cancelPreloading(){
        preloader->close();

        if(location == LOC_SIGNIN){

            buttonSignIn->show();
            buttonSignIn->setGraphicsEffect(opacity);

            QPropertyAnimation *animations[3];

            animations[0] = new QPropertyAnimation(labelSignUp, "pos");
            animations[0]->setDuration(DURATION);
            animations[0]->setEndValue(QPoint(labelSignUp->x(),buttonSignIn->y()+buttonSignIn->height()+this->height()/26));

            animations[1] = new QPropertyAnimation(labelForgotPass, "pos");
            animations[1]->setDuration(DURATION);
            animations[1]->setEndValue(QPoint(labelForgotPass->x(),buttonSignIn->y()+buttonSignIn->height()+this->height()/26));

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
            animations[0]->setEndValue(QPoint(labelSignIn->x(),buttonSignUp->y()+buttonSignUp->height()+this->height()/26));
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
        else if(location==LOC_RECOVERY){
            buttonOk->show();
            buttonOk->setGraphicsEffect(opacity);

            QPropertyAnimation *animations[3];

            animations[0] = new QPropertyAnimation(labelSignUp, "pos");
            animations[0]->setDuration(DURATION);
            animations[0]->setEndValue(QPoint(labelSignUp->x(),linePass->y()+linePass->height()+this->height()/26));

            animations[1] = new QPropertyAnimation(labelSignIn, "pos");
            animations[1]->setDuration(DURATION);
            animations[1]->setEndValue(QPoint(labelSignIn->x(),linePass->y()+linePass->height()+this->height()/26));

            animations[2] = new QPropertyAnimation(opacity, "opacity");
            animations[2]->setDuration(DURATION);
            animations[2]->setEndValue(1);

            animations[0]->start(QAbstractAnimation::DeleteWhenStopped);
            animations[1]->start(QAbstractAnimation::DeleteWhenStopped);
            animations[2]->start(QAbstractAnimation::DeleteWhenStopped);

            lineLog->setEnabled(true);
        }

        labelConnectionFailedBackground->show();
        QPropertyAnimation *animation = new QPropertyAnimation(labelConnectionFailedBackground, "pos");
        animation->setDuration(DURATION*2);
        animation->setEndValue(QPoint(0,labelConnectionFailedBackground->y()));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        QTimer::singleShot(5000, this, SLOT(errorHide()));
}

void AuthWindow::errorHide()
{
    QPropertyAnimation *animation = new QPropertyAnimation(labelConnectionFailedBackground, "pos");
    animation->setDuration(DURATION*2);
    animation->setEndValue(QPoint(0-labelConnectionFailedBackground->width(), labelConnectionFailedBackground->y()));
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    connect(animation, SIGNAL(finished()), labelConnectionFailedBackground, SLOT(close()));
}

void AuthWindow::test(){
    labelRegistrationSuccessful->setGraphicsEffect(opacity);
    QPropertyAnimation *animation = new QPropertyAnimation(opacity, "opacity");
    animation->setDuration(DURATION*2);
    animation->setStartValue(0);
    animation->setEndValue(1);
    labelRegistrationSuccessful->show();
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(5000, this, SLOT(registrationSuccess()));
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

void ClickableLabel::mouseReleaseEvent(QMouseEvent *){
    emit released();
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

    socket->close();
    delete socket;

    delete preloader;
    delete opacity;
}

ClickableLabel::ClickableLabel(QWidget* parent, bool isUnderlined) : QLabel(parent){
    this->isUnderlined = isUnderlined;
}

ClickableLabel::~ClickableLabel(){}
