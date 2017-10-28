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
    this->setFixedSize(sizeX,sizeY);
    //this->setStyleSheet("background: #C8A4E5;");
    this->setStyleSheet("background-image: url(:fon/fon.png);");

    log = new QLineEdit(this);
    pass = new QLineEdit(this);
    confirmPass = new QLineEdit(this);
    email = new QLineEdit(this);
    buttonSignIn = new QPushButton(this);
    buttonSignUp = new QPushButton(this);
    buttonPassRecovery = new QPushButton(this);
    errorLabel = new QLabel(this);
    buttonClose = new QPushButton(this);
    forgotPass = new ClickableLabel(this);
    signUpLabel = new ClickableLabel(this);
    signInLabel = new ClickableLabel(this);
    buttonEye = new QPushButton(pass);

    log->setGeometry((sizeX-((sizeX/13)*7))/2,(sizeY/13)*4,(sizeX/13)*7,sizeY/13);
    pass->setGeometry((sizeX-((sizeX/13)*7))/2,((sizeY/13)*4)+(sizeY/13)+(sizeY/26),(sizeX/13)*7,sizeY/13);
    confirmPass->setGeometry(-140,140,(sizeX/13)*7,sizeY/13);
    email->setGeometry(-140, 50, 140,20);
    buttonSignIn->setGeometry(90,140,75,25);
    buttonSignUp->setGeometry(260, 170,75,25);
    buttonPassRecovery->setGeometry(90,110, 75,25);
    errorLabel->setGeometry(70,140,140,20);
    buttonClose->setGeometry(236,0,24,16);
    forgotPass->setGeometry(60,170,90,20);
    signUpLabel->setGeometry(160,170,70,20);
    signInLabel->setGeometry(160,200,70,20);
    buttonEye->setGeometry(120,0,20,20);
    buttonEye->setCursor(Qt::ArrowCursor);

    email->setValidator(new QRegExpValidator(QRegExp("([^@])+(@?)([^@])+")));
    log->setValidator(new QRegExpValidator(QRegExp("([^@])+(@?)([^@])+")));
    pass->setValidator(new QRegExpValidator(QRegExp("^(\\d|\\w|[._]){6,32}$")));

    pass->setEchoMode(QLineEdit::Password);
    confirmPass->setEchoMode(QLineEdit::Password);
    signInLabel->close();
    buttonPassRecovery->close();

    log->setPlaceholderText("Login or email");
    pass->setPlaceholderText("Password");
    confirmPass->setPlaceholderText("Confirm password");
    email->setPlaceholderText("Email");
    buttonSignIn->setText("Sign in");
    buttonSignUp->setText("Sign Up");
    buttonPassRecovery->setText("Ok");
    buttonClose->setText("X");
    forgotPass->setText("Forgot password?");
    signUpLabel->setText("Sign up");
    signInLabel->setText("Sign in");

    buttonSignIn->setStyleSheet("border-radius: 6px;"
                          "border: 1px solid black;"
                          "background: #ACE6A8;");
    buttonSignUp->setStyleSheet("border-radius: 6px;"
                          "border: 1px solid black;"
                          "background: #ACE6A8;");
    buttonPassRecovery->setStyleSheet("border-radius: 6px;"
                                "border: 1px solid black;"
                                "background: #ACE6A8;");

    buttonClose->setStyleSheet("QPushButton{font-weight:bold; background: transparent;}"
                "QPushButton:hover{background: red; color: white; border: 0px;}");
    errorLabel->setStyleSheet("background: transparent;"
                              "color: #E94954;");
    forgotPass->setStyleSheet("background: transparent;"
                              "color: #B5EBEE;");
    signUpLabel->setStyleSheet("background: transparent;"
                 "color: #B5EBEE;");
    signInLabel->setStyleSheet("background: transparent;"
                               "color: #B5EBEE;");

    buttonEye->setStyleSheet("QPushButton{background: transparent;}"
                       "QPushButton:hover{background: rgba(123,55,65,50);}");
    buttonEye->setIcon(QIcon(":fon/eye.png"));
    buttonEye->setIconSize(QSize(20,20));
    errorLabel->close();

    connect(buttonSignIn, SIGNAL(released()), this, SLOT(signIn_released()));
    connect(buttonEye, SIGNAL(released()), this, SLOT(eye_released()));
    connect(signUpLabel, SIGNAL(released()),this, SLOT(signUpLabel_released()));
    connect(signInLabel, SIGNAL(released()),this, SLOT(signInLabel_released()));
    connect(forgotPass, SIGNAL(released()), this,SLOT(forgotPass_released()));
    connect(buttonPassRecovery, SIGNAL(released()), this, SLOT(passRecovery_released()));
    connect(confirmPass, SIGNAL(textChanged(QString)),this, SLOT(checkingConfirming(QString)));
    connect(log, SIGNAL(textChanged(QString)), this, SLOT(logChange(QString)));
    connect(log, SIGNAL(editingFinished()), this, SLOT(checkingNickname()));
    connect(socket, SIGNAL(readyRead()),this,SLOT(socketReading()));
    connect(buttonClose, SIGNAL(released()), this, SLOT(close()));
}

