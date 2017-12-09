#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QListWidget>
#include <QKeyEvent>
#include <QLabel>
#include <QScrollBar>
#include <QPlainTextEdit>
#include <QFontMetrics>
#include <QClipboard>
#include <QApplication>
#include <QQueue>
#include <clickablelabel.h>
#include "floodtimer.h"
#include "distance_damerau_levenshtein.h"
#include "udpclient.h"
#include "globaltextedit.h"
#include "privatetextedit.h"
#include "wraplabel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    UDPClient *client;
    QWidget *mainWidget;
    QGridLayout *mainLayout;
    QListWidget *listOfGlobalMessages;
    QPushButton *buttonSend;
    QPushButton *buttonPrivateMessages;
    QPushButton *buttonFriends;
    GlobalTextEdit *textMessage;
    ClickableLabel *labelFloodError;
    QLabel *labelTimerShow;

    FloodTimer *floodTimer;

    QQueue<QString> lastMessages;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void sendMessage();
    void printMessages();
    void floodErrorHide();
    void updateTime();
public slots:
    void start(QByteArray sessionKey);
};


#endif // MAINWINDOW_H
