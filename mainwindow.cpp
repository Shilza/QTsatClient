#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    clientt = new MyUDP;

    ui->setupUi(this);
    ui->listWidget->addScrollBarWidget(ui->verticalScrollBar, Qt::AlignRight);
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

    ui->textEdit->installEventFilter(this);
    connect(clientt, SIGNAL(updating()), this, SLOT(printMessages()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printMessages(){
    if(clientt->buffer.size()!=0){
        ui->listWidget->addItem(clientt->buffer);
        ui->listWidget->scrollToBottom();
    }
}

void MainWindow::on_sendButton_clicked()
{
    QString a = ui->textEdit->toPlainText();

    if(a!=NULL){
    ui->textEdit->clear();
    clientt->HelloUDP(a);
    }
}
