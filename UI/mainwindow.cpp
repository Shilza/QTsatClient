#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new UDPClient;
    resize(660,444);

    mainWidget = new QWidget(this);
    mainLayout = new QHBoxLayout(mainWidget);

    stackOfWidgets = new QStackedWidget(mainWidget);

    globalChatWidget = new QWidget(stackOfWidgets);
    globalChatLayout = new QGridLayout(globalChatWidget);

    menuListWidget = new QWidget(mainWidget);
    menuListLayout = new QVBoxLayout(menuListWidget);

    listOfGlobalMessages = new QListWidget(globalChatWidget);

    buttonUserPage = new QPushButton(menuListWidget);
    buttonPrivateMessages = new QPushButton(menuListWidget);
    buttonFriends = new QPushButton(menuListWidget);

    affixImageWidget = new AffixImageWidget(globalChatWidget);

    sendWidget = new SendWidget(globalChatWidget);

    imageView = new ImageView(this);

    setCentralWidget(mainWidget);

    menuListWidget->setLayout(menuListLayout);
    mainWidget->setLayout(mainLayout);
    mainLayout->setMargin(2);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(stackOfWidgets, 6);
    mainLayout->addWidget(menuListWidget, 1);

    listOfGlobalMessages->setMinimumSize(300, 250);
    listOfGlobalMessages->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    listOfGlobalMessages->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listOfGlobalMessages->setStyleSheet("border-color: gray;");
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

    QString buttonDefaultStyle = "QPushButton{"
                                 "background: transparent;"
                                 "border: 0px;"
                                 "}"
                                 "QPushButton:hover{"
                                 "background: #D3E9E9;"
                                 "}";

    buttonUserPage->setFixedSize(50, 50);
    buttonUserPage->setStyleSheet("background: black;"
                                  "border-radius: 25px;");

    buttonPrivateMessages->setFixedSize(120, 30);
    buttonPrivateMessages->setStyleSheet(buttonDefaultStyle);
    buttonPrivateMessages->setText("Messages");

    buttonFriends->setFixedSize(120, 30);
    buttonFriends->setStyleSheet(buttonDefaultStyle);
    buttonFriends->setText("Friends");

    globalChatWidget->setFixedSize(536, 440);
    globalChatWidget->setLayout(globalChatLayout);
    globalChatLayout->setSpacing(0);
    globalChatLayout->setMargin(3);

    globalChatLayout->addWidget(listOfGlobalMessages, 0, 0, 8, 9);
    globalChatLayout->addWidget(affixImageWidget->getSendedImage(), 6,0,2,9, Qt::AlignLeft | Qt::AlignBottom);
    globalChatLayout->addWidget(sendWidget->getMainWidget(), 8, 0, 2, 9);

    menuListLayout->setSpacing(0);
    menuListLayout->setContentsMargins(0, 4, 0, 0);
    menuListLayout->addWidget(buttonUserPage, 0, Qt::AlignTop | Qt::AlignCenter);
    menuListLayout->addWidget(buttonFriends, 0, Qt::AlignTop);
    menuListLayout->addWidget(buttonPrivateMessages, 0, Qt::AlignTop);
    menuListLayout->addWidget(new QWidget(menuListWidget), 7);

    stackOfWidgets->addWidget(globalChatWidget);
    stackOfWidgets->setCurrentWidget(globalChatWidget);

    connect(sendWidget, SIGNAL(messageSended()), this, SLOT(printMessages()));
    connect(sendWidget, SIGNAL(messageSended()), this, SLOT(sendMessage()));
    connect(sendWidget, SIGNAL(imageReceived(QPixmap)), affixImageWidget, SLOT(receivedImageTreatment(QPixmap)));
    connect(affixImageWidget, SIGNAL(originalSizeReleased(QPixmap)), imageView, SLOT(setPicture(QPixmap)));
    connect(affixImageWidget, SIGNAL(detachmentImage()), sendWidget, SLOT(decrementing()));
}

void MainWindow::start(QByteArray sessionKey){
    client->sessionKey = sessionKey;
    this->show();
}

void MainWindow::sendMessage(){

    /*    QString a = ui->textEdit->toPlainText();

    if(a!=NULL){
        ui->textEdit->clear();
        client->sendMessage(client->sessionKey+'|'+a);
    }
*/
}

void MainWindow::printMessages(){
    QWidget *widget = new QWidget(listOfGlobalMessages);
    QGridLayout *layout = new QGridLayout(widget);
    layout->setContentsMargins(2,5,5,5);
    layout->setSpacing(0);
    layout->setHorizontalSpacing(5);
    layout->setVerticalSpacing(5);
    widget->setLayout(layout);

    QLabel *nickname = new QLabel("Sosik", widget);
    QLabel *timeOfMessage = new QLabel("22:45:11", widget);
    //The purpose of this slot is to select a random line from our list of fortunes, encode it into a QByteArray using QDataStream, and then write it to the connecting socket. This is a common way to transfer binary data using QTcpSocket. First we create a QByteArray and a QDataStream object, passing the bytearray to QDataStream's constructor. We then explicitly set the protocol version of QDataStream to QDataStream::Qt_4_0 to ensure that we can communicate with clients from future versions of Qt (see QDataStream::setVersion()). We continue by streaming in a random fortune.
    WrapLabel *textOfMessage = new WrapLabel(widget);
    QLabel *button = new QLabel("Sas", widget);

    button->setStyleSheet("background: black;");
    button->setFixedSize(30,30);

    nickname->setFixedHeight(10);
    timeOfMessage->setFixedHeight(10);

    textOfMessage->setFixedWidth(450);
    textOfMessage->setWordWrap(true);
    textOfMessage->setStyleSheet("border: 0px;"
                                 "background:transparent;");
    textOfMessage->setTextInteractionFlags(textOfMessage->textInteractionFlags() | Qt::TextSelectableByMouse);
    textOfMessage->wrapText(sendWidget->getTextMessage()->toPlainText()); //temporary

    layout->addWidget(nickname, 0, 1, 1, 1);
    layout->addWidget(timeOfMessage, 0, 7, 1, 1, Qt::AlignRight);
    layout->addWidget(textOfMessage, 1, 1, 2, 7, Qt::AlignLeft | Qt::AlignTop);

    QWidgetItem *widgetItem = new QWidgetItem(button);
    widgetItem->setGeometry(QRect(0,0,30,30));
    layout->addItem(widgetItem, 0,0,3,1, Qt::AlignTop);

    QListWidgetItem* item = new QListWidgetItem(listOfGlobalMessages);
    item->setSizeHint(QSize(widget->width(), layout->sizeHint().height()));
    listOfGlobalMessages->setItemWidget(item, widget);
}

MainWindow::~MainWindow(){
    delete ui;
    delete client;

    delete globalChatWidget;
    delete globalChatLayout;
    delete listOfGlobalMessages;
    delete buttonPrivateMessages;
    delete buttonFriends;
}
