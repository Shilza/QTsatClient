#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    client = new UDPClient();

    ui->setupUi(this);
    this->setMaximumSize(800,600);

    mainWidget = new QWidget(this);
    mainLayout = new QGridLayout(mainWidget);
    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    listOfGlobalMessages = new QListWidget(mainWidget);
    //listOfGlobalMessages->setItemDelegate(new MyItemDelegate(listOfGlobalMessages));

    textMessage = new QTextEdit(mainWidget);
    buttonSend = new QPushButton(mainWidget);
    buttonPrivateMessages = new QPushButton(mainWidget);
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

    //textMessage->setMinimumSize(300,50);
    //textMessage->setMaximumSize(600,100);
    textMessage->setFixedHeight(40);
    textMessage->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    textMessage->setStyleSheet("border: 1px solid gray;"
                               "border-right: 0px;"
                               "border-top: 0px;");

    listOfGlobalMessages->setMinimumSize(300,250);
    //listOfGlobalMessages->setMaximumSize(600,700);
    listOfGlobalMessages->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    //buttonSend->setMinimumSize(50,50);
    //buttonSend->setMaximumSize(200,100);
    buttonSend->setFixedSize(40,40);
    buttonSend->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    buttonSend->setStyleSheet("border: 1px solid gray;"
                              "border-left: 0px;"
                              "border-top: 0px;"
                              "background: black;");

    buttonPrivateMessages->setMinimumSize(30,30);

    mainLayout->addWidget(listOfGlobalMessages, 0, 0, 8, 9);
    mainLayout->addWidget(textMessage, 8, 0, 1, 8);
    mainLayout->addWidget(buttonSend, 8, 8, 1, 1);
    mainLayout->addWidget(buttonPrivateMessages, 0,9,1,2);

    connect(buttonSend, SIGNAL(released()), this, SLOT(printMessages()));
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
    layout->setContentsMargins(2,0,5,0);
    layout->setSpacing(0);
    layout->setHorizontalSpacing(5);
    widget->setLayout(layout);

    QLabel *nickname = new QLabel("Sosik", widget);
    QLabel *timeOfMessage = new QLabel("22:45:11", widget);
    QLabel *textOfMessage = new QLabel("The purpose of this slot is to select a random line from our list of fortunes, encode it into a QByteArray using QDataStream, and then write it to the connecting socket. This is a common way to transfer binary data using QTcpSocket. First we create a QByteArray and a QDataStream object, passing the bytearray to QDataStream's constructor. We then explicitly set the protocol version of QDataStream to QDataStream::Qt_4_0 to ensure that we can communicate with clients from future versions of Qt (see QDataStream::setVersion()). We continue by streaming in a random fortune.", widget);
    QLabel *button = new QLabel("Sas", widget);

    button->setStyleSheet("background: black;");
    button->setFixedSize(30,30);

    textOfMessage->setWordWrap(true);
    //layout->addWidget(button, 0, 0, 3, 1, Qt::AlignHCenter | Qt::AlignTop);
    layout->addWidget(nickname, 0, 1, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(timeOfMessage, 0, 7, 1, 1, Qt::AlignRight | Qt::AlignTop);
    layout->addWidget(textOfMessage, 1, 1, 2, 7, Qt::AlignLeft | Qt::AlignTop);

    //widget->resize(layout->sizeHint());
    QWidgetItem *widgetItem = new QWidgetItem(button);
    widgetItem->setGeometry(QRect(0,0,30,30));
    layout->addItem(widgetItem, 0,0,3,1, Qt::AlignTop);

    QListWidgetItem* item = new QListWidgetItem(listOfGlobalMessages);
    item->setSizeHint(QSize(widget->width(), layout->sizeHint().height()));
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

