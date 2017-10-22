#include "authwindow.h"
#include "ui_authwindow.h"
#include <iostream>

AuthWindow::AuthWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AuthWindow)
{
    ui->setupUi(this);

    socket = new QUdpSocket(this);

    socket->bind(49002);
    host.setAddress(HOST_IP);

    log = new QLineEdit(this);
    pass = new QLineEdit(this);
    signIn = new QPushButton(this);

    log->setGeometry(60,70,140,20);
    pass->setGeometry(60,100,140,20);
    signIn->setGeometry(90,130,75,25);

    log->setPlaceholderText("Login");
    pass->setPlaceholderText("Password");
    signIn->setText("Log in");

    log->show();
    pass->show();
    signIn->show();

    connect(signIn, SIGNAL(released()), this, SLOT(signIn_released()));
    connect(socket, SIGNAL(readyRead()),this,SLOT(socketReading()));
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
    emit sessionKeyReceived(sessionKey);
    emit sessionKeyReceived();
}

void AuthWindow::signIn_released(){
    QString log = this->log->text();
    QString pass = this->pass->text();
    handshaking(log,pass);
}
