#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new UDPClient;
    resize(650,440);

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

    sendedImage = new QPushButton(globalChatWidget);
    sendedImage->installEventFilter(this);

    toolTipAffixClose = new QLabel(globalChatWidget);

    picture = new QLabel(this);

    originalSize = new QLabel(sendedImage);

    buttonCloseAffixedPicture = new QPushButton(sendedImage);

    originalSize->setGeometry(1, 1, 68, 68);
    originalSize->setPixmap(QPixmap(":/images/originalSize.png").scaled(68,68));
    originalSize->setStyleSheet("background: transparent;"
                                "border: 0px;");
    originalSize->close();

    setCentralWidget(mainWidget);

    menuListWidget->setLayout(menuListLayout);
    mainWidget->setLayout(mainLayout);
    mainLayout->setMargin(2);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(stackOfWidgets, 6);
    mainLayout->addWidget(menuListWidget, 1);

    initSendWidget();

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

    sendedImage->setFixedSize(70,70);
    sendedImage->setStyleSheet("border: 1px solid black;");
    sendedImage->close();

    buttonCloseAffixedPicture->setGeometry(56, 2, 12, 12);
    buttonCloseAffixedPicture->setIcon(QIcon(":/images/affixClose.png"));
    buttonCloseAffixedPicture->setIconSize(QSize(12, 12));
    buttonCloseAffixedPicture->setStyleSheet("background: white;"
                                             "border-radius: 6px;"
                                             "border: 0px;");
    buttonCloseAffixedPicture->installEventFilter(this);

    toolTipAffixClose->setStyleSheet("background: transparent;"
                                     "border: 0px;");
    toolTipAffixClose->setGeometry(0, 0, 100, 26);
    toolTipAffixClose->setPixmap(QPixmap(":/images/toolTipAffixClose.png").scaled(toolTipAffixClose->width(), toolTipAffixClose->height()));
    toolTipAffixClose->close();

    globalChatWidget->setFixedSize(536, 440);
    globalChatWidget->setLayout(globalChatLayout);
    globalChatLayout->setSpacing(0);
    globalChatLayout->setMargin(3);

    globalChatLayout->addWidget(listOfGlobalMessages, 0, 0, 8, 9);
    globalChatLayout->addWidget(sendedImage, 6,0,2,9, Qt::AlignLeft);
    globalChatLayout->addWidget(sendWidget, 8, 0, 2, 9);

    menuListLayout->setSpacing(0);
    menuListLayout->setContentsMargins(0, 4, 0, 0);
    menuListLayout->addWidget(buttonUserPage, 0, Qt::AlignTop | Qt::AlignCenter);
    menuListLayout->addWidget(buttonFriends, 0, Qt::AlignTop);
    menuListLayout->addWidget(buttonPrivateMessages, 0, Qt::AlignTop);
    menuListLayout->addWidget(new QWidget(menuListWidget), 7);

    stackOfWidgets->addWidget(globalChatWidget);
    stackOfWidgets->setCurrentWidget(globalChatWidget);

    connect(buttonSend, SIGNAL(released()), this, SLOT(printMessages()));
    connect(textMessage, SIGNAL(enter()), this, SLOT(printMessages()));
    connect(textMessage, SIGNAL(enter()), this, SLOT(sendMessage()));
    connect(buttonSend, SIGNAL(released()), this, SLOT(sendMessage()));
    connect(floodTimer, SIGNAL(errorTimeout()), this, SLOT(floodErrorHide()));
    connect(floodTimer, SIGNAL(showTimeout()), this, SLOT(updateTime()));
    connect(textMessage, SIGNAL(textChanged()), this, SLOT(showSymbolsCount()));
    connect(sendedImage, SIGNAL(released()), this, SLOT(showAffixedPicture()));
    connect(buttonCloseAffixedPicture, SIGNAL(released()), this, SLOT(buttonCloseAffixedPicture_released()));
    connect(textMessage, SIGNAL(imageReceived(QPixmap)), this, SLOT(receivedImageTreatment(QPixmap)));
}

