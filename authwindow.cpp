#include "authwindow.h"
#include "ui_authwindow.h"

AuthWindow::AuthWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AuthWindow)
{
    ui->setupUi(this);

    socket = new QUdpSocket(this);

    socket->bind(49002);
    host.setAddress(HOST_IP);

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(260,260);
    this->setStyleSheet("background: #C8A4E5;");

    log = new QLineEdit(this);
    pass = new QLineEdit(this);
    signIn = new QPushButton(this);
    errorLabel = new QLabel(this);
    closeButton = new QPushButton(this);

    log->setGeometry(60,80,140,20);
    pass->setGeometry(60,110,140,20);
    signIn->setGeometry(90,140,75,25);
    errorLabel->setGeometry(70,140,140,20);
    closeButton->setGeometry(236,0,24,16);

    log->setPlaceholderText("Login");
    pass->setPlaceholderText("Password");
    signIn->setText("Log in");
    closeButton->setText("X");

    closeButton->setStyleSheet("color: red;");
    errorLabel->setStyleSheet("color: #E94954;");
    errorLabel->close();

    connect(signIn, SIGNAL(released()), this, SLOT(signIn_released()));
    connect(socket, SIGNAL(readyRead()),this,SLOT(socketReading()));
    connect(closeButton, SIGNAL(released()), this, SLOT(close()));
}

AuthWindow::~AuthWindow()
{
    delete ui;
}

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

        QPropertyAnimation *animation = new QPropertyAnimation(signIn, "pos");
        animation->setDuration(300);
        animation->setEndValue(QPoint(90,164));
        animation->start();

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

    if(log=="")
        return;
    else if(pass==""){
        this->pass->setStyleSheet("border: 1px solid red;");
        return;
    }
    this->pass->setStyleSheet("QLineEdit:hover{ border: 2px solid grey;}");

    handshaking(log,pass);
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