ClickableLabel::ClickableLabel(QWidget* parent)
    : QLabel(parent)
{}

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
        errorLabel->setText("Wrong login or password");

        QPropertyAnimation *animations[3];
        animations[0]=new QPropertyAnimation(buttonSignIn, "pos");
        animations[1]=new QPropertyAnimation(forgotPass, "pos");
        animations[2]=new QPropertyAnimation(signUpLabel, "pos");

        animations[0]->setEndValue(QPoint(90,164));
        animations[1]->setEndValue(QPoint(60,194));
        animations[2]->setEndValue(QPoint(160,194));

        for(int i=0;i<3;i++){
            animations[i]->setDuration(DURATION);
            animations[i]->start(QAbstractAnimation::DeleteWhenStopped);
        }

        errorLabel->show();
    }
    else if(serverAnswer=="EXIST" || serverAnswer=="NEXIST"){
        if(serverAnswer=="EXIST")
            log->setStyleSheet("border: 1px solid red;");
        else
            log->setStyleSheet("QLineEdit:hover{ border: 2px solid grey;}");
    }
    else{
        emit sessionKeyReceived(serverAnswer);
        emit sessionKeyReceived();
    }
}

void AuthWindow::signIn_released(){

    QString log = this->log->text();
    QString pass = this->pass->text();

    if(log=="")
        return;
    else if(pass==""){
        this->pass->setStyleSheet("border: 1px solid red;");
        return;
    }

    this->pass->setStyleSheet("QLineEdit:hover{ border: 2px solid grey;}");

    handshaking(log,pass);
}

void AuthWindow::eye_released(){
    if(pass->echoMode()==QLineEdit::Password){
        pass->setEchoMode(QLineEdit::Normal);
        confirmPass->setEchoMode(QLineEdit::Normal);
    }
    else{
        pass->setEchoMode(QLineEdit::Password);
        confirmPass->setEchoMode(QLineEdit::Password);
    }
}

void AuthWindow::forgotPass_released(){
    location=LOC_RECOVERY;

    signInLabel->setProperty("pos", QPoint(60, 260));
    signInLabel->show();

    QPropertyAnimation *animations[5];

    animations[0] = new QPropertyAnimation(forgotPass, "pos");
    animations[1] = new QPropertyAnimation(buttonSignIn, "pos");
    animations[2] = new QPropertyAnimation(signInLabel, "pos");
    animations[3] = new QPropertyAnimation(signUpLabel, "pos");
    animations[4] = new QPropertyAnimation(pass, "pos");

    animations[0]->setEndValue(QPoint(-90,170));
    animations[1]->setEndValue(QPoint(90,110));
    animations[2]->setEndValue(QPoint(60, 140));
    animations[3]->setEndValue(QPoint(160,140));
    animations[4]->setEndValue(QPoint(-140,110));

    for(int i=0;i<5;i++){
        animations[i]->setDuration(DURATION);
        animations[i]->start(QAbstractAnimation::DeleteWhenStopped);
    }

    buttonPassRecovery->setProperty("pos", QPoint(90,110));
    connect(animations[1], SIGNAL(finished()), buttonPassRecovery, SLOT(show()));
    connect(animations[1], SIGNAL(finished()), buttonSignIn, SLOT(close()));
}

void AuthWindow::signUpLabel_released(){
    log->clear();
    pass->clear();

    log->setPlaceholderText("Nickname");
    log->setValidator(new QRegExpValidator(QRegExp("^(\\d|\\w|[_]){2,12}$")));

    QPropertyAnimation *animations[5];

    animations[0] = new QPropertyAnimation(forgotPass, "pos");
    animations[1] = new QPropertyAnimation(buttonSignIn, "pos");
    animations[2] = new QPropertyAnimation(confirmPass, "pos");
    animations[3] = new QPropertyAnimation(email, "pos");
    animations[4] = new QPropertyAnimation(signUpLabel, "pos");

    animations[0]->setEndValue(QPoint(-90,170));
    animations[2]->setEndValue(QPoint(60,140));
    animations[3]->setEndValue(QPoint(60,50));
    animations[4]->setEndValue(QPoint(160,200));

    if(location==LOC_RECOVERY){
        buttonSignUp->close();
        signInLabel->close();

        QPropertyAnimation *localAnimations[2];
        localAnimations[0] = new QPropertyAnimation(pass, "pos");
        localAnimations[1] = new QPropertyAnimation(buttonPassRecovery, "pos");

        localAnimations[0]->setDuration(DURATION);
        localAnimations[0]->setEndValue(QPoint(60,110));
        localAnimations[0]->start(QAbstractAnimation::DeleteWhenStopped);

        localAnimations[1]->setDuration(DURATION);
        localAnimations[1]->setEndValue(QPoint(90,170));
        localAnimations[1]->start(QAbstractAnimation::DeleteWhenStopped);

        animations[1]->setEndValue(QPoint(260,110));

        buttonSignUp->setProperty("pos", QPoint(90,170));
        connect(localAnimations[1], SIGNAL(finished()), buttonPassRecovery, SLOT(close()));
        connect(localAnimations[1], SIGNAL(finished()), buttonSignUp, SLOT(show()));
    }
    else{
        animations[1]->setEndValue(QPoint(260,140));
        QPropertyAnimation *animation = new QPropertyAnimation(buttonSignUp, "pos");
        animation->setDuration(DURATION);
        animation->setEndValue(QPoint(90,170));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }

    for(int i=0;i<5;i++){
        animations[i]->setDuration(DURATION);
        animations[i]->start(QAbstractAnimation::DeleteWhenStopped);
    }

    location=LOC_SIGNUP;
    signInLabel->setProperty("pos",QPoint(160,200));
    connect(animations[4],SIGNAL(finished()), signUpLabel, SLOT(close()));
    connect(animations[4],SIGNAL(finished()), signInLabel, SLOT(show()));
}

