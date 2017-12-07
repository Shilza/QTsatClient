#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    client = new UDPClient();

    this->resize(650,400);

    mainWidget = new QWidget(this);
    mainLayout = new QGridLayout(mainWidget);
    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    listOfGlobalMessages = new QListWidget(mainWidget);

    textMessage = new GlobalTextEdit(mainWidget);
    buttonSend = new QPushButton(mainWidget);
    buttonPrivateMessages = new QPushButton(mainWidget);
    buttonFriends = new QPushButton(mainWidget);
    labelFloodError = new ClickableLabel(listOfGlobalMessages, false);
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

    //textMessage->setMinimumSize(300,50);
    //textMessage->setMaximumSize(600,100);
    textMessage->setFixedHeight(40);
    textMessage->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    textMessage->setStyleSheet("border: 1px solid gray;"
                               "border-right: 0px;"
                               "border-top: 0px;");

    listOfGlobalMessages->setMinimumSize(300,250);
    listOfGlobalMessages->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    listOfGlobalMessages->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listOfGlobalMessages->setStyleSheet("border-color: #DBDBDB;");

    buttonSend->setFixedSize(40,40);
    buttonSend->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    buttonSend->setStyleSheet("border: 1px solid gray;"
                              "border-left: 0px;"
                              "border-top: 0px;"
                              "background: black;");

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

    mainLayout->addWidget(listOfGlobalMessages, 0, 0, 8, 9);
    mainLayout->addWidget(textMessage, 8, 0, 1, 8);
    mainLayout->addWidget(buttonSend, 8, 8, 1, 1);
    mainLayout->addWidget(buttonFriends, 0,9,1,2);
    mainLayout->addWidget(buttonPrivateMessages, 1,9,1,2);

    labelFloodError->setStyleSheet("color: red;"
                                   "border: 0px;");
    labelFloodError->setText("Flood");
    labelFloodError->close();

    connect(buttonSend, SIGNAL(released()), this, SLOT(printMessages()));
    connect(textMessage, SIGNAL(enter()), this, SLOT(printMessages()));
    connect(textMessage, SIGNAL(enter()), this, SLOT(sendMessage()));
    connect(buttonSend, SIGNAL(released()), this, SLOT(sendMessage()));
}

QString wrapText(QString text, QFont font);

void MainWindow::start(QByteArray sessionKey){
    client->sessionKey = sessionKey;
    this->show();
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
        if(count>=3){
            labelFloodError->show();
            return;
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
    textOfMessage->setText(wrapText(textMessage->toPlainText(), textOfMessage->font()));

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

GlobalTextEdit::GlobalTextEdit(QWidget *parent) : QTextEdit(parent){
}

//Crazy bicycle, please don't touch it, it's dangerous...
QString wrapText(QString text, QFont font){
    QFontMetrics *tempFontSize = new QFontMetrics(font);
    for(int i=text.indexOf("  ");i!=-1;i=text.indexOf("  "))
        text.remove(i,1);
    QString final = text;

    int tempCountOfPixels = 0;
    bool isSpace = false;

    if(tempFontSize->width(text)>445){
        final="";
        for(int i=0;i<text.length();i++){
            tempCountOfPixels+=tempFontSize->width(text[i]);
            isSpace = false;
            if(text[i]=="\n")
                tempCountOfPixels=0;
            if(tempCountOfPixels>=445){
                for(int j=i;j>final.length();j--){
                    if(text[j]==" "){
                        i=j;
                        isSpace=true;
                        final = text.mid(0,j);
                        break;
                    }
                }
                if(!isSpace){
                    final+="  ";
                    final+=text[i];
                }
                tempCountOfPixels=0;
            }
            else if(!isSpace)
                final+=text[i];
        }
    }

    return final;
}

void GlobalTextEdit::keyPressEvent(QKeyEvent *e){
    bool previousSpace = false;

    if(this->toPlainText().length() > 0)
        previousSpace = this->toPlainText().at(this->toPlainText().length()-1) == " ";

    if((e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)){
        if(e->modifiers() != Qt::ControlModifier){
            emit enter();
            return;
        }
        else if(!previousSpace){
            if(this->toPlainText().indexOf('\n')==-1)
                QTextEdit::keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier));
            return;
        }
    }
    else if(e->key() == Qt::Key_Space && previousSpace)
        return;

    QTextEdit::keyPressEvent(e);
}

void WrapLabel::keyPressEvent(QKeyEvent *event)
{
    QLabel::keyPressEvent(event);
    if(event->matches(QKeySequence::Copy) || event->matches(QKeySequence::Cut)){
        QString tempText = QApplication::clipboard()->text();
        for(int i=tempText.indexOf("  ");i!=-1;i=tempText.indexOf("  "))
            tempText.remove(i,2);
        QApplication::clipboard()->setText(tempText);
    }
}

void PrivateTextEdit::keyPressEvent(QKeyEvent *e)
{
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

MainWindow::~MainWindow(){
    delete client;
}

PrivateTextEdit::PrivateTextEdit(QWidget *parent) : QTextEdit(parent){}

WrapLabel::WrapLabel(QWidget *parent): QLabel(parent){}

WrapLabel::~WrapLabel(){}

/*
 *  QModelIndexList selectedList = ui->listWidget->selectionModel()->selectedIndexes();
    std::sort(selectedList.begin(),selectedList.end(),[](const QModelIndex& a, const QModelIndex& b)->bool{return a.row()>b.row();});
    for(const QModelIndex& singleIndex : selectedList)
    ui->listWidget->model()->removeRow(singleIndex.row());
*/
