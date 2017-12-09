#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QScrollBar>
#include <QQueue>
#include "UI/Widgets/clickablelabel.h"
#include "UI/Widgets/globaltextedit.h"
#include "UI/Widgets/privatetextedit.h"
#include "UI/Widgets/wraplabel.h"
#include "Util/floodtimer.h"
#include "Util/distance_damerau_levenshtein.h"
#include "Util/udpclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    UDPClient *client;
    QWidget *mainWidget;
    QGridLayout *mainLayout;
    QListWidget *listOfGlobalMessages;
    QPushButton *buttonSend;
    QPushButton *buttonPrivateMessages;
    QPushButton *buttonFriends;
    GlobalTextEdit *textMessage;
    ClickableLabel *labelFloodError;
    ClickableLabel *labelBan;
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