void MainWindow::initSendWidget(){
    sendWidget = new QWidget(globalChatWidget);
    sendLayout = new QGridLayout(sendWidget);

    textMessage = new GlobalTextEdit(sendWidget);
    subAffixWidget = new QWidget(sendWidget);
    affixWidget = new QPushButton(subAffixWidget);
    affixLayout = new QHBoxLayout(affixWidget);

    buttonPhotos = new QPushButton(affixWidget);
    buttonVideos = new QPushButton(affixWidget);
    buttonAudios = new QPushButton(affixWidget);
    buttonDocuments = new QPushButton(affixWidget);

    labelBicycle = new QLabel(sendWidget);
    buttonSend = new QPushButton(sendWidget);

    buttonAffix = new QPushButton(sendWidget);
    labelFloodError = new ClickableLabel(textMessage, false);
    labelBan = new QLabel(textMessage);

    floodTimer = new FloodTimer(textMessage);

    labelTimerShow = new QLabel(textMessage);
    labelSymbolsCount = new QLabel(textMessage);

    subAffixWidget->setStyleSheet("background: transparent;"
                                  "border: 0px;");

    labelBicycle->setStyleSheet("background: #E5F0F0;"
                                "border: 0px;");

    sendWidget->setLayout(sendLayout);
    sendWidget->setStyleSheet("background: #E5F0F0;"
                              "border: 1px solid gray;"
                              "border-top: 0px;");

    affixWidget->setLayout(affixLayout);
    affixWidget->setMaximumHeight(100);
    affixWidget->setMinimumHeight(19);
    affixWidget->setFixedWidth(118);
    affixWidget->move(150, 6);
    affixWidget->setContentsMargins(0,0,0,4);
    affixWidget->setStyleSheet("background: transparent;"
                               "border-bottom: 1px solid black;");

    buttonDocuments->setIcon(QIcon(":/images/documentsGray"));
    buttonAudios->setIcon(QIcon(":/images/audiosGray.png"));
    buttonVideos->setIcon(QIcon(":/images/videosGray.png"));
    buttonPhotos->setIcon(QIcon(":/images/photosGray.png"));

    buttonPhotos->setFixedSize(15, 15);
    buttonVideos->setFixedSize(15, 15);
    buttonAudios->setFixedSize(15, 15);
    buttonDocuments->setFixedSize(15, 15);

    buttonPhotos->setStyleSheet("border: 0px;");
    buttonVideos->setStyleSheet("border: 0px;");
    buttonAudios->setStyleSheet("border: 0px;");
    buttonDocuments->setStyleSheet("border: 0px;");

    buttonPhotos->setIconSize(QSize(15,15));
    buttonVideos->setIconSize(QSize(15,15));
    buttonAudios->setIconSize(QSize(15,15));
    buttonDocuments->setIconSize(QSize(15,15));

    affixLayout->setMargin(0);
    affixLayout->setSpacing(8);

    affixLayout->addWidget(buttonDocuments, 1, Qt::AlignVCenter);
    affixLayout->addWidget(buttonVideos, 1, Qt::AlignVCenter);
    affixLayout->addWidget(buttonAudios, 1, Qt::AlignVCenter);
    affixLayout->addWidget(buttonPhotos, 1, Qt::AlignVCenter);

    labelTimerShow->setAlignment(Qt::AlignCenter);
    labelTimerShow->setFont(QFont("Times New Roman", 11));
    labelTimerShow->setContentsMargins(0,0,5,2);
    labelTimerShow->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labelTimerShow->setStyleSheet("background: transparent;"
                                  "border: 0px;");
    labelTimerShow->close();


    labelSymbolsCount->setContentsMargins(0,0,5,2);
    labelSymbolsCount->setFont(QFont("Times New Roman", 10));
    labelSymbolsCount->setStyleSheet("background: transparent;"
                                     "border: 0px;");
    labelSymbolsCount->close();


    buttonAffix->setFixedSize(15,20);
    buttonAffix->setIcon(QIcon(":images/affix30.png"));
    buttonAffix->setIconSize(QSize(15,20));
    buttonAffix->setStyleSheet("background: transparent;"
                               "border: 0px;");

    textMessage->setFixedHeight(50);
    textMessage->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    textMessage->setStyleSheet("border-radius: 8px;"
                               "border: 1px solid gray;"
                               "background-color: white;");
    textMessage->setContentsMargins(2,1,2,1);
    textMessage->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{"
                                    "background: white;"
                                    "border-top-right-radius: 1px;"
                                    "border-bottom-right-radius: 1px;"
                                    "width: 8px;"
                                    "margin: 0px;"
                                    "}"
                                    "QScrollBar:handle:vertical{"
                                    "background: gray;"
                                    "border-radius: 1px;"
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

    buttonSend->setFixedSize(70, 26);
    buttonSend->setCursor(Qt::PointingHandCursor);
    buttonSend->setText("Send");
    buttonSend->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    buttonSend->setStyleSheet("background: transparent;"
                              "border: 2px solid black;"
                              "border-radius: 10px;"
                              "color: black;");

    sendLayout->setContentsMargins(7,5,7,5);
    sendLayout->setVerticalSpacing(5);
    sendLayout->setHorizontalSpacing(16);

    sendLayout->addWidget(textMessage,       0, 0, 1, 9);
    sendLayout->addWidget(labelFloodError,   0, 0, 1, 8);
    sendLayout->addWidget(labelTimerShow,    0, 8, 1, 1);
    sendLayout->addWidget(labelSymbolsCount, 0, 0, 1, 9, Qt::AlignRight | Qt::AlignBottom);
    sendLayout->addWidget(labelBan,          0, 0, 1, 8);
    sendLayout->addWidget(labelBicycle,      1, 7, -1, -1);
    sendLayout->addWidget(buttonSend,        1, 8, 1, 1);
    sendLayout->addWidget(buttonAffix,       1, 7, 1, 1);
    sendLayout->addWidget(subAffixWidget,    1, 4, -1, -1);

    QFont fontGothic("Century Gothic");
    fontGothic.setBold(true);
    fontGothic.setPointSize(16);
    labelFloodError->setFont(fontGothic);
    labelFloodError->setAlignment(Qt::AlignCenter);
    labelFloodError->setText("Flood");
    labelFloodError->setStyleSheet("color: red;"
                                   "background: transparent;"
                                   "border: 0px;");
    labelFloodError->close();


    fontGothic.setBold(true);
    fontGothic.setPointSize(16);
    labelBan->setFont(fontGothic);
    labelBan->setAlignment(Qt::AlignCenter);
    labelBan->setText("Ban");
    labelBan->setStyleSheet("color: red;"
                            "background: transparent;"
                            "border: 0px;");
    labelBan->close();

    buttonAffix->installEventFilter(this);
    affixWidget->installEventFilter(this);
    buttonPhotos->installEventFilter(this);
    buttonVideos->installEventFilter(this);
    buttonAudios->installEventFilter(this);
    buttonDocuments->installEventFilter(this);
}

void MainWindow::start(QByteArray sessionKey){
    client->sessionKey = sessionKey;
    this->show();
}

void MainWindow::showSymbolsCount()
{
    labelSymbolsCount->setText(QString::number(textMessage->toPlainText().length())+"/"+QString::number(MAX_GLOBAL_MESSAGE_SIZE));
    labelSymbolsCount->show();
}

void MainWindow::receivedImageTreatment(QPixmap image)
{
    affixImage = image;
    if(image.height() > image.width()){
        int leftTop = image.height()/2-image.width()/2;

        QRect rect(0, leftTop, image.width(), image.width());
        image = image.copy(rect);
    }
    else if(image.height() < image.width()){
        int leftTop = image.width()/2-image.height()/2;

        QRect rect(leftTop, 0, image.height(), image.height());
        image = image.copy(rect);
    }

    if(image.width()<68)
        image = image.scaled(68,68);

    sendedImage->setIcon(QIcon(image));
    sendedImage->setIconSize(QSize(68,68));
    sendedImage->show();
}

void MainWindow::buttonCloseAffixedPicture_released(){
    sendedImage->close();
}

void MainWindow::showAffixedPicture(){
    picture->setGeometry(50,50, affixImage.width(), affixImage.height());
    picture->setPixmap(affixImage);
}

void MainWindow::sendMessage(){
    QString text = textMessage->toPlainText();
    if(lastMessages.size()>=3){
        QVector<quint8> levenshteinDistances;
        for(QString comparisonString : lastMessages)
            levenshteinDistances.push_back(levenshteinDistance(text.toStdString(), comparisonString.toStdString()));
        quint8 count=0;
        for(quint8 temp : levenshteinDistances){
            if(temp<=(text.length()/2 < 1 ? 1 : text.length()/2))
                count++;
        }
        if(count>=3 && floodTimer->getCounter()<3){
            labelFloodError->show();
            labelTimerShow->show();
            textMessage->setDisabled(true);
            buttonSend->setDisabled(true);
            buttonAffix->setDisabled(true);
            floodTimer->start();
            return;
        }
        else if(count>=3){
            labelBan->show();
            textMessage->setDisabled(true);
        }
    }
    if(lastMessages.size()==5)
        lastMessages.pop_front();
    lastMessages.push_back(text);


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
    textOfMessage->wrapText(textMessage->toPlainText());

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

void MainWindow::floodErrorHide(){
    labelFloodError->close();
    labelTimerShow->close();
    textMessage->setEnabled(true);
    buttonSend->setEnabled(true);
    buttonAffix->setEnabled(true);
    textMessage->setFocus();
}

void MainWindow::updateTime()
{
    int time = floodTimer->remainingTime()/10;

    QString result = QString::number(float(time)/100.0);
    if(time%100 == 0)
        result += ".00";
    else while(time!=0)
        if(time%10 != 0)
            break;
        else{
            result += "0";
            time = time/10;
        }
    labelTimerShow->setText(result);
}

bool MainWindow::eventFilter(QObject *target, QEvent *event){
    if((target == buttonAffix || target == affixWidget) && buttonAffix->isEnabled()){
        if (event->type() == QEvent::HoverEnter){
            buttonAffix->setIcon(QIcon(":/images/affix30gray.png"));
            QPropertyAnimation *animation = new QPropertyAnimation(affixWidget, "pos");
            animation->setEndValue(QPoint(30, 6));
            animation->setDuration(200);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        else if (event->type() == QEvent::HoverLeave){
            buttonAffix->setIcon(QIcon(":/images/affix30.png"));
            QPropertyAnimation *animation = new QPropertyAnimation(affixWidget, "pos");
            animation->setEndValue(QPoint(150, 6));
            animation->setDuration(200);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
    else if(target == buttonPhotos){
        if(event->type() == QEvent::HoverEnter)
            buttonPhotos->setIcon(QIcon(":/images/photos.png"));
        else if(event->type() == QEvent::HoverLeave)
            buttonPhotos->setIcon(QIcon(":/images/photosGray.png"));
    }
    else if(target == buttonAudios){
        if(event->type() == QEvent::HoverEnter)
            buttonAudios->setIcon(QIcon(":/images/audios.png"));
        else if(event->type() == QEvent::HoverLeave)
            buttonAudios->setIcon(QIcon(":/images/audiosGray.png"));
    }
    else if(target == buttonVideos){
        if(event->type() == QEvent::HoverEnter)
            buttonVideos->setIcon(QIcon(":/images/videos.png"));
        else if(event->type() == QEvent::HoverLeave)
            buttonVideos->setIcon(QIcon(":/images/videosGray.png"));
    }
    else if(target == buttonDocuments){
        if(event->type() == QEvent::HoverEnter)
            buttonDocuments->setIcon(QIcon(":/images/documents.png"));
        else if(event->type() == QEvent::HoverLeave)
            buttonDocuments->setIcon(QIcon(":/images/documentsGray.png"));
    }
    else if(target == buttonCloseAffixedPicture){
        if(event->type() == QEvent::HoverEnter){
            QPoint point = globalChatWidget->mapFromGlobal(QCursor::pos());
            toolTipAffixClose->move(point.x()-20, point.y()-toolTipAffixClose->height()-toolTipAffixClose->height()/5);
            toolTipAffixClose->show();
        }
        else if(event->type() == QEvent::HoverLeave)
            toolTipAffixClose->close();
    }
    else if(target==sendedImage){
        if(event->type() == QEvent::HoverEnter)
            originalSize->show();
        else if(event->type() == QEvent::HoverLeave)
            originalSize->close();
    }

    return QMainWindow::eventFilter(target, event);
}

MainWindow::~MainWindow(){
    delete ui;
    delete client;

    delete globalChatWidget;
    delete globalChatLayout;
    delete listOfGlobalMessages;
    delete buttonSend;
    delete buttonPrivateMessages;
    delete buttonFriends;
    delete textMessage;
    delete labelFloodError;
    delete labelTimerShow;

    delete floodTimer;
}
