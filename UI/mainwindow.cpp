#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new UDPClient;
    this->resize(650,440);

    stackOfWidgets = new QStackedWidget(this);
    this->setCentralWidget(stackOfWidgets);

    mainWidget = new QWidget(stackOfWidgets);
    mainLayout = new QGridLayout(mainWidget);
    mainWidget->setLayout(mainLayout);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    sendWidget = new QWidget(mainWidget);
    sendLayout = new QGridLayout(sendWidget);
    sendWidget->setLayout(sendLayout);
    sendWidget->setStyleSheet("background: #E5F0F0;"
                              "border: 1px solid gray;"
                              "border-top: 0px;");
    sendLayout->setContentsMargins(7,5,7,5);
    sendLayout->setVerticalSpacing(5);
    sendLayout->setHorizontalSpacing(16);

    listOfGlobalMessages = new QListWidget(mainWidget);

    textMessage = new GlobalTextEdit(sendWidget);


    subAffixWidget = new QWidget(sendWidget);
    affixWidget = new QWidget(subAffixWidget);
    affixLayout = new QHBoxLayout(affixWidget);


    affixWidget->setFixedWidth(75);
    affixWidget->move(96, 6);

    affixWidget->setContentsMargins(0,0,0,0);
    affixLayout->setMargin(0);
    affixLayout->setSpacing(5);

    QPushButton *tempButtons[4];
    tempButtons[0] = new QPushButton(affixWidget);
    tempButtons[1] = new QPushButton(affixWidget);
    tempButtons[2] = new QPushButton(affixWidget);
    tempButtons[3] = new QPushButton(affixWidget);
    for(int i=0; i<4; i++){
        tempButtons[i]->setFixedSize(15, 15);
        affixLayout->addWidget(tempButtons[i], 1, Qt::AlignVCenter);
        tempButtons[i]->setStyleSheet("background: black;"
                                      "border: 1px solid white;");
    }


    affixWidget->setStyleSheet("background: black;");



    labelBicycle = new QLabel(sendWidget);
    buttonSend = new QPushButton(sendWidget);
    buttonPrivateMessages = new QPushButton(mainWidget);
    buttonFriends = new QPushButton(mainWidget);
    buttonAffix = new QPushButton(sendWidget);
    labelFloodError = new ClickableLabel(textMessage, false);
    labelBan = new ClickableLabel(textMessage, false);

    subAffixWidget->setStyleSheet("background: transparent;"
                                  "border: 0px;");

    floodTimer = new FloodTimer(textMessage);

    labelTimerShow = new QLabel(textMessage);
    labelTimerShow->setAlignment(Qt::AlignCenter);
    QFont fontTimerShow("Times New Roman", 11);
    labelTimerShow->setFont(fontTimerShow);
    labelTimerShow->setContentsMargins(0,0,5,2);
    labelTimerShow->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    labelTimerShow->setStyleSheet("background: transparent;"
                                  "border: 0px;");
    labelTimerShow->close();

    labelSymbolsCount = new QLabel(textMessage);
    labelSymbolsCount->setContentsMargins(0,0,5,2);
    labelSymbolsCount->setFont(QFont("Times New Roman", 11));
    labelSymbolsCount->setStyleSheet("background: transparent;"
                                     "border: 0px;");
    labelSymbolsCount->close();


    affixWidget->setLayout(affixLayout);
    labelBicycle->setStyleSheet("background: #E5F0F0;"
                                "border: 0px;");

    buttonAffix->setStyleSheet("background: transparent;"
                         "border: 0px;");
    //affix->setFixedSize(19,26);
    buttonAffix->setFixedSize(15,20);
    buttonAffix->setIcon(QIcon(":images/affix30.png"));
    buttonAffix->setIconSize(QSize(15,20));

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

    textMessage->setFixedHeight(48);
    textMessage->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    textMessage->setStyleSheet("border-radius: 8px;"
                               "border: 1px solid gray;"
                               "background-color: white;"
                               );
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

    listOfGlobalMessages->setMinimumSize(300,250);
    listOfGlobalMessages->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    listOfGlobalMessages->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listOfGlobalMessages->setStyleSheet("border-color: gray;");

    buttonSend->setFixedSize(70,26);
    buttonSend->setText("Send");
    buttonSend->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    /*
    buttonSend->setStyleSheet("background: black;"
                              "border-radius: 10px;"
                              "color: white;");
                              */
    buttonSend->setStyleSheet("background: transparent;"
                              "border: 2px solid black;"
                              "border-radius: 10px;"
                              "color: black;");
    QString buttonDefaultStyle = "QPushButton{"
                                 "background: transparent;"
                                 "border: 0px;"
                                 "}"
                                 "QPushButton:hover{"
                                 "background: #D3E9E9;"
                                 "}";

    buttonPrivateMessages->setMinimumSize(30,30);
    buttonPrivateMessages->setStyleSheet(buttonDefaultStyle);
    buttonPrivateMessages->setText("Messages");

    buttonFriends->setMinimumSize(30,30);
    buttonFriends->setStyleSheet(buttonDefaultStyle);
    buttonFriends->setText("Friends");

    sendLayout->addWidget(textMessage, 0, 0, 1, 9);
    sendLayout->addWidget(labelFloodError, 0, 0, 1, 8);
    sendLayout->addWidget(labelTimerShow, 0, 8, 1, 1);
    sendLayout->addWidget(labelSymbolsCount, 0,0,1,9, Qt::AlignRight | Qt::AlignBottom);
    sendLayout->addWidget(labelBan, 0, 0, 1, 8);
    sendLayout->addWidget(labelBicycle, 1, 7, -1, -1);
    sendLayout->addWidget(buttonSend, 1, 8, 1, 1);
    sendLayout->addWidget(buttonAffix, 1, 7, 1, 1);
    sendLayout->addWidget(subAffixWidget, 1, 5, -1, -1);

    mainLayout->setContentsMargins(8,2,0,6);
    mainLayout->addWidget(listOfGlobalMessages, 0, 0, 8, 9);
    mainLayout->addWidget(sendWidget, 8,0,2,9);
    mainLayout->addWidget(buttonFriends, 0,9,1,2);
    mainLayout->addWidget(buttonPrivateMessages, 1,9,1,2);



    stackOfWidgets->addWidget(mainWidget);
    stackOfWidgets->setCurrentWidget(mainWidget);

    labelFloodError->setStyleSheet("color: red;"
                                   "background: transparent;"
                                   "border: 0px;");
    QFont fontGothic("Century Gothic");
    fontGothic.setBold(true);
    fontGothic.setPointSize(16);
    labelFloodError->setFont(fontGothic);
    labelFloodError->setAlignment(Qt::AlignCenter);
    labelFloodError->setText("Flood");
    labelFloodError->close();

    labelBan->setStyleSheet("color: red;"
                            "background: transparent;"
                            "border: 0px;");
    fontGothic.setBold(true);
    fontGothic.setPointSize(16);
    labelBan->setFont(fontGothic);
    labelBan->setAlignment(Qt::AlignCenter);
    labelBan->setText("Ban");
    labelBan->close();

    buttonAffix->installEventFilter(this);
    connect(buttonSend, SIGNAL(released()), this, SLOT(printMessages()));
    connect(textMessage, SIGNAL(enter()), this, SLOT(printMessages()));
    connect(textMessage, SIGNAL(enter()), this, SLOT(sendMessage()));
    connect(buttonSend, SIGNAL(released()), this, SLOT(sendMessage()));
    connect(floodTimer, SIGNAL(errorTimeout()), this, SLOT(floodErrorHide()));
    connect(floodTimer, SIGNAL(showTimeout()), this, SLOT(updateTime()));
    connect(labelBan, SIGNAL(released()), SLOT(close()));
    connect(textMessage, SIGNAL(textChanged()), this, SLOT(showSymbolsCount()));
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

void MainWindow::floodErrorHide()
{
    labelFloodError->close();
    labelTimerShow->close();
    textMessage->setEnabled(true);
    buttonSend->setEnabled(true);
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

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if(target == buttonAffix){
        if (event->type() == QEvent::HoverEnter){
            buttonAffix->setIcon(QIcon(":/images/affix30gray.png"));
            QPropertyAnimation *animation = new QPropertyAnimation(affixWidget, "pos");
            animation->setEndValue(QPoint(9, 6));
            animation->setDuration(300);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        else if(event->type() == QEvent::HoverLeave)
            buttonAffix->setIcon(QIcon(":/images/affix30.png"));
    }
    return QMainWindow::eventFilter(target, event);
}

MainWindow::~MainWindow(){
    delete ui;
    delete client;

    delete mainWidget;
    delete mainLayout;
    delete listOfGlobalMessages;
    delete buttonSend;
    delete buttonPrivateMessages;
    delete buttonFriends;
    delete textMessage;
    delete labelFloodError;
    delete labelTimerShow;

    delete floodTimer;
}


