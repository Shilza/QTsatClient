#include "sendwidget.h"

SendWidget::SendWidget(QWidget *parent): QWidget(parent){
    mainWidget = new QWidget(this);
    sendLayout = new QGridLayout(mainWidget);

    textMessage = new GlobalTextEdit(mainWidget);
    subAffixWidget = new QWidget(mainWidget);
    affixWidgetContainer = new QPushButton(subAffixWidget);
    affixLayout = new QHBoxLayout(affixWidgetContainer);

    buttonPhotos = new QPushButton(affixWidgetContainer);
    buttonVideos = new QPushButton(affixWidgetContainer);
    buttonAudios = new QPushButton(affixWidgetContainer);
    buttonDocuments = new QPushButton(affixWidgetContainer);

    labelBicycle = new QLabel(mainWidget);
    buttonSend = new QPushButton(mainWidget);

    buttonAffix = new QPushButton(mainWidget);
    labelFloodError = new ClickableLabel(textMessage, false);
    labelBan = new QLabel(textMessage);

    floodTimer = new FloodTimer(textMessage);

    labelTimerShow = new QLabel(textMessage);
    labelSymbolsCount = new QLabel(textMessage);

    subAffixWidget->setStyleSheet("background: transparent;"
                                  "border: 0px;");

    labelBicycle->setStyleSheet("background: #E5F0F0;"
                                "border: 0px;");

    mainWidget->setLayout(sendLayout);
    mainWidget->setStyleSheet("background: #E5F0F0;"
                        "border: 1px solid gray;"
                        "border-top: 0px;");

    affixWidgetContainer->setLayout(affixLayout);
    affixWidgetContainer->setMaximumHeight(100);
    affixWidgetContainer->setMinimumHeight(19);
    affixWidgetContainer->setFixedWidth(118);
    affixWidgetContainer->move(150, 6);
    affixWidgetContainer->setContentsMargins(0,0,0,4);
    affixWidgetContainer->setStyleSheet("background: transparent;"
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
    affixWidgetContainer->installEventFilter(this);
    buttonPhotos->installEventFilter(this);
    buttonVideos->installEventFilter(this);
    buttonAudios->installEventFilter(this);
    buttonDocuments->installEventFilter(this);

    connect(buttonSend, SIGNAL(released()), this, SLOT(send()));
    connect(textMessage, SIGNAL(enter()),this, SLOT(send()));
    connect(textMessage, SIGNAL(textChanged()), this, SLOT(showSymbolsCount()));
    connect(floodTimer, SIGNAL(errorTimeout()), this, SLOT(floodErrorHide()));
    connect(floodTimer, SIGNAL(showTimeout()), this, SLOT(updateTime()));
    connect(textMessage, SIGNAL(imageReceived(QPixmap)), this, SLOT(imageReceivedRedirect(QPixmap)));
    connect(buttonPhotos, SIGNAL(released()), this, SLOT(selectImage()));
}

void SendWidget::floodErrorHide(){
    labelFloodError->close();
    labelTimerShow->close();
    textMessage->setEnabled(true);
    buttonSend->setEnabled(true);
    buttonAffix->setEnabled(true);
    textMessage->setFocus();
}

void SendWidget::updateTime(){
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

void SendWidget::showSymbolsCount(){
    labelSymbolsCount->setText(QString::number(textMessage->toPlainText().length())+"/"+QString::number(MAX_GLOBAL_MESSAGE_SIZE));
    labelSymbolsCount->show();
}

void SendWidget::send()
{
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

    emit messageSended();
}

void SendWidget::imageReceivedRedirect(QPixmap image){
    if(countOfAttachment<1){
        countOfAttachment++;
        emit imageReceived(image);
    }
}

void SendWidget::selectImage(){
    static QString lastPath = QDir::homePath();
    QString temp = QFileDialog::getOpenFileName(this, QObject::tr("Choose an image"), lastPath, QObject::tr("Image file (*.png *.jpg *.jpeg *.jpe *.bmp);;Все файлы (*.*)"));
    if(temp != ""){
        imageReceivedRedirect(QPixmap(temp));
        lastPath = temp;
    }

}

void SendWidget::decrementing(){
    countOfAttachment--;
}

SendWidget::~SendWidget(){
    delete affixWidgetContainer;
    delete subAffixWidget;
    delete affixLayout;
    delete labelBicycle;
    delete buttonAffix;
    delete textMessage;
    delete buttonSend;
    delete labelFloodError;
    delete labelBan;
    delete labelTimerShow;
    delete labelSymbolsCount;
    delete sendLayout;

    delete buttonPhotos;
    delete buttonVideos;
    delete buttonAudios;
    delete buttonDocuments;
}

QWidget *SendWidget::getMainWidget(){
    return mainWidget;
}

GlobalTextEdit *SendWidget::getTextMessage(){
    return textMessage;
}

bool SendWidget::eventFilter(QObject *target, QEvent *event){
    if((target == buttonAffix || target == affixWidgetContainer) && buttonAffix->isEnabled()){
        if (event->type() == QEvent::HoverEnter){
            buttonAffix->setIcon(QIcon(":/images/affix30gray.png"));
            QPropertyAnimation *animation = new QPropertyAnimation(affixWidgetContainer, "pos");
            animation->setEndValue(QPoint(30, 6));
            animation->setDuration(200);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        else if (event->type() == QEvent::HoverLeave){
            buttonAffix->setIcon(QIcon(":/images/affix30.png"));
            QPropertyAnimation *animation = new QPropertyAnimation(affixWidgetContainer, "pos");
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

    return QWidget::eventFilter(target, event);
}