void AuthWindow::signInLabel_released(){
    log->clear();
    pass->clear();
    email->clear();
    confirmPass->clear();

    log->setValidator(new QRegExpValidator(QRegExp("([^@])+(@?)([^@])+")));
    buttonSignIn->setProperty("pos",QPoint(-140,140));
    buttonSignIn->show();

    QPropertyAnimation *animations[6];

    animations[0] = new QPropertyAnimation(forgotPass, "pos");
    animations[1] = new QPropertyAnimation(buttonSignIn, "pos");
    animations[2] = new QPropertyAnimation(confirmPass, "pos");
    animations[3] = new QPropertyAnimation(email, "pos");
    animations[4] = new QPropertyAnimation(buttonSignUp, "pos");
    animations[5] = new QPropertyAnimation(signInLabel, "pos");

    animations[0]->setEndValue(QPoint(60,170));
    animations[1]->setEndValue(QPoint(90,140));
    animations[2]->setEndValue(QPoint(-140,140));
    animations[3]->setEndValue(QPoint(-140, 50));
    animations[4]->setEndValue(QPoint(260, 170));
    animations[5]->setEndValue(QPoint(160,170));

    if(location==LOC_RECOVERY){

        QPropertyAnimation *localAnimations[3];
        localAnimations[0] = new QPropertyAnimation(buttonPassRecovery,"pos");
        localAnimations[1] = new QPropertyAnimation(pass,"pos");
        localAnimations[2] = new QPropertyAnimation(signUpLabel, "pos");

        localAnimations[0]->setDuration(DURATION);
        localAnimations[0]->setEndValue(QPoint(260,110));
        localAnimations[0]->start(QAbstractAnimation::DeleteWhenStopped);

        localAnimations[1]->setDuration(DURATION);
        localAnimations[1]->setEndValue(QPoint(60,110));
        localAnimations[1]->start(QAbstractAnimation::DeleteWhenStopped);

        localAnimations[2]->setDuration(DURATION);
        localAnimations[2]->setEndValue(QPoint(160,170));
        localAnimations[2]->start(QAbstractAnimation::DeleteWhenStopped);

        signInLabel->close();
        connect(localAnimations[0], SIGNAL(finished()), buttonPassRecovery, SLOT(close()));
    }
    else{
        log->setPlaceholderText("Login or email");
        signUpLabel->setProperty("pos",QPoint(160,170));
        connect(animations[5],SIGNAL(finished()), signInLabel, SLOT(close()));
        connect(animations[5],SIGNAL(finished()), signUpLabel, SLOT(show()));
    }

    for(int i=0;i<6;i++){
        animations[i]->setDuration(DURATION);
        animations[i]->start(QAbstractAnimation::DeleteWhenStopped);
    }
    location = LOC_SIGNIN;

}

void AuthWindow::passRecovery_released(){}

void AuthWindow::checkingConfirming(QString text)
{
    if(text=="")
        confirmPass->setStyleSheet("QLineEdit:hover{ border: 2px solid grey;}");
    else if(text!=this->pass->text())
        confirmPass->setStyleSheet("border: 1px solid red;");
    else
        confirmPass->setStyleSheet("QLineEdit:hover{ border: 2px solid grey;}");
}

void AuthWindow::logChange(QString text)
{
    if(text =="" && location==LOC_SIGNUP)
        log->setStyleSheet("QLineEdit:hover{ border: 2px solid grey;}");
}

void AuthWindow::checkingNickname()
{
    if(location==LOC_SIGNUP && log->text()!=""){
        QByteArray par;
        socket->writeDatagram(par.append("DoesExNick|"+log->text()), host, 49003);
    }
}

void AuthWindow::mousePressEvent(QMouseEvent *event)
{
    mpos = event->pos();
}

void AuthWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mpos.x() >= 0 && event->buttons() && Qt::LeftButton)
    {
        QPoint diff = event->pos() - mpos;
        QPoint newpos = this->pos() + diff;

        this->move(newpos);
    }
}

void AuthWindow::mouseReleaseEvent(QMouseEvent *)
{
    mpos = QPoint(-1, -1);
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *)
{
    emit released();
}

AuthWindow::~AuthWindow()
{
    delete ui;
}

ClickableLabel::~ClickableLabel(){

}
