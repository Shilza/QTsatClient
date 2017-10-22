#include "auth.h"
#include "ui_authwindow.h"

Auth::Auth(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Auth)
{
    ui->setupUi(this);

    log = new QLineEdit(this);
    pass = new QLineEdit(this);
    signIn = new QPushButton(this);

    log->setGeometry(60,70,140,20);
    pass->setGeometry(60,100,140,20);
    signIn->setGeometry(90,130,75,25);

    //auth->setStyleSheet("background: magenta;");

    log->setPlaceholderText("Login");
    pass->setPlaceholderText("Password");
    signIn->setText("Log in");

    log->show();
    pass->show();
    signIn->show();

    //this->setAttribute(Qt::WA_TranslucentBackground);
    //this->setWindowFlags(Qt::FramelessWindowHint);

    connect(signIn, SIGNAL(released()), this, SLOT(signIn_clicked()));
}

Auth::~Auth()
{
    delete ui;
}

void Auth::signIn_clicked()
{

}
