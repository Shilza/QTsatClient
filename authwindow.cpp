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
    //this->setFixedSize(sizeX,sizeY);
    //this->setStyleSheet("background: #C8A4E5;");
    this->setStyleSheet("background-image: url(:fon/fon.png);");

    lineLog = new LineEdit(this);
    linePass = new LineEdit(this);
    lineConfirmPass = new LineEdit(this);
    lineEmail = new LineEdit(this);
    buttonSignIn = new QPushButton(this);
    buttonSignUp = new QPushButton(this);
    buttonOk = new QPushButton(this);
    buttonMinimize = new QPushButton(this);
    buttonClose = new QPushButton(this);
    buttonEye = new QPushButton(linePass);
    labelError = new QLabel(this);
    labelForgotPass = new ClickableLabel(this);
    labelSignUp = new ClickableLabel(this);
    labelSignIn = new ClickableLabel(this);
    labelUncorrectNickname = new QLabel(this);

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
    buttonSignIn->setText("Sign in");
    buttonSignUp->setText("Sign Up");
    buttonOk->setText("Ok");
    labelForgotPass->setText("Forgot password?");
    labelForgotPass->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    labelSignUp->setText("Sign up");
    labelSignUp->setAlignment(Qt::AlignRight | Qt::AlignTop);
    labelSignIn->setText("Sign in");
    labelUncorrectNickname->setText("Nickname already exists");
    labelUncorrectNickname->setAlignment(Qt::AlignCenter | Qt::AlignBottom);


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

    int defaultFontSize = (width()/260)*11;
    buttonSignIn->setStyleSheet(strButtonStyle.arg(defaultFontSize));
    buttonSignUp->setStyleSheet(strButtonStyle.arg(defaultFontSize));
    buttonOk->setStyleSheet(strButtonStyle.arg(defaultFontSize));

    labelError->setStyleSheet(QString("font-family: Century Gothic;"
                                      "font-size: %1px;"
                                      "background: transparent;"
                                      "color: #E94954;").arg(defaultFontSize));

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
    labelUncorrectNickname->setStyleSheet(QString("font-family: Century Gothic;"
                                                  "font-size: %1px;").arg((defaultFontSize/11)*9));

    lineConfirmPass->setStyleSheet(QString("font-family: Century Gothic;"
                                           "font-size: %1px;"
                                           "background: transparent;"
                                           "color: #B5EBEE;").arg(defaultFontSize));
    linePass->setStyleSheet(QString("font-family: Century Gothic;"
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

    QFontMetrics *tempFontSize = new QFontMetrics(labelUncorrectNickname->font());
    labelUncorrectNickname->setProperty("size", QSize(tempFontSize->width(labelUncorrectNickname->text()), tempFontSize->height()));
    delete tempFontSize;

    labelError->close();
    labelSignIn->close();
    buttonOk->close();
    buttonSignUp->close();
    lineConfirmPass->close();
    lineEmail->close();
    labelUncorrectNickname->close();

    connect(buttonSignIn, SIGNAL(released()), this, SLOT(signIn_released()));
    connect(buttonEye, SIGNAL(released()), this, SLOT(eye_released()));
    connect(labelSignUp, SIGNAL(released()),this, SLOT(signUpLabel_released()));
    connect(labelSignIn, SIGNAL(released()),this, SLOT(signInLabel_released()));
    connect(labelForgotPass, SIGNAL(released()), this,SLOT(forgotPass_released()));
    connect(buttonOk, SIGNAL(released()), this, SLOT(passRecovery_released()));
    connect(lineConfirmPass, SIGNAL(textChanged(QString)),this, SLOT(checkingConfirming(QString)));
    connect(lineLog, SIGNAL(textChanged(QString)), this, SLOT(logChange(QString)));
    connect(lineLog, SIGNAL(editingFinished()), this, SLOT(checkingNickname()));
    connect(socket, SIGNAL(readyRead()),this,SLOT(socketReading()));
    connect(buttonClose, SIGNAL(released()), this, SLOT(close()));
    connect(buttonMinimize, SIGNAL(released()), this, SLOT(buttonMinimize_released()));
}


void AuthWindow::resizeAll()
{
    //  quint16 windowSize = (QApplication::desktop()->width()/100)*20;

    quint16 windowSize = (1366/100)*20;
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

    quint16 labelErrorW = windowSize/26*7;
    quint16 labelErrorH = windowSize/13*7;
    quint16 labelErrorX = labelErrorH;
    quint16 labelErrorY = windowSize/13;

    quint16 labelSignInW = labelSignUpW;
    quint16 labelSignInH = labelSignUpH;
    quint16 labelSignInX_inSIGNUP = labelSignUpX_inSIGNIN;
    quint16 labelSignInY_inSIGNUP = buttonSignUpY + buttonSignUpH + windowSize/26;

    quint16 labelUncorrectNicknameX = lineLogX;
    quint16 labelUncorrectNicknameY = lineLogY - windowSize/26 - labelUncorrectNickname->height();

    lineLog->setGeometry(lineLogX, lineLogY, lineLogW, lineLogH);
    linePass->setGeometry(linePassX, linePassY, linePassW, linePassH);
    lineConfirmPass->resize(lineConfirmPassW, lineConfirmPassH);
    lineEmail->resize(lineEmailW, lineEmailH);

    buttonSignIn->setGeometry(buttonSignInX, buttonSignInY, buttonSignInW,buttonSignInH);
    buttonSignUp->setGeometry(buttonSignUpX, buttonSignUpY, buttonSignUpW, buttonSignUpH);
    buttonOk->setGeometry(buttonOkX, buttonOkY, buttonOkW, buttonOkH);
    buttonClose->setGeometry(buttonCloseX, 0, buttonCloseW, buttonCloseH);
    buttonMinimize->setGeometry(buttonMinimizeX, 0, buttonMinimizeW, buttonMinimizeH);
    buttonEye->setGeometry(buttonEyeX, 0, buttonEyeW, buttonEyeH);

    labelError->setGeometry(labelErrorX, labelErrorY, labelErrorW, labelErrorH);
    labelForgotPass->setGeometry(labelForgotPassX, labelForgotPassY, labelForgotPassW, labelForgotPassH);
    labelSignUp->setGeometry(labelSignUpX_inSIGNIN, labelSignUpY_inSIGNIN, labelSignUpW,labelSignUpH);
    labelSignIn->setGeometry(labelSignInX_inSIGNUP, labelSignInY_inSIGNUP, labelSignInW, labelSignInH);
    labelUncorrectNickname->setProperty("pos", QPoint(labelUncorrectNicknameX, labelUncorrectNicknameY));
}

void AuthWindow::handshaking(QString log, QString pass)
{
    QByteArray par;
    socket->writeDatagram(par.append("handshake|"+log+"|"+pass), host, 49003);
}

void AuthWindow::socketReading()
{
    QByteArray serverAnswer;
    serverAnswer.resize(socket->pendingDatagramSize());
    socket->readDatagram(serverAnswer.data(),serverAnswer.size());

    if(serverAnswer=="ERROR_AUTH"){
        labelError->setText("Wrong login or password");

        QPropertyAnimation *animations[3];
        animations[0]=new QPropertyAnimation(buttonSignIn, "pos");
        animations[1]=new QPropertyAnimation(labelForgotPass, "pos");
        animations[2]=new QPropertyAnimation(labelSignUp, "pos");

        animations[0]->setEndValue(QPoint(90,164));
        animations[1]->setEndValue(QPoint(60,194));
        animations[2]->setEndValue(QPoint(160,194));

        for(int i=0;i<3;i++){
            animations[i]->setDuration(DURATION);
            animations[i]->start(QAbstractAnimation::DeleteWhenStopped);
        }

        labelError->show();
    }
    else if(serverAnswer=="EXIST" || serverAnswer=="NEXIST"){
        if(serverAnswer=="EXIST")
            lineLog->setStyleSheet("border: 1px solid red;");
        else
            lineLog->setStyleSheet("QLineEdit:hover{ border: 2px solid grey;}");
    }
    else{
        emit sessionKeyReceived(serverAnswer);
        emit sessionKeyReceived();
    }
}

void AuthWindow::signIn_released(){
    QString log = this->lineLog->text();
    QString pass = this->linePass->text();

    if(log=="")
        return;
    else if(pass==""){
        this->linePass->setStyleSheet("border: 1px solid red;");
        return;
    }

    this->linePass->setStyleSheet("QLineEdit:hover{ border: 2px solid grey;}");

    handshaking(log,pass);
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

void AuthWindow::forgotPass_released(){
    location=LOC_RECOVERY;

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
    labelSignIn->setProperty("pos",QPoint(labelSignUp->x(), buttonSignUp->y() + buttonSignUp->height() + height()/26));

    if(location==LOC_RECOVERY){
        labelSignIn->close();

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

        buttonSignUp->setProperty("pos", QPoint(buttonOk->x(), labelSignUp->y() + labelSignUp->height() + height()/26));
        connect(localAnimations[1], SIGNAL(finished()), buttonOk, SLOT(close()));
        connect(localAnimations[1], SIGNAL(finished()), buttonSignUp, SLOT(show()));
    }
    else{
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

void AuthWindow::passRecovery_released(){}

void AuthWindow::checkingConfirming(QString text){
    if(text=="")
        lineConfirmPass->setStyleSheet("QLineEdit:hover{ border: 2px solid grey;}");
    else if(text!=this->linePass->text())
        lineConfirmPass->setStyleSheet("border: 1px solid red;");
    else
        lineConfirmPass->setStyleSheet("QLineEdit:hover{ border: 2px solid grey;}");
}

void AuthWindow::logChange(QString text){
    if(text =="" && location==LOC_SIGNUP)
        lineLog->setStyleSheet("QLineEdit:hover{ border: 2px solid grey;}");
}

void AuthWindow::checkingNickname(){
    if(location==LOC_SIGNUP && lineLog->text()!=""){
        QByteArray par;
        socket->writeDatagram(par.append("DoesExNick|"+lineLog->text()), host, 49003);
    }
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
        QWindowStateChangeEvent* event = static_cast< QWindowStateChangeEvent* >( e );

        if(event->oldState() & Qt::WindowMinimized){
            QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
            animation->setDuration(150);
            animation->setEndValue(1.0);
            this->showNormal();
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        /*     else if(event->oldState() == Qt::WindowNoState && this->windowState() == Qt::WindowMaximized){
            qDebug() << "Window Maximized!";
        }*/
    }
}

void AuthWindow::mousePressEvent(QMouseEvent *event){
    mpos = event->pos();
}

void AuthWindow::mouseMoveEvent(QMouseEvent *event){
    if (mpos.x() >= 0 && event->buttons() && Qt::LeftButton){
        QPoint diff = event->pos() - mpos;
        QPoint newpos = this->pos() + diff;

        this->move(newpos);
    }
}

void AuthWindow::mouseReleaseEvent(QMouseEvent *){
    mpos = QPoint(-1, -1);
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *){
    emit released();
}

bool AuthWindow::eventFilter(QObject *target, QEvent *event){
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
        if(event->type() == QEvent::HoverEnter){
            qDebug() << "sos";
        }
        else if(event->type() == QEvent::HoverLeave){
            qDebug() << "sas";
        }
    }

    return QMainWindow::eventFilter(target, event);
}

void ClickableLabel::enterEvent(QEvent* event)
{
    QFont f = font();
    f.setUnderline(true);
    setFont(f);
}

void ClickableLabel::leaveEvent(QEvent* event)
{
    QFont f = font();
    f.setUnderline(false);
    setFont(f);
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
}

ClickableLabel::ClickableLabel(QWidget* parent) : QLabel(parent){}

ClickableLabel::~ClickableLabel(){}
