#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    client = new MyUDP(CHAT_USER,CHAT_PASSWORD);

    ui->setupUi(this);
    /*
    ui->listWidget->addScrollBarWidget(new QScrollBar, Qt::AlignRight);
    //ui->listWidget->verticalScrollBar()->setFixedWidth(8);
    ui->listWidget->verticalScrollBar()->setStyleSheet(
                "QScrollBar:ver"
                "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
                "border: 2px solid grey;"
                "width: 3px;height: 3px;"
                "background: white;}"
                "QScrollBar::handle:vertical {"
                "border-radius: 8px;"
                "background: white;}");
     */

    ui->listWidget->setWordWrap(true);

    auth = new QWidget(this);
    log = new QLineEdit(auth);
    pass = new QLineEdit(auth);
    signIn = new QPushButton(auth);


    auth->setGeometry(0,0,400,400);
    log->setGeometry(60,70,140,20);
    pass->setGeometry(60,100,140,20);
    signIn->setGeometry(90,130,75,25);

    auth->setStyleSheet("background: magenta;");
    //log->setStyleSheet();

    log->setPlaceholderText("Login");
    pass->setPlaceholderText("Password");
    signIn->setText("Log in");

    log->show();
    pass->show();
    signIn->show();
    auth->show();

    ui->listWidget->close();
    ui->sendButton->close();
    ui->textEdit->close();

    //this->setAttribute(Qt::WA_TranslucentBackground);
    //this->setWindowFlags(Qt::FramelessWindowHint);

    connect(client, SIGNAL(updating()), this, SLOT(printMessages()));
    connect(ui->textEdit, SIGNAL(enter()), this, SLOT(on_sendButton_clicked()));
    connect(signIn, SIGNAL(released()), this, SLOT(signIn_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete log;
    delete pass;
    delete signIn;
    delete auth;
    delete client;
}

void MainWindow::printMessages(){
        ui->listWidget->addItem(client->nickname + ": " + client->buffer);
        ui->listWidget->scrollToBottom();
        client->buffer.clear();
}

void MainWindow::on_sendButton_clicked()
{
    QString a = ui->textEdit->toPlainText();

    if(a!=NULL){
    ui->textEdit->clear();
    client->sendMessage(client->sessionKey+'|'+a);
    }
}

/*
 *  QModelIndexList selectedList = ui->listWidget->selectionModel()->selectedIndexes();
    std::sort(selectedList.begin(),selectedList.end(),[](const QModelIndex& a, const QModelIndex& b)->bool{return a.row()>b.row();});
    for(const QModelIndex& singleIndex : selectedList)
    ui->listWidget->model()->removeRow(singleIndex.row());
*/

void MainWindow::signIn_clicked()
{
    log->close();
    pass->close();
    signIn->close();
    auth->close();
    ui->listWidget->show();
    ui->sendButton->show();
    ui->textEdit->show();
}
