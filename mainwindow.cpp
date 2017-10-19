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
    connect(client, SIGNAL(updating()), this, SLOT(printMessages()));
    connect(ui->textEdit, SIGNAL(enter()), this, SLOT(on_sendButton_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
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
