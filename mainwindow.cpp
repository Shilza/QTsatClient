#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    client = new UDPClient();

    ui->setupUi(this);

    mainWidget = new QWidget(this);
    mainLayout = new QGridLayout(mainWidget);
    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(2);

    listOfGlobalMessages = new QListWidget(mainWidget);
    textMessage = new QTextEdit(mainWidget);
    buttonSend = new QPushButton(mainWidget);
    listOfGlobalMessages->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{"
                                                             "background: white;"
                                                             "border-top-right-radius: 4px;"
                                                             "border-bottom-right-radius: 4px;"
                                                             "width: 8px;"
                                                             "margin: 0px;"
                                                             "}"
                                                             "QScrollBar:handle:vertical{"
                                                             "background: gray;"
                                                             "border-radius: 2px;"
                                                             "min-height: 20px;"
                                                             "margin: 0px 2px 0px 2px;"
                                                             "}"
                                                             "QScrollBar:add-line:vertical{"
                                                             "background: transparent;"
                                                             "height: 0px;"
                                                             "subcontrol-position: right;"
                                                             "subcontrol-origin: margin;"
                                                             "}"
                                                             "QScrollBar:sub-line:vertical{"
                                                             "background: transparent;"
                                                             "height: 0px;"
                                                             "subcontrol-position: left;"
                                                             "subcontrol-origin: margin;"
                                                             "}");

/*    listOfGlobalMessages->setMinimumSize(200,100);
    listOfGlobalMessages->setMaximumSize(400,200);
    listOfGlobalMessages->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);*/
//    (QWidget *, int row, int column, int rowSpan, int columnSpan, Qt::Alignment = Qt::Alignment());
    mainLayout->addWidget(listOfGlobalMessages, 0, 0, 1, 8);
    mainLayout->addWidget(textMessage, 1, 0, 6, 7);
    mainLayout->addWidget(buttonSend, 1, 7, 6, 1);
    connect(buttonSend, SIGNAL(released()), this, SLOT(printMessages()));

 //   this->setFixedSize(360,250);
}

MainWindow::~MainWindow(){
    delete ui;
    delete client;
}

void MainWindow::start(QByteArray sessionKey){
    client->sessionKey = sessionKey;
    this->show();
}

void MainWindow::on_sendButton_clicked(){
/*    QString a = ui->textEdit->toPlainText();

    if(a!=NULL){
        ui->textEdit->clear();
        client->sendMessage(client->sessionKey+'|'+a);
    }*/
}

void MainWindow::printMessages(){
    QWidget *widget = new QWidget(listOfGlobalMessages);
    QGridLayout *layout = new QGridLayout(widget);
    layout->setContentsMargins(0,0,5,0);
    layout->setSpacing(0);

    QPushButton *buttonTemp = new QPushButton(widget);
    buttonTemp->setMinimumSize(20,20);
    buttonTemp->setMaximumSize(100,100);
    buttonTemp->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//    buttonTemp->setStyleSheet("border-image: url(testIcon.png) stretch;");

    layout->addWidget(buttonTemp, 0, 0, 3, 1, Qt::AlignCenter);
    layout->addWidget(new QLabel("Sosik", this), 0, 1, 1, 1, Qt::AlignLeft | Qt::AlignBottom);
    layout->addWidget(new QLabel("22:45:11"), 0, 7, 1, 1, Qt::AlignRight | Qt::AlignBottom);
    layout->addWidget(new QLabel("TEXT"), 1, 1, 2, 7);


    QListWidgetItem* item = new QListWidgetItem(listOfGlobalMessages);
    item->setSizeHint(QSize(listOfGlobalMessages->width()-5-listOfGlobalMessages->verticalScrollBar()->width(), 50));
    listOfGlobalMessages->setItemWidget(item, widget);
}

/*
 *  QModelIndexList selectedList = ui->listWidget->selectionModel()->selectedIndexes();
    std::sort(selectedList.begin(),selectedList.end(),[](const QModelIndex& a, const QModelIndex& b)->bool{return a.row()>b.row();});
    for(const QModelIndex& singleIndex : selectedList)
    ui->listWidget->model()->removeRow(singleIndex.row());
*/

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent){
}

void TextEdit::keyPressEvent(QKeyEvent *e){
    bool previousSpace = false;
    bool previousEndl = false;
    bool prePreviousEndl = false;

    if(this->toPlainText().length() > 0){
        previousSpace = this->toPlainText().at(this->toPlainText().length()-1) == " ";
        previousEndl = this->toPlainText().at(this->toPlainText().length()-1) == "\n";
    }

    if(this->toPlainText().length() > 1)
        prePreviousEndl = this->toPlainText().at(this->toPlainText().length()-2) == "\n";

    if((e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)){
        if(e->modifiers() != Qt::ControlModifier){
            emit enter();
            return;
        }
        else if((!previousSpace && !previousEndl) || !prePreviousEndl){
          QTextEdit::keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter,Qt::NoModifier));
          return;
        }
    }
    else if(e->key() == Qt::Key_Space && (previousSpace || previousEndl))
        return;

    QTextEdit::keyPressEvent(e);
}

