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
    signIn = new QPushButton(this);
    errorLabel = new QLabel(this);
    closeButton = new QPushButton(this);
    forgotPass = new ClickableLabel(this);
    signUp = new ClickableLabel(this);
    eye = new QPushButton(this);

    log->setGeometry((sizeX-((sizeX/13)*7))/2,(sizeY/13)*4,(sizeX/13)*7,sizeY/13);
    pass->setGeometry((sizeX-((sizeX/13)*7))/2,((sizeY/13)*4)+(sizeY/13)+(sizeY/26),(sizeX/13)*7,sizeY/13);
    signIn->setGeometry(90,140,75,25);
    errorLabel->setGeometry(70,140,140,20);
    closeButton->setGeometry(236,0,24,16);
    forgotPass->setGeometry(60,170,90,20);
    signUp->setGeometry(160,170,70,20);
    eye->setGeometry(180,((sizeY/13)*4)+(sizeY/13)+(sizeY/26),20,20);

    pass->setEchoMode(QLineEdit::Password);

    log->setPlaceholderText("Login");
    pass->setPlaceholderText("Password");
    signIn->setText("Log in");
    closeButton->setText("X");
    forgotPass->setText("Forgot password?");
    signUp->setText("Sign up");

    signIn->setStyleSheet("border-radius: 6px;"
                          "border: 1px solid black;"
                          "background: #ACE6A8;");
    closeButton->setStyleSheet("QPushButton{font-weight:bold; background: transparent;}"
                "QPushButton:hover{background: red; color: white; border: 0px;}");
    errorLabel->setStyleSheet("background: transparent;"
                              "color: #E94954;");
    forgotPass->setStyleSheet("background: transparent;"
                              "color: #B5EBEE;");
    signUp->setStyleSheet("background: transparent;"
                 "color: #B5EBEE;");

    eye->setStyleSheet("QPushButton{background: transparent;}"
                       "QPushButton:hover{background: rgba(123,55,65,50);}");
    eye->setIcon(QIcon(":fon/eye.png"));
    eye->setIconSize(QSize(20,20));
    errorLabel->close();

    connect(signIn, SIGNAL(released()), this, SLOT(signIn_released()));
    connect(eye, SIGNAL(released()), this, SLOT(eye_released()));
    connect(signUp, SIGNAL(released()),this, SLOT(signUp_released()));
    connect(socket, SIGNAL(readyRead()),this,SLOT(socketReading()));
    connect(closeButton, SIGNAL(released()), this, SLOT(close()));
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
    QByteArray sessionKey;
    sessionKey.resize(socket->pendingDatagramSize());
    socket->readDatagram(sessionKey.data(),sessionKey.size());
    if(sessionKey=="ERROR_AUTH"){
        errorLabel->setText("Wrong login or password");

        QPropertyAnimation *animations[3];
        animations[0]=new QPropertyAnimation(signIn, "pos");
        animations[1]=new QPropertyAnimation(forgotPass, "pos");
        animations[2]=new QPropertyAnimation(signUp, "pos");

        animations[0]->setEndValue(QPoint(90,164));
        animations[1]->setEndValue(QPoint(60,194));
        animations[2]->setEndValue(QPoint(160,194));

        for(int i=0;i<3;i++){
            animations[i]->setDuration(300);
            animations[i]->start(QAbstractAnimation::DeleteWhenStopped);
        }

        errorLabel->show();
    }
    else{
        emit sessionKeyReceived(sessionKey);
        emit sessionKeyReceived();
    }
}

void AuthWindow::signIn_released(){
    QString log = this->log->text();
    QString pass = this->pass->text();

    this->pass->setEchoMode(QLineEdit::Normal);

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
    if(this->pass->echoMode()==QLineEdit::Password)
        this->pass->setEchoMode(QLineEdit::Normal);
    else
        this->pass->setEchoMode(QLineEdit::Password);
}

void AuthWindow::forgotPass_released(){

}

void AuthWindow::signUp_released(){
    QPropertyAnimation *animations[2];
    animations[0] = new QPropertyAnimation(forgotPass, "pos");
    animations[1] = new QPropertyAnimation(signIn, "pos");

    animations[0]->setDuration(150);
    animations[0]->setEndValue(QPoint(-140,170));
    animations[0]->start(QAbstractAnimation::DeleteWhenStopped);

    animations[1]->setDuration(150);
    animations[1]->setEndValue(QPoint(260,140));
    animations[1]->start(QAbstractAnimation::DeleteWhenStopped);
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
